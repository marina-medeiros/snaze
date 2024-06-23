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

/*!
* Checks if a directory exists.
*
* @param target The path of the directory to check.
*
* @return True if the directory exists, false otherwise.
*/

bool file_exists(const std::string &str) {
    std::ifstream fs{str};
    return fs.is_open();
}


std::string validate_input(int argc, char* argv[]){
    if(argc!=2){
        std::cout<< "Invalid number of arguments, try again" <<std::endl;
        exit(1);
    }
    if(!file_exists(argv[1])){
        std::cout<< "File doesn't exist, try again" <<std::endl;
        exit(1);
    }
    return argv[1];
}

int main(int argc, char* argv[]) {
    Data data(validate_input(argc,argv));
    // life::Life lifeManager(data);
    
    // while (not hg.game_over()) {
    //     hg.process_events();
    //     hg.update();
    //     hg.render();
    // }
    return EXIT_SUCCESS; 
}