#include <vector>
#include "test/performance/zzzallocator_test.h"
#include "test/performance/ptmalloc_test.h"
#include "test/performance/jemalloc_test.h"

using namespace std;

vector<Test*> ts;

void Init() {
    ts.push_back(new ptmallocTest());
    ts.push_back(new zzzallocatorTest());
    ts.push_back(new jemallocTest());
}
void Release() {
    for (const auto & t : ts) {
        delete t;
    }
}
void Test(unsigned long long s, const string &name) {
    printf("Test%llu\n", s);
    for (const auto & t : ts) {
        t->Print(s, name);
    }
}
void Run(const string &name) {
    for (int i = 10; i <= 10; ++i) {
        Test(3000*i, name);
    }
}
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("%s name\n", argv[0]);
        return 1;
    }
    Init();
    Run(argv[1]);
    Release();
    return 0;
}