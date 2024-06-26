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
    int size = 1;
    Direction headFacing;
    char headChar;
    std::deque<std::pair<int, int>> body;
    // Constructor
    Snake(int r, int c, Direction d)
        : headLocation(r, c), headFacing(d) {
        // Initial body can be empty or contain the head location
        body.push_back(headLocation);
        switch (d)
        {
        case UP: headChar = '^';
            break;
        case DOWN: headChar = 'V';
            break;
        case LEFT: headChar = '<';
            break;
        case RIGHT: headChar = '>';
            break;
        default:
            break;
        }
    }
};

void update_matrix(std::vector<std::vector<char>>& matrix, Snake& snake) {
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
            snake.headChar = '^';
        break;
    case DOWN:
            snake.headChar = 'V';
        break;
    case LEFT:
            snake.headChar = '<';
        break;
    case RIGHT:
            snake.headChar = '>';
        break;
    default:
        break;
    }

    // Update matrix with the snake's head
    matrix[snake.headLocation.first][snake.headLocation.second] = snake.headChar;
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

bool snake_die_check(Snake& snake, std::vector<std::vector<char>>& matrix) {
    int headRow = snake.headLocation.first;
    int headCol = snake.headLocation.second;
    if(matrix[headRow][headCol] == 'x'){
        snake.isAlive = false;
        return true;
    }
    update_matrix(matrix, snake);
    return false;
}

bool move_snake(Snake& snake, std::vector<std::vector<char>>& matrix) {
    std::pair<int, int> newHead = snake.headLocation;
    Direction dir = snake.headFacing;
    if(matrix[newHead.first][newHead.second] != 'x'){
        switch (dir) {
        case UP:
            if (newHead.first - 1 >= 0) {
                newHead.first--;
                snake.headChar = '^';
            } else {
                return false;
            }
            break;

        case DOWN:
            if (newHead.first + 1 < static_cast<int>(matrix.size())) {
                newHead.first++;
                snake.headChar = 'V';
            } else {
                return false;
            }
            break;
        case LEFT:
            if (newHead.second - 1 >= 0) {
                newHead.second--;
                snake.headChar = '<';
            } else {
                return false;
            }
            break;
        case RIGHT:
            if (newHead.second + 1 < static_cast<int>(matrix[0].size())) {
                newHead.second++;
                snake.headChar = '>';
            } else {
                return false;
            }
            break;
        default:
            break;
        }
    }
    
    
    snake.headLocation = newHead;
    bool ateFood = snake_eat_check(snake, matrix);
    snake.body.push_back(newHead);
    if (!ateFood) {
        snake.body.pop_front(); // Remove the tail only if not eating
    }
    if (snake_die_check(snake, matrix)) {
        update_matrix(matrix, snake);
        return false;
    }
    
    update_matrix(matrix, snake);
    return true;
}

void print_matrix(const std::vector<std::vector<char>>& matrix, int rows, int cols) {
    for(int ii = 0; ii < rows; ii++){
        for(int kk = 0; kk < 5; kk++){
            for(int jj = 0; jj < cols; jj++){
                if(matrix[ii][jj] == ' '){
                    std::cout << "     ";
                } else {
                    std::cout << matrix[ii][jj] << matrix[ii][jj] << matrix[ii][jj] << matrix[ii][jj] << matrix[ii][jj];
                }
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
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
        switch (entrada)
        {
            case 5: cascavel.headFacing = UP; break;
            case 2: cascavel.headFacing = DOWN; break;
            case 1: cascavel.headFacing = LEFT; break;
            case 3: cascavel.headFacing = RIGHT; break;
        default:
            break;
        }
        move_snake(cascavel, maze);
        clear_screen();
        print_matrix(maze, 5, 5);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\n \n \n";
        if(!cascavel.isAlive){
            std::cout << "sorry, the snake died" << std::endl;
            print_matrix(maze, 5, 5);
        }
    }

    return 0;
}
