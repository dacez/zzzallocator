#include "zzzallocator.h"

int main() {
    za_Allocator *A = za_New();
    za_size_t start = 1;
    za_size_t count = 100000;
    void **ptrs = (void **)malloc(sizeof(void*) * count);
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
    za_Print(A);
    za_Release(A);
    free(ptrs);
    return 0;
}