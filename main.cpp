#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <ctime>
#include <sys/time.h>
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

int main(int argc, char** argv) {
    double T = stod(string(argv[1]));
    double lambda = stod(string(argv[2]));
    double mu = stod(string(argv[3]));

    vector<double> probabilities;
    assert(string(argv[4])=="1");
    assert(string(argv[argc-1])=="0");

    for (int i=4; i<argc; i++){
        probabilities.push_back(stod(string(argv[i])));
    }

    vector<suseconds_t> Ti(probabilities.size(), 0);
    vector<suseconds_t> Zi(probabilities.size(), 0);
    vector<suseconds_t> Ti_time(probabilities.size(), 0);

    struct timespec start, end, last_package, now;
    last_package.tv_nsec = 0;

    int pkgs_in_buffer = 0;
    int pkgs_accepted = 0;
    int pkgs_arrived = 0;

    queue<int> packages;
    queue<suseconds_t> entry_times;
    suseconds_t service_start = 0;
    suseconds_t total_wait = 0;

    default_random_engine generator;

    clock_gettime(CLOCK_MONOTONIC, &start);
    clock_gettime(CLOCK_MONOTONIC, &end);
    while( (end.tv_nsec - start.tv_nsec) < ( T * 1000 ) ) {
        clock_gettime(CLOCK_MONOTONIC, &now);

        // 1/Mu time has passed since service started
        if ( !packages.empty() && ( (now.tv_nsec - service_start) >= ( (1/mu) * 1000 ) ) ) {
            pkgs_in_buffer = packages.size();
            Ti_time[pkgs_in_buffer-1] = now.tv_nsec;
            Ti[pkgs_in_buffer] += now.tv_nsec - Ti_time[pkgs_in_buffer];

            packages.pop();
            entry_times.pop();

            clock_gettime(CLOCK_MONOTONIC, &now);
            // Service has started for the next package
            if ( !packages.empty() ) {
                service_start = now.tv_nsec;
                total_wait += now.tv_nsec - entry_times.front();
            }
        }

        // Lambda time has passed
        if( (now.tv_nsec - last_package.tv_nsec) >= ( (1/lambda) * 1000) ) {
            clock_gettime(CLOCK_MONOTONIC, &last_package);
            pkgs_arrived++;
            pkgs_in_buffer = packages.size();

            double x = probabilities[pkgs_in_buffer];
            discrete_distribution<int> distribution({1-x, x});
            int number = distribution(generator);

            if ( number == 1 ) {
                // A package has entered the buffer
                clock_gettime(CLOCK_MONOTONIC, &now);

                Ti_time[pkgs_in_buffer+1] = now.tv_nsec;
                Ti[pkgs_in_buffer] += now.tv_nsec - Ti_time[pkgs_in_buffer];
                packages.push(pkgs_accepted);
                entry_times.push(now.tv_nsec);
                if ( pkgs_in_buffer == 0 ) {
                    service_start = now.tv_nsec;
                }
                pkgs_accepted++;
            }
        }

        clock_gettime(CLOCK_MONOTONIC, &end);
    }

    while ( !packages.empty() ) {
        clock_gettime(CLOCK_MONOTONIC, &now);

        // 1/Mu time has passed since service started
        if ( !packages.empty() && ( (now.tv_nsec - service_start) >= ( (1/mu) * 1000 ) ) ) {
            pkgs_in_buffer = packages.size();
            Ti_time[pkgs_in_buffer-1] = now.tv_nsec;
            Ti[pkgs_in_buffer] += now.tv_nsec - Ti_time[pkgs_in_buffer];

            packages.pop();
            entry_times.pop();

            clock_gettime(CLOCK_MONOTONIC, &now);
            // Service has started for the next package
            if ( !packages.empty() ) {
                service_start = now.tv_nsec;
                total_wait += now.tv_nsec - entry_times.front();
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double T_tag = ((double)(end.tv_nsec - start.tv_nsec)) / 1000;

    cout << pkgs_accepted << " ";
    cout << pkgs_arrived - pkgs_accepted << " ";
    cout << T_tag << " ";

    for (auto x : Ti) {
        cout << x << " ";
    }





    return 0;
}
