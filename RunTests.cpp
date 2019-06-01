#include <iostream>
#include "ResUtils.h"
#include "TestResilientPQ.h"

#define MAX_ELEM 1000
#define NUM_TESTS 1000

using namespace std;

int main() {
  ResUtils utils(10);
  utils.testAllMerge();
  utils.testAllSort();

  cout<<"Testing Resilient Priority Queue";
  //testResilientPQ();
  return 0;
}