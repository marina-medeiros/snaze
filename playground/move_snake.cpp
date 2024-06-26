#include <iostream>
#include <algorithm>    // std::replace
#include <vector>
#include <deque>
#include <cstdlib> // for system
#include <thread>   // for sleep_for
#include <chrono>   // for chrono::seconds

void clear_screen() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Snake {
    bool isAlive = true;
    std::pair<int, int> headLocation;
    Direction headFacing;
    int size = 1;
    std::deque<std::pair<int, int>> body;
    // Constructor
    Snake(int r, int c, Direction d)
        : headLocation(r, c), headFacing(d) {
        // Initial body can be empty or contain the head location
        body.push_back(headLocation);
    }
};

void print_matrix(const std::vector<std::vector<char>>& matrix, int rows, int cols) {
    std::string square = "\u25FC";          // ◼
    std::string snakeBody = "\u25CF";       // ●
    std::string facing_right = "\u25B6";   // ▶
    std::string facing_left = "\u25C0";   // ◀
    std::string facing_up = "\u25B2";     // ▲
    std::string facing_down = "\u25BC";   // ▼
    for(int ii = 0; ii < rows; ii++){
        for(int kk = 0; kk < 5; kk++){
            for(int jj = 0; jj < cols; jj++){
                if(matrix[ii][jj] == ' '){
                    std::cout << "     ";
                }else if(matrix[ii][jj] == 'x'){
                     std::cout << square << square << square << square << square;
                }else if(matrix[ii][jj] == 'O'){
                     std::cout << snakeBody << snakeBody << snakeBody << snakeBody << snakeBody;
                }else {
                    std::cout << matrix[ii][jj] << matrix[ii][jj] << matrix[ii][jj] << matrix[ii][jj] << matrix[ii][jj];
                }
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

bool snake_eat_check(Snake& snake, std::vector<std::vector<char>>& matrix) {
    int headRow = snake.headLocation.first;
    int headCol = snake.headLocation.second;
    if(matrix[headRow][headCol] == '9') {
        snake.size++;
        return true;
    }
    return false;
}

void update_snake_isAlive(Snake& snake, std::vector<std::vector<char>>& matrix) {
    int headRow = snake.headLocation.first;
    int headCol = snake.headLocation.second;
    if(matrix[headRow][headCol] == 'x'){
        snake.isAlive = false;
    }
}

void update_matrix(std::vector<std::vector<char>>& matrix, Snake& snake){
    // Clear previous snake positions
    for (auto& row : matrix) {
        std::replace(row.begin(), row.end(), 'O', ' ');
        std::replace(row.begin(), row.end(), 'V', ' ');
        std::replace(row.begin(), row.end(), '>', ' ');
        std::replace(row.begin(), row.end(), '<', ' ');
        std::replace(row.begin(), row.end(), '^', ' ');
    }
    
    // Update matrix with new snake positions
    for (const auto& coord : snake.body) {
        matrix[coord.first][coord.second] = 'O';
    }

    switch (snake.headFacing) {
    case UP:
            matrix[snake.headLocation.first][snake.headLocation.second] = '^';
        break;
    case DOWN:
            matrix[snake.headLocation.first][snake.headLocation.second] = 'V';
        break;
    case LEFT:
            matrix[snake.headLocation.first][snake.headLocation.second] = '<';
        break;
    case RIGHT:
            matrix[snake.headLocation.first][snake.headLocation.second] = '>';
        break;
    default:
        break;
    }
}

void move_snake(Snake& snake, std::vector<std::vector<char>>& matrix){
    std::pair<int, int> newHead = snake.headLocation;
    Direction dir = snake.headFacing;
    switch (dir) {
    case UP:
        if (newHead.first - 1 >= 0) newHead.first--;
        break;
    case DOWN:
        if (newHead.first + 1 < static_cast<int>(matrix.size())) newHead.first++;
        break;
    case LEFT:
        if (newHead.second - 1 >= 0) newHead.second--;
        break;
    case RIGHT:
        if (newHead.second + 1 < static_cast<int>(matrix[0].size())) newHead.second++;
        break;
    default:
        break;
    }
    if(matrix[newHead.first][newHead.second] == 'x'){
        snake.isAlive = false;
        update_matrix(matrix, snake);
        return;
    }
    snake.headLocation = newHead;
    bool ateFood = snake_eat_check(snake, matrix);
    snake.body.push_back(newHead);
    if (!ateFood) {
        snake.body.pop_front(); // Remove the tail only if not eating
    }
    update_matrix(matrix, snake);
}

void update_headFacing(Snake& snake, int e){
    switch (e)
    {
        case 5: snake.headFacing = UP; break;
        case 2: snake.headFacing = DOWN; break;
        case 1: snake.headFacing = LEFT; break;
        case 3: snake.headFacing = RIGHT; break;
    default:
        break;
    }
}

int main() {
    std::vector<std::vector<char>> maze = {
        {' ', 'x', ' ', ' ', ' '},
        {' ', 'x', ' ', 'x', ' '},
        {'9', ' ', ' ', 'x', ' '},
        {' ', 'x', 'x', 'x', 'x'},
        {'9', ' ', '9', ' ', ' '}
    };

    int entrada;

    Snake cascavel(0, 0, DOWN);
    update_matrix(maze, cascavel);
    print_matrix(maze, 5, 5);

    while(cascavel.isAlive){
        std::cin >> entrada;
        update_headFacing(cascavel, entrada);
        move_snake(cascavel, maze);
        clear_screen();
        print_matrix(maze, 5, 5);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\n \n \n";
        if(!cascavel.isAlive){
            std::cout << "sorry, the snake died" << std::endl;
        }
    }

    return 0;
}