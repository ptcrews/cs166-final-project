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
#include "ResilientPQ.h"
#include "TestResilientPQ.h"

using namespace std;

#define DELTA 5
const vector<size_t> TEST_SIZES = {  250000, 550000, 575000};
//const vector<size_t> TEST_SIZES = {  10,  100, 1000, 2000, 3000, /*4000,*/ 5000, 10000, 20000, 30000, 40000, 50000,
	//100000, 200000, 300000, 400000, 500000, 1000000};
const vector<size_t> NUM_TESTS  = {       1,      1,      1 };
//const vector<size_t> NUM_TESTS  = {1000, 1000, 1000, 1000, 1000, /*1000,*/ 1000,   100,   100,   100,   100,   100,
//	    10,      1,      1,      1,      1,       1};

// Macrobenchmark functions
void runMacroBenchmarks();
void runSingleMacroBenchmark(size_t delta, size_t numTests, size_t arr_size);
vector<size_t> benchmarkInsertReference(vector<size_t> elems, priority_queue<size_t>& refpq);
vector<size_t> benchmarkInsertResilientPQ(vector<size_t> elems, ResilientPQ& rpq);
size_t benchmarkFindReference(priority_queue<size_t>& refpq);
size_t benchmarkFindResilientPQ(ResilientPQ& rpq);

vector<size_t> benchmarkDeleteReference(size_t count, priority_queue<size_t>& refpq);
vector<size_t> benchmarkDeleteResilientPQ(size_t count, ResilientPQ& rpq);

// Microbenchmark functions
void runMicroBenchmarks();
void runSingleMicroBenchmark(size_t delta, size_t numTests, size_t arr_size, bool merge);

// Misc utilities
vector<size_t> genRandomVec(size_t size, size_t max_elem);
void printStats(map<string, vector<double>> stats, string prefix, string title);
double vectorAvg(vector<size_t> vec);
double vectorAvg(vector<double> vec);

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
  cout << endl;
  cout << "**************************************************" << endl;
  cout << "************** BEGIN MACROBENCHMARK **************" << endl;
  cout << "**************************************************" << endl;
  cout << endl;
  for (size_t i = 0; i < TEST_SIZES.size(); i++)
    runSingleMacroBenchmark(DELTA, NUM_TESTS[i], TEST_SIZES[i]);
  cout << endl;
  cout << "**************************************************" << endl;
  cout << "**************  END MACROBENCHMARK ***************" << endl;
  cout << "**************************************************" << endl;
  cout << endl;
}

#define MAX_ELEM 1000
void runSingleMacroBenchmark(size_t delta, size_t numTests, size_t arr_size) {
  // Construct the two priority queues

  string title = "Macrobenchmark:\t delta=" + to_string(delta) + "\t numTests="
    + to_string(numTests) + "\t arr_size=" + to_string(arr_size);

  string test_prefix = "Macrobench:" + to_string(delta) + ","
    + to_string(numTests) + "," + to_string(arr_size) + ": ";

  map<string, vector<double>> stats;

  for (size_t i = 0; i < numTests; i++) {
    vector<size_t> random_elements = genRandomVec(arr_size, MAX_ELEM);
    priority_queue<size_t> refpq;
    ResilientPQ cpq(delta, random_elements.size(), false);
    ResilientPQ rpq(delta, random_elements.size(), true);

    auto ref_insert = benchmarkInsertReference(random_elements, refpq);
    auto cache_insert = benchmarkInsertResilientPQ(random_elements, cpq);
    auto res_insert = benchmarkInsertResilientPQ(random_elements, rpq);
    stats["Insert: Reference:"].push_back(vectorAvg(ref_insert));
    stats["Insert: Cache Oblivious:"].push_back(vectorAvg(cache_insert));
    stats["Insert: Resilient:"].push_back(vectorAvg(res_insert));

    size_t ref_find = benchmarkFindReference(refpq);
    size_t cache_find = benchmarkFindResilientPQ(cpq);
    size_t res_find = benchmarkFindResilientPQ(rpq);
    stats["Find: Reference"].push_back(ref_find);
    stats["Find: Cache Oblivious"].push_back(cache_find);
    stats["Find: Resilient"].push_back(res_find);

    auto ref_delete = benchmarkDeleteReference(random_elements.size(), refpq);
    auto cache_delete = benchmarkDeleteResilientPQ(random_elements.size(), cpq);
    auto res_delete = benchmarkDeleteResilientPQ(random_elements.size(), rpq);
    stats["Delete: Reference:"].push_back(vectorAvg(ref_delete));
    stats["Delete: Cache Oblivious:"].push_back(vectorAvg(cache_delete));
    stats["Delete: Resilient:"].push_back(vectorAvg(res_delete));
  }
  printStats(stats, test_prefix, title);
}

// In ns
vector<size_t> benchmarkInsertReference(vector<size_t> elems, priority_queue<size_t>& refpq) {
  vector<size_t> timing;
  for (auto elem: elems) {
    auto start = chrono::high_resolution_clock::now();
    refpq.push(elem);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
    timing.push_back(duration.count());
  }
  return timing;
}

vector<size_t> benchmarkInsertResilientPQ(vector<size_t> elems, ResilientPQ& rpq) {
  vector<size_t> timing;
  for (auto elem: elems) {
    auto start = chrono::high_resolution_clock::now();
    rpq.insert(elem);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
    timing.push_back(duration.count());
  }
  return timing;
}

size_t benchmarkFindReference(priority_queue<size_t>& refpq) {
  auto start = chrono::high_resolution_clock::now();
  refpq.top();
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
  return duration.count();
}
size_t benchmarkFindResilientPQ(ResilientPQ& rpq) {
  auto start = chrono::high_resolution_clock::now();
  rpq.findmin();
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
  return duration.count();
}

vector<size_t> benchmarkDeleteReference(size_t count, priority_queue<size_t>& refpq) {
  vector<size_t> timing;
  for (size_t i = 0; i < count; i++) {
    auto start = chrono::high_resolution_clock::now();
    refpq.pop();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
    timing.push_back(duration.count());
  }
  return timing;
}

vector<size_t> benchmarkDeleteResilientPQ(size_t count, ResilientPQ& rpq) {
  vector<size_t> timing;
  for (size_t i = 0; i < count; i++) {
    auto start = chrono::high_resolution_clock::now();
    rpq.deletemin();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
    timing.push_back(duration.count());
  }
  return timing;
}

void runMicroBenchmarks() {
  cout << endl;
  cout << "**************************************************" << endl;
  cout << "************** BEGIN MICROBENCHMARK **************" << endl;
  cout << "**************************************************" << endl;
  cout << endl;
  for (size_t i = 0; i < TEST_SIZES.size(); i++) {
    runSingleMicroBenchmark(DELTA, NUM_TESTS[i], TEST_SIZES[i], true);
  }
  for (size_t i = 0; i < TEST_SIZES.size(); i++) {
    runSingleMicroBenchmark(DELTA, NUM_TESTS[i], TEST_SIZES[i], false);
  }
  cout << endl;
  cout << "**************************************************" << endl;
  cout << "**************  END MICROBENCHMARK ***************" << endl;
  cout << "**************************************************" << endl;
  cout << endl;
}

void runSingleMicroBenchmark(size_t delta, size_t numTests, size_t arr_size, bool merge) {

  string prefix;
  if (merge)
    prefix = "Merge: ";
  else
    prefix = "Sort: ";
  string title = "Microbenchmark: " + prefix + "\t delta=" + to_string(delta) + "\t numTests="
    + to_string(numTests) + "\t arr_size=" + to_string(arr_size);

  string test_prefix = "Microbench:" + to_string(delta) + ","
    + to_string(numTests) + "," + to_string(arr_size) + ": ";

  map<string, vector<double>> stats;
  ResUtils util = ResUtils(delta);
  for (size_t i = 0; i < numTests; i++) {
    vector<pair<string, size_t>> single_test;
    if (merge)
      single_test = util.benchAllMerge(arr_size, arr_size, delta);
    else
      single_test = util.benchAllSort(arr_size, delta);
    for (auto e: single_test) {
      stats[e.first].push_back(e.second);
    }
  }
  printStats(stats, test_prefix, title);
}

// Everything should be in ns
void printStats(map<string, vector<double>> stats, string prefix, string title) {
  cout << "*****START TEST*****" << endl;
  cout << title << endl;
  for (auto e: stats) {
    cout << prefix + e.first << "\t" << vectorAvg(e.second) << " ns" << endl;
  }
  cout << "*****END TEST*****" << endl;
}

double vectorAvg(vector<double> vec) {
  size_t total = 0;
  for (size_t v : vec)
    total += v;
  return ((double) total) / vec.size();
}

double vectorAvg(vector<size_t> vec) {
  size_t total = 0;
  for (size_t v : vec)
    total += v;
  return ((double) total) / vec.size();
}

vector<size_t> genRandomVec(size_t size, size_t max_elem) {
  vector<size_t> vec;
  for (size_t i = 0; i < size; i++) {
    vec.push_back(rand() % max_elem);
  }
  return vec;
}
