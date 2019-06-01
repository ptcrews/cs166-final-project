/* ResUtils.cpp
 *
 * This file implements the resilient merging and sorting
 * algorithms presented in the Resilient TODO
 */

#include "ResUtils.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <time.h>

using namespace std;

ResUtils::ResUtils(size_t delta) {
    this->delta = delta;
}

pair<size_t, size_t> scanMin(vector<size_t> arr, size_t offset, size_t delta) {
    size_t min_idx = offset;
    // Find the min index in B[j: j + \delta]
    size_t max_pos = min(arr.size(), offset + delta + 1);
    for (size_t i = offset; i < max_pos; i++) {
        if (arr[i] < arr[min_idx]) {
            min_idx = i;
        }
    }
    return pair<size_t, size_t>(min_idx, arr[min_idx]);
}

// Returns the count of keys smaller than A[i] in A[i+1; i + 2\delta + 1]
size_t countW(vector<size_t> arr, size_t offset, size_t delta) {
    size_t cmp_val = arr[offset];
    size_t max_pos = min(arr.size(), offset + 2*delta + 1 + 1);

    size_t count = 0;
    for (size_t i = offset + 1; i < max_pos; i++) {
        if (arr[i] < cmp_val) {
            count++;
        }
    }
    return count;
}

void rShiftVec(vector<size_t> arr, size_t first_pos, size_t last_pos) {
    size_t n_elems = last_pos - first_pos;
    for (size_t i = 1; i < n_elems; i++) {
        size_t pos = last_pos - i;
        arr[pos+1] = arr[pos];
    }
}

size_t partitionW(vector<size_t> W, size_t cmp, size_t offset, size_t delta, vector<size_t> out) {
    vector<size_t> greater;
    size_t max_pos = min(W.size(), offset + 2*delta + 1 + 1);

    for (size_t i = offset; i < max_pos; i++) {
        if (W[i] > cmp) {
            greater.push_back(W[i]);
        } else {
            out.push_back(W[i]);
        }
    }

    size_t new_offset = max_pos - greater.size();
    for (size_t i = 0; i < greater.size(); i++) {
        W[new_offset + i] = greater[i];
    }

    return new_offset;
}

vector<size_t> ResUtils::UnbalancedMerge(vector<size_t> A, vector<size_t> B) {
    assert(A.size() >= B.size());

    size_t i = 0, j = 0;
    vector<size_t> out;

    /*
     *  b = min(B[j: j+\delta])
     */
    while(i < A.size() && j < B.size()) {
        auto pair = scanMin(B, j, this->delta);
        size_t h = pair.first;
        size_t b = pair.second;
        rShiftVec(B, j, h);
        B[j] = b;
        j++;

        while(i < A.size()) {
            if (A[i] <= b) {
                // Append to output
                out.push_back(A[i]);
                i++;
            } else { // if (A[i] > b)
                size_t t = countW(A, i, this->delta);
                if (t >= this->delta + 1) {
                    // A[i] must be corrupted; continue
                    i++;
                    continue;
                }
                // Otherwise, parition W
                i = partitionW(A, b, i, this->delta, out);
                break;
            }
        }

        out.push_back(b);
    }

    for (; i < A.size(); i++) {
        out.push_back(A[i]);
    }

    for (; j < B.size(); j++) {
        out.push_back(B[j]);
    }

    return out;
}

pair<vector<size_t>, vector<size_t>> ResUtils::genRandomVectors(size_t size_A,
        size_t size_B, size_t max_elem) {

    vector<size_t> A;
    vector<size_t> B;

    A.reserve(size_A);
    B.reserve(size_B);

    for (size_t i = 0; i < size_A; i++) {
        A.push_back(rand() % (max_elem + 1));
    }

    for (size_t j = 0; j < size_B; j++) {
        B.push_back(rand() % (max_elem + 1));
    }

    return pair<vector<size_t>, vector<size_t>>(A, B);
}

#define TEST_SIZE 100
#define SIZE_A 100
#define SIZE_B 50
#define MAX_ELEM 1000
// TODO: Test resilience
void ResUtils::testUnbalancedMerge() {
    vector<size_t> A, B, correctMerge;

    auto seed = time(NULL);
    cout << seed << endl;
    srand(seed);
    auto AB = this->genRandomVectors(SIZE_A, SIZE_B, MAX_ELEM);
    A = AB.first;
    B = AB.second;

    sort(A.begin(), A.end());
    sort(B.begin(), B.end());

    correctMerge.reserve(SIZE_A + SIZE_B);
    merge(A.begin(), A.end(), B.begin(), B.end(), correctMerge.begin());

    auto out = this->UnbalancedMerge(A, B);

    for (size_t i = 0; i < out.size(); i++) {
        cout << out[i] << " " << correctMerge[i] << endl;
        assert(out[i] == correctMerge[i]);
    }
    cout << endl;

    cout << "SUCCESS" << endl;
}

vector<size_t> ResUtils::NaiveSort(vector<size_t> F) {

    return F;
}

size_t handleInversion(vector<size_t>& working, vector<size_t>& main, vector<size_t>& F, size_t main_idx, size_t working_idx) {
    // Reset
    F.push_back(working[working_idx]);
    F.push_back(working[working_idx+1]);
    // TODO: Inefficient
    working.erase(working.begin() + working_idx, working.begin() + working_idx + 2);
    // Check bounds
    if (main_idx < main.size()) {
        working.push_back(main[main_idx++]);
    }
    if (main_idx < main.size()) {
        working.push_back(main[main_idx++]);
    }
    return main_idx;
}

size_t refillArray(vector<size_t> &arr, vector<size_t> parent, size_t target, size_t parent_idx) {
    size_t room_left = target - arr.size();
    size_t amount = min(room_left, parent.size() - parent_idx);
    size_t refill_count = amount;
    cout << "Refill count: " << refill_count << endl;
    cout << parent.size() << " " << parent_idx << "  " << room_left << endl;

    for (size_t i = 0; i < refill_count; i++) {
        arr.push_back(parent[parent_idx + i]);
    }
    return parent_idx + refill_count;
}

pair<vector<size_t>, vector<size_t>> ResUtils::PurifyingMerge(vector<size_t> X, vector<size_t> Y) {
    vector<size_t> Z, F, X_aux, Y_aux, Z_aux;
    //X_aux.reserve(2*this->delta + 1);
    //Y_aux.reserve(2*this->delta + 1);
    Z_aux.reserve(this->delta);

    size_t x_idx = 0, y_idx = 0;
    // X_aux, Y_aux full; Z_aux empty
    size_t i = 0, j = 0, k = 0;

    x_idx = refillArray(X_aux, X, 2*this->delta + 1, x_idx);
    y_idx = refillArray(Y_aux, Y, 2*this->delta + 1, y_idx);
    while(X_aux.size() > 0 || Y_aux.size() > 0) {
        cout << "STATS" << endl;
        cout << i << " " << X_aux.size() << " " << X.size() << endl;
        cout << j << " " << Y_aux.size() << " " << Y.size() << endl;
        cout << "END STATS" << endl;

        // If Y_aux is empty or X_aux nonempty and X_aux top less than Y_aux top
        if (Y_aux.size() <= j || (X_aux.size() > i && X_aux[i] <= Y_aux[j])) {
            if (X_aux.size() <= i+1 || X_aux[i] <= X_aux[i+1]) {
                // Append element
                Z_aux[k++] = X_aux[i++];
            } else {
                x_idx = handleInversion(X_aux, X, F, x_idx, i);
                // Reset indices
                i = 0, j = 0, k = 0;
            }
        // TODO: Confirm don't need additional guards here
        } else {
            if (Y_aux.size() <= j+1 || Y_aux[j] <= Y_aux[j+1]) {
                // Append element
                Z_aux[k++] = Y_aux[j++];
            } else {
                y_idx = handleInversion(Y_aux, Y, F, y_idx, j);
                // Reset indices
                i = 0, j = 0, k = 0;
            }
        }

        // If Z_aux full
        cout << k << endl;
        if (k == this->delta) {
            bool x_inversion = false, y_inversion = false;
            for (size_t i_tmp = i; i_tmp < X_aux.size(); i_tmp++) {
                if (Z_aux[k] > X_aux[i_tmp]) {
                    // Find inversion
                    x_inversion = true;
                    break;
                }
            }

            for (size_t j_tmp = j; j_tmp < Y_aux.size(); j_tmp++) {
                if (Z_aux[k] > Y_aux[j_tmp]) {
                    // Find inversion
                    y_inversion = true;
                    break;
                }
            }

            if (x_inversion) {
                for (size_t i_tmp = i; i_tmp < X_aux.size()-1; i_tmp++) {
                    if (X_aux[i_tmp] > X_aux[i_tmp+1]) {
                        x_idx = handleInversion(X_aux, X, F, x_idx, i_tmp);
                        break;
                    }
                }
                i = 0, j = 0, k = 0;
            }

            if (y_inversion) {
                for (size_t j_tmp = j; j_tmp < Y_aux.size()-1; j_tmp++) {
                    if (Y_aux[j_tmp] > Y_aux[j_tmp+1]) {
                        y_idx = handleInversion(Y_aux, Y, F, y_idx, j_tmp);
                        break;
                    }
                }
                i = 0, j = 0, k = 0;
            }
        }

        if (k != 0) {
            for (size_t k_tmp = 0; k_tmp <= k; k_tmp++) {
                Z.push_back(Z_aux[k_tmp]);
                k = 0;
            }
            if (i > 0) {
                X_aux.erase(X_aux.begin(), X_aux.begin() + i + 1);
            }
            if (j > 0) {
                Y_aux.erase(Y_aux.begin(), Y_aux.begin() + j + 1);
            }
            i = 0, j = 0;
            x_idx = refillArray(X_aux, X, 2*this->delta + 1, x_idx);
            y_idx = refillArray(Y_aux, Y, 2*this->delta + 1, y_idx);
        }
    }

    if (k != 0) {
        for (size_t k_tmp = 0; k_tmp <= k; k_tmp++) {
            Z.push_back(Z_aux[k_tmp]);
        }
    }

    return pair<vector<size_t>, vector<size_t>>(Z, F);
}

vector<size_t> ResUtils::ResilientMerge(vector<size_t> X, vector<size_t> Y) {
    auto ZF = this->PurifyingMerge(X, Y);
    vector<size_t> Z = ZF.first;
    vector<size_t> F = ZF.second;

    vector<size_t> F_prime = this->NaiveSort(F);

    vector<size_t> S = this->UnbalancedMerge(Z, F_prime);

    return S;
}

// TODO: Test resilience
void ResUtils::testPurifyingMerge() {
    vector<size_t> A, B, correctMerge;

    auto seed = time(NULL);
    cout << seed << endl;
    srand(seed);
    auto AB = this->genRandomVectors(SIZE_A, SIZE_B, MAX_ELEM);
    A = AB.first;
    B = AB.second;

    //sort(A.begin(), A.end());
    //sort(B.begin(), B.end());
    size_t first = A[10];
    A[10] = A[11];
    A[11] = first;

    correctMerge.reserve(SIZE_A + SIZE_B);
    merge(A.begin(), A.end(), B.begin(), B.end(), correctMerge.begin());

    auto ZF = this->PurifyingMerge(A, B);
    auto Z = ZF.first;
    auto F = ZF.second;

    cout << "Z: ";
    for (size_t i = 0; i < Z.size(); i++)
        cout << Z[i] << " ";
    cout << endl;

    cout << "F: ";
    for (size_t i = 0; i < F.size(); i++)
        cout << F[i] << " ";
    cout << endl;

/*
    for (size_t i = 0; i < out.size(); i++) {
        cout << out[i] << " " << correctMerge[i] << endl;
        assert(out[i] == correctMerge[i]);
    }
    cout << endl;
    */

    cout << "SUCCESS" << endl;
}
