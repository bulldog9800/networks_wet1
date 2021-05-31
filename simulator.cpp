#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <ctime>
#include <queue>
#include <random>

using std::string;
using std::endl;
using std::cin;
using std::cout;
using std::vector;
using std::queue;
using std::default_random_engine;
using std::discrete_distribution;
using std::random_device;
using std::uniform_real_distribution;

int main(int argc, char** argv) {
    double total_allowed = stod(string(argv[1]));
    double lambda = stod(string(argv[2]));
    double mu = stod(string(argv[3]));

    std::random_device rd;
    default_random_engine generator(rd());
    uniform_real_distribution<double> distr(0, 1);


    vector<double> probabilities;
    assert(string(argv[4])=="1");
    assert(string(argv[argc-1])=="0");

    for (int i=4; i<argc; i++){
        probabilities.push_back(stod(string(argv[i])));
    }

    vector<double> Ti(probabilities.size(), 0);


    double total_time = 0, service_time = 0;

    while (total_time <= total_allowed) {
         int i = 0;
    }

    return 0;
}
