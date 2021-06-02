#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <ctime>
#include <queue>
#include <random>
#include <cmath>

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

double max (double a, double b) {
    if (a >= b) {
        return a;
    }
    return b;
}

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
    vector<double> Ti_time(probabilities.size(), 0);
    queue<double> packages;

    int pkgs_arrived = 0;
    int pkgs_accepted = 0;
    int pkgs_in_buffer = 0;

    double package_time = 0, total_service = 0, last_service = 0, total_wait = 0;
    double d, s;

    while (package_time <= total_allowed) {
         distr(generator);

         while ( package_time <= total_allowed && packages.empty() ) {
             d = (-1*log(distr(generator))) / lambda;
             package_time += d;
             cout << "package arrived at: " << package_time << endl;
             pkgs_arrived++;

             pkgs_in_buffer = packages.size();

             double x = probabilities[pkgs_in_buffer];
             discrete_distribution<int> distribution({1-x, x});
             int number = distribution(generator);

             if ( number == 1 ) {
                 cout << "Queue was empty" << endl;
                 cout << "package accepted at: " << package_time << endl;
                 cout << "Ti : ";
                 for (auto t : Ti) {
                     cout << t << " ";
                 }
                 cout << endl;
                 pkgs_accepted++;
                 Ti_time[pkgs_in_buffer+1] = package_time;
                 Ti[pkgs_in_buffer] += package_time - Ti_time[pkgs_in_buffer];
                 packages.push(package_time);
                 last_service = package_time;
             }
         }

         s = (-1*log(distr(generator))) / mu;
         total_service += s;

         cout << "\nStarting to process package at time: " << last_service << endl;
         cout << "Processing should be done after " << s << " time\n" << endl;

         while ( package_time < last_service + s ) {
             d = (-1*log(distr(generator))) / lambda;
             package_time += d;
             cout << "package arrived at: " << package_time << endl;
             pkgs_arrived++;

             pkgs_in_buffer = packages.size();

             double x = probabilities[pkgs_in_buffer];
             discrete_distribution<int> distribution({1-x, x});
             int number = distribution(generator);

             if ( number == 1 ) {
                 cout << "package accepted at: " << package_time << endl;
                 cout << "Ti : ";
                 for (auto t : Ti) {
                     cout << t << " ";
                 }
                 pkgs_accepted++;
                 Ti_time[pkgs_in_buffer+1] = package_time;
                 Ti[pkgs_in_buffer] += package_time - Ti_time[pkgs_in_buffer];
                 packages.push(package_time);
             }
         }

         // Popping
         double t = last_service + s;
         cout << "\nFinished processing package at time " << t << endl;
         cout << "Ti : ";
         for (auto t : Ti) {
             cout << t << " ";
         }
         pkgs_in_buffer = packages.size();
         Ti_time[pkgs_in_buffer-1] = t;
         Ti[pkgs_in_buffer] += t - Ti_time[pkgs_in_buffer];

         packages.pop();
         last_service = t;
    }

    while ( !packages.empty() ) {
        s = (-1*log(distr(generator))) / mu;
        total_service += s;

        double t = last_service + s;
        pkgs_in_buffer = packages.size();
        Ti_time[pkgs_in_buffer-1] = t;
        Ti[pkgs_in_buffer] += t - Ti_time[pkgs_in_buffer];

        packages.pop();
        last_service = t;
    }

    double t_tag = last_service;

    cout << pkgs_accepted << " " << pkgs_arrived - pkgs_accepted << " " <<
    t_tag << endl;
    for (auto t : Ti) {
        cout << t << " ";
    }
    cout << endl;


    return 0;
}
