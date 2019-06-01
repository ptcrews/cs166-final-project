#include "ResilientPQ.h"
#include <time.h>
#include <vector>
#include <cassert>
#include <queue>
#include <stdlib.h>
#include <iostream>

#define MAX_ELEM 1000

void testResilientPQ() {
  srand(time(NULL));

  priority_queue<int, vector<int>, greater<int>> q;
  size_t delta = 5;
  size_t n = 2000;
  ResilientPQ rpq(delta, n);

  cout<<"\n Generating random numbers!"<<n;
  vector<int> randomNumbers;
  for (size_t i = 0; i < n; i++) {
    randomNumbers.push_back(rand() % MAX_ELEM);
  }
  cout<<"\n Now testing insert and findmin()";
  for (size_t i = 0; i < n; i++) {
    cout<<"\n Inserting "<<i<<" "<<randomNumbers[i];
    q.push(randomNumbers[i]);
    rpq.insert(randomNumbers[i]);
    assert(q.top() == rpq.findmin());
  }
  
  cout<<"\n Now testing Deletemin()";
  while (q.size() > 0) {
    int toDelete = q.top();
    cout<<"\n Deleting "<<toDelete;
    q.pop();
    int e = rpq.deletemin();
    cout<<"\n Checking if same elements were deleted; Expected "<<toDelete<<" Actual "<<e;
    assert( toDelete == e);
  }
}
