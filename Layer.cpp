#include "Layer.h"
#include <math.h>

#include <vector>
#include <cstddef>

using namespace std;

Layer::Layer(size_t delta, size_t n, Layer* prev, size_t index) {
  this->threshold = 2 * (log2(n)*log2(n) + delta * delta);
    
  this->upBuffer.resize(this->threshold/2); // Up buffer can contain |U_i| <= s_i/2 elements
  this->downBuffer.resize(this->threshold); // Down buffer can contain s_i/2 <= |D_i| <= s_i elements

  this->prev = prev;
  this->next = NULL;
  this->index = index;
}

Layer::~Layer() {

}

size_t Layer::getThreshold() {
  return this->threshold;
}
