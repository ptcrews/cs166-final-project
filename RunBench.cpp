/* RunBench.cpp
 *
 * This file contains benchmarking for the files in this project. It
 * performs both micro and macro benchmarking over a range of sizes.
 */
#include <iostream>
#include <chrono>
#include <queue>
#include <map>
#include "ResUtils.h"
#include "TestResilientPQ.h"

using namespace std;

#define NUM_TESTS 1
#define DELTA 10
const vector<size_t> TEST_SIZES = {10, 100, 1000, 10000, 100000, 1000000};

// Macrobenchmark functions
void runMacroBenchmarks();

// Microbenchmark functions
void runMicroBenchmarks();
void runSingleMicroBenchmark(size_t delta, size_t numTests, size_t arr_size);
void printStats(map<string, vector<size_t>> stats, string prefix, string title);
double vectorAvg(vector<size_t> vec);

int main() {
  cout << "**************************************************" << endl;
  cout << "***************** BEGIN BENCHMARK ****************" << endl;
  cout << "**************************************************" << endl;
  runMicroBenchmarks();
  runMacroBenchmarks();
  cout << "**************************************************" << endl;
  cout << "****************** END BENCHMARK *****************" << endl;
  cout << "**************************************************" << endl;
  return 0;
}

void runMacroBenchmarks() {

}

void runMicroBenchmarks() {
  for (size_t test_size : TEST_SIZES)
    runSingleMicroBenchmark(10, NUM_TESTS, test_size);
}

void runSingleMicroBenchmark(size_t delta, size_t numTests, size_t arr_size) {

  string title = "Microbenchmark:\t delta=" + to_string(delta) + "\t numTests="
    + to_string(numTests) + "\t arr_size=" + to_string(arr_size);

  string test_prefix = "Microbench:" + to_string(delta) + ","
    + to_string(numTests) + "," + to_string(arr_size) + ": ";

  map<string, vector<size_t>> stats;
  ResUtils util = ResUtils(delta);
  for (size_t i = 0; i < numTests; i++) {
    vector<pair<string, size_t>> single_test = util.benchAllMerge(arr_size, arr_size, delta);
    for (auto e: single_test) {
      stats[e.first].push_back(e.second);
    }
  }
  printStats(stats, test_prefix, title);
}

// Everything should be in ns
void printStats(map<string, vector<size_t>> stats, string prefix, string title) {
  cout << "*****START TEST*****" << endl;
  cout << title << endl;
  for (auto e: stats) {
    cout << prefix + e.first << "\t" << vectorAvg(e.second) << " ns" << endl;
  }
  cout << "*****END TEST*****" << endl;
}

double vectorAvg(vector<size_t> vec) {
  size_t total = 0;
  for (size_t v : vec)
    total += v;
  return ((double) total) / vec.size();
}

void macroRunRefPQ(vector<size_t> elems, priority_queue<size_t>& refpq) {
  for (size_t e : elems){
    refpq.push(e);
  }
}