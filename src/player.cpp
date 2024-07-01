#include <cstdlib> 
#include <time.h> 
#include <algorithm>    // std::replace
#include <vector>
#include <cstdlib> // for system

#include "player.h"

Direction Player::randomly_generate_direction(Snake& snake, std::vector<std::vector<char>> matrix, std::vector<std::pair<int, int>> emptyLocations) {
    std::pair<int, int> currentHead = snake.get_headLocation();
    Direction dir = snake.get_headFacing();
    std::vector<Direction> possibleDirections;
    std::vector<Direction> backupDirections;

    auto isValidMove = [&](int x, int y) {
        return matrix[x][y] == ' ' || matrix[x][y] == '9';
    };

    if((snake.get_visitedLocations()).size() == emptyLocations.size()){
        (snake.get_visitedLocations()).clear(); 
    }

    auto isVisited = [&](int x, int y) {
        return std::find(snake.get_visitedLocations().begin(), snake.get_visitedLocations().end(), std::make_pair(x, y)) != snake.get_visitedLocations().end();
    };

    switch (dir) {
    case UP:
        if (isValidMove(currentHead.first, currentHead.second - 1)) {
            if (!isVisited(currentHead.first, currentHead.second - 1)) {
                possibleDirections.push_back(LEFT);
            } else {
                backupDirections.push_back(LEFT);
            }
        }
        if (isValidMove(currentHead.first, currentHead.second + 1)) {
            if (!isVisited(currentHead.first, currentHead.second + 1)) {
                possibleDirections.push_back(RIGHT);
            } else {
                backupDirections.push_back(RIGHT);
            }
        }
        if (isValidMove(currentHead.first - 1, currentHead.second)) {
            if (!isVisited(currentHead.first - 1, currentHead.second)) {
                possibleDirections.push_back(UP);
            } else {
                backupDirections.push_back(UP);
            }
        }
        break;
    case DOWN:
        if (isValidMove(currentHead.first, currentHead.second - 1)) {
            if (!isVisited(currentHead.first, currentHead.second - 1)) {
                possibleDirections.push_back(LEFT);
            } else {
                backupDirections.push_back(LEFT);
            }
        }
        if (isValidMove(currentHead.first, currentHead.second + 1)) {
            if (!isVisited(currentHead.first, currentHead.second + 1)) {
                possibleDirections.push_back(RIGHT);
            } else {
                backupDirections.push_back(RIGHT);
            }
        }
        if (isValidMove(currentHead.first + 1, currentHead.second)) {
            if (!isVisited(currentHead.first + 1, currentHead.second)) {
                possibleDirections.push_back(DOWN);
            } else {
                backupDirections.push_back(DOWN);
            }
        }
        break;
    case LEFT:
        if (isValidMove(currentHead.first - 1, currentHead.second)) {
            if (!isVisited(currentHead.first - 1, currentHead.second)) {
                possibleDirections.push_back(UP);
            } else {
                backupDirections.push_back(UP);
            }
        }
        if (isValidMove(currentHead.first + 1, currentHead.second)) {
            if (!isVisited(currentHead.first + 1, currentHead.second)) {
                possibleDirections.push_back(DOWN);
            } else {
                backupDirections.push_back(DOWN);
            }
        }
        if (isValidMove(currentHead.first, currentHead.second - 1)) {
            if (!isVisited(currentHead.first, currentHead.second - 1)) {
                possibleDirections.push_back(LEFT);
            } else {
                backupDirections.push_back(LEFT);
            }
        }
        break;
    case RIGHT:
        if (isValidMove(currentHead.first - 1, currentHead.second)) {
            if (!isVisited(currentHead.first - 1, currentHead.second)) {
                possibleDirections.push_back(UP);
            } else {
                backupDirections.push_back(UP);
            }
        }
        if (isValidMove(currentHead.first + 1, currentHead.second)) {
            if (!isVisited(currentHead.first + 1, currentHead.second)) {
                possibleDirections.push_back(DOWN);
            } else {
                backupDirections.push_back(DOWN);
            }
        }
        if (isValidMove(currentHead.first, currentHead.second + 1)) {
            if (!isVisited(currentHead.first, currentHead.second + 1)) {
                possibleDirections.push_back(RIGHT);
            } else {
                backupDirections.push_back(RIGHT);
            }
        }
        break;
    default:
        break;
    }

    // If there are no possible directions, use the backup directions
    if (possibleDirections.empty()) {
        possibleDirections = backupDirections;
    }

    // If there are no possible or backup directions, return the current direction
    if (possibleDirections.empty()) {
        return dir;
    }

    // Seed the random number generator
    srand(time(0));
    // Return a random direction from the possible directions
    return possibleDirections[rand() % possibleDirections.size()];
}