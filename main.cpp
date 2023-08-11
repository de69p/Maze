#include <iostream>
#include <vector>
#include <stack>
#include <random>

struct Node {
    bool visited = false;
    bool walls[4] = {true, true, true, true}; // Top, Right, Bottom, Left
};

class Maze {
private:
    std::vector<std::vector<Node>> nodes;
    int rows, cols;

public:
    Maze(int rows, int cols) : rows(rows), cols(cols) {
        nodes.resize(rows, std::vector<Node>(cols));
    }

    void generateMaze();

    void drawMaze() {
        // Draw start point
        std::cout << " S ";
        for (int j = 1; j < cols; ++j) {
            std::cout << (nodes[0][j].walls[0] ? "+--" : "+  ");
        }
        std::cout << "+\n";

        // Draw rest of the maze
        for (int i = 1; i < rows; ++i) {
            // Draw top walls
            for (int j = 0; j < cols; ++j) {
                std::cout << (nodes[i][j].walls[0] ? "+--" : "+  ");
            }
            std::cout << "+\n";

            // Draw side walls
            for (int j = 0; j < cols; ++j) {
                std::cout << (nodes[i][j].walls[3] ? "|  " : "   ");
            }
            std::cout << "|\n";
        }

        // Draw bottom walls
        for (int j = 0; j < cols - 1; ++j) {
            std::cout << "+--";
        }
        std::cout << "+ E\n"; // End point
    }
};

void Maze::generateMaze() {
    std::random_device rd; // Random device engine
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<size_t> distrib(0, 3); // Uniform distribution

    std::stack<std::pair<int, int>> stack;
    int currentRow = 0, currentCol = 0;
    nodes[currentRow][currentCol].visited = true;
    stack.emplace(currentRow, currentCol);

    while (!stack.empty()) {
        int row = stack.top().first;
        int col = stack.top().second;
        stack.pop();

        std::vector<std::pair<int, int>> neighbors;

        // Check neighbors
        if (row > 0 && !nodes[row - 1][col].visited) neighbors.emplace_back(row - 1, col);
        if (col < cols - 1 && !nodes[row][col + 1].visited) neighbors.emplace_back(row, col + 1);
        if (row < rows - 1 && !nodes[row + 1][col].visited) neighbors.emplace_back(row + 1, col);
        if (col > 0 && !nodes[row][col - 1].visited) neighbors.emplace_back(row, col - 1);

        if (!neighbors.empty()) {
            stack.emplace(row, col);
            size_t randomIndex = distrib(gen) % neighbors.size(); // Use C++11 random library
            int newRow = neighbors[randomIndex].first;
            int newCol = neighbors[randomIndex].second;

            // Remove walls
            if (newRow == row - 1) {
                nodes[row][col].walls[0] = false;
                nodes[newRow][newCol].walls[2] = false;
            } else if (newCol == col + 1) {
                nodes[row][col].walls[1] = false;
                nodes[newRow][newCol].walls[3] = false;
            } else if (newRow == row + 1) {
                nodes[row][col].walls[2] = false;
                nodes[newRow][newCol].walls[0] = false;
            } else if (newCol == col - 1) {
                nodes[row][col].walls[3] = false;
                nodes[newRow][newCol].walls[1] = false;
            }

            nodes[newRow][newCol].visited = true;
            stack.emplace(newRow, newCol);
        }
    }
}


int main() {
    int rows = 6, cols = 6;
    Maze maze(rows, cols);

    char choice;
    do {
        std::cout << "1. Generate Maze\n2. Display Maze\n3. Exit\nEnter choice: ";
        std::cin >> choice;

        switch (choice) {
            case '1':
                maze.generateMaze();
                std::cout << "Maze generated!\n";
                break;
            case '2':
                maze.drawMaze();
                break;
            case '3': // Exit case
                std::cout << "Exiting program. Goodbye!\n";
                return 0; // Exit the program
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (true); // Infinite loop, will only exit when '3' is chosen

}
