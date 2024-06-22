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
    void read_config(std::string path);
    // Data(const std::string& iniFile) : iniPath(iniFile) {read_ini(iniFile);}
    // void read_ini(std::string iniFile);// Getter for variablesAndValues
    // const std::unordered_map<std::string, std::string>& get_variablesAndValues() const {
    //     return variablesAndValues;
    // }
};

#endif