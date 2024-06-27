#include <cstdlib> 
#include <iostream> 
#include <time.h> 
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
    std::vector<std::pair<int, int>> visitedLocations;
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

void print_matrix(const std::vector<std::vector<char>>& matrix) {
    int rows = static_cast<int>(matrix.size());
    int cols = static_cast<int>(matrix[0].size());
    std::string square = "\u25FC";          // ◼
    std::string snakeBody = "\u25CF";       // ●
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

void print_matrix_tiny(const std::vector<std::vector<char>>& matrix) {
    int rows = static_cast<int>(matrix.size());
    int cols = static_cast<int>(matrix[0].size());
    std::string square = "\u25FC";          // ◼
    std::string snakeBody = "\u25CF";       // ●
    for(int ii = 0; ii < rows; ii++){
        for(int jj = 0; jj < cols; jj++){
            if(matrix[ii][jj] == ' '){
                std::cout << " ";
            }else if(matrix[ii][jj] == 'x'){
                    std::cout << square;
            }else if(matrix[ii][jj] == 'O'){
                    std::cout << snakeBody;
            }else {
                std::cout << matrix[ii][jj];
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool check_coord_in_body(std::pair<int, int> coord, Snake& snake){
    for(auto& cell : snake.body){
        if(cell.first == coord.first && cell.second == coord.second){
            return true;
        }
    }
    return false;
}

void randomly_place_foood(std::vector<std::vector<char>>& maze, Snake& snake){
    int rows = static_cast<int>(maze.size());
    int cols = static_cast<int>(maze[0].size());
    std::vector<std::pair<int, int>> available_coords;

    for(int ii = 0; ii < rows; ii++){
        for(int jj = 0; jj < cols; jj++){
            if(maze[ii][jj] == ' ' && !check_coord_in_body(std::make_pair(ii,jj), snake)){
                available_coords.push_back(std::make_pair(ii,jj));
            }
        }
    }

    srand(time(0));

    int randomIndex = rand() % static_cast<int>(available_coords.size());

    std::pair<int, int> foodCoord = available_coords[randomIndex];

    maze[foodCoord.first][foodCoord.second] = '9';
}

bool snake_eat_check(Snake& snake, std::vector<std::vector<char>>& matrix) {
    int headRow = snake.headLocation.first;
    int headCol = snake.headLocation.second;
    if(matrix[headRow][headCol] == '9') {
        snake.size++;
        randomly_place_foood(matrix, snake);
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

Direction randomly_generate_direction(Snake& snake, std::vector<std::vector<char>>& matrix) {
    std::pair<int, int> currentHead = snake.headLocation;
    Direction dir = snake.headFacing;
    std::vector<Direction> possibleDirections;
    std::vector<Direction> backupDirections;

    auto isValidMove = [&](int x, int y) {
        return matrix[x][y] == ' ' || matrix[x][y] == '9';
    };

    auto isVisited = [&](int x, int y) {
        return std::find(snake.visitedLocations.begin(), snake.visitedLocations.end(), std::make_pair(x, y)) != snake.visitedLocations.end();
    };

    switch (dir) {
    case UP:
        if (isValidMove(currentHead.first, currentHead.second - 1)) {
            if (!isVisited(currentHead.first, currentHead.second - 1)) {
                possibleDirections.push_back(LEFT);
            } else {
                backupDirections.push_back(LEFT);
            }
        }
        if (isValidMove(currentHead.first, currentHead.second + 1)) {
            if (!isVisited(currentHead.first, currentHead.second + 1)) {
                possibleDirections.push_back(RIGHT);
            } else {
                backupDirections.push_back(RIGHT);
            }
        }
        if (isValidMove(currentHead.first - 1, currentHead.second)) {
            if (!isVisited(currentHead.first - 1, currentHead.second)) {
                possibleDirections.push_back(UP);
            } else {
                backupDirections.push_back(UP);
            }
        }
        break;
    case DOWN:
        if (isValidMove(currentHead.first, currentHead.second - 1)) {
            if (!isVisited(currentHead.first, currentHead.second - 1)) {
                possibleDirections.push_back(LEFT);
            } else {
                backupDirections.push_back(LEFT);
            }
        }
        if (isValidMove(currentHead.first, currentHead.second + 1)) {
            if (!isVisited(currentHead.first, currentHead.second + 1)) {
                possibleDirections.push_back(RIGHT);
            } else {
                backupDirections.push_back(RIGHT);
            }
        }
        if (isValidMove(currentHead.first + 1, currentHead.second)) {
            if (!isVisited(currentHead.first + 1, currentHead.second)) {
                possibleDirections.push_back(DOWN);
            } else {
                backupDirections.push_back(DOWN);
            }
        }
        break;
    case LEFT:
        if (isValidMove(currentHead.first - 1, currentHead.second)) {
            if (!isVisited(currentHead.first - 1, currentHead.second)) {
                possibleDirections.push_back(UP);
            } else {
                backupDirections.push_back(UP);
            }
        }
        if (isValidMove(currentHead.first + 1, currentHead.second)) {
            if (!isVisited(currentHead.first + 1, currentHead.second)) {
                possibleDirections.push_back(DOWN);
            } else {
                backupDirections.push_back(DOWN);
            }
        }
        if (isValidMove(currentHead.first, currentHead.second - 1)) {
            if (!isVisited(currentHead.first, currentHead.second - 1)) {
                possibleDirections.push_back(LEFT);
            } else {
                backupDirections.push_back(LEFT);
            }
        }
        break;
    case RIGHT:
        if (isValidMove(currentHead.first - 1, currentHead.second)) {
            if (!isVisited(currentHead.first - 1, currentHead.second)) {
                possibleDirections.push_back(UP);
            } else {
                backupDirections.push_back(UP);
            }
        }
        if (isValidMove(currentHead.first + 1, currentHead.second)) {
            if (!isVisited(currentHead.first + 1, currentHead.second)) {
                possibleDirections.push_back(DOWN);
            } else {
                backupDirections.push_back(DOWN);
            }
        }
        if (isValidMove(currentHead.first, currentHead.second + 1)) {
            if (!isVisited(currentHead.first, currentHead.second + 1)) {
                possibleDirections.push_back(RIGHT);
            } else {
                backupDirections.push_back(RIGHT);
            }
        }
        break;
    default:
        break;
    }

    // If there are no possible directions, use the backup directions
    if (possibleDirections.empty()) {
        possibleDirections = backupDirections;
    }

    // If there are no possible or backup directions, return the current direction
    if (possibleDirections.empty()) {
        return dir;
    }

    // Seed the random number generator
    srand(time(0));
    // Return a random direction from the possible directions
    return possibleDirections[rand() % possibleDirections.size()];
}

void move_snake(Snake& snake, std::vector<std::vector<char>>& matrix){
    std::pair<int, int> newHead = snake.headLocation;
    Direction dir = snake.headFacing;
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
        if (newHead.first + 1 < static_cast<int>(matrix.size())){
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
        if (newHead.second + 1 < static_cast<int>(matrix[0].size())) {
            newHead.second++;
        }else{
            inBounds = false;
        }
        break;
    default:
        break;
    }
    if(matrix[newHead.first][newHead.second] == 'x'){
        snake.isAlive = false;
        update_matrix(matrix, snake);
        return;
    }
    if(!inBounds){
        return;
    }
    (snake.visitedLocations).push_back(snake.headLocation);
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
    // std::vector<std::vector<char>> maze = {
    //     {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'},
    //     {'x', ' ', 'x', ' ', ' ', ' ', ' ', 'x', ' ', 'x'},
    //     {'x', ' ', 'x', ' ', 'x', ' ', ' ', 'x', ' ', 'x'},
    //     {'x', ' ', ' ', ' ', 'x', ' ', ' ', ' ', ' ', 'x'},
    //     {'x', ' ', 'x', 'x', 'x', 'x', ' ', ' ', ' ', 'x'},
    //     {'x', ' ', ' ', ' ', ' ', ' ', ' ', 'x', 'x', 'x'},
    //     {'x', ' ', 'x', ' ', ' ', ' ', 'x', 'x', ' ', 'x'},
    //     {'x', ' ', 'x', ' ', 'x', ' ', ' ', 'x', ' ', 'x'},
    //     {'x', ' ', ' ', ' ', ' ', ' ', ' ', 'x', 'x', 'x'},
    //     {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}
    // };
    std::vector<std::vector<char>> maze = {
        {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'},
        {'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x'},
        {'x', ' ', 'x', 'x', 'x', 'x', ' ', 'x', ' ', 'x'},
        {'x', ' ', 'x', ' ', ' ', ' ', ' ', 'x', ' ', 'x'},
        {'x', ' ', 'x', ' ', 'x', 'x', ' ', 'x', ' ', 'x'},
        {'x', ' ', 'x', ' ', 'x', 'x', ' ', 'x', ' ', 'x'},
        {'x', ' ', 'x', ' ', 'x', 'x', ' ', 'x', ' ', 'x'},
        {'x', ' ', 'x', ' ', 'x', 'x', 'x', 'x', ' ', 'x'},
        {'x', ' ', 'x', ' ', ' ', ' ', ' ', ' ', ' ', 'x'},
        {'x', ' ', 'x', ' ', 'x', 'x', ' ', 'x', 'x', 'x'},
        {'x', ' ', 'x', ' ', 'x', ' ', ' ', ' ', ' ', 'x'},
        {'x', ' ', 'x', ' ', ' ', ' ', ' ', 'x', ' ', 'x'},
        {'x', ' ', 'x', 'x', ' ', 'x', 'x', 'x', ' ', 'x'},
        {'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'x'},
        {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}
    };

    //int entrada;

    Snake cascavel(1, 1, DOWN);
    update_matrix(maze, cascavel);
    randomly_place_foood(maze, cascavel);
    print_matrix_tiny(maze);

    while(cascavel.isAlive){
        Direction newDir = randomly_generate_direction(cascavel, maze);
        //std::cin >> entrada;
        //update_headFacing(cascavel, entrada);
        cascavel.headFacing = newDir;
        move_snake(cascavel, maze);
        clear_screen();
        print_matrix_tiny(maze);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "\n \n \n";
        if(!cascavel.isAlive){
            std::cout << "sorry, the snake died" << std::endl;
        }
    }

    return 0;
}