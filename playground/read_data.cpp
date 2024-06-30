#include <fstream>
#include <vector>
#include <iostream>
#include <utility>
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <cstdlib> // for exit()
#include <thread>   // for sleep_for
#include <chrono>   // for chrono::seconds

struct Level{
    int rows;
    int cols;
    std::vector<std::vector<char>> maze;
    std::vector<std::pair<int, int>> emptyLocations;
};

bool file_exists(const std::string &str) {
    std::ifstream fs{str};
    return fs.is_open();
}

void read_config(std::string path, int numberOfLevels){
    std::vector<Level> levels;
    path = "../" + path;
    std::ifstream inputFile(path);

    if(!file_exists(path)){
    std::cout<< ">>> Level configuration file [" << path << "] doesn't exist, try again" <<std::endl;
    exit(1);
    }

    if (!inputFile.is_open()) { 
        std::cerr << "Error opening the level intiation file! " << path << std::endl;
    }

    for(int levelIndex = 0; levelIndex < numberOfLevels; levelIndex++){
        Level level;
        std::vector<std::vector<char>> levelMatrix;

        std::string line;

        std::getline(inputFile, line);

        size_t spacePos = line.find(' ');

        level.rows = (stoi(line.substr(0, spacePos)));
        level.cols = (stoi(line.substr(spacePos + 1)));

        levelMatrix.resize(level.rows, std::vector<char>(level.cols));  // Initialize levelMatrix with rows and columns

        for(int ii = 0; ii < level.rows; ++ii){
            std::getline(inputFile, line);
            int jj = 0;
            for(char &c : line){
                levelMatrix[ii][jj] = c;
                if(c == ' '){
                    (level.emptyLocations).push_back(std::make_pair(ii, jj));
                }
                // if(c == '&'){
                //     level.add_spawnLocation(std::make_pair(ii, jj));
                // }
                jj++;
            }
        }

        level.maze = (levelMatrix);

        auto matrix = level.maze;

        int rows = static_cast<int>(matrix.size());
        int cols = static_cast<int>(matrix[0].size());
        std::string square = "\u25FC";          // ◼
        std::string snakeBody = "\u25CF";       // ●
        for(int ii = 0; ii < rows; ii++){
            for(int kk = 0; kk < 5; kk++){
                for(int jj = 0; jj < cols; jj++){
                    if(matrix[ii][jj] == ' '){
                        std::cout << "     ";
                    }else if(matrix[ii][jj] == 'x'){
                        std::cout << square << square << square << square << square;
                    }else if(matrix[ii][jj] == 'O'){
                        std::cout << snakeBody << snakeBody << snakeBody << snakeBody << snakeBody;
                    }else {
                        std::cout << matrix[ii][jj] << matrix[ii][jj] << matrix[ii][jj] << matrix[ii][jj] << matrix[ii][jj];
                    }
                }
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;

        levels.push_back(level);
    }

    inputFile.close();
}

int main(){
    read_config("../data/version1.dat", 2);
}