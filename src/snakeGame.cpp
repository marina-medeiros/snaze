#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <utility>

#include "snakeGame.h"

bool file_exists(const std::string &str) {
    std::ifstream fs{str};
    return fs.is_open();
}

void Controller::read_config(std::string path){
    path = "../" + path;
    std::ifstream inputFile(path);

    if(!file_exists(path)){
    std::cout<< ">>> Level configuration file [" << path << "] doesn't exist, try again" <<std::endl;
    exit(1);
    }

    if (!inputFile.is_open()) { 
        std::cerr << "Error opening the level intiation file! " << path << std::endl;
        return;
    }

    for(int levelIndex = 0; levelIndex < numberOfLevels; levelIndex++){
        Level level;
        std::vector<std::vector<char>> levelMatrix;

        std::string line;

        std::getline(inputFile, line);

        size_t spacePos = line.find(' ');

        level.set_rows(stoi(line.substr(0, spacePos)));
        level.set_cols(stoi(line.substr(spacePos + 1)));

        for(int ii = 0; ii < level.get_rows(); ++ii){
            std::getline(inputFile, line);
            int jj = 0;
            for(char &c : line){
                levelMatrix[ii][jj] = c;
                if(c == level.get_emptySpace()){
                    level.add_emptyLocation(std::make_pair(ii, jj));
                }
                jj++;
            }
        }

        level.set_levelMaze(levelMatrix);

        levels.push_back(level);
    }

    inputFile.close();
}