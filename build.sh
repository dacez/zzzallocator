g++ -o function_test test/function/function_test.cpp -I ./ -std=c++11
./function_test

g++ -o performance_test test/performance/performance_test.cpp -I./ -std=c++11 -g -O3
./performance_test zzzalloc
./performance_test ptmalloc