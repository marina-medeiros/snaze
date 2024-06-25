#include <cstdlib> // EXIT_SUCCESS
#include <iostream>
#include <string> 
#include <dirent.h>
#include <iostream>
#include <dirent.h>
#include <cstdlib> 
#include <string>
#include <fstream>

#include "data.h"
#include "snakeGame.h"

/*!
* Checks if a directory exists.
*
* @param target The path of the directory to check.
*
* @return True if the directory exists, false otherwise.
*/

bool file_exists1(const std::string &str) {
    std::ifstream fs{str};
    return fs.is_open();
}


std::string validate_input(int argc, char* argv[]){
    if(argc!=2){
        std::cout<< "Invalid number of arguments, try again" <<std::endl;
        exit(1);
    }
    if(!file_exists1(argv[1])){
        std::cout<< "File doesn't exist, try again" <<std::endl;
        exit(1);
    }
    return argv[1];
}

int main(int argc, char* argv[]) {
    Data data(validate_input(argc,argv));
    Controller snaze (data);
    while (!snaze.game_over) {
        snaze.process_events();
        snaze.update();
        snaze.render();
    }
    return EXIT_SUCCESS; 
}