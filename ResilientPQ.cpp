#include "ResilientPQ.h"
#include <math.h>

ResilientPQ::ResilientPQ(size_t delta, size_t n) {
  this->delta = delta;
  this->n = n;
  this->bufferThreshold = delta + log2(n); 
}

ResilientPQ::~ResilientPQ() {

}

void ResilientPQ::insert(int key) {

}

int ResilientPQ::deletemin() {
  return -1;
}
