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

vector<size_t> ResUtils::genRandomVector(size_t vec_size,
        size_t max_elem) {

    vector<size_t> vec;
    vec.reserve(vec_size);

    for (size_t i = 0; i < vec_size; i++) {
        vec.push_back(rand() % (max_elem + 1));
    }

    return vec;
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
    A = genRandomVector(SIZE_A, MAX_ELEM);
    B = genRandomVector(SIZE_B, MAX_ELEM);

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

// Finds the min resiliently
size_t resilientMin(vector<size_t> vec, size_t start, size_t end) {
    size_t min_idx = start;
    end = min(end, vec.size());
    for (size_t i = start; i < end; i++) {
        if (vec[i] < vec[min_idx]) {
            min_idx = i;
        }
    }
    return min_idx;
}

vector<size_t> ResUtils::NaiveMerge(vector<size_t> A, vector<size_t> B) {
    vector<size_t> merged;
    size_t min_A = resilientMin(A, 0, this->delta + 1 + 1);
    size_t min_B = resilientMin(B, 0, this->delta + 1 + 1);
    while (A.size() > 0 && B.size() > 0) {

        if (A[min_A] <= B[min_B]) {
            merged.push_back(A[min_A]);
            A.erase(A.begin() + min_A);
            min_A = resilientMin(A, 0, this->delta + 1 + 1);
        } else {
            merged.push_back(B[min_B]);
            B.erase(B.begin() + min_B);
            min_B = resilientMin(B, 0, this->delta+ 1 + 1);
        }
    }

    if (A.size() > 0) {
        for (size_t i = 0; i < A.size(); i++) {
            merged.push_back(A[i]);
        }
    }

    if (B.size() > 0) {
        for (size_t i = 0; i < B.size(); i++) {
            merged.push_back(B[i]);
        }
    }
    return merged;
}

// Merge sort
vector<size_t> ResUtils::NaiveSort(vector<size_t> F) {
    for (size_t chunk_size = 1; chunk_size < F.size(); chunk_size *= 2) {

        for (size_t first_arr = 0; first_arr < F.size(); first_arr += 2*chunk_size) {
            size_t second_arr = first_arr + chunk_size;
            size_t second_end = min(F.size(), second_arr + chunk_size);
            if (second_arr >= F.size()) {
                break;
            }


            vector<size_t> A;
            vector<size_t> B;
            for (size_t i = first_arr; i < first_arr + chunk_size; i++) {
                A.push_back(F[i]);
            }

            for (size_t i = second_arr; i < second_end; i++) {
                B.push_back(F[i]);
            }

            vector<size_t> merged = this->NaiveMerge(A, B);
            for (size_t i = 0; i < merged.size(); i++)
                F[first_arr + i] = merged[i];
        }
    }
    return F;
}

void ResUtils::testNaiveSort() {
    vector<size_t> unsorted, correctSort;

    auto seed = time(NULL);
    cout << seed << endl;
    srand(seed);
    unsorted = genRandomVector(100, 2*MAX_ELEM);
    correctSort = unsorted;

    cout << unsorted[0];
    sort(correctSort.begin(), correctSort.end());
    cout << unsorted[0];

    //vector<size_t> resSorted = NaiveSort(unsorted);
    vector<size_t> resSorted = ResilientSort(unsorted);

    cout << endl;
    for (size_t i = 0; i < correctSort.size(); i++) {
        cout << resSorted[i] << " " << correctSort[i] << "\t";
        assert(correctSort[i] == resSorted[i]);
    }
    cout << endl;
    cout << "SUCCESS" << endl;
}

size_t handleInversion(vector<size_t>& working, vector<size_t>& main, vector<size_t>& F, size_t main_idx, size_t working_idx) {
    cout << "HANDLING INVERSION: " << working[working_idx] << " " << working[working_idx+1] << endl;
    // Reset
    F.push_back(working[working_idx]);
    F.push_back(working[working_idx+1]);
    // TODO: Inefficient
    working.erase(working.begin() + working_idx, working.begin() + working_idx + 2);
    cout << "HANDLING INVERSION: " << working[working_idx] << " " << working[working_idx+1] << endl;
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
    cout << parent.size() << " " << parent_idx << "  " << room_left <<  " " << refill_count << endl;

    for (size_t i = 0; i < refill_count; i++) {
        arr.push_back(parent[parent_idx + i]);
    }
    return parent_idx + refill_count;
}

bool safetyCheck(vector<size_t> &XY_aux, size_t offset, size_t top) {
    for (size_t i = offset; i < XY_aux.size(); i++) {
        if (top > XY_aux[i]) {
            // Find inversion
            cout << "Found inversion in sanity check: " << top << " " << i<< endl;
            return false;
        }
    }
    return true;
}

size_t findInversion(vector<size_t> &XY_aux, size_t offset, vector<size_t> &XY, size_t xy_idx, vector<size_t> &F) {

    for (size_t i = offset; i < XY_aux.size() - 1; i++) {
        if (XY_aux[i] > XY_aux[i + 1]) {
            return handleInversion(XY_aux, XY, F, xy_idx, i);
        }
    }
    // THIS SHOULD NOT HAPPEN
    assert(false);
    return 0;
}

pair<vector<size_t>, vector<size_t>> ResUtils::PurifyingMerge(vector<size_t> X, vector<size_t> Y) {
    vector<size_t> Z, F, X_aux, Y_aux, Z_aux;
    //Z_aux.reserve(this->delta);

    size_t x_idx = 0, y_idx = 0;
    // X_aux, Y_aux full; Z_aux empty
    size_t i = 0, j = 0;

    x_idx = refillArray(X_aux, X, 2*this->delta + 1, x_idx);
    y_idx = refillArray(Y_aux, Y, 2*this->delta + 1, y_idx);
    while(X_aux.size() > 0 || Y_aux.size() > 0) {
        cout << "STATS" << endl;
        cout << i << " " << X_aux.size() << " " << x_idx << " " << X.size() << endl;
        cout << j << " " << Y_aux.size() << " " << y_idx << " " << Y.size() << endl;
        cout << "END STATS" << endl;

        // While Z_aux is not full
        while (Z_aux.size() < this->delta && (i < X_aux.size() || j < Y_aux.size())) {
            // If Y_aux is empty or X_aux nonempty and X_aux top less than Y_aux top
            if (Y_aux.size() <= j || (X_aux.size() > i && X_aux[i] <= Y_aux[j])) {
                cout << "In first if branch: "<< Y_aux.size() << " " << j << " " << X_aux.size() << " " << i << endl;
                if (X_aux.size() <= i+1 || X_aux[i] <= X_aux[i+1]) {
                    // Append element
                    cout << "In inner condition" << endl;
                    Z_aux.push_back(X_aux[i++]);
                } else {
                    cout << "X INVERSION" << endl;
                    x_idx = handleInversion(X_aux, X, F, x_idx, i);
                    // Reset indices
                    Z_aux.clear();
                    i = 0, j = 0;
                }
            // TODO: Confirm don't need additional guards here
            } else {
                if (Y_aux.size() <= j+1 || Y_aux[j] <= Y_aux[j+1]) {
                    // Append element
                    Z_aux.push_back(Y_aux[j++]);
                } else {
                    cout << "Y INVERSION" << endl;
                    y_idx = handleInversion(Y_aux, Y, F, y_idx, j);
                    // Reset indices
                    Z_aux.clear();
                    i = 0, j = 0;
                }
            }
        }

        cout << "END OF FN BEFORE Z CALL" << endl;
        cout << i << " " << X_aux.size() << " " << x_idx << " " << X.size() << endl;
        cout << j << " " << Y_aux.size() << " " << y_idx << " " << Y.size() << endl;
        cout << "END" << endl;

        // If Z_aux full
        cout << "Z_AUX SIZE: " << Z_aux.size() << endl;
        if (Z_aux.size() == this->delta || (j == Y_aux.size() && i == X_aux.size())) {
            cout << "HERE!!!!" << endl;
            size_t z_top = Z_aux[Z_aux.size() -1];

            if (!safetyCheck(X_aux, i, z_top)) {
                x_idx = findInversion(X_aux, i, X, x_idx, F);
                Z_aux.clear();
                i = 0, j = 0;
                continue;
            }

            if (!safetyCheck(Y_aux, j, z_top)) {
                y_idx = findInversion(Y_aux, j, Y, y_idx, F);
                Z_aux.clear();
                i = 0, j = 0;
                continue;
            }

            for (size_t k = 0; k < Z_aux.size(); k++) {
                Z.push_back(Z_aux[k]);
            }
            if (i > 0) {
                X_aux.erase(X_aux.begin(), X_aux.begin() + i);
            }
            if (j > 0) {
                Y_aux.erase(Y_aux.begin(), Y_aux.begin() + j);
            }
            Z_aux.clear();
            i = 0, j = 0;
            x_idx = refillArray(X_aux, X, 2*this->delta + 1, x_idx);
            y_idx = refillArray(Y_aux, Y, 2*this->delta + 1, y_idx);
        }

        cout << "HERE 2" << endl;
    }

    if (Z_aux.size() != 0) {
        for (size_t k = 0; k < Z_aux.size(); k++) {
            Z.push_back(Z_aux[k]);
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

vector<size_t> ResUtils::ResilientSort(vector<size_t> I) {
    for (size_t chunk_size = 1; chunk_size < I.size(); chunk_size *= 2) {

        for (size_t first_arr = 0; first_arr < I.size(); first_arr += 2*chunk_size) {
            size_t second_arr = first_arr + chunk_size;
            size_t second_end = min(I.size(), second_arr + chunk_size);
            if (second_arr >= I.size()) {
                break;
            }


            vector<size_t> A;
            vector<size_t> B;
            for (size_t i = first_arr; i < first_arr + chunk_size; i++) {
                A.push_back(I[i]);
            }

            for (size_t i = second_arr; i < second_end; i++) {
                B.push_back(I[i]);
            }

            vector<size_t> merged = this->ResilientMerge(A, B);
            for (size_t i = 0; i < merged.size(); i++)
                I[first_arr + i] = merged[i];
        }
    }
    return I;
}

// TODO: Test resilience
void ResUtils::testPurifyingMerge() {
    vector<size_t> A, B, correctMerge;

    auto seed = time(NULL);
    cout << seed << endl;
    srand(seed);
    A = genRandomVector(15, MAX_ELEM);
    B = genRandomVector(27, MAX_ELEM);

    sort(A.begin(), A.end());
    sort(B.begin(), B.end());
    size_t first = A[10];
    A[10] = A[11];
    A[11] = first;
    size_t second = B[15];
    B[15] = B[16];
    B[16] = second;

    B[10] = 2*MAX_ELEM;
    B[22] = 0;

    correctMerge.reserve(SIZE_A + SIZE_B);
    merge(A.begin(), A.end(), B.begin(), B.end(), correctMerge.begin());

    auto ZF = this->PurifyingMerge(A, B);
    auto Z = ZF.first;
    auto F = ZF.second;

    cout << "A: ";
    for (size_t i = 0; i < A.size(); i++)
        cout << A[i] << " ";
    cout << endl;

    cout << "B: ";
    for (size_t i = 0; i < B.size(); i++)
        cout << B[i] << " ";
    cout << endl;

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
