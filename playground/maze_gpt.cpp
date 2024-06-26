#include <iostream>
#include <algorithm>    // std::reverse
#include <vector>
#include <stack>
#include <utility>

using namespace std;

enum VisitStatus { NOT_VISITED, VISITED_ONCE, VISITED_TWICE };

struct Cell {
    int row;
    int col;
    VisitStatus status;
    Cell(int r, int c) : row(r), col(c), status(NOT_VISITED) {}
};

vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

bool isValid(const vector<vector<int>>& maze, int row, int col) {
    return row >= 0 && row < static_cast<int>(maze.size()) && col >= 0 && col < static_cast<int>(maze[0].size()) && maze[row][col] != 1;
}

vector<Cell> getUnvisitedNeighbors(const vector<vector<int>>& maze, const Cell& cell, vector<vector<Cell>>& visitStatus) {
    vector<Cell> neighbors;
    for (const auto& dir : directions) {
        int newRow = cell.row + dir.first;
        int newCol = cell.col + dir.second;
        if (isValid(maze, newRow, newCol) && (visitStatus[newRow][newCol].status == NOT_VISITED || visitStatus[newRow][newCol].status == VISITED_ONCE)) {
            neighbors.emplace_back(newRow, newCol);
        }
    }
    return neighbors;
}

vector<Cell> tremauxMazeSolver(const vector<vector<int>>& maze, const Cell& start, const Cell& end) {
    stack<Cell> path;
    path.push(start);
    vector<vector<Cell>> visitStatus(maze.size(), vector<Cell>(maze[0].size(), Cell(0, 0)));

    for (size_t i = 0; i < maze.size(); ++i) {
        for (size_t j = 0; j < maze[0].size(); ++j) {
            visitStatus[i][j] = Cell(i, j);
        }
    }

    visitStatus[start.row][start.col].status = VISITED_ONCE;

    while (!path.empty()) {
        Cell current = path.top();

        if (current.row == end.row && current.col == end.col) {
            vector<Cell> result;
            while (!path.empty()) {
                result.push_back(path.top());
                path.pop();
            }
            reverse(result.begin(), result.end());
            return result;
        }

        auto neighbors = getUnvisitedNeighbors(maze, current, visitStatus);

        if (!neighbors.empty()) {
            Cell next = neighbors.back();
            path.push(next);
            visitStatus[next.row][next.col].status = VISITED_ONCE;
        } else {
            visitStatus[current.row][current.col].status = VISITED_TWICE;
            path.pop();
        }
    }

    return {}; // Return empty path if no solution found
}

int main() {
    vector<vector<int>> maze = {
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    };

    Cell start(0, 0);
    Cell end(4, 4);

    vector<Cell> path = tremauxMazeSolver(maze, start, end);

    if (!path.empty()) {
        cout << "Path found:" << endl;
        for (const auto& cell : path) {
            cout << "(" << cell.row << ", " << cell.col << ")" << endl;
        }
    } else {
        cout << "No path found." << endl;
    }

    return 0;
}
