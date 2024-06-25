#include <fstream>
#include <vector>
#include <iostream>
#include <utility>
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <cstdlib> // for exit()

#include "snakeGame.h"

/**
 * @brief Clears the console screen.
 */
void clear_screen() {
    #ifdef _WIN32
        system("CLS");
    #else
        // Assume POSIX
        std::cout << "\033[2J\033[1;1H";
    #endif
}

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
/**
 * @brief Processes events based on the current game state.
 */
void Controller::process_events(){
  switch (m_game_state) {
    case game_state_e::STARTING:
      break;
    case game_state_e::WELCOME:
      m_next = read_user_enter();
      break;
    case game_state_e::RUNNING:
      m_next = read_user_enter();
      break;
  }
}

void Controller::update(){
  switch (m_game_state) {
    case game_state_e::STARTING:
      change_state(game_state_e::WELCOME);
      break;
    case game_state_e::WELCOME:
      if (m_next) {
          change_state(game_state_e::RUNNING);
      };
      break;
    case game_state_e::RUNNING:
      if (m_next) {game_over=true;}
      break;
  }
}

void Controller::render() const{
    switch (m_game_state) {
    case game_state_e::STARTING:
      break;
    case game_state_e::WELCOME:
      display_welcome();
      break;
    case game_state_e::RUNNING:
      display_running();
      break;
    }
}


/**
 * @brief Reads a confirmation enter from the user.
 * @return True if the user confirmed with enter, otherwise false.
 */
bool Controller::read_user_enter() const {
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    char c = std::cin.get();
    return c == '\n';
}

void Controller::change_state(game_state_e new_state) {
  if(!(m_game_state == game_state_e::RUNNING)){
  clear_screen();
  }
  m_game_state = new_state;
}

void Controller::display_welcome() const {
    std::cout<< "--->  Welcome to the classic snake game  <---" << std::endl;
    std::cout<< "      -copyright DIMAp/UFRN 2017-2024-       " << std::endl;
    show_game_options();
    show_enter();
}

void Controller::show_game_options() const {
    std::cout<< "-----------------------------------------------------" << std::endl;
    std::cout<< "Levels loaded: "<< numberOfLevels <<" | Snake lives: "<< snakeLives <<" | Apples to eat: " << totalFood <<std::endl;
    std::cout<< "Clear all levels to win the game. Good luck!!!" << std::endl;
    std::cout<< "-----------------------------------------------------" << std::endl;
}

void Controller::show_enter() const {
    std::cout<< ">>> Press ‹ENTER> to start the game!" << std::endl;
}