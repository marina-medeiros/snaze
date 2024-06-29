#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <time.h>
#include <utility>
#include <algorithm>    // std::replace

#include "level.h"
#include "snake.h"

void Level::randomly_place_foood(Snake& snake){
    int rows = static_cast<int>(m_levelMaze.size());
    int cols = static_cast<int>(m_levelMaze[0].size());
    std::vector<std::pair<int, int>> available_coords;

    for(int ii = 0; ii < rows; ii++){
        for(int jj = 0; jj < cols; jj++){
            if(m_levelMaze[ii][jj] == ' ' && !snake.check_coord_in_body(std::make_pair(ii,jj))){
                available_coords.push_back(std::make_pair(ii,jj));
            }
        }
    }

    srand(time(0));

    int randomIndex = rand() % static_cast<int>(available_coords.size());

    std::pair<int, int> foodCoord = available_coords[randomIndex];

    m_levelMaze[foodCoord.first][foodCoord.second] = '9';
}

void Level::update_matrix( Snake& snake){
    // Clear previous snake positions
    for (auto& row : m_levelMaze) {
        std::replace(row.begin(), row.end(), 'O', ' ');
        std::replace(row.begin(), row.end(), 'V', ' ');
        std::replace(row.begin(), row.end(), '>', ' ');
        std::replace(row.begin(), row.end(), '<', ' ');
        std::replace(row.begin(), row.end(), '^', ' ');
    }
    
    // Update matrix with new snake positions
    for (const auto& coord : snake.get_body()) {
        m_levelMaze[coord.first][coord.second] = 'O';
    }

    switch (snake.get_headFacing()) {
    case Direction::UP:
            m_levelMaze[snake.get_headLocation().first][snake.get_headLocation().second] = 'V';
        break;
    case Direction::DOWN:
            m_levelMaze[snake.get_headLocation().first][snake.get_headLocation().second] = '^';
        break;
    case Direction::LEFT:
            m_levelMaze[snake.get_headLocation().first][snake.get_headLocation().second] = '>';
        break;
    case Direction::RIGHT:
            m_levelMaze[snake.get_headLocation().first][snake.get_headLocation().second] = '<';
        break;
    default:
        break;
    }
}