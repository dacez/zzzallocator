#include "test/test.h"
#include "test/performance/jemalloc/include/jemalloc/jemalloc.h"

class jemallocTest : public Test
{
  public:
    jemallocTest()
    {
        m_name = "jemalloc";
    }
    virtual bool New(unsigned long long *ms)
    {
        void **ptrs = new void *[m_size];
        *ms = Now();
        for (unsigned long long i = 0; i < m_size; ++i)
        {
            ptrs[i] = malloc(i + 1);
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
            free(ptrs[i]);
        }
        delete[] ptrs;
        return true;
    }
    virtual bool Free(unsigned long long *ms)
    {
        void **ptrs = new void *[m_size];
        for (unsigned long long i = 0; i < m_size; ++i)
        {
            ptrs[i] = malloc(i + 1);
            if (ptrs[i] == 0)
            {
                printf("fail %d\n", __LINE__);
                return false;
            }
        }
        *ms = Now();
        for (unsigned long long i = 0; i < m_size; ++i)
        {
            free(ptrs[i]);
        }
        *ms = Now() - *ms;
        delete[] ptrs;
        return true;
    }
    virtual bool All(unsigned long long *ms)
    {
        void **ptrs = new void *[m_size];
        *ms = Now();
        for (int c = 0; c < 10; ++c)
        {
            for (unsigned long long i = 0; i < m_size; ++i)
            {
                ptrs[i] = malloc(i + 1);
                if (ptrs[i] == 0)
                {
                    printf("fail %d\n", __LINE__);
                    return false;
                }
                memset(ptrs[i], 1, i);
            }
            for (unsigned long long i = 0; i < m_size; ++i)
            {
                free(ptrs[i]);
            }
        }
        *ms = Now() - *ms;
        delete[] ptrs;
        return true;
    }
};