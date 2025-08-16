#include <cstdlib> 
#include <time.h> 
#include <algorithm>    // std::replace
#include <vector>
#include <cstdlib> // for system
#include <deque>

#include "player.h"

std::vector<std::pair<int, int>> find_possible_coordenates(Direction headFacing, std::pair<int,int> currentHead, std::vector<std::vector<char>> matrix, std::vector<std::pair<int, int>> emptyLocations, std::deque<Direction> &route){
    std::vector<std::pair<int, int>> possible_coordenates;

    auto isValidMove = [&](int x, int y) {
        return matrix[x][y] == ' ' || matrix[x][y] == '9'; // a move is valid if the position on the map is empty or if it is food
    };

    switch (headFacing) {
    case UP:
        if (isValidMove(currentHead.first, currentHead.second - 1)) {
            possible_coordenates.push_back(std::make_pair(currentHead.first, currentHead.second - 1));
            route.push_back(LEFT);
        }
        if (isValidMove(currentHead.first, currentHead.second + 1)) {
            possible_coordenates.push_back(std::make_pair(currentHead.first, currentHead.second + 1));
            route.push_back(RIGHT);
        }
        if (isValidMove(currentHead.first - 1, currentHead.second)) {
            possible_coordenates.push_back(std::make_pair(currentHead.first - 1, currentHead.second));
            route.push_back(UP);
        }
        break;
    case DOWN:
        if (isValidMove(currentHead.first, currentHead.second - 1)) {
            possible_coordenates.push_back(std::make_pair(currentHead.first, currentHead.second - 1));
            route.push_back(LEFT);
        }
        if (isValidMove(currentHead.first, currentHead.second + 1)) {
            possible_coordenates.push_back(std::make_pair(currentHead.first, currentHead.second + 1));
            route.push_back(RIGHT);
        }
        if (isValidMove(currentHead.first + 1, currentHead.second)) {
            possible_coordenates.push_back(std::make_pair(currentHead.first + 1, currentHead.second));
            route.push_back(DOWN);
        }
        break;
    case LEFT:
        if (isValidMove(currentHead.first - 1, currentHead.second)) {
            possible_coordenates.push_back(std::make_pair(currentHead.first - 1, currentHead.second));
            route.push_back(UP);
        }
        if (isValidMove(currentHead.first + 1, currentHead.second)) {
            possible_coordenates.push_back(std::make_pair(currentHead.first + 1, currentHead.second));
            route.push_back(DOWN);
        }
        if (isValidMove(currentHead.first, currentHead.second - 1)) {
            possible_coordenates.push_back(std::make_pair(currentHead.first, currentHead.second - 1));
            route.push_back(LEFT);
        }
        break;
    case RIGHT:
        if (isValidMove(currentHead.first - 1, currentHead.second)) {
            possible_coordenates.push_back(std::make_pair(currentHead.first - 1, currentHead.second));
            route.push_back(UP);
        }
        if (isValidMove(currentHead.first + 1, currentHead.second)) {
            possible_coordenates.push_back(std::make_pair(currentHead.first + 1, currentHead.second));
            route.push_back(DOWN);
        }
        if (isValidMove(currentHead.first, currentHead.second + 1)) {
            possible_coordenates.push_back(std::make_pair(currentHead.first, currentHead.second + 1));
            route.push_back(RIGHT);
        }
        break;
    default:
        break;
    }

    return possible_coordenates;
}

// breadth-first search
/*
    def pesqusia(nome){
        fila de pesquisa = deque()
        fila de pesquisa += grafo[vc] vizinhos
        verificadas[] --> vetor

        while(fila_de_pesquisa){
            pessoa = fila_de_pesquisa.popleft()
            if(pessoa not in verificadas){
                if (pessoa_eh_vendedor(pessoa)){
                    return true;
                }
                else{
                    fila_de_pesquisa += grafo(pessoa)
                    verificadas.append(pessoa)
                }
            }
        }
        return false
    }
        
*/

bool Player::find_solution(Snake& snake, std::vector<std::vector<char>> matrix, std::vector<std::pair<int, int>> emptyLocations){
    std::pair<int, int> currentHead = snake.get_headLocation();
    Direction dir = snake.get_headFacing();
    //std::vector<Direction> route; 
    std::vector<std::pair<int, int>> checked_locations;
    std::deque<std::pair<int, int>> search_queue = {};

    auto isVisited = [&](int x, int y) {
        return std::find(checked_locations.begin(), checked_locations.end(), std::make_pair(x, y)) != checked_locations.end();
    };

    std::vector<std::pair<int,int>> possible_coordenates = find_possible_coordenates(dir, currentHead, matrix, emptyLocations, route);

    for (std::pair<int,int> &i : possible_coordenates){
        search_queue.push_front(i);
    }

    // atualizar dir e currentHead

    while(search_queue.size() > 0){
        std::pair<int,int> position = search_queue.front();
        search_queue.pop_front();

        if(!isVisited(position.first, position.second)){
            if(matrix[position.first][position.second] == '9'){
                return true;
            }
            else{
                std::vector<std::pair<int,int>> possible_coordenates = find_possible_coordenates(route.front(), position, matrix, emptyLocations, route);

                for (std::pair<int,int> &i : possible_coordenates){
                    search_queue.push_back(i);
                }
                checked_locations.push_back(position);
            }
        }
    }  
    return false;  
}

/**
 * @brief Randomly generates a new direction for the snake to move in.
 * 
 * The function prioritizes unvisited positions. If all adjacent positions are visited, it chooses from the available visited positions.
 * 
 * @param snake The snake object whose direction is to be determined.
 * @param matrix The current level matrix containing walls, empty spaces, and food.
 * @param emptyLocations A list of all empty locations in the level.
 * @return The new direction for the snake to move in.
 */
Direction Player::randomly_generate_direction(Snake& snake, std::vector<std::vector<char>> matrix, std::vector<std::pair<int, int>> emptyLocations) {
    std::pair<int, int> currentHead = snake.get_headLocation();
    Direction dir = snake.get_headFacing();
    std::vector<Direction> possibleDirections;
    std::vector<Direction> backupDirections;

    auto isValidMove = [&](int x, int y) {
        return matrix[x][y] == ' ' || matrix[x][y] == '9'; // a move is valid if the position on the map is empty or if it is food
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