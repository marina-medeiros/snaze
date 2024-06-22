#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <utility>

#include "level.h"

bool file_exists(const std::string &str) {
    std::ifstream fs{str};
    return fs.is_open();
}

void Level::read_config(std::string path){
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

    std::string line;

    std::getline(inputFile, line);

    size_t spacePos = line.find(' ');

    m_rows = stoi(line.substr(0, spacePos));
    m_cols = stoi(line.substr(spacePos + 1));

    for(int ii = 0; ii < m_rows; ++ii){
        std::getline(inputFile, line);
        int jj = 0;
        for(char &c : line){
            m_levelMaze[ii][jj] = c;
            jj++;
        }
    }

    inputFile.close();
}