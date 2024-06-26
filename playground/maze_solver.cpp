#include <iostream>
#include <vector>

enum VisitStatus { NOT_VISITED, VISITED_ONCE, VISITED_TWICE };

bool is_intersection(std::vector<std::vector<int>>& matrix, int row, int col){
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
    int openPaths = 0;
    std::vector<bool> pathExists(4, false);

    for(int ii = 0; ii < 4; ++ii){
        int newRow = row + directions[ii].first;
        int newCol = col + directions[ii].second;

        if (newRow >= 0 && newRow < matrix.size() && newCol >= 0 && newCol < matrix[0].size()){
            if(matrix[newRow][newCol] != 9){
                openPaths++;
                pathExists[ii] = true;
            }
        }
    }

    if (openPaths == 1) {
        return false;
    } else if (openPaths == 3) {
        return true;
    } else if (openPaths == 2) {
        return (pathExists[0] && pathExists[2]) || (pathExists[1] && pathExists[3]);
    }

    return false;
}


int main(){

}