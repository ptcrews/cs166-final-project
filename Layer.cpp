#include "Layer.h"
#include <math.h>

#include <vector>
#include <cstddef>

using namespace std;

Layer::Layer(size_t delta, size_t n, Layer* prev, size_t index) {
  this->threshold = pow(2, index+1) * (log2(n)*log2(n) + delta * delta);
  this->prev = prev;
  this->next = NULL;
  this->index = index;
}

Layer::~Layer() {

}

size_t Layer::getThreshold() {
  return this->threshold;
}
