#include "test/test.h"
#include "zzzallocator.h"
class zzzallocatorTest : public Test
{
  public:
    zzzallocatorTest()
    {
        m_name = "zzzalloc";
    }
    virtual bool New(unsigned long long *ms)
    {
        void **ptrs = new void *[m_size];
        za_Allocator *A = za_New();
        *ms = Now();
        for (unsigned long long i = 0; i < m_size; ++i)
        {
            ptrs[i] = za_Alloc(A, i + 1);
            if (ptrs[i] == 0)
            {
                printf("fail %d\n", __LINE__);
                return false;
            }
            memset(ptrs[i], 1, i);
        }
        *ms = Now() - *ms;
        for (unsigned long long i = 0; i < m_size; ++i)
        {
            za_Free(A, ptrs[i]);
        }
        za_Release(A);
        delete[] ptrs;
        return true;
    }
    virtual bool Free(unsigned long long *ms)
    {
        void **ptrs = new void *[m_size];
        za_Allocator *A = za_New();
        for (unsigned long long i = 0; i < m_size; ++i)
        {
            ptrs[i] = za_Alloc(A, i + 1);
            if (ptrs[i] == 0)
            {
                printf("fail %d\n", __LINE__);
                return false;
            }
        }
        *ms = Now();
        for (unsigned long long i = 0; i < m_size; ++i)
        {
            za_Free(A, ptrs[i]);
        }
        za_Release(A);
        *ms = Now() - *ms;
        delete[] ptrs;
        return true;
    }
    virtual bool All(unsigned long long *ms)
    {
        void **ptrs = new void *[m_size];
        za_Allocator *A = za_New();
        *ms = Now();
        for (int c = 0; c < 10; ++c)
        {
            for (unsigned long long i = 0; i < m_size; ++i)
            {
                ptrs[i] = za_Alloc(A, i + 1);
                if (ptrs[i] == 0)
                {
                    printf("fail %d\n", __LINE__);
                    return false;
                }
                memset(ptrs[i], 1, i);
            }
            for (unsigned long long i = 0; i < m_size; ++i)
            {
                za_Free(A, ptrs[i]);
            }
        }
        za_Release(A);
        *ms = Now() - *ms;
        delete[] ptrs;
        return true;
    }
};