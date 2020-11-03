/* Your code here! */
#include "maze.h"
#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <random>
#include <queue>
#include <stack>
#include <algorithm>

// Default Constructor
SquareMaze::SquareMaze() {}

// Function that creates the maze with a given width and height
void SquareMaze::makeMaze(int width, int height) {
  // Checks to see if width and height are valid
  if (width < 1 || height < 1)
    return;

  // Initializes variables
  int r, i = 0, j = 0;
  width_ = width;
  height_ = height;
  set_.addelements(width * height);
  for (int i = 0; i < width * height; i++) {
    distance_.push_back(0);
    wallRight_.push_back(1);
    wallDown_.push_back(1);
  }

  // Random number generator to pick the walls
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> d(0, 1);
  while (set_.size(0) != width * height) {
    if (i == width) {
      i = 0;
      j++;
    }
    else
      i++;
    if (j == height)
     j = 0;
    r = d(gen);
    if (r == 0) {
      if (((j * width) + (i + 1) < width * height) && ((i + 1) < width)) {
        if (set_.find((j * width) + i + 1) != set_.find(j * width + i)) {
          set_.setunion((j * width) + i + 1, (j * width) + i);
          wallRight_[j * width + i] = 0;
        }
      }
    }
    else {
      if (((j + 1) * width + i < width * height) && ((j + 1) < height)) {
        if (set_.find((j + 1) * width + i) != set_.find(j * width + i)) {
          set_.setunion(j * width + i, (j + 1) * width + i);
          wallDown_[j * width + i] = 0;
        }
      }
    }
  }
}

// Checks to see if there you can travel in a certain directio at point (x,y)
bool SquareMaze::canTravel(int x, int y, int dir) const {
  // Returns false if coordinates are out of bounds
  if (y * width_ + x >= width_ * height_)
    return 0;
  if (y * width_ + x < 0)
    return 0;

  // Checks to see if there is a wall at a certain point
  if ((dir == 0) && (x + 1) < width_) {
    if (wallRight_[y * width_ + x] == 0) {
      return 1;
    }
  }
  else if ((dir == 1) && (y + 1 < height_)) {
    if (wallDown_[y * width_ + x] == 0)
      return 1;
  }
  else if ((dir == 2) && (x > 0)) {
    if (wallRight_[y * width_ + x - 1] == 0)
      return 1;
  }
  else if ((dir == 3) && (y > 0)) {
    if (wallDown_[(y - 1) * width_ + x] == 0)
      return 1;
  }
  return 0;
}

// Function to set a wall to the right or bottom
void SquareMaze::setWall(int x, int y, int dir, bool exits) {
  // Checks bounds
  if (y * width_ + x >= width_ * height_)
    return;
  if (y * width_ + x < 0)
    return;

  // Sets the wall based on the given arguments
  if (dir == 1) {
    wallDown_[y * width_ + x] = exits;
  }
  else {
    wallRight_[y * width_ + x] = exits;
  }
}

// Recursive helper function to solve maze
std::vector<int> SquareMaze::solveRecursive(int x, int y) {
  // Initializes variables
  std::vector<int> v;

  // Base cases
  if (x < 0 || x >= width_ || y < 0 || y >= height_) {
    return v;
  }
  if (x == 0 && y == 0) {
    return v;
  }

  // Checks to see which point is closer to the start
  if (x != 0) {
    if ((distance_[y * width_ + x] - 1 == distance_[y * width_ + x - 1]) && canTravel(x, y, 2)) {
      v = solveRecursive(x - 1, y);
      v.push_back(0);
      return v;
    }
  }
  if (y != 0) {
    if ((distance_[y * width_ + x] - 1 == distance_[(y - 1) * width_ + x]) && canTravel(x, y, 3)) {
      v = solveRecursive(x, y - 1);
      v.push_back(1);
      return v;
    }
  }
  if (x != (width_ - 1)) {
    if ((distance_[y * width_ + x] - 1 == distance_[y * width_ + x + 1]) && canTravel(x, y, 0)) {
      v = solveRecursive(x + 1, y);
      v.push_back(2);
      return v;
    }
  }
  if (y != (height_ - 1)) {
    if ((distance_[y * width_ + x] - 1 == distance_[(y + 1) * width_ + x]) && canTravel(x, y, 1)) {
      v = solveRecursive(x, y + 1);
      v.push_back(3);
      return v;
    }
  }
  return v;
}

// Helper to find distance using a DFS
void SquareMaze::distanceHelperDFS_() {
  // Initializes variables
  int k, x, y;
  std::stack<int> s;
  s.push(0);

  // Sets the distances up
  while(!s.empty()) {
    k = s.top();
    x = k % width_;
    y = k / width_;
    s.pop();
    if (canTravel(x, y, 0)) {
      if (distance_[y * width_ + x + 1] == 0) {
        distance_[y * width_ + x + 1] = distance_[y * width_ + x] + 1;
        s.push(y * width_ + x + 1);
      }
    }
    if (canTravel(x, y, 1)) {
      if (distance_[y * width_ + x + width_] == 0) {
        distance_[y * width_ + x + width_] = distance_[y * width_ + x] + 1;
        s.push(y * width_ + x + width_);
      }
    }
    if (canTravel(x, y, 2)) {
      if (distance_[y * width_ + x - 1] == 0 && (y * width_ + x - 1) != 0) {
        distance_[y * width_ + x - 1] = distance_[y * width_ + x] + 1;
        s.push(y * width_ + x - 1);
      }
    }
    if (canTravel(x, y, 3)) {
      if (distance_[y * width_ + x - width_] == 0 && (y * width_ + x - width_) != 0) {
        distance_[y * width_ + x - width_] = distance_[y * width_ + x] + 1;
        s.push(y * width_ + x - width_);
      }
    }
  }
}

// Helper to find the new distance
void SquareMaze::distanceHelper_() {
  // Initializes variables
  int k, x, y;
  std::queue<int> q;
  q.push(0);

  // Sets the distances up
  while(!q.empty()) {
    k = q.front();
    x = k % width_;
    y = k / width_;
    q.pop();
    if (canTravel(x, y, 0)) {
      if (distance_[y * width_ + x + 1] == 0) {
        distance_[y * width_ + x + 1] = distance_[y * width_ + x] + 1;
        q.push(y * width_ + x + 1);
      }
    }
    if (canTravel(x, y, 1)) {
      if (distance_[y * width_ + x + width_] == 0) {
        distance_[y * width_ + x + width_] = distance_[y * width_ + x] + 1;
        q.push(y * width_ + x + width_);
      }
    }
    if (canTravel(x, y, 2)) {
      if (distance_[y * width_ + x - 1] == 0 && (y * width_ + x - 1) != 0) {
        distance_[y * width_ + x - 1] = distance_[y * width_ + x] + 1;
        q.push(y * width_ + x - 1);
      }
    }
    if (canTravel(x, y, 3)) {
      if (distance_[y * width_ + x - width_] == 0 && (y * width_ + x - width_) != 0) {
        distance_[y * width_ + x - width_] = distance_[y * width_ + x] + 1;
        q.push(y * width_ + x - width_);
      }
    }
  }
}
// Function to use DFS to find a solution to the maze
std::vector<int> SquareMaze::solveMaze() {
  // Initializes the variables
  int maxX = 0;
  std::vector<int> ret;
  distanceHelperDFS_();

  // Finds the point with the largest distance from the root
  for (int i = 0; i < width_; i++) {
    if (distance_[(height_ - 1) * width_ + maxX] < distance_[(height_ - 1) * width_ + i]) {
      maxX = i;
    }
  }

  // Finds the path to the origin
  int index = (height_ - 1) * width_ + maxX;
  while (distance_[index] != 0) {
    if (canTravel(index % width_, index / width_, 0)) {
      if (distance_[index] - 1 == distance_[index + 1]) {
        ret.push_back(2);
        index += 1;
      }
    }
    if (canTravel(index % width_, index / width_, 1)) {
      if (distance_[index] - 1 == distance_[index + width_]) {
        ret.push_back(3);
        index += width_;
      }
    }
    if (canTravel(index % width_, index / width_, 2)) {
      if (distance_[index] - 1 == distance_[index - 1]) {
        ret.push_back(0);
        index -= 1;
      }
    }
    if (canTravel(index % width_, index / width_, 3)) {
      if (distance_[index] - 1 == distance_[index - width_]) {
        ret.push_back(1);
        index -= width_;
      }
    }
  }

  // Returns the vector of the path
  std::reverse(ret.begin(), ret.end());
  return ret;
}

// Function that draws the maze out
cs225::PNG* SquareMaze::drawMaze() const {
  // Initializes variables
  cs225::PNG* pic = new cs225::PNG((10 * width_) + 1, (10 * height_) + 1);
  cs225::HSLAPixel wall = cs225::HSLAPixel(0, 1, 0);

  // Blackens the borders
  for (int i = 0; i < (10 * width_) + 1; i++) {
    pic->getPixel(0, i) = wall;
  }
  for (int i = 10; i < (10 * height_) + 1; i++) {
    pic->getPixel(i, 0) = wall;
  }

  // Fills in the walls for each point
  for (int i = 0; i < width_ * height_; i++) {
    if (wallRight_[i] == 1) {
      for (int j = 0; j < 11; j++)
        pic->getPixel((i % width_ + 1) * 10, i / width_ * 10 + j) = wall;
    }
    if (wallDown_[i] == 1) {
      for (int j = 0; j < 11; j++)
        pic->getPixel((i % width_) * 10 + j, (i / width_ + 1) * 10) = wall;
    }
  }

  return pic;
}

// Function for the creative maze
cs225::PNG* SquareMaze::drawCreativeMaze(SquareMaze &other) {
  // Initializes variables
  cs225::PNG* pic = new cs225::PNG((10 * width_) + 1, (20 * height_) + 1);
  cs225::HSLAPixel wall = cs225::HSLAPixel(0, 1, 0);

  // Blackens the borders
  for (int i = 0; i < (10 * width_) + 1; i++) {
    pic->getPixel(0, i) = wall;
  }
  for (int i = 10; i < (20 * height_) + 1; i++) {
    pic->getPixel(i, 0) = wall;
  }
  for (int i = (5 * width_) + 1; i < (10 * width_) + 1; i++) {
    for (int j = (10 * height_) + 1; j < (20 * height_) + 1; j++) {
      pic->getPixel(i, j) = wall;
    }
  }

  // Fills in the walls for each point
  for (int i = 0; i < width_ * height_; i++) {
    if (wallRight_[i] == 1) {
      for (int j = 0; j < 11; j++)
        pic->getPixel((i % width_ + 1) * 10, i / width_ * 10 + j) = wall;
    }
    if (wallDown_[i] == 1) {
      for (int j = 0; j < 11; j++)
        pic->getPixel((i % width_) * 10 + j, (i / width_ + 1) * 10) = wall;
    }
  }
  for (int i = 0; i < other.width_ * other.height_; i++) {
    if (other.wallRight_[i] == 1) {
      for (int j = 0; j < 11; j++)
        pic->getPixel((i % other.width_ + 1) * 10, i / other.width_ * 10 + j + height_ * 10) = wall;
    }
    if (other.wallDown_[i] == 1) {
      for (int j = 0; j < 11; j++)
        pic->getPixel((i % other.width_) * 10 + j, (i / other.width_ + 1) * 10 + height_ * 10) = wall;
    }
  }

  // Creates variables for the solution
  distanceHelper_();
  other.distanceHelper_();
  std::vector<int> sol = solveRecursive(0, height_ - 1);
  std::vector<int> solTwo = other.solveRecursive(0, height_ - 1);
  cs225::HSLAPixel red = cs225::HSLAPixel(0, 1, 0.5, 1);
  cs225::HSLAPixel white = cs225::HSLAPixel();
  int x = 5, y = 5;

  // Creates the line for the solution
  for (unsigned i = 0; i < sol.size(); i++) {
    if (sol[i] == 0) {
      for (int j = x; j < x + 11; j++)
        pic->getPixel(j, y) = red;
      x += 10;
    }
    else if (sol[i] == 1) {
      for (int j = y; j < y + 11; j++)
        pic->getPixel(x, j) = red;
      y += 10;
    }
    else if (sol[i] == 2) {
      for (int j = x; j > x - 11; j--)
        pic->getPixel(j, y) = red;
      x -= 10;
    }
    else {
      for (int j = y; j > y - 11; j--)
        pic->getPixel(x, j) = red;
      y -= 10;
    }
  }

  // Opens the wall at the last square
  x = (x - 5);
  y = y + 5;
  for (int i = 1; i < 10; i++)
    pic->getPixel(x + i, y) = white;
  for (int i = 0; i < 10; i++)
    pic->getPixel(5, height_ * 10 + i - 5) = red;
  x = 5;
  y = height_ * 10 + 5;
  for (unsigned i = 0; i < solTwo.size(); i++) {
    if (solTwo[i] == 0) {
      for (int j = x; j < x + 11; j++)
        pic->getPixel(j, y) = red;
      x += 10;
    }
    else if (solTwo[i] == 1) {
      for (int j = y; j < y + 11; j++)
        pic->getPixel(x, j) = red;
      y += 10;
    }
    else if (solTwo[i] == 2) {
      for (int j = x; j > x - 11; j--)
        pic->getPixel(j, y) = red;
      x -= 10;
    }
    else {
      for (int j = y; j > y - 11; j--)
        pic->getPixel(x, j) = red;
      y -= 10;
    }
  }
  for (int i = 1; i < 10; i++)
    pic->getPixel(i, height_ * 20) = white;

  return pic;
}

// Returns the maze with the solution
cs225::PNG* SquareMaze::drawMazeWithSolution() {
  // Creates the maze and the solution to the maze along with the necessary variables
  cs225::PNG* pic = drawMaze();
  std::vector<int> sol = solveMaze();
  cs225::HSLAPixel red = cs225::HSLAPixel(0, 1, 0.5, 1);
  cs225::HSLAPixel white = cs225::HSLAPixel();
  int x = 5, y = 5;

  // Creates the line for the solution
  for (unsigned i = 0; i < sol.size(); i++) {
    if (sol[i] == 0) {
      for (int j = x; j < x + 11; j++)
        pic->getPixel(j, y) = red;
      x += 10;
    }
    else if (sol[i] == 1) {
      for (int j = y; j < y + 11; j++)
        pic->getPixel(x, j) = red;
      y += 10;
    }
    else if (sol[i] == 2) {
      for (int j = x; j > x - 11; j--)
        pic->getPixel(j, y) = red;
      x -= 10;
    }
    else {
      for (int j = y; j > y - 11; j--)
        pic->getPixel(x, j) = red;
      y -= 10;
    }
  }

  // Opens the wall at the last square
  x = (x - 5);
  y = y + 5;
  for (int i = 1; i < 10; i++)
    pic->getPixel(x + i, y) = white;

  return pic;
}
