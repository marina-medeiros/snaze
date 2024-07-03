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

/*!
* Checks if a file exists.
*
* @param target The path of the file to check.
*
* @return True if the directory file, false otherwise.
*/
bool file_exists(const std::string &str) {
    std::ifstream fs{str};
    return fs.is_open();
}

/**
 * @brief Reads and loads the level configuration from a file.
 * 
 * This function reads a level configuration file specified by the given path and 
 * loads the configuration of each level into a vector of Level objects.
 * 
 * @param path Path to the level configuration file.
 */
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
                else if(c == level.get_spawn()){
                    level.add_spawnLocation(std::make_pair(ii, jj));
                }
                // else if((c != level.get_invisibleWall()) && (c!= level.get_wall())){
                //     std::cerr << "Error: the puzzle you're trying to play is invalid, fix it and try again" << std::endl;
                //     return;
                // }
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

/**
 * @brief Updates the game state.
 */
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
            snake.initialize(levels[currentLevel].get_spawnLocation().first, levels[currentLevel].get_spawnLocation().second, DOWN);
            snake.set_foodEaten(0);
            levels[currentLevel].update_matrix(snake);
            levels[currentLevel].randomly_place_foood(snake);
            m_start = false; // Resetar m_start após inicialização
        }

        {
            // Bloco adicional para controlar o escopo de novas variáveis locais
            Direction newDir = player.randomly_generate_direction(snake, levels[currentLevel].get_levelMaze(), levels[currentLevel].get_emptyLocations());
            snake.set_headFacing(newDir);
            int foodEaten = snake.get_foodEaten();
            snake.move_snake(levels[currentLevel]);
            if(foodEaten < snake.get_foodEaten()){
                player.set_score(player.get_score() + 10);
            }
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
                snake.reset((levels[currentLevel]).get_spawnLocation());
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

/**
 * @brief Renders the current game state to the console.
 */
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

/**
 * @brief Changes the current game state.
 * @param new_state The new game state to switch to.
 */
void Controller::change_state(game_state_e new_state) {
  if(!(m_game_state == game_state_e::RUNNING)){
  clear_screen();
  }
  m_game_state = new_state;
}

/**
 * @brief Displays the welcome message for the game.
 * 
 * This function outputs a welcome message to the console and shows the game options
 * and the prompt to enter the game.
 */
void Controller::display_welcome() const {
    std::cout<< "--->  Welcome to the classic snake game  <---" << std::endl;
    std::cout<< "       -copyright DIMAp/UFRN 2017-2024-      " << std::endl;
    show_game_options();
    show_enter();
}

/**
 * @brief Displays the running state of the game.
 * 
 * This function shows the current level options, prints the game matrix, and 
 * pauses execution for a brief moment to create a visual effect.
 */
void Controller::display_running()const{
  show_level_options();
  print_matrix();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << std::endl << std::endl << std::endl;
}

/**
 * @brief Displays a message when the snake crashes.
 * 
 * This function outputs a crash message to the console and shows the prompt to enter the game.
 */
void Controller::display_crashed() const {
    std::cout << std::endl;
    std::cout<< "-----------------------------------------------------" << std::endl;
    std::cout<< "        OPS...LOOKS LIKE YOUR SNAKE CRASHED          "<<std::endl;
    std::cout<< "-----------------------------------------------------" << std::endl;
    show_enter();
}

/**
 * @brief Displays a message when the player levels up.
 * 
 * This function outputs a level-up message to the console and shows the prompt to enter the next level.
 */
void Controller::display_levelup() const {
    std::cout << std::endl;
    std::cout<< "----------------------------------------------------" << std::endl;
    std::cout<< "        AMAZING! YOU LEVELED UP! R U READY?        "<<std::endl;
    std::cout<< "----------------------------------------------------" << std::endl;
    show_enter();
}

/**
 * @brief Displays a message when the player loses the game.
 * 
 * This function outputs a losing message to the console and shows the prompt to hit enter.
 */
void Controller::display_lost() const {
    std::cout << std::endl;
    std::cout<< "-----------------------------------------------------" << std::endl;
    std::cout<< "        SORRY, YOU LOST (JK HAHAHAHA LOSER)          "<<std::endl;
    std::cout<< "-----------------------------------------------------" << std::endl;
    show_enter();
}

/**
 * @brief Displays a message when the player wins the game.
 * 
 * This function outputs a winning message to the console and shows the prompt to enter the game.
 */
void Controller::display_won() const {
    std::cout << std::endl;
    std::cout<< "---------------------------------------------------------" << std::endl;
    std::cout<< " CONGRATULATIONS, YOU WON! YOU ARE THE REAL SNAKE MASTER "<<std::endl;
    std::cout<< "---------------------------------------------------------" << std::endl;
    show_enter();
}

/**
 * @brief Displays the game options at the start.
 * 
 * This function outputs the number of levels, snake lives, and apples to eat, along with a message encouraging the player.
 */
void Controller::show_game_options() const {
    std::cout<< "-----------------------------------------------------" << std::endl;
    std::cout<< "Levels loaded: "<< numberOfLevels <<" | Snake lives: "<< snakeLives <<" | Apples to eat: " << totalFood <<std::endl;
    std::cout<< "Clear all levels to win the game. Good luck!!!" << std::endl;
    std::cout<< "-----------------------------------------------------" << std::endl;
}

/**
 * @brief Displays the current level options including lives, score, and food eaten.
 * 
 * This function outputs the current number of lives (represented as hearts), the player's score,
 * and the amount of food eaten by the snake out of the total food required.
 */
void Controller::show_level_options() const {
    std::string heart = "\u2665";           // ♥
    std::cout<< "-----------------------------------------------------" << std::endl;
    std::cout<< "Lives: ";
    for (int ii = 0; ii< snakeLives; ii++) {std::cout<<heart<<" ";}
    std::cout<<" | Score: " << player.get_score() << " | Food eaten " << snake.get_foodEaten() << " out of " << totalFood <<std::endl;
    std::cout<< "-----------------------------------------------------" << std::endl;
}

/**
 * @brief Displays a prompt for the user to press Enter to continue.
 * 
 * This function outputs a message prompting the user to press the Enter key to proceed.
 */
void Controller::show_enter() const {
    std::cout<< ">>> Press <ENTER> to continue" << std::endl;
}

/**
 * @brief Prints the current level's matrix to the console.
 * 
 * This function displays the current level's maze matrix, using special characters
 * to represent different elements of the game, such as the walls, snake body, and fruit.
 */
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