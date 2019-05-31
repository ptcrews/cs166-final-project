#ifndef Layer_Included
#define Layer_Included

#include <vector>
#include <cstddef>

using namespace std;

class Layer {
public:
  Layer(size_t delta, size_t n, Layer* prev);
  ~Layer();
private:
  vector<int> upBuffer;
  vector<int> downBuffer;
  Layer* prev;
  Layer* next;
  size_t threshold; // equal to 2 *(log^2 n + delta^2)
};
#endif
