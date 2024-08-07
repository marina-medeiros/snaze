#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <string> // std::string
#include <vector>

#include "level.h"
#include "player.h"
#include "snake.h"
#include "data.h"

class Controller{
private:
    enum class game_state_e : short {
    STARTING = 0,       //!< Beginning the game.
    WELCOME,            //!< Opening messasges.
    RUNNING,            //!< Playing the game.
    CRASHED,
    LEVELUP,
    WON,
    LOST,
  };
    
    //=== Data members
    game_state_e m_game_state = game_state_e::STARTING; //!< Current game state.
    bool m_next = false; 
    bool m_start = true;

    //=== Game related members
    Player player;
    Snake snake;
    std::vector<Level> levels;
    std::string cfgFile;
    int numberOfLevels;
    int currentLevel= 0;
    int fps;
    int snakeLives;
    int totalFood;

public:

    //reads the .dat file
    void read_config(std::string path);

    Controller(const Data& data) {
        // Initialize member variables using the Data object
        const auto& config = data.get_variablesAndValues();

        if (config.find("input_cfg") != config.end()) {
            cfgFile = config.at("input_cfg");
            if(cfgFile.length() >=2 && cfgFile.front() == '"'  && cfgFile.back() == '"'){
                cfgFile = cfgFile.substr(1, cfgFile.length() - 2);
            }
        }
        if (config.find("levels") != config.end()) {
            numberOfLevels = std::stoi(config.at("levels"));
        }
        if (config.find("fps") != config.end()) {
            fps = std::stoi(config.at("fps"));
        }
        if (config.find("snake_lives") != config.end()) {
            snakeLives =  std::stoi(config.at("snake_lives"));
        }
        if (config.find("food") != config.end()) {
            totalFood =  std::stoi(config.at("food"));
        }
        read_config(cfgFile);
    }

    //=== Common methods for the Game Loop design pattern.
    
    /// Process user input events, depending on the current game state.
    void process_events();
    /// Update the game based on the current game state.
    void update();
    /// Renders the game to the user.
    void render() const;

    /// true when the game is over.
    bool game_over;

private:
    /// Reads the user confirmation, enter.
    bool read_user_enter() const;

    /// Show the welcome message.
    void display_welcome() const;
    /// Show game screen
    void display_running()const;

    void display_crashed()const;
    /// Show level up message
    void display_levelup()const;
    /// Show won message
    void display_won()const;
    /// Show lost message
    void display_lost()const;
    /// Show game options
    void show_game_options()const;
    /// Show level options
    void show_level_options() const;
    /// Show press enter
    void show_enter()const;

    /// Show maze
    void print_matrix()const;

    /// Changes the state of the game.
    void change_state(game_state_e new_state);
};


#endif