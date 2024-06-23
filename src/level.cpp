#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <time.h>
#include <utility>

#include "level.h"

void Level::set_random_food_location(){
    srand(time(0));

    int numberOfPossibleLocations = static_cast<int>(m_emptyLocations.size());

    int locationIndex = rand() % numberOfPossibleLocations;

    m_foodLocation = m_emptyLocations[locationIndex];
}
