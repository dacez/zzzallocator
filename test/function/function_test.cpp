#include "test/test.h"
#include "zzzallocator.h"

void AllocAndFreeTest() {
    za_Allocator *A = za_New();
    za_size_t **ptrs = new za_size_t *[1000];
    memset(ptrs, 0, sizeof(za_size_t) * 1000);
    for (za_size_t i = 0; i < 100; ++i) {
        ptrs[i] = (za_size_t *)za_Alloc(A, (i+1) * sizeof(za_size_t));
        for (za_size_t j = 0; j < i + 1; ++j) {
            ptrs[i][j] = i;
        }
    }
    for (za_size_t i = 0; i < 100; ++i) {
        for (za_size_t j = 0; j < i + 1; ++j) {
            CheckRet(__FILE__, __LINE__, ptrs[i][j] == i);
        }
    }
    for (za_size_t i = 100; i < 200; ++i) {
        ptrs[i] = (za_size_t *)za_Alloc(A, (i+1) * sizeof(za_size_t));
        for (za_size_t j = 0; j < i + 1; ++j) {
            ptrs[i][j] = i;
        }
    }
    for (za_size_t i = 0; i < 100; ++i) {
        for (za_size_t j = 0; j < i + 1; ++j) {
            CheckRet(__func__, __LINE__, ptrs[i][j] == i);
        }
    }
    for (za_size_t i = 100; i < 200; ++i) {
        for (za_size_t j = 0; j < i + 1; ++j) {
            CheckRet(__func__, __LINE__, ptrs[i][j] == i);
        }
    }
    for (za_size_t i = 0; i < 100; ++i) {
        za_Free(A, ptrs[i]);
    }
    for (za_size_t i = 100; i < 200; ++i) {
        for (za_size_t j = 0; j < i + 1; ++j) {
            CheckRet(__func__, __LINE__, ptrs[i][j] == i);
        }
    }
    for (za_size_t i = 100; i < 200; ++i) {
        za_Free(A, ptrs[i]);
    }

    memset(ptrs, 0, sizeof(za_size_t) * 1000);
    for (za_size_t i = 0; i < 100; ++i) {
        ptrs[i] = (za_size_t *)za_Alloc(A, (i+1) * sizeof(za_size_t));
        for (za_size_t j = 0; j < i + 1; ++j) {
            ptrs[i][j] = i;
        }
    }
    for (za_size_t i = 0; i < 100; ++i) {
        for (za_size_t j = 0; j < i + 1; ++j) {
            CheckRet(__FILE__, __LINE__, ptrs[i][j] == i);
        }
    }
    for (za_size_t i = 100; i < 200; ++i) {
        ptrs[i] = (za_size_t *)za_Alloc(A, (i+1) * sizeof(za_size_t));
        for (za_size_t j = 0; j < i + 1; ++j) {
            ptrs[i][j] = i;
        }
    }
    for (za_size_t i = 0; i < 100; ++i) {
        for (za_size_t j = 0; j < i + 1; ++j) {
            CheckRet(__func__, __LINE__, ptrs[i][j] == i);
        }
    }
    for (za_size_t i = 100; i < 200; ++i) {
        for (za_size_t j = 0; j < i + 1; ++j) {
            CheckRet(__func__, __LINE__, ptrs[i][j] == i);
        }
    }
    for (za_size_t i = 0; i < 100; ++i) {
        za_Free(A, ptrs[i]);
    }
    for (za_size_t i = 100; i < 200; ++i) {
        for (za_size_t j = 0; j < i + 1; ++j) {
            CheckRet(__func__, __LINE__, ptrs[i][j] == i);
        }
    }
    for (za_size_t i = 100; i < 200; ++i) {
        za_Free(A, ptrs[i]);
    }

    za_Release(A);
    delete[] ptrs;
}

void FreeTest() {
    za_Allocator *A = za_New();
    za_size_t **ptrs = new za_size_t *[1000];
    memset(ptrs, 0, sizeof(za_size_t) * 1000);
    for (za_size_t i = 0; i < 100; ++i)
    {
        ptrs[i] = (za_size_t *)za_Alloc(A, (i + 1) * sizeof(za_size_t));
        for (za_size_t j = 0; j < i + 1; ++j)
        {
            ptrs[i][j] = i;
        }
    }
    for (za_size_t i = 0; i < 100; ++i)
    {
        za_Free(A, ptrs[i]);
    }
    for (za_size_t i = 0; i < 100; ++i)
    {
        za_Free(A, ptrs[i]);
    }
    for (za_size_t i = 0; i < 100; ++i)
    {
        za_Free(A, ptrs[i]);
    }
    for (za_size_t i = 0; i < 100; ++i)
    {
        za_Free(A, ptrs[i]);
    }
    for (za_size_t i = 0; i < 100; ++i)
    {
        za_Free(A, ptrs[i]);
    }
    for (za_size_t i = 0; i < 100; ++i)
    {
        ptrs[i] = (za_size_t *)za_Alloc(A, (i + 1) * sizeof(za_size_t));
        for (za_size_t j = 0; j < i + 1; ++j)
        {
            ptrs[i][j] = i;
        }
    }
    for (za_size_t i = 0; i < 100; ++i)
    {
        ptrs[i+100] = (za_size_t *)za_Alloc(A, (i + 1) * sizeof(za_size_t));
        for (za_size_t j = 0; j < i + 1; ++j)
        {
            ptrs[i+100][j] = i+100;
        }
    }
    for (za_size_t i = 0; i < 100; ++i) {
        for (za_size_t j = 0; j < i + 1; ++j) {
            CheckRet(__func__, __LINE__, ptrs[i][j] == i);
        }
    }
    return;
}

int main() {
    AllocAndFreeTest();
    FreeTest();
    PrintResult();
    return 0;
}