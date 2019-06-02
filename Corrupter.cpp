#include "Corrupter.h"
#include <iostream>

using namespace std;

Corrupter::Corrupter(double corruption_rate, size_t delta) {
    this->corruption_rate = corruption_rate;
    this->corruption_rate = 0.001;
    this->delta = delta;
    this->n_corruptions = 0;
    this->generator = uniform_real_distribution<>(0.0, 1.0);
    random_device rd;
    std::mt19937 gen(rd());
    this->rng = gen;
}

void Corrupter::CorruptArray(vector<size_t>& vec) {

    for (size_t i = 0; i < vec.size(); i++) {
        if (this->n_corruptions >= 5)//this->delta)
            return;
        double number = this->generator(this->rng);
        if (number <= this->corruption_rate) {
            if (vec[i] % 2 != 0) {
                //vec[i] = 2*(rand() % 500);
                vec[i] = 2*(rand() /2 );
                this->n_corruptions++;
                /*
                cout << "Corrupted " << i << endl;
                cout << this->corruption_rate << " " << number << endl;
                cout << this->delta << " " << this->n_corruptions << endl;
                cout << vec[i] << endl;
                */
            }
        }
    }

}