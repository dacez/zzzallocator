g++ -o performance_test test/performance/performance_test.cpp test/performance/jemalloc/lib/libjemalloc.a -I./ -std=c++11 -ldl -pthread -O3
#g++ -o performance_test test/performance/performance_test.cpp -I./ -std=c++11 -ldl -pthread -O3
./performance_test ptmallocTest
./performance_test jemallocTest
./performance_test zzzallocTest

./performance_test jemallocTest
./performance_test ptmallocTest
./performance_test zzzallocTest

./performance_test zzzallocTest
./performance_test ptmallocTest
./performance_test jemallocTest