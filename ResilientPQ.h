#ifndef ResilientPQ_Included
#define ResilientPQ_Included

#include <vector>

class ResilientPQ {

public:
  ResilientPQ();
  ~ResilientPQ();
  
  void insert(int key);
  int deletemin();

private:
  vector<int> buffer;
  size_t buffer_threshold;

  vector<Layer> layers;

    /* Fun with C++: these next two lines disable implicitly-generated copy
   * functions that would otherwise cause weird errors if you tried to
   * implicitly copy an object of this type. You don't need to touch these
   * lines.
   */
  ResilientPQ(ResilientPQ const &) = delete;
  void operator=(ResilientPQ const &) = delete;
}
#endif
