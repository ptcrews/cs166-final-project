#ifndef ResilientPQ_Included
#define ResilientPQ_Included

#include "Layer.h"
#include <vector>
#include <cstddef>

using namespace std;

class ResilientPQ {

public:
  ResilientPQ(size_t delta, size_t n);
  ~ResilientPQ();

  void insert(int key);
  int deletemin();
  int findmin();

private:
  vector<int> buffer;
  size_t bufferThreshold;

  // Total number of corruptions in the memory
  size_t delta;
  // Total number of elements to be inserted in the priority queue
  size_t n;

  // Layer of the priority queue
  vector<Layer> layers;

  // TODO: Non-resilient merge; replace this with a call to resilient merge
  vector<int> merge(vector<int> v1, vector<int> v2);

  // Find minimum element and its position in a given range in a vector;
  // does a simple linear scan
  pair<int, int> findmin(vector<int> v1, size_t lo, size_t hi);

  // PULL primitive
  void pull(size_t index);

  // PUSH primitive
  void push(size_t index);
  
  //Print RPQ
  void printRPQ();

  // Delete the given element from the buffer and right shift all the elements

    /* Fun with C++: these next two lines disable implicitly-generated copy
   * functions that would otherwise cause weird errors if you tried to
   * implicitly copy an object of this type. You don't need to touch these
   * lines.
   */
  ResilientPQ(ResilientPQ const &) = delete;
  void operator=(ResilientPQ const &) = delete;
};
#endif
