#include <fstream>
#include <vector>
#include <iostream>
#include <utility>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // for exit()
#include <thread>   // for sleep_for
#include <chrono>   // for chrono::seconds

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

        //std::getline(inputFile, line);

        if (!std::getline(inputFile, line)) {
            std::cerr << "Error reading line from file!" << std::endl;
            return;
        }
        size_t spacePos = line.find(' ');

        if (spacePos == std::string::npos) {
            std::cerr << "Error parsing line: " << line << std::endl;
            return;
        }

        level.set_rows(stoi(line.substr(0, spacePos)));
        level.set_cols(stoi(line.substr(spacePos + 1)));

        levelMatrix.resize(level.get_rows(), std::vector<char>(level.get_cols()));  // Initialize levelMatrix with rows and columns

        for(int ii = 0; ii < level.get_rows(); ++ii){
            if (!std::getline(inputFile, line)) {
                std::cerr << "Error reading line from file!" << std::endl;
                return;
            }
            int jj = 0;
            for(char &c : line){
                if (jj >= level.get_cols()) {
                    std::cerr << "Error: line length exceeds expected number of columns" << std::endl;
                    return;
                }
                levelMatrix[ii][jj] = c;
                if(c == level.get_emptySpace()){
                    level.add_emptyLocation(std::make_pair(ii, jj));
                }
                if(c == level.get_spawn()){
                    level.add_spawnLocation(std::make_pair(ii, jj));
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
      break;
    case game_state_e::CRASHED:
      m_next = read_user_enter();
      break;
    case game_state_e::LEVELUP:
      m_next = read_user_enter();
      break;
    case game_state_e::LOST:
      m_next = read_user_enter();
      break;
    case game_state_e::WON:
      m_next = read_user_enter();
      break;
  }
}

void Controller::update() {
switch (m_game_state) {
    case game_state_e::STARTING:
        change_state(game_state_e::WELCOME);
        break;

    case game_state_e::WELCOME:
        if (m_next) {
            change_state(game_state_e::RUNNING);
            m_next = false; // Resetar m_next após transição de estado
        }
        break;

    case game_state_e::RUNNING:
        if (m_start) {
            //initializeSnake(levels[currentLevel].get_spawnLocation().first, levels[currentLevel].get_spawnLocation().second, DOWN); //SEGMENTATION FAULT
            snake.initialize(levels[currentLevel].get_spawnLocation().first, levels[currentLevel].get_spawnLocation().second, DOWN);
            levels[currentLevel].update_matrix(snake);
            levels[currentLevel].randomly_place_foood(snake);
            m_start = false; // Resetar m_start após inicialização
        }

        {
            // Bloco adicional para controlar o escopo de novas variáveis locais
            Direction newDir = player.randomly_generate_direction(snake, levels[currentLevel].get_levelMaze(), levels[currentLevel].get_emptyLocations());
            snake.set_headFacing(newDir);
            snake.move_snake(levels[currentLevel]);
            (levels[currentLevel]).update_matrix(snake);
            clear_screen();
            player.set_score(player.get_score() + 1);

            if (!snake.get_isAlive()) {
                snakeLives--;
              if((snakeLives != 0)){
                change_state(game_state_e::CRASHED);
              }else{
                change_state(game_state_e::LOST);
              }
            }
            if(snake.get_foodEaten() == totalFood){
              if(currentLevel == (static_cast<int>(levels.size())-1)){
                change_state(game_state_e::WON);
              }else{
                change_state(game_state_e::LEVELUP);
              }
            }

        }
        break;

    case game_state_e::CRASHED:
        if (m_next) {
            snake.reset(levels[currentLevel].get_spawnLocation());
            change_state(game_state_e::RUNNING);
            m_next = false; // Resetar m_next após transição de estado
        }
        break;

    case game_state_e::LEVELUP:
        if (m_next) {
            currentLevel++;    
            snake.reset(levels[currentLevel].get_spawnLocation());
            m_start = true;
            m_next = false;
            change_state(game_state_e::RUNNING);
        }
        break;

    case game_state_e::LOST:
        if (m_next) {
            game_over = true;
        }
        break;

    case game_state_e::WON:
        if (m_next) {
            game_over = true;
        }
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
      if(!m_start){
        display_running();
        int frame_duration_ms = 1000 / fps;
        std::this_thread::sleep_for(std::chrono::milliseconds(frame_duration_ms));
      }
      break;
    case game_state_e::CRASHED:
        display_crashed();
      break;
    case game_state_e::LEVELUP:
        display_levelup();
      break;
    case game_state_e::LOST:
        if(!game_over){display_lost();}
      break;
    case game_state_e::WON:
        if(!game_over){display_won();}
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
    std::cout<< "       -copyright DIMAp/UFRN 2017-2024-      " << std::endl;
    show_game_options();
    show_enter();
}

void Controller::display_running()const{
  show_level_options();
  print_matrix();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << std::endl << std::endl << std::endl;
}

void Controller::display_crashed() const {
    std::cout << std::endl;
    std::cout<< "-----------------------------------------------------" << std::endl;
    std::cout<< "        OPS...LOOKS LIKE YOUR SNAKE CRASHED          "<<std::endl;
    std::cout<< "-----------------------------------------------------" << std::endl;
    show_enter();
}
void Controller::display_levelup() const {
    std::cout << std::endl;
    std::cout<< "----------------------------------------------------" << std::endl;
    std::cout<< "        AMAZING! YOU LEVELED UP! R U READY?        "<<std::endl;
    std::cout<< "----------------------------------------------------" << std::endl;
    show_enter();
}

void Controller::display_lost() const {
    std::cout << std::endl;
    std::cout<< "-----------------------------------------------------" << std::endl;
    std::cout<< "        SORRY, YOU LOST (JK HAHAHAHA LOSER)          "<<std::endl;
    std::cout<< "-----------------------------------------------------" << std::endl;
    show_enter();
}

void Controller::display_won() const {
    std::cout << std::endl;
    std::cout<< "---------------------------------------------------------" << std::endl;
    std::cout<< " CONGRATULATIONS, YOU WON! YOU ARE THE REAL SNAKE MASTER "<<std::endl;
    std::cout<< "---------------------------------------------------------" << std::endl;
    show_enter();
}

void Controller::show_game_options() const {
    std::cout<< "-----------------------------------------------------" << std::endl;
    std::cout<< "Levels loaded: "<< numberOfLevels <<" | Snake lives: "<< snakeLives <<" | Apples to eat: " << totalFood <<std::endl;
    std::cout<< "Clear all levels to win the game. Good luck!!!" << std::endl;
    std::cout<< "-----------------------------------------------------" << std::endl;
}

void Controller::show_level_options() const {
    std::string heart = "\u2665";           // ♥
    std::cout<< "-----------------------------------------------------" << std::endl;
    std::cout<< "Lives: ";
    for (int ii = 0; ii< snakeLives; ii++) {std::cout<<heart<<" ";}
    std::cout<<" | Score: " << player.get_score() << " | Food eaten " << snake.get_foodEaten() << " out of " << totalFood <<std::endl;
    std::cout<< "-----------------------------------------------------" << std::endl;
}

void Controller::show_enter() const {
    std::cout<< ">>> Press <ENTER> to continue" << std::endl;
}

void Controller::print_matrix()const {
    int rows = static_cast<int>(levels[currentLevel].get_levelMaze().size());
    int cols = static_cast<int>(levels[currentLevel].get_levelMaze()[0].size());
    std::string square = "\u25AA";          // ◼
    std::string snakeBody = "\u2022";       // ●
    std::string fruit = "\u2736"; // ✶
    for(int ii = 0; ii < rows; ii++){
        for(int jj = 0; jj < cols; jj++){
            if(levels[currentLevel].get_levelMaze()[ii][jj] == ' '){
                std::cout << " ";
            }else if(levels[currentLevel].get_levelMaze()[ii][jj] == '#'){
                    std::cout << square;
            }else if(levels[currentLevel].get_levelMaze()[ii][jj] == 'O'){
                    std::cout << snakeBody;
            }else if(levels[currentLevel].get_levelMaze()[ii][jj] == '9'){
                    std::cout << fruit;
            }else {
                std::cout << levels[currentLevel].get_levelMaze()[ii][jj];
            }
        }
        std::cout << std::endl;
    }
}