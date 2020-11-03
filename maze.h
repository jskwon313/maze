/* Your code here! */
#pragma once
#include <vector>
#include "cs225/PNG.h"
#include "dsets.h"

class SquareMaze {
  public:
    SquareMaze();
    void makeMaze(int width, int height);
    bool canTravel(int x, int y, int dir) const;
    void setWall(int x, int y, int dir, bool exits);
    std::vector<int> solveMaze();
    cs225::PNG* drawMaze() const;
    cs225::PNG* drawMazeWithSolution();
    cs225::PNG* drawCreativeMaze(SquareMaze &other);
  private:
    int height_;
    int width_;
    DisjointSets set_;
    std::vector<bool> wallRight_;
    std::vector<bool> wallDown_;
    std::vector<int> distance_;
    std::vector<int> solveRecursive(int x, int y);
    void distanceHelper_();
    void distanceHelperDFS_();
};
