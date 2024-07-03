#include <vector>
#include <deque>

#include "snake.h"
#include "level.h"

/**
 * @brief Checks if a given coordinate is part of the snake's body.
 * 
 * @param coord The coordinate to check.
 * @return true if the coordinate is part of the snake's body, false otherwise.
 */
bool Snake::check_coord_in_body(std::pair<int, int> coord){
    for(auto& cell : m_body){
        if(cell.first == coord.first && cell.second == coord.second){
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if the snake has eaten food.
 * 
 * @param level The current level containing the maze and food.
 * @return true if the snake has eaten food, false otherwise.
 */
bool Snake::snake_ate_check(Level &level) {
    int headRow = m_headLocation.first;
    int headCol = m_headLocation.second;
    if(level.get_levelMaze()[headRow][headCol] == '9') {
        m_size++;
        m_foodEaten++;
        level.randomly_place_foood(*this);
        return true;
    }
    return false;
}

/**
 * @brief Moves the snake in the current facing direction.
 * 
 * @param level The current level containing the maze.
 */
void Snake::move_snake(Level &level){
    std::pair<int, int> newHead = m_headLocation;
    Direction dir = m_headFacing;
    bool inBounds = true;
    switch (dir) {
    case UP:
        if (newHead.first - 1 >= 0){
            newHead.first--;
        }else{
            inBounds = false;
        }
        break;
    case DOWN:
        if (newHead.first + 1 < static_cast<int>(level.get_levelMaze().size())){
            newHead.first++;
        }else{
            inBounds = false;
        }
        break;
    case LEFT:
        if (newHead.second - 1 >= 0) {
            newHead.second--;
        }else{
            inBounds = false;
        }
        break;
    case RIGHT:
        if (newHead.second + 1 < static_cast<int>(level.get_levelMaze()[0].size())) {
            newHead.second++;
        }else{
            inBounds = false;
        }
        break;
    default:
        break;
    }
    if(level.get_levelMaze()[newHead.first][newHead.second] != ' ' && level.get_levelMaze()[newHead.first][newHead.second] != '9'){
        m_isAlive = false;
        //level.update_matrix(*this);
        return;
    }
    if(!inBounds){
        return;
    }
    (m_visitedLocations).push_back(m_headLocation);
    m_headLocation = newHead;
    bool ateFood = snake_ate_check(level);
    m_body.push_back(newHead);
    if (!ateFood) {
        m_body.pop_front(); // Remove the tail only if not eating
    }
    //level.update_matrix(*this);
}

/**
 * @brief Updates the direction the snake is facing.
 * 
 * @param e The new direction to face (5 for UP, 2 for DOWN, 1 for LEFT, 3 for RIGHT).
 */
void Snake::update_headFacing(int e){
    switch (e)
    {
        case 5: m_headFacing = UP; break;
        case 2: m_headFacing = DOWN; break;
        case 1: m_headFacing = LEFT; break;
        case 3: m_headFacing = RIGHT; break;
    default:
        break;
    }
}