#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string> // std::string
#include <vector>

#include "snake.h"

class Player{
private:
    int score = 0;

public:
    int get_score()const{return score;}
    void set_score(int s){score = s;}
    Direction next_move();
    bool find_solution();
    Direction randomly_generate_direction(Snake& snake, std::vector<std::vector<char>> matrix, std::vector<std::pair<int, int>> emptyLocations);
};

#endif