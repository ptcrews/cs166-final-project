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
    return pair(min_idx, arr[min_idx]);
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

pair<vector<size_t>, vector<size_t>> ResUtils::PurifyingMerge(vector<size_t> X, vector<size_t> Y) {
    vector<size_t> Z, F, X_aux, Y_aux, Z_aux;
    X_aux.reserve(2*this->delta + 1);
    Y_aux.reserve(2*this->delta + 1);
    Z_aux.reserve(this->delta);


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