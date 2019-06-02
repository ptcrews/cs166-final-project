#include "ResilientPQ.h"
#include <time.h>
#include <vector>
#include <cassert>
#include <queue>
#include <stdlib.h>
#include <iostream>

#define MAX_ELEM 1000

void testResilientPQ(bool is_resilient) {
  priority_queue<size_t, vector<size_t>, greater<size_t>> q;
  size_t delta = 5;
  size_t n = 200000;
  ResilientPQ rpq(delta, n, is_resilient);

  cout << "Generating random numbers!"<< n << endl;
  vector<size_t> randomNumbers;
  for (size_t i = 0; i < n; i++) {
    randomNumbers.push_back(rand() % MAX_ELEM);
  }
  cout<<"Now testing insert and findmin()" << endl;
  for (size_t i = 0; i < n; i++) {
    //cout<<"\n Inserting "<<i<<" "<<randomNumbers[i];
    q.push(randomNumbers[i]);
    rpq.insert(randomNumbers[i]);
    assert(q.top() == rpq.findmin());
  }

  cout<<"Now testing Deletemin()" << endl;;
  while (q.size() > 0) {
    size_t toDelete = q.top();
    //cout<<"\n Deleting "<<toDelete;
    q.pop();
    size_t e = rpq.deletemin();
    //cout<<"\n Checking if same elements were deleted; Expected "<<toDelete<<" Actual "<<e;
    assert( toDelete == e);
  }

}

void testResilientPQ() {
  srand(time(NULL));
  cout << "Testing resilient version" << endl;
  testResilientPQ(true);
  cout << "Testing non-resilient version" << endl;
  testResilientPQ(false);
}
