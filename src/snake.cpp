#include <string> // std::string
#include <vector>
#include <deque>

#include "snake.h"
#include "level.h"

bool Snake::check_coord_in_body(std::pair<int, int> coord){
    for(auto& cell : m_body){
        if(cell.first == coord.first && cell.second == coord.second){
            return true;
        }
    }
    return false;
}

//precisam do mapa:
bool Snake::snake_ate_check(Level &level) {
    int headRow = m_headLocation.first;
    int headCol = m_headLocation.second;
    if(level.get_levelMaze()[headRow][headCol] == '9') {
        m_size++;
        m_foodEaten++;
        level.randomly_place_foood(*this);
        return true;
    }
    return false;
}

void Snake::update_snake_isAlive(std::vector<std::vector<char>>& matrix) {
    int headRow = m_headLocation.first;
    int headCol = m_headLocation.second;
    if(matrix[headRow][headCol] == '#'){
        m_isAlive = false;
    }
}

void Snake::move_snake(Level &level){
    std::pair<int, int> newHead = m_headLocation;
    Direction dir = m_headFacing;
    bool inBounds = true;
    switch (dir) {
    case UP:
        if (newHead.first - 1 >= 0){
            newHead.first--;
        }else{
            inBounds = false;
        }
        break;
    case DOWN:
        if (newHead.first + 1 < static_cast<int>(level.get_levelMaze().size())){
            newHead.first++;
        }else{
            inBounds = false;
        }
        break;
    case LEFT:
        if (newHead.second - 1 >= 0) {
            newHead.second--;
        }else{
            inBounds = false;
        }
        break;
    case RIGHT:
        if (newHead.second + 1 < static_cast<int>(level.get_levelMaze()[0].size())) {
            newHead.second++;
        }else{
            inBounds = false;
        }
        break;
    default:
        break;
    }
    if(level.get_levelMaze()[newHead.first][newHead.second] == '#'){
        m_isAlive = false;
        //level.update_matrix(*this);
        return;
    }
    if(!inBounds){
        return;
    }
    (m_visitedLocations).push_back(m_headLocation);
    m_headLocation = newHead;
    bool ateFood = snake_ate_check(level);
    m_body.push_back(newHead);
    if (!ateFood) {
        m_body.pop_front(); // Remove the tail only if not eating
    }
    //level.update_matrix(*this);
}

void Snake::update_headFacing(int e){
    switch (e)
    {
        case 5: m_headFacing = UP; break;
        case 2: m_headFacing = DOWN; break;
        case 1: m_headFacing = LEFT; break;
        case 3: m_headFacing = RIGHT; break;
    default:
        break;
    }
}

void Snake::initialize(int r, int c, Direction d) {
    // Inicialização da Snake
    m_headLocation = std::make_pair(r, c);
    m_headFacing = d;
    m_body.push_back(m_headLocation);
}