# zzzallocator
## 简介
zzzallocator是一个快速内存分配器，用于快速分配和回收内存，提高内存的再利用率，仅支持单线程。
对于以下场景，zzzallocator相对于ptmalloc和jemalloc有明显优势
1. 分配1000个对象
2. 删除1000个对象
3. 再分配1000个对象

## 接口
### za_New
创建一个分配器，如果返回0，则表示失败。

### za_Release
释放一个分配器。

### za_Alloc
分配内存，如果返回0，则表示失败。

### za_Free
释放内存，便于重复利用，也可以不释放，在za_Release的时候统一处理。

## 例子
```
#include "zzzallocator.h"

int main() {
    za_Allocator *A = za_New();
    za_size_t start = 1;
    za_size_t count = 10000;
    void **ptrs = (void **)za_Alloc(A, sizeof(void*) * count);
    if (ptrs == 0) {
        printf("1fail!\n");
        return 1;
    }
    for (za_size_t i = start; i <= count; ++i) {
        ptrs[i-1] = za_Alloc(A, i);
        if (ptrs[i-1] == 0) {
            printf("2fail!\n");
        }
    }
    for (za_size_t i = start; i <= count; ++i) {
        if (ptrs[i-1] == 0) {
            printf("3fail!\n");
            continue;
        }
        za_Free(A, ptrs[i-1]);
    }
    for (za_size_t i = start; i <= count; ++i) {
        ptrs[i-1] = za_Alloc(A, i);
        if (ptrs[i-1] == 0) {
            printf("4fail!\n");
        }
    }
    za_Free(A, ptrs);
    za_Release(A);
    return 0;
}
