#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

double calculateF(int x) {
    if (x > 1024 || x < 0) return 0.0;
    double y = 0.47;
    double z = -1.32;
    double term1 = cos(abs(pow(x, 3) + z)) / (3 + pow(x, 2) + pow(sin(pow(y, 3)), 2));
    double term2 = (x - x * z) / (y + 1);
    return term1 + term2;
}

string toBin(int x) {
    string b = "";
    for (int i = 0; i < 11; i++) {
        b = (x % 2 == 0 ? "0" : "1") + b;
        x /= 2;
    }
    return b;
}

int toDec(string b) {
    int x = 0;
    for (int i = 0; i < 11; i++) {
        x = x * 2 + (b[i] - '0');
    }
    return x;
}

int main() {
    srand(time(NULL));
    int N = 20;
    int MKI = 150;
    
    vector<int> pop(N);
    for (int i = 0; i < N; i++) {
        pop[i] = rand() % 1025;
    }

    int best_x = pop[0];
    double best_f = calculateF(pop[0]);

    for (int iter = 0; iter < MKI; iter++) {
        double total_f = 0;
        vector<double> f(N);
        
        for (int i = 0; i < N; i++) {
            f[i] = calculateF(pop[i]);
            total_f += f[i];
            if (f[i] > best_f) {
                best_f = f[i];
                best_x = pop[i];
            }
        }

        vector<int> new_pop(N);
        for (int i = 0; i < N; i++) {
            double r = (double)rand() / RAND_MAX * total_f;
            double sum = 0;
            for (int j = 0; j < N; j++) {
                sum += f[j];
                if (sum >= r) {
                    new_pop[i] = pop[j];
                    break;
                }
            }
        }

        for (int i = 0; i < N; i += 2) {
            if (i + 1 < N && rand() % 100 < 80) {
                string p1 = toBin(new_pop[i]);
                string p2 = toBin(new_pop[i + 1]);
                int pt = rand() % 10 + 1;
                string c1 = p1.substr(0, pt) + p2.substr(pt);
                string c2 = p2.substr(0, pt) + p1.substr(pt);
                new_pop[i] = toDec(c1);
                new_pop[i + 1] = toDec(c2);
            }
        }

        for (int i = 0; i < N; i++) {
            if (rand() % 100 < 10) {
                string p = toBin(new_pop[i]);
                int bit = rand() % 11;
                p[bit] = (p[bit] == '0' ? '1' : '0');
                new_pop[i] = toDec(p);
            }
            if (new_pop[i] > 1024) new_pop[i] = 1024;
        }
        pop = new_pop;
    }

    cout << "Optimal x: " << best_x << endl;
    cout << "Maximum function value: " << best_f << endl;

    return 0;
}