#include <iostream>
#include <vector>
#include "dsets.h"
#include "maze.h"
#include "cs225/PNG.h"

using namespace std;
using namespace cs225;

int main()
{
    // Creates creative maze
    SquareMaze maze, mazeTwo;
    maze.makeMaze(100, 50);
    mazeTwo.makeMaze(50, 50);
  	PNG * actualOutput = maze.drawCreativeMaze(mazeTwo);
  	actualOutput->writeToFile("creative"+ string(".png"));
    return 0;
}
