#include <iostream>
#include "ResUtils.h"

using namespace std;

int main() {
  ResUtils utils(10);
  //utils.testUnbalancedMerge();
  utils.testPurifyingMerge();
  //utils.testNaiveSort();
  return 0;
}
