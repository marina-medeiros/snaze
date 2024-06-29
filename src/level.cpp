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
