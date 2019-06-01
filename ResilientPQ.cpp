#include "ResilientPQ.h"
#include <math.h>
#include <bits/stdc++.h>
#include <algorithm>

ResilientPQ::ResilientPQ(size_t delta, size_t n) {
  this->delta = delta;
  this->n = n;
  this->bufferThreshold = delta + log2(n) + 1; // The insertion buffer can contain upto \delta + log n + 1 elements
}

ResilientPQ::~ResilientPQ() {

}

void ResilientPQ::insert(int key) {
  //cout<<"\nInside insert function !";
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
    Layer layer(delta, n, NULL, 0);
    for (size_t i = 0; i < this->buffer.size(); i++) {
      layer.downBuffer.push_back(this->buffer[i]);
    }
    this->layers.push_back(layer);
  } else if (this->layers.size() == 1 && this->layers[0].downBuffer.size() < this->layers[0].getThreshold()/2) {
   // if there is only 1 layer  (L_0) and its down buffer is not filled enough, we simply merge into D_0
    vector<int> merged = merge(this->buffer, this->layers[0].downBuffer);
    this->layers[0].downBuffer.clear();
    this->layers[0].downBuffer = merged;
  } else {
    // There are elements in the up buffer;
    // TODO: Call resilient merge
    vector<int> merged = merge(this->buffer, this->layers[0].upBuffer);
    this->layers[0].upBuffer.clear();
    // Add all elements into the upBuffer (U_0)
    for (size_t i = 0; i < merged.size(); i++) {
      this->layers[0].upBuffer.push_back(merged[i]);
    }

    // Now we need to call push primitive if required
    // This will push elements to the lower buffer till all the size invariants are satisfied
    if (this->layers[0].upBuffer.size() > this->layers[0].getThreshold()/2) {
      cout<<"\n Upbuffer overflow ! Calling PUSH";
      push(0);
    }
  }

  this->buffer.clear(); // delete all elements from the insertion buffer
  this->buffer.push_back(key);
  
  cout<<"\nCurrent Layer sizes "<<this->bufferThreshold;
  size_t total = 0;
  for (size_t i = 0; i < this->layers.size(); i++) {
    cout<<"\nLayer"<<i<<" Threshold "<<this->layers[i].getThreshold();
    cout<<"\nLayer"<<i<<"\n U "<<this->layers[i].upBuffer.size()<<" L "<<this->layers[i].downBuffer.size();
    total += this->layers[i].upBuffer.size() + this->layers[i].downBuffer.size();
  }
  total += this->buffer.size();
  cout<<"\n Total elements in RPQ "<<total;

}

int ResilientPQ::findmin() {
  if (this->layers.size() < 1) {
    if (this->buffer.size() == 0) return INT_MAX;
    return findmin(this->buffer, 0, this->buffer.size()).first;
  }
  // Find the minimum of first delta + 1 elements in D_0, U_0 and I
  pair<int, int> min1 = findmin(this->layers[0].upBuffer, 0, this->delta+1);
  pair<int, int> min2 = findmin(this->layers[0].downBuffer, 0, this->delta+1);
  pair<int, int> min3 = findmin(this->buffer, 0, this->buffer.size());
  // Find the minimum element
  return min(min1.first, min(min2.first, min3.first));
}

int ResilientPQ::deletemin() {
  int minEle = -1;
  if (this->layers.size() < 1) {
    pair<int, int> min1 = findmin(this->buffer, 0, this->buffer.size());
    minEle = min1.first;
    this->buffer.erase(this->buffer.begin() + min1.second);
    // SInce no layers exist, we return
    return minEle;
  } else {
    // Find the minimum of first delta + 1 elements in D_0, U_0 and I
    pair<int, int> min1 = findmin(this->layers[0].upBuffer, 0, this->delta+1);
    pair<int, int> min2 = findmin(this->layers[0].downBuffer, 0, this->delta+1);
    pair<int, int> min3 = findmin(this->buffer, 0, this->buffer.size());

    // Find the minimum element
    minEle = min(min1.first, min(min2.first, min3.first));
    /** From paper :  Delete the minimum element and right shift all the elements
      in the affected buffer from the beginning upto the position of the minimum
      --- As we are using vectors, we can erase the element at that position
    **/
    if (minEle == min1.first) {
      this->layers[0].upBuffer.erase(this->layers[0].upBuffer.begin() + min1.second);
    } else if (minEle == min2.first) {
      this->layers[0].downBuffer.erase(this->layers[0].downBuffer.begin() + min2.second);
    } else {
      this->buffer.erase(this->buffer.begin() + min3.second);
    }
  }
  // TODO : Check if all the invariants hold; Call pull primitive if required
  // Check if D_0 underflows i.e. it has less than s_i/2 elements
  if (this->layers[0].downBuffer.size() < this->layers[0].getThreshold()/2) {
    // check of L_0 is the only layer in priority queue
    // PULL primitive should be called only if there are Layers other than L_0
    if (this->layers.size() > 1) {
      pull(0);
    }
  }
  return minEle;
}
// PULL primitive, accepts index of the layer on which pull function needs to be called
// Note that PULL primitive is called on a down buffer D_i
// The buffers involved are D_i, U_{i+1} and D_{i+1}
void ResilientPQ::pull(size_t index) {
  cout<<"\n Inside PULL; index "<<index<<" Layers "<<this->layers.size();
  if (index+1 == this->layers.size()) return; // There is no next layer; therefore no pull primitive possible

  // Merge D_i and U_{i+1}
  vector<int> merged = merge(this->layers[index].downBuffer, this->layers[index+1].upBuffer);
  // Merge the vector merged and D_{i+1}
  vector<int> finalMerged = merge(merged, this->layers[index+1].downBuffer);
  // Now redistribute elements after merging
  size_t threshold = this->layers[index].getThreshold();
  // First write first s_i elements to D_i
  vector<int> downBuffer;
  size_t i = 0;
  for (; i < finalMerged.size() && i < threshold; i++) {
    downBuffer.push_back(finalMerged[i]);
  }
  // Next write |D_{i+1}| - (s_i - |D_i|) - \delta to D_{i+1}
  vector<int> nextDownBuffer;
  int nextDownBufferSize = this->layers[index+1].downBuffer.size() - (threshold - downBuffer.size()) - this->delta;
  //for (; nextDownBufferSize > 0 && i < finalMerged.size() && i < downBuffer.size() + nextDownBufferSize; i++) {
  for (; i < finalMerged.size() && i < downBuffer.size() + nextDownBufferSize; i++) {
    nextDownBuffer.push_back(finalMerged[i]);
  }

  // If there are elements still left, then write them to U_{i+1}
  vector<int> nextUpBuffer;
  while (i < finalMerged.size()) {
    nextUpBuffer.push_back(finalMerged[i]);
    i++;
  }
  this->layers[index].downBuffer.clear();
  this->layers[index].downBuffer = downBuffer;
  this->layers[index+1].upBuffer.clear();
  this->layers[index+1].upBuffer = nextUpBuffer;
  this->layers[index+1].downBuffer.clear();
  this->layers[index+1].downBuffer = nextDownBuffer;

  // Call PULL primitive on D_{i+1} if required; i.e. it is smaller than s_{i+1} / 2
  if (this->layers[index+1].downBuffer.size() < this->layers[index+1].getThreshold()/2) {
    pull(index+1);
  }
  cout<<"\nBefore PUSH; Current Layer sizes "<<this->bufferThreshold;
  size_t total = 0;
  for (size_t i = 0; i < this->layers.size(); i++) {
    cout<<"\nLayer"<<i<<" Threshold "<<this->layers[i].getThreshold();
    cout<<"\nLayer"<<i<<"\n U "<<this->layers[i].upBuffer.size()<<" L "<<this->layers[i].downBuffer.size();
    total += this->layers[i].upBuffer.size() + this->layers[i].downBuffer.size();
  }
  total += this->buffer.size();
  cout<<"\n Total elements in RPQ "<<total;
  // Now investigate all the up buffers and call push primitives on up buffers if there is an overflow
  for (size_t j = index+1; j < this->layers.size(); j++) {
    if (this->layers[j].upBuffer.size() > this->layers[j].getThreshold()/2) {
      //push(index+1);
      push(j);
    }
  }
}

// Find elements in range [lo, hi) in the given vector
pair<int, int> ResilientPQ::findmin(vector<int> v1, size_t lo, size_t hi) {
  if (v1.size() == 0) return pair<int, int>(INT_MAX, -1);
  int minPos = lo;
  int minEle = v1[lo];
  for (size_t i = lo+1; i < v1.size() && i < hi; i++) {
    if (minEle > v1[i]) {
      minEle = v1[i];
      minPos = i;
    }
  }
  //cout<<"\n Size of vector "<<v1.size()<<" minEle "<<minEle<<" minPos "<<minPos;
  return pair<int, int>(minEle,minPos);
}

// PUSH primitive, accepts index of the layer on which the push function needs to be called
// Note that PUSH primitive is called on an up buffer U_i
// The buffers involved are U_i, D_i, and U_{i+1} (or D_{i+1} if i is the last layer)
void ResilientPQ::push(size_t index) {
  bool isLast = (index == this->layers.size() - 1);
  cout<<"\n Inside PUSH; index "<<index<<" isLast "<<isLast<<" Layers "<<this->layers.size();
  // Merge U_i and D_i
  vector<int> merged = merge(this->layers[index].upBuffer, this->layers[index].downBuffer);
  // Merge the vector merged and U_{i+1} (unless i is the last layer)
  vector<int> finalMerged = merged;
  if (!isLast) {
    finalMerged = merge(merged, this->layers[index+1].upBuffer);
  }

  // Now redistribute elements after merging
  // First write first |D_i| - \delta elements to D_i
  vector<int> downBuffer;
  int downBufferSize = this->layers[index].downBuffer.size() - this->delta;
  for (int i = 0; i < downBufferSize; i++) {
    downBuffer.push_back(finalMerged[i]);
  }
  // Write remaining |U_{i+1}| + |U_i| + \delta to U_{i+1} (or D_{i+1} if i is the last layer)
  vector<int> nextBuffer;
  for (size_t i = max(downBufferSize,0); i < finalMerged.size(); i++) {
    nextBuffer.push_back(finalMerged[i]);
  }

  // Replace U_i with empty up buffer
  this->layers[index].upBuffer.clear();
  this->layers[index].downBuffer.clear();
  this->layers[index].downBuffer = downBuffer;
  if (!isLast) {
    this->layers[index+1].upBuffer.clear();
    this->layers[index+1].upBuffer = nextBuffer;
  } else {
    // if i is last layer, make a new layer
    size_t newLayerIndex = this->layers.size();
    Layer newLayer(this->delta, this->n, &this->layers[newLayerIndex - 1], newLayerIndex);
    this->layers.push_back(newLayer);

    this->layers[index+1].downBuffer.clear();
    this->layers[index+1].downBuffer = nextBuffer;
  }
  //cout<<" Checking size of U_{i+1} "<<index+1<<"Size "<<this->layers[index+1].upBuffer.size();
  // if new buffers violate size invariants, invoke primitives accordingly
  cout<<"\n Before PUSH/PULL; Layer sizes";
  cout<<"\nCurrent Layer sizes "<<this->bufferThreshold;
  size_t total = 0;
  for (size_t i = 0; i < this->layers.size(); i++) {
    cout<<"\nLayer"<<i<<" Threshold "<<this->layers[i].getThreshold();
    cout<<"\nLayer"<<i<<"\n U "<<this->layers[i].upBuffer.size()<<" L "<<this->layers[i].downBuffer.size();
    total += this->layers[i].upBuffer.size() + this->layers[i].downBuffer.size();
  }
  total += this->buffer.size();
  cout<<"\n Total elements in RPQ "<<total;

  if (this->layers[index+1].upBuffer.size() > this->layers[index+1].getThreshold()/2) {
    push(index+1);
  }
  
  if (this->layers[index].downBuffer.size() < this->layers[index].getThreshold()/2) {
    cout<<"\n Calling PULL primitive";
    pull(index);
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
    i++;
  }

  while (j < v2.size()) {
    merged.push_back(v2[j]);
    j++;
  }
  return merged;
}
