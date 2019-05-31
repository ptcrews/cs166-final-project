#include <iostream>
#include "ResUtils.h"

using namespace std;

int main() {
  ResUtils utils(10);
  //utils.testUnbalancedMerge();
  utils.testPurifyingMerge();
  return 0;
}
