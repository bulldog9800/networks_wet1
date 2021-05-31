#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <chrono>
#include <sys/time.h>
#include <random>
#include <time.h>

using std::cout;
using std::time;
using std::endl;
using std::default_random_engine;
using std::discrete_distribution;
using std::vector;

int main(){
    int rolls = 100000;
    default_random_engine generator;
    vector<int> count(2, 0);
    struct timespec start, end;
    double x = 0;
    discrete_distribution<int> distribution({x, 1-x});

    for(int i=0; i<rolls; i++){
        start = {0, 0};
        cout << "start = " << start.tv_nsec << endl;
        clock_gettime(CLOCK_MONOTONIC, &start);
        cout << "start = " << start.tv_nsec << endl;

        int number = distribution(generator);
        count[number]++;

        end = {0, 0};
        cout << "end = " << end.tv_nsec << endl;
        clock_gettime(CLOCK_MONOTONIC, &end);
        cout << "end = " << end.tv_nsec << endl;
        cout << "Time it took: " << end.tv_nsec - start.tv_nsec << " microseconds" << endl;
    }

    cout << "0: " << count[0] << endl;
    cout << "1: " << count[1] << endl;

    return 0;
}
