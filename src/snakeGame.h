#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <string> // std::string
#include <vector>
#include "level.h"
#include "player.h"
#include "data.h"

class Controller{
private:
    enum class game_state_e : short {
    STARTING = 0,       //!< Beginning the game.
    WELCOME,            //!< Opening messasges.
    THINKING,
    RUNNING,            //!< Playing the game.
    EATING,
    CRASHED,
    LEVELUP,
    WON,
    LOST,
    ENDING,             //!< Closing the game (final message).
  };

    /*
      enum class match_e : short {
    ON = 1,      //!< Match still open and running.
    PLAYER_WON,  //!< Match ended and player WON.
    PLAYER_LOST, //!< Match ended and player LOST.
    UNDEFINED,   //!< Undefined match status.
    };
    */ 

    //=== Data members
    game_state_e m_game_state = game_state_e::STARTING; //!< Current game state.
    bool m_match_ended = false;   //!< Flag that indicates whether the current match has ended or not.
    bool m_next = false; 

    //=== Game related members
    Player player;
    //Snake snake
    std::vector<Level> levels;
    std::string cfgFile;
    int numberOfLevels;
    int fps;
    int snakeLives;
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