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
        level.randomly_place_foood(*this);
        return true;
    }
    return false;
}

void Snake::update_snake_isAlive(){

}

void Snake::move_snake(){

}