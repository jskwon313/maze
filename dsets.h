/* Your code here! */
#include <vector>
#pragma once

class DisjointSets {
  public:
    void addelements(int num);
    int find(int elem);
    void setunion(int a, int b);
    int size(int elem);
  private:
    std::vector<int> elem_;
};
