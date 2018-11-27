#ifndef TEST_H
#define TEST_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>

static int SuccCount = 0;
static int FailCount = 0;

static void CheckRet(const char *func, unsigned int line, bool ret)
{
    if (ret == false)
    {
        std::cout << "\033[31m" << func << "(" << line << "):fail"
                  << "\033[37m" << std::endl;
        ++FailCount;
    }
    else
    {
        ++SuccCount;
    }
}

static void PrintResult()
{
    std::cout << "\033[32m"
              << "SuccCount:" << SuccCount << "\033[37m" << std::endl;
    if (FailCount == 0)
        std::cout << "\033[32m"
                  << "FailCount:" << FailCount << "\033[37m" << std::endl;
    else
        std::cout << "\033[31m"
                  << "FailCount:" << FailCount << "\033[37m" << std::endl;
}

static uint64_t Now()
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

class Test
{
  public:
    virtual bool New(unsigned long long *ms)
    {
        return false;
    }
    virtual bool Free(unsigned long long *ms)
    {
        return false;
    }
    virtual bool All(unsigned long long *ms)
    {
        return false;
    }
    bool NewTest()
    {
        return New(&m_new_time);
    }
    bool FreeTest()
    {
        return Free(&m_free_time);
    }
    bool AllTest()
    {
        return All(&m_all_time);
    }
    std::string Name()
    {
        return m_name;
    }
    void Print(unsigned long long s, const std::string &name) {
        if (name != m_name) return;
        m_size = s;
        if (NewTest() == false) m_new_time = 0;
        if (FreeTest() == false) m_free_time = 0;
        if (AllTest() == false) m_all_time = 0;
        printf("%s\t%llu\t%llu\t%llu\n", m_name.c_str(), m_new_time, m_free_time, m_all_time);
    }
    std::string m_name;
    unsigned long long m_size;
  private:
    unsigned long long m_new_time;
    unsigned long long m_free_time;
    unsigned long long m_all_time;
};
#endif