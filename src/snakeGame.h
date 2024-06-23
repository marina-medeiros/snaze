#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <string> // std::string
#include <vector>
#include "level.h"
#include "player.h"

class Controller{
private:
    Player player;
    //Snake snake
    std::vector<Level> levels;
    int numberOfLevels;
    int totalFood;
    int foodEaten;

public:
    void read_config(std::string path);
    // Data(const std::string& iniFile) : iniPath(iniFile) {read_ini(iniFile);}
    // void read_ini(std::string iniFile);// Getter for variablesAndValues
    // const std::unordered_map<std::string, std::string>& get_variablesAndValues() const {
    //     return variablesAndValues;
    // }
};

#endif