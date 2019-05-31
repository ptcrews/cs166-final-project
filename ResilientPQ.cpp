#include "ResilientPQ.h"
#include <math.h>
#include <bits/stdc++.h>

ResilientPQ::ResilientPQ(size_t delta, size_t n) {
  this->delta = delta;
  this->n = n;
  this->bufferThreshold = delta + log2(n); 
}

ResilientPQ::~ResilientPQ() {

}

void ResilientPQ::insert(int key) {
  // Insertion buffer is not full; simply append the element to the insertion buffer
  if (this->buffer.size() < this->bufferThreshold) {
    this->buffer.push_back(key);
    return;
  }
  // Insertion buffer is full; first sort elements in the buffer and 
  //then perform a faithful merge with the up_buffer
  
  //TODO: implement resilient sorting
  sort(this->buffer.begin(), this->buffer.end());
  
  //Check if a merge is required; Not required if layers haven't been initialized
  if (this->layers.size() == 0) {
    Layer layer(delta, n, NULL, -1);
    for (size_t i = 0; i < this->buffer.size(); i++) {
      layer.upBuffer.push_back(this->buffer[i]);
    }
    this->buffer.clear(); // delete all elements from the insertion buffer

    // Now we need to call push primitive if required 
    // This will push elements to the lower buffer till all the size invariants are satisfied
    //TODO: Call push primitive

  } else {
    // There are elements in the up buffer;
    // TODO: Call resilient merge
    vector<int> merged = merge(this->buffer, this->layers[0].upBuffer);
    
    // As we have merged both the vectors, reset the insertion buffer
    this->buffer.clear();
    
    // Add all elements into the upBuffer (U_0)
    for (size_t i = 0; i < merged.size(); i++) {
      this->layers[0].upBuffer.push_back(merged[i]);
    }
    // Now we need to call push primitive if required 
    // This will push elements to the lower buffer till all the size invariants are satisfied
    //TODO: Call push primitive 
  }
}

vector<int> ResilientPQ::merge(vector<int> v1, vector<int> v2) {
  vector<int> merged;
  size_t i = 0, j = 0;
  while(i < v1.size() && j < v2.size()) {
    if (v1[i] < v2[j]) {
      merged.push_back(v1[i]);
      i++;
    } else {
      merged.push_back(v2[i]);
      j++;
    }
  }
  while (i < v1.size()) {
    merged.push_back(v1[i]);
  }

  while (j < v2.size()) {
    merged.push_back(v2[j]);
  }
  return merged;
}

int ResilientPQ::deletemin() {
  return -1;
}
