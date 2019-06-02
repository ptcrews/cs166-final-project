#include <iostream>
#include "ResUtils.h"
#include "TestResilientPQ.h"

#define MAX_ELEM 1000
#define NUM_TESTS 100

using namespace std;

void normalTests();
void corruptionTests();

int main() {
  #if CORRUPT_ELEMS == 1
    corruptionTests();
  #else
    normalTests();
  #endif
  return 0;
}

void corruptionTests() {
  vector<double> avg(5, 0);
  size_t count = 0;
  for (size_t i = 0; i < NUM_TESTS; i++) {
    vector<size_t> locations = testResilientPQCorruptions();
    if (locations.size() > 0)
      count++;
    for (size_t j = 0; j < locations.size(); j++)
      avg[j] += locations[j];
  }
  cout << "First corruption location average: " << avg[0] / count << endl;
}

void normalTests() {
  ResUtils utils(10);
  utils.testAllMerge();
  utils.testAllSort();

  cout << "Testing Resilient Priority Queue" << endl;
  testResilientPQ(true);
  testResilientPQ(false);

}