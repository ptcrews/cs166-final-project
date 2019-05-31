#include "ResilientPQ.h"
#include <math.h>
#include <bits/stdc++.h>
#include <algorithm>

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

int ResilientPQ::deletemin() {
  // Find the minimum of first delta + 1 elements in D_0, U_0 and I
  pair<int, int> min1 = findmin(this->layers[0].upBuffer, 0, this->delta+1);
  pair<int, int> min2 = findmin(this->layers[0].downBuffer, 0, this->delta+1);
  pair<int, int> min3 = findmin(this->buffer, 0, this->delta+1);
  
  // Find the minimum element
  int minEle = min(min1.first, min(min2.first, min3.first));
  
  /** From paper :  Delete the minimum element and right shift all the elements 
      in the affected buffer from the beginning upto the position of the minimum
      --- As we are using vectors, we can erase the element at that position
  **/
  if (minEle == min1.first) {
    this->layers[0].upBuffer.erase(this->layers[0].upBuffer.begin() + min1.second);
  } else if (minEle == min2.first) {
    this->layers[0].downBuffer.erase(this->layers[0].downBuffer.begin() + min2.second);
  } else {
    this->buffer.erase(this->layers[0].downBuffer.begin() + min3.second);
  }
  // Check if all the invariants hold; Call pull primitive if required
  return minEle;
}

// Find elements in range [lo, hi) in the given vector
pair<int, int> ResilientPQ::findmin(vector<int> v1, size_t lo, size_t hi) {
  if (v1.size() == 0) return pair<int, int>(INT_MAX, -1);
  int minPos = lo;
  int minEle = v1[lo];
  for (size_t i = lo+1; i < hi; i++) {
    if (minEle > v1[i]) {
      minEle = v1[i];
      minPos = i;
    }  
  }
  return pair<int, int>(minEle,minPos);
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

