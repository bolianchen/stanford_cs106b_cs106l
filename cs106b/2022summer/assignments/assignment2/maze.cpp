// This file provides a maze solver and the corresponding helper functions
// the solver is based on breadth first search by gradually extending each existing path one step outward
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* Generate valid locations to move from the current location
 * @maze maze grid
 * @cur Current location in the maze
 * returns locations reachable from curr with one step
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;

    if (!maze.inBounds(cur.row,  cur.col) || !maze.get(cur.row, cur.col)) {
        return neighbors;
    }
    for (int i = -1; i <= 1; i+=2) {
        if (maze.inBounds(cur.row + i, cur.col) && maze.get(cur.row + i, cur.col)) {
            neighbors.add(GridLocation(cur.row + i, cur.col));
        }
        if (maze.inBounds(cur.row, cur.col + i) && maze.get(cur.row, cur.col + i)) {
            neighbors.add(GridLocation(cur.row, cur.col + i));
        }
    }
    return neighbors;
}

/* Check if a path is valid
 * @maze A grid of boolean values
 * @path A sequenece of locations representing a path in the maze
 * returns true if the input path is valid; otherwise, returns false
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeEntry = {0,  0};

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    Set<GridLocation> checkedLocations;
    GridLocation nextLocation = path.pop();
    GridLocation currLocation;

    while (!path.isEmpty()) {
        currLocation = path.pop();
        if (generateValidMoves(maze, currLocation).contains(nextLocation)) {
            if (!checkedLocations.contains(nextLocation)) {
                checkedLocations.add(nextLocation);
            } else {
                error("A loop in the path");
            }
        } else {
            error("No valid moves between two consecutive locations in the path");
        }
        nextLocation = currLocation;
    }

    if (currLocation != mazeEntry) {
        error("Path does not start at maze entry");
    }
}

/* Solve a maze based on breadth first search
 * @maze a grid of booleans
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);

    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeEntry = {0,  0};

    Queue<Stack<GridLocation>> candidatePaths;
    Stack<GridLocation> path;
    Stack<GridLocation> subPath;
    Set<GridLocation> visitedLocations;
    path.push(mazeEntry);
    visitedLocations.add(mazeEntry);
    candidatePaths.enqueue(path);

    while (!candidatePaths.isEmpty()) {
        path = candidatePaths.dequeue();
        MazeGraphics::highlightPath(path, "red", 33);

        if (path.peek() == mazeExit) {
            break;
        } else {
            for (GridLocation nextLocation: generateValidMoves(maze, path.peek())) {
                if (!visitedLocations.contains(nextLocation)) {
                    // path is extended to sub-paths by adding valid movements
                    subPath = path;
                    subPath.push(nextLocation);
                    visitedLocations.add(nextLocation);
                    candidatePaths.enqueue(subPath);
                }
            }
        }
    }

    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = {{0 ,0}, {1, 0}, {1, 1}};

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
STUDENT_TEST("generateValidMoves on location of 1x2 grid with walls") {
    Grid<bool> maze = {{true, true}};
    GridLocation corner = {0, 1};
    Set<GridLocation> expected = {{0, 0}};
    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

STUDENT_TEST("generateValidMoves on location in the center of 3x3 grid with no corridors") {
    Grid<bool> maze = {{false, false, false},
                       {false, true, false},
                       {false, false, false}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected;
    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("generateValidMoves on location of 1x1 grid") {
    Grid<bool> maze = {{true}};
    GridLocation center = {0, 0};
    Set<GridLocation> expected = {};
    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("validatePath on paths containing loop") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    Stack<GridLocation> valid_path1 = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}};
    Stack<GridLocation> valid_path2 = {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}};
    Stack<GridLocation> contain_a_loop_in_middle = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {0, 2}, {1, 2} ,{2, 2}};
    Stack<GridLocation> contain_a_loop_at_start = {{0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 1}, {0, 2}, {1, 2}, {0, 2}, {1, 2} ,{2, 2}};
    Stack<GridLocation> contain_a_loop_at_end = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {2, 1}, {2, 2}};

    EXPECT_NO_ERROR(validatePath(maze, valid_path1));
    EXPECT_NO_ERROR(validatePath(maze, valid_path2));
    EXPECT_ERROR(validatePath(maze, contain_a_loop_in_middle));
    EXPECT_ERROR(validatePath(maze, contain_a_loop_at_start));
    EXPECT_ERROR(validatePath(maze, contain_a_loop_at_end));
}

STUDENT_TEST("validatePath on a path with exit blocked") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, false}};
    Stack<GridLocation> path = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}};

    EXPECT_ERROR(validatePath(maze, path));
}

STUDENT_TEST("validatePath on a path with entry blocked") {
    Grid<bool> maze = {{false, true, true},
                       {true, true, true},
                       {true, true, true}};
    Stack<GridLocation> path = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}};

    EXPECT_ERROR(validatePath(maze, path));
}
