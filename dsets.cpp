/* Your code here! */
#include "dsets.h"
#include <vector>

// Adds num number of disjoint sets to the object
void DisjointSets::addelements(int num) {
  // Adds num number of elements
  for (int i = 0; i < num; ++i) 
    elem_.push_back(-1);
}

// Finds the root of the disjoint set at index elem
int DisjointSets::find(int elem) {
  // Initializes variable
  int ret = -1;

  // Finds the root that contains elem and condenses paths
  if (elem_[elem] < 0)
    return elem;
  else {
    ret = find(elem_[elem]);
    elem_[elem] = ret;
  }
  return ret;
}

// Unions the smaller set into the larger one
void DisjointSets::setunion(int a, int b) {
  // Initializes variables
  int one = find(a);
  int two = find(b);
  int temp = 0;

  // If the sizes are the same, then the second points to the first
  if (elem_[one] == elem_[two]) {
    temp = elem_[two];
    elem_[two] = one;
    elem_[one] += temp;
    return;
  }

  // Swaps the first and second index if the size of the first is smaller
  if (elem_[one] > elem_[two]) {
    temp = one;
    one = two;
    two = temp;
  }

  // Combines the two disjoint sets together
  temp = elem_[two];
  elem_[two] = one;
  elem_[one] += temp;
}

// Function to find the size of the disjoint set
int DisjointSets::size(int elem) {
  // Initializes variables
  int size = find(elem);

  return abs(elem_[size]);
}
