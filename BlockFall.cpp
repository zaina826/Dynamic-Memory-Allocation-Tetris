#include "BlockFall.h"
#include <fstream>
#include <iostream>
#include <sstream>

//OG:
BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

std::vector<std::vector<bool>> BlockFall::turntoVector(std::string blocktext) {
    std::vector<vector<bool>> vector;
    std::vector<bool> row;
    bool num=false;
    for (char c : blocktext) {
        if (c == '0' || c == '1') {
            num=true;
            bool value = (c == '1');
            row.push_back(value);
        } else if (c == '\n') {
            if (num){
            vector.push_back(row);
            row.clear();}
        }
    }
    //Because otherwise the last row won't be added :
    if (!row.empty()) {
        vector.push_back(row);
    }
    return vector;
};

//Left rotate
vector<vector<bool>> BlockFall::transpose(vector<vector<bool>> inputVector) {
  vector<vector<bool>> rotatedMatrix(inputVector[0].size(), std::vector<bool>(inputVector.size()));
    for (int i = 0; i < inputVector.size(); ++i) {
        for (int j = 0; j < inputVector[0].size(); ++j) {
            rotatedMatrix[inputVector[0].size() - 1 - j][i] = inputVector[i][j];
        }
    }

    return rotatedMatrix;
}


void  BlockFall::setupRotations(Block* current) {
    Block* left = new Block;
    left->shape = transpose(current->shape);

    if (*current!= *left) {

        // More than one rotation
        current->left_rotation = left;
        left->right_rotation = current;

        Block* left2 = new Block;
        left2->shape = transpose(left->shape);

        if (*current != *left2) {

            // Four unique rotations
            Block* left3 = new Block;
            left3->shape = transpose(left2->shape);

            left->left_rotation = left2;
            left2->left_rotation = left3;
            left3->left_rotation = current;

            current->right_rotation = left3;
            left2->right_rotation = left;
            left3->right_rotation = left2;
        } else {
            // Two rotations
            delete left2;
            left->left_rotation = current;
            current->right_rotation = left;
        }
    } else {
        // Only one rotation
        delete left;
        current->left_rotation = current;
        current->right_rotation = current;
    }
}
//Notice there are no 3 unique rotations, there is no such thing.


void BlockFall::read_blocks(const string &input_file) {
    std::ifstream blocksdata(input_file);
    if (!blocksdata.is_open()) {
        std::cerr << "Error while opening Blocks data file...";
        return;
    }

    std::string line, stringdata;
    while (blocksdata >> line) {
        stringdata = stringdata + line + "\n";
    }

    int startint = stringdata.find('[');
    int endint = stringdata.find(']');
    std::string firstBlockString = stringdata.substr(startint + 1, endint - startint - 1);
    stringdata.erase(startint, endint - startint + 1);

    initial_block = new Block;
    initial_block->shape = turntoVector(firstBlockString);
    setupRotations(initial_block);

    Block* current = initial_block;

    // Iterate through the remaining blocks
    while ((startint = stringdata.find('[')) != std::string::npos &&
           (endint = stringdata.find(']')) != std::string::npos) {
        std::string blockString = stringdata.substr(startint + 1, endint - startint - 1);
        stringdata.erase(startint, endint - startint + 1);

        current->next_block = new Block;
        current->next_block->shape = turntoVector(blockString);
        current->right_rotation->next_block = current->next_block;
        current->right_rotation->right_rotation->next_block = current->next_block;
        current->right_rotation->right_rotation->right_rotation->next_block = current->next_block;

        setupRotations(current->next_block);
        current = current->next_block;
        
       char searchChar = '[';
int count = 0;

for (int it = 0; it != stringdata.size(); ++it) {
    if (stringdata[it] == searchChar) {
        count++;
    }
}

// Check if there is only one '[' in the string
if (count == 1) {
    // Erase '[]' characters
    size_t found = stringdata.find('[');
    if (found != std::string::npos) {
        stringdata.erase(found, 1);
    }

    found = stringdata.find(']');
    if (found != std::string::npos) {
        stringdata.erase(found, 1);
    }

    power_up = turntoVector(stringdata);
}

    }

  
    active_rotation=initial_block;
}


void BlockFall::initialize_grid(const string &input_file) {
    //First off: Opening the file:
    std::ifstream griddata(input_file);
    if(!griddata.is_open()){
            std::cerr<<"Error while opening Grid data file...";
            return;
        }
    std::string line;
    //Line by line, we insert into the vector:
    while (std::getline(griddata, line)) {
        std::istringstream iss(line);
        std::vector<int> row;
        int cell;
        while (iss >> cell) {
            row.push_back(cell);
        }
        grid.push_back(row);
    }
    rows = static_cast<int>(grid.size());
    if (rows == 0) {
        std::cerr << "Empty grid provided....." << std::endl;
        return;
    }
    cols = static_cast<int>(grid[0].size());

    
    // DONE: Initialize "rows" and "cols" member variables 
    // DONE: Initialize "grid" member variable using the command-line argument 1 in main 
}

//Destructor
BlockFall::~BlockFall() {
    while (initial_block != nullptr) {
        Block* nextBlock = initial_block->next_block;

        Block* current = initial_block;
        Block* left = current->left_rotation;
        Block* left2 = current->left_rotation->left_rotation;
        Block* left3 = current->right_rotation;

        if (current == left) {
            delete current;
        } else {
            if (current == left2) {
                delete left;
                delete current;

            } else {
                delete current;
                delete left;
                delete left2;
                delete left3;
            }
        }
        initial_block = nextBlock;
    }
}
