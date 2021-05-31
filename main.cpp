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

    struct timeval start, end, last_package, now;
    last_package.tv_usec = 0;

    int pkgs_in_buffer = 0;
    int pkgs_accepted = 0;
    int pkgs_arrived = 0;

    queue<int> packages;
    queue<suseconds_t> entry_times;
    suseconds_t service_start = 0;
    suseconds_t total_wait = 0;

    default_random_engine generator;

    gettimeofday(&start, nullptr);
    gettimeofday(&end, nullptr);
    while( (end.tv_usec - start.tv_usec) < ( T * 1000 ) ) {
        gettimeofday(&now, nullptr);

        // 1/Mu time has passed since service started
        if ( !packages.empty() && ( (now.tv_usec - service_start) >= ( (1/mu) * 1000 ) ) ) {
            pkgs_in_buffer = packages.size();
            Ti_time[pkgs_in_buffer-1] = now.tv_usec;
            Ti[pkgs_in_buffer] += now.tv_usec - Ti_time[pkgs_in_buffer];

            packages.pop();
            entry_times.pop();

            gettimeofday(&now, nullptr);
            // Service has started for the next package
            if ( !packages.empty() ) {
                service_start = now.tv_usec;
                total_wait += now.tv_usec - entry_times.front();
            }
        }

        // Lambda time has passed
        if( (now.tv_usec - last_package.tv_usec) >= ( (1/lambda) * 1000) ) {
            gettimeofday(&last_package, nullptr);
            pkgs_arrived++;
            pkgs_in_buffer = packages.size();

            double x = probabilities[pkgs_in_buffer];
            discrete_distribution<int> distribution({1-x, x});
            int number = distribution(generator);

            if ( number == 1 ) {
                // A package has entered the buffer
                gettimeofday(&now, nullptr);

                Ti_time[pkgs_in_buffer+1] = now.tv_usec;
                Ti[pkgs_in_buffer] += now.tv_usec - Ti_time[pkgs_in_buffer];
                packages.push(pkgs_accepted);
                entry_times.push(now.tv_usec);
                if ( pkgs_in_buffer == 0 ) {
                    service_start = now.tv_usec;
                }
                pkgs_accepted++;
            }
        }

        gettimeofday(&end, nullptr);
    }

    while ( !packages.empty() ) {
        gettimeofday(&now, nullptr);

        // 1/Mu time has passed since service started
        if ( !packages.empty() && ( (now.tv_usec - service_start) >= ( (1/mu) * 1000 ) ) ) {
            pkgs_in_buffer = packages.size();
            Ti_time[pkgs_in_buffer-1] = now.tv_usec;
            Ti[pkgs_in_buffer] += now.tv_usec - Ti_time[pkgs_in_buffer];

            packages.pop();
            entry_times.pop();

            gettimeofday(&now, nullptr);
            // Service has started for the next package
            if ( !packages.empty() ) {
                service_start = now.tv_usec;
                total_wait += now.tv_usec - entry_times.front();
            }
        }
    }
    gettimeofday(&end, nullptr);

    double T_tag = ((double)(end.tv_usec - start.tv_usec)) / 1000;

    cout << pkgs_accepted << " ";
    cout << pkgs_arrived - pkgs_accepted << " ";
    cout << T_tag << " ";

    for (auto x : Ti) {
        cout << x << " ";
    }





    return 0;
}
