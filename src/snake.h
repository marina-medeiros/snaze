#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <string> // std::string
#include <vector>
#include "player.h"

class Snake{
private:
    int m_lives;
    int m_lenght;
    std::pair<int, int> m_headLocation;
    Direction m_headFacing;

public:
    int get_lives(){return m_lives;}
    int get_lenght(){return m_lenght;}
    std::pair<int, int> get_headLocation(){return m_headLocation;}
    Direction get_headFacing(){return m_headFacing;}

    void set_lives(int l){m_lives = l;}
    void set_lenght(int l){m_lenght = l;}
};

#endif