#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <ctime>
#include <sys/time.h>
#include <queue>
#include <random>
#include <chrono>

using std::string;
using std::endl;
using std::cin;
using std::cout;
using std::vector;
using std::queue;
using std::default_random_engine;
using std::discrete_distribution;

int main(int argc, char** argv) {
    std::random_device rd;
    default_random_engine generator(rd());
    int zeros, ones;

    for (int i=0; i<100000; i++){
        discrete_distribution<int> distribution({0.75, 0.25});
        int number = distribution(generator);

        if (number ==1){
            ones++;
        }
        else{
            assert(number==0);
            zeros++;
        }
    }

    cout << "0: " << zeros << endl;
    cout << "1: " << ones << endl;

    return 0;
}
