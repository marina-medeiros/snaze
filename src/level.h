#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <string> // std::string
#include <vector>

class Level{
private:
    int m_rows;
    int m_cols;
    int m_levelNumber;
    char m_wall = '#';
    char m_invisibleWall = '.';
    char m_spawnLocation = '&';
    char m_emptySpace = ' ';
    std::vector<std::vector<char>> m_levelMaze;

public:
    int get_rows(){return m_rows;}
    int get_cols(){return m_cols;}
    int get_levelNumber(){return m_levelNumber;}
    std::vector<std::vector<char>> get_levelMaze(){return m_levelMaze;}

    void set_rows(int r){m_rows = r;}
    void set_cols(int c){m_cols = c;}
    void set_levelMaze(std::vector<std::vector<char>> maze){m_levelMaze = maze;}
    // Data(const std::string& iniFile) : iniPath(iniFile) {read_ini(iniFile);}
    // void read_ini(std::string iniFile);// Getter for variablesAndValues
    // const std::unordered_map<std::string, std::string>& get_variablesAndValues() const {
    //     return variablesAndValues;
    // }
};

#endif