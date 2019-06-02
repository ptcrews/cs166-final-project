#ifndef ResilientPQ_Included
#define ResilientPQ_Included

#include "ResUtils.h"
#include "Layer.h"
#include <vector>
#include <cstddef>

using namespace std;

class ResilientPQ {

public:
  ResilientPQ(size_t delta, size_t n, bool is_resilient);
  ~ResilientPQ();

  void insert(size_t key);
  size_t deletemin();
  size_t findmin();

private:
  vector<size_t> buffer;
  size_t bufferThreshold;

  ResUtils res_util;

  // Total number of corruptions in the memory
  size_t delta;
  // Total number of elements to be inserted in the priority queue
  size_t n;

  // Whether to use resilient sorting/merging or not
  bool is_resilient;

  // Layer of the priority queue
  vector<Layer> layers;

  // TODO: Non-resilient merge; replace this with a call to resilient merge
  vector<size_t> merge(vector<size_t> v1, vector<size_t> v2);

  // Find minimum element and its position in a given range in a vector;
  // does a simple linear scan
  pair<size_t, size_t> findmin(vector<size_t> v1, size_t lo, size_t hi);

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
