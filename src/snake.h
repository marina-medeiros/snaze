#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <vector>
#include <deque>

// Forward declaration of the Level class
class Level;

enum Direction { UP, DOWN, LEFT, RIGHT };
class Snake{
private:
    int m_foodEaten = 0;
    int m_size = 1;
    bool m_isAlive = true;
    std::pair<int, int> m_headLocation;
    std::vector<std::pair<int, int>> m_visitedLocations;
    Direction m_headFacing;
    std::deque<std::pair<int, int>> m_body;

public:
    // Constructor
    Snake()= default; // Construtor vazio

    void initialize(int r, int c, Direction d) {
        // Inicialização da Snake
        m_headLocation = std::make_pair(r, c);
        m_headFacing = d;
        m_body.push_back(m_headLocation);
        m_visitedLocations.push_back(m_headLocation);  // Add the head to visited locations
    }

    Snake(int r, int c, Direction d)
        : m_headLocation(r, c), m_headFacing(d) {
        // Initial body can be empty or contain the head location
        m_body.push_back(m_headLocation);
    }
    int get_foodEaten()const{return m_foodEaten;}
    int get_size(){return m_size;}
    bool get_isAlive(){return m_isAlive;}
    std::pair<int, int> get_headLocation(){return m_headLocation;}
    std::vector<std::pair<int, int>> get_visitedLocations() {return m_visitedLocations;}
    Direction get_headFacing(){return m_headFacing;}
    std::deque<std::pair<int, int>> get_body(){return m_body;}

    void set_foodEaten(int l){m_foodEaten = l;}
    void set_lenght(int l){m_size = l;}
    void set_headFacing(Direction d){m_headFacing = d;}
    void reset(std::pair<int, int> spawn) {
        m_body.clear();  // Clear the body
        m_headLocation = spawn;  // Set the new head location
        //m_body.push_back(m_headLocation);  // Add the head to the body
        m_size = 1;  // Reset size to 1
        m_visitedLocations.clear();  // Clear visited locations
        m_visitedLocations.push_back(m_headLocation);  // Add the head to visited locations
        m_isAlive = true;  // Reset isAlive status
    }

    bool check_coord_in_body(std::pair<int, int> coord);
    bool snake_ate_check(Level &level);
    void move_snake(Level &level);
    void update_headFacing(int e);
};

#endif