#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <vector>

#include "snake.h"

class Level{
private:
    int m_rows;
    int m_cols;
    char m_wall = '#';
    char m_invisibleWall = '.';
    char m_spawn = '&';
    char m_emptySpace = ' ';
    std::pair<int, int> m_foodLocation;
    std::pair<int, int> m_spawnLocation;
    std::vector<std::vector<char>> m_levelMaze;
    std::vector<std::pair<int, int>> m_emptyLocations;
    Snake snake;

public:
    int get_rows(){return m_rows;}
    int get_cols(){return m_cols;}
    char get_wall(){return m_wall;}
    char get_invisibleWall(){return m_invisibleWall;}
    char get_spawn(){return m_spawn;}
    char get_emptySpace(){return m_emptySpace;}
    std::pair<int, int> get_spawnLocation(){return m_spawnLocation;}
    std::pair<int, int> get_foodLocation(){return m_foodLocation;}
    std::vector<std::vector<char>> get_levelMaze() const{return m_levelMaze;}
    std::vector<std::pair<int, int>> get_emptyLocations(){return m_emptyLocations;}

    void set_rows(int r){m_rows = r;}
    void set_cols(int c){m_cols = c;}
    void set_levelMaze(std::vector<std::vector<char>> maze){m_levelMaze = maze;}
    void add_spawnLocation(std::pair<int, int> spawnloc){m_spawnLocation = spawnloc;}
    void add_emptyLocation(std::pair<int, int> loc){m_emptyLocations.push_back(loc);}

    void randomly_place_foood(Snake& snake);
    void update_matrix(Snake& snake);

};

#endif