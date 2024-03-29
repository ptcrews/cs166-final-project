#ifndef Layer_Included
#define Layer_Included

#include <vector>
#include <cstddef>

using namespace std;

class Layer {
public:
  Layer(size_t delta, size_t n, Layer* prev, size_t index);
  ~Layer();
  vector<size_t> upBuffer;
  vector<size_t> downBuffer;
  size_t getThreshold();
private:
  Layer* prev;
  Layer* next;
  size_t index; // Index of current layer (0-indexed)
  size_t threshold; // equal to 2^{i+1} *(log^2 n + delta^2)
};
#endif
