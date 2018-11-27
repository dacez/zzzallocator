#ifndef ZZZ_ALLOCATOR_H
#define ZZZ_ALLOCATOR_H

#include <string.h>  // 使用其 memcpy 函数
#include <stdlib.h>  // 使用其 atof malloc free 函数
#include <stdio.h>   // 使用其 sprintf 函数
#include <stdint.h>  // 使用其 uint32_t
#include <stdbool.h> // 使用其 bool

// 默认每次内存翻倍
#ifndef za_Delta
#define za_Delta 2
#endif

// 默认是64位操作系统
#ifndef za_OS_BIT
#define za_OS_BIT 64
#endif

#if za_OS_BIT == 64
typedef uint64_t za_size_t;
#elif
typedef uint32_t za_size_t;
#endif

#ifndef za_INIT_SIZE
#define za_INIT_SIZE (1024 * 128)
#endif

#ifndef za_BIN_LEVEL
#define za_BIN_LEVEL 5
#endif

#ifndef za_BIN_LEVEL_SIZE
#define za_BIN_LEVEL_SIZE 16
#endif

#ifndef za_BIN_MIN_CHUNK
#define za_BIN_MIN_CHUNK 8
#endif

#if defined(__GNUC__) || defined(__clang__)
#define za_LIKELY(x) __builtin_expect(x, 1)
#define za_UNLIKELY(x) __builtin_expect(x, 0)
#else
#define za_LIKELY(x) x
#define za_UNLIKELY(x) x
#endif

// 内存基本单元链表
typedef struct za_binNode
{
    void *Ptr;
    struct za_binNode *Next;

} za_binNode;

typedef struct za_bin
{
    za_size_t ChunkSize;
    za_binNode *Head;
    za_binNode *FreeHead;

} za_bin;

// 内存分配器的节点
typedef struct za_allocatorNode
{
    void *Data;
    za_size_t Pos;
    za_size_t Size;
    struct za_allocatorNode *Next;
} za_allocatorNode;

// 内存分配器
typedef struct za_Allocator
{
    za_allocatorNode *Head;
    za_allocatorNode *End;
    za_bin Bins[za_BIN_LEVEL * za_BIN_LEVEL_SIZE];
    za_size_t LevelMins[za_BIN_LEVEL];
    za_size_t LevelMaxs[za_BIN_LEVEL];
} za_Allocator;

static inline void *za_innerNew(za_size_t size);
static inline void za_innerFree(void *pointer);

// 新建一个Allocator
static inline za_Allocator *za_New()
{
    // 分配大块内存
    void *ptr = za_innerNew(sizeof(za_Allocator) + sizeof(za_allocatorNode) + za_INIT_SIZE);
    if (za_UNLIKELY(ptr == 0))
        return 0;
    za_Allocator *allocator = (za_Allocator *)ptr;
    allocator->Head = (za_allocatorNode *)((char *)ptr + sizeof(za_Allocator));
    allocator->End = allocator->Head;
    memset(allocator->Bins, 0, sizeof(allocator->Bins));
    za_size_t chunk_size = za_BIN_MIN_CHUNK;
    for (za_size_t i = 0; i < za_BIN_LEVEL; ++i)
    {
        allocator->LevelMins[i] = chunk_size;
        for (za_size_t j = 0; j < za_BIN_LEVEL_SIZE;)
        {
            za_bin *b = allocator->Bins + i * za_BIN_LEVEL_SIZE + j;
            b->Head = 0;
            b->ChunkSize = chunk_size * (++j);
        }
        allocator->LevelMaxs[i] = chunk_size * za_BIN_LEVEL_SIZE;
        chunk_size = chunk_size * za_BIN_LEVEL_SIZE * 2;
    }

    allocator->Head->Size = za_INIT_SIZE;
    allocator->Head->Data = (char *)ptr + sizeof(za_Allocator) + sizeof(za_allocatorNode);
    allocator->Head->Pos = 0;
    allocator->Head->Next = 0;
    return allocator;
}

// 释放一个Allocator
static inline void za_Release(za_Allocator *allocator)
{
    // 遍历整个链表，每次释放一块内存
    za_allocatorNode *next = allocator->Head->Next;
    while (za_LIKELY(next != 0))
    {
        za_allocatorNode *nn = next->Next;
        za_innerFree((void *)next);
        next = nn;
    }
    // 最后释放第一块内存
    za_innerFree((void *)allocator);
}

static inline za_bin *za_findBin(za_Allocator *allocator, za_size_t size);
static inline void *za_alloc(za_Allocator *allocator, za_size_t size);
static inline void za_Free(za_Allocator *allocator, void *ptr);
// 分配内存
static inline void *za_Alloc(za_Allocator *allocator, za_size_t size)
{
    if (za_UNLIKELY(size == 0)) return 0;
    za_bin *bin = za_findBin(allocator, size);
    za_size_t *s = 0;
    if (za_UNLIKELY(bin == 0))
    {
        s = (za_size_t *)za_innerNew(sizeof(za_size_t) + size);
        if (za_UNLIKELY(s == 0))
            return 0;
        *s = size;
        return (void *)(s + 1);
    }
    if (bin->Head != 0)
    {
        s = (za_size_t *)bin->Head->Ptr;
        za_binNode *bn = bin->Head;
        bin->Head = bin->Head->Next;

        bn->Next = bin->FreeHead;
        bin->FreeHead = bn;

        return (void *)(s + 1);
    }
    s = (za_size_t *)za_alloc(allocator, sizeof(za_size_t) + bin->ChunkSize);
    if (za_UNLIKELY(s == 0))
        return 0;
    *s = bin->ChunkSize;
    return (void *)(s + 1);
}

// 释放内存
static inline void za_Free(za_Allocator *allocator, void *ptr)
{
    za_size_t *s = (za_size_t *)ptr - 1;
    za_bin *bin = za_findBin(allocator, *s);
    if (za_UNLIKELY(bin == 0))
    {
        za_innerFree(s);
        return;
    }
    za_binNode *bn;
    if (bin->FreeHead != 0)
    {
        bn = bin->FreeHead;
        bin->FreeHead = bin->FreeHead->Next;
    }
    else
    {
        bn = (za_binNode *)za_alloc(allocator, sizeof(za_binNode));
        if (za_UNLIKELY(bn == 0))
        {
            return;
        }
    }

    bn->Ptr = (void *)s;
    bn->Next = bin->Head;
    bin->Head = bn;
    return;
}
// 打印
static inline void za_Print(za_Allocator *allocator)
{
    printf("==============================\n");
    printf("Allocator:\n");
    printf("Ptr %p Head %p End %p Bins %p\n", allocator, allocator->Head, allocator->End, allocator->Bins);
    printf("    Mins: ");
    for (za_size_t i = 0; i < za_BIN_LEVEL; ++i) {
#if za_OS_BIT == 64
        printf("%lu ", allocator->LevelMins[i]);
#else
        printf("%u ", allocator->LevelMins[i]);
#endif
    }
    printf("\n");
    printf("    Maxs: ");
    for (za_size_t i = 0; i < za_BIN_LEVEL; ++i) {
#if za_OS_BIT == 64
        printf("%lu ", allocator->LevelMaxs[i]);
#else
        printf("%lu ", allocator->LevelMaxs[i]);
#endif
    }
    printf("\n");
    za_allocatorNode *n = allocator->Head;
    printf("AllocatorNodes:\n");
    while (n)
    {
#if za_OS_BIT == 64
        printf("    Ptr %p Data %p Pos %lu Size %lu Next %p\n", n, n->Data, n->Pos, n->Size, n->Next);
#else
        printf("    Ptr %p Data %p Pos %u Size %u Next %p\n", n, n->Data, n->Pos, n->Size, n->Next);
#endif
        n = n->Next;
    }
    printf("Bins:\n");
    for (za_size_t i = 0; i < za_BIN_LEVEL * za_BIN_LEVEL_SIZE; ++i)
    {
        za_bin *b = allocator->Bins + i;
        if (b->Head == 0 && b->FreeHead == 0) continue;
        za_size_t bn_c = 0;
        za_size_t fbn_c = 0;
        if (b->Head != 0)
        {
            za_binNode *bn = b->Head;
            while (bn)
            {
                // printf("        BinNode %p Ptr %p Next %p\n", bn, bn->Ptr, bn->Next);
                bn = bn->Next;
                ++bn_c;
            }
        }
        if (b->FreeHead != 0)
        {
            za_binNode *bn = b->FreeHead;
            while (bn)
            {
                // printf("        FreeBN  %p Ptr %p Next %p\n", bn, bn->Ptr, bn->Next);
                bn = bn->Next;
                ++fbn_c;
            }
        }
#if za_OS_BIT == 64
        printf("    [%lu] Ptr %p ChunkSize %lu Head %p FreeHead %p BNSize %lu FBNSize %lu\n", i, b, b->ChunkSize, b->Head, b->FreeHead, bn_c, fbn_c);
#else
        printf("    [%u] Ptr %p ChunkSize %u Head %p FreeHead %p BNSize %lu FBNSize %lu\n", i, b, b->ChunkSize, b->Head, b->FreeHead, bn_c, bn_fc);
#endif
    }
}
// 找到合适的Bin
static inline za_bin *za_findBin(za_Allocator *allocator, za_size_t size)
{
    for (za_size_t i = 0; i < za_BIN_LEVEL; ++i) {
        if (za_LIKELY(size <= allocator->LevelMaxs[i])) {
            return allocator->Bins + ((size-1) / allocator->LevelMins[i]) + za_BIN_LEVEL_SIZE * i;
        }
    }
    return 0;
}
// 追加一个Allocator
static inline bool za_appendChild(za_size_t init_size, struct za_Allocator *allocator)
{
    // 每次分配一大块内存，避免多次分配
    void *ptr = za_innerNew(sizeof(za_allocatorNode) + init_size);
    if (za_UNLIKELY(ptr == 0))
        return false;
    za_allocatorNode *node = (za_allocatorNode *)ptr;
    node->Size = init_size;
    node->Data = (char *)ptr + sizeof(za_allocatorNode);
    node->Pos = 0;
    node->Next = 0;
    // 在ANode组成的链表最后加一个ANode
    allocator->End->Next = node;
    allocator->End = node;
    return true;
}

// 分配大小为size的内存
static inline void *za_alloc(za_Allocator *allocator, za_size_t size)
{
    za_allocatorNode *cur_node = allocator->End;
    za_size_t s = cur_node->Size;
    if (za_UNLIKELY(cur_node->Pos + size > s))
    {
        s *= za_Delta;
        // 通过循环计算最终需要的空间大小
        // 这里应该有更好的方法，就是直接通过计算所得
        while (za_UNLIKELY(size > s))
            s *= za_Delta; // 每次分配内存的大小是上次的za_Delta倍
        if (za_UNLIKELY(za_appendChild(s, allocator) == false))
            return 0;
        cur_node = allocator->End;
    }
    void *ret = (char *)(cur_node->Data) + cur_node->Pos;
    cur_node->Pos += size;
    return ret;
}

static inline void *za_innerNew(za_size_t size)
{
    return malloc(size);
}
static inline void za_innerFree(void *pointer) { free(pointer); }

#endif