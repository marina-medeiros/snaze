#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string> // std::string
#include <vector>

enum class Direction { Up, Down, Left, Right, None };
class Player{
private:
    int score = 0;

public:
    int get_score(){return score;}
    void set_score(int s){score = s;}
    Direction next_move();
    bool find_solution();
};

#endif