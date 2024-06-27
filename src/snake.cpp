#include <string> // std::string
#include <vector>
#include <deque>

#include "player.h"
#include "snake.h"

bool Snake::check_coord_in_body(std::pair<int, int> coord){
    for(auto& cell : m_body){
        if(cell.first == coord.first && cell.second == coord.second){
            return true;
        }
    }
    return false;
}

//precisam do mapa:
bool Snake::snake_ate_check(){
    
}

void Snake::update_snake_isAlive(){

}

void Snake::move_snake(){

}