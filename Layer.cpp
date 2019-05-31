#include "Layer.h"
#include <math.h>

#include <vector>

using namespace std;

Layer::Layer(size_t delta, size_t n, Layer* prev) {
  this->threshold = 2 * (log2(n)*log2(n) + delta * delta);
    
  this->upBuffer.resize(this->threshold/2);
  this->downBuffer.resize(this->downBuffer/2);

  this->prev = prev;
  this->next = NULL;
}

Layer::~Layer() {

}
