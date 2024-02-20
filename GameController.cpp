#include "GameController.h"
#include "BlockFall.h" 
#include <fstream>
#include <iostream>
using namespace std;
#include "Leaderboard.h"
#include <chrono>
#include <ctime>
using namespace std;
std::vector<std::vector<int>> GameController::check(const std::vector<std::vector<int>>& grid,std::vector<std::vector<bool>> powerup,unsigned long* scorePtr) {
    int rows = grid.size();
    int cols = grid[0].size();
    bool powerUpFound = false;
    for (int i = 0; i <= rows - powerup.size(); ++i) {
        for (int j = 0; j <= cols - powerup[0].size(); ++j) {
            bool found = true;
            for (int k = 0; k < powerup.size(); ++k) {
                for (int l = 0; l < powerup[0].size(); ++l) {
                    if (grid[i + k][j + l] != powerup[k][l]) {
                        found = false;
                        break;
                    }
                }
                if (!found) {
                    break;
                }
            }
            if (found) {
                powerUpFound = true;
                std::cout << "Before clearing:"<< std::endl;
                 for (const auto &row : grid) {
                    for (int value : row) {
                        std::cout << (value == 0 ? unoccupiedCellChar : occupiedCellChar) ;
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
                std::cout << std::endl;
                 int trueCount = 0;
            for (const auto& row : grid) {
                for (bool cell : row) {
                    if (cell) {
                        trueCount++;
                    }
                }
            };
                *scorePtr=*scorePtr+(1000+trueCount);
                std::vector<std::vector<int>> clearedGrid(rows, std::vector<int>(cols, 0));
                return clearedGrid;
            }
        }
    }
    std::vector<int> onesVector(cols, 1);
    std::vector<int> zerosVector(cols, 0);

    std::vector<std::vector<int>> checkedgrid = grid;
    int checker=0;
    for (int i = 0; i < rows; ++i) {
        if (grid[i] == onesVector) {
            if (!gravityclear&&checker==0){
            checker++;
             std::cout << "Before clearing:"<< std::endl;
                 for (const auto &row : grid) {
                    for (int value : row) {
                        std::cout << (value == 0 ? unoccupiedCellChar : occupiedCellChar) ;
                    }
                    std::cout << std::endl;
                }
                std::cout <<endl <<std::endl;}
                *scorePtr = *scorePtr + (cols);

            checkedgrid.erase(checkedgrid.begin() + i); // Delete the full line
            checkedgrid.insert(checkedgrid.begin(), zerosVector); 
        }
    }
    return checkedgrid;
}



//Just printing the grid :
void GameController::print_grid(std::vector<std::vector<int>> grid, int score, int highscore) {
    std::cout<<"Score: "<<score<<std::endl;
    std::cout<<"High Score: "<< highscore<<std::endl;
    for (const auto &row : grid) {
        for (int value : row) {
            std::cout << (value == 0 ? unoccupiedCellChar : occupiedCellChar);
        }
        std::cout << std::endl;
    }
}

//Printing a block:
void GameController::print_block(std::vector<std::vector<bool>> grid) {
    for (const auto &row : grid) {
        for (int value : row) {
            std::cout << (!value ? unoccupiedCellChar : occupiedCellChar);
        }
        std::cout << std::endl;
    }
}

//Checking if a block can be placed
bool GameController::canPlaceBlock(const std::vector<std::vector<int>>& grid, const std::vector<std::vector<bool>>& block, int startI, int startJ) {
    for (int i = 0; i < block.size(); ++i) {
        int newRow = startI + i;
        for (int j = 0; j < block[i].size(); ++j) {
            int newCol = startJ + j;

            // Check if the insertion position is within the grid's borders
            if (newRow >= 0 && newRow < grid.size() && newCol >= 0 && newCol < grid[0].size()) {
                // Check if the cell is occupied in the grid before insertion
                if (block[i][j] && grid[newRow][newCol] == 1) {
                    return false; 
                }
            } else {
                return false; 
            }
        }
    }
    return true; //Case where the block can be placed
}

//Inserting a certain block into a certain cell into the grid
std::vector<std::vector<int>> GameController::insert(const std::vector<std::vector<int>>& grid, const std::vector<std::vector<bool>>& block, int startI, int startJ) {
    std::vector<std::vector<int>> updatedGrid = grid;
    //Iterating over the rows and the columns of the block:
    for (int i = 0; i < block.size(); ++i) {
        int newRow = startI + i;
        for (int j = 0; j < block[i].size(); ++j) {
            int newCol = startJ + j;

            // Check if the insertion position is within the grid's borders
            if (newRow >= 0 && newRow < updatedGrid.size() && newCol >= 0 && newCol < updatedGrid[0].size()) {
                // Check if the cell is empty in the grid before insertion
                if (block[i][j] && updatedGrid[newRow][newCol] != 1) {
                    updatedGrid[newRow][newCol] = 1;
                }
            }
        }
    }
    return updatedGrid;
}

bool GameController::play(BlockFall& game, const string& commands_file){
    //We will need to initiate i and j:
    int i=0;
    int j=0;
    std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
    LeaderboardEntry* currentplayer = new LeaderboardEntry(game.current_score, std::chrono::system_clock::to_time_t(currentTime), game.player_name);
    highscore=  (game.leaderboard.head_leaderboard_entry!=nullptr?game.leaderboard.head_leaderboard_entry->score:0);
    //We will need this temporary grid: to display before dropping and finalizing:
    std::vector<std::vector<int>> tempGrid; 

    //Reading the commands
    std::ifstream commands(commands_file);
    if(!commands.is_open()){
            std::cerr<<"Error while opening commands data file...";
            return false;
    }
    std::string line;

    //Now we start iterating over the lines:
    while (commands >> line) {
        std::vector<std::vector<bool>>& curr = game.active_rotation->shape;

       if (line == "PRINT_GRID") {
            //Update curr first:
            tempGrid = insert(game.grid, curr, i, j); 
            // game.grid=check(game.grid,game.power_up);

            print_grid(tempGrid, game.current_score, highscore);
            std::cout<<std::endl;
            std::cout<<std::endl;
        }

        
        if (line == "ROTATE_RIGHT" || line == "ROTATE_LEFT") {
            int startI= i;
            int startJ=j;
            Block* tempRotatedBlock = game.active_rotation;
            Block* next = game.active_rotation->next_block;
            
            if (line == "ROTATE_RIGHT") {
            tempRotatedBlock= tempRotatedBlock->right_rotation;

            } else {
            tempRotatedBlock= tempRotatedBlock->left_rotation;}

            bool canPlace = canPlaceBlock(game.grid, tempRotatedBlock->shape, startI, startJ);

            if (canPlace) {
                game.active_rotation=tempRotatedBlock;

            }
        }
        
         
      if (line == "MOVE_RIGHT") {
    if (canPlaceBlock(game.grid, curr, i, j + 1)) {
        j += 1;
    }
}

if (line == "MOVE_LEFT") {
    if (canPlaceBlock(game.grid, curr, i, j - 1)) {
        j -= 1;
    }
}
        if (line == "DROP") {
            //First for the score we need to find the number of 1s/ trues.
            int trueCount = 0;
            for (const auto& row : curr) {
                for (bool cell : row) {
                    if (cell) {
                        trueCount++;
                    }
                }
            }
            
            //No gravity
            if (!game.gravity_mode_on) {
                //We check required rows to make sure it stops when we reach the end of the grid:
                int required_rows = game.rows - curr.size();
                // int drop= game.rows;
                //This is where the block should be inserted:
                int stop=required_rows;
                int i=0;
                //Now iterating over the rows, to find the smallest row under one of the blocks columns that has a 1.
                while (i < required_rows) {
                    for (int row = 0; row < curr.size(); row++) {
                        for (int col = 0; col < curr[0].size(); col++) {
                            if (curr[row][col] == 1 && game.grid[i + row+1][j + col] == 1) {
                                stop= (stop>=i)?i:stop;
                                // drop=min(drop,stop);
                            }
                        }
                    }
                    i++;
                }
                game.grid = insert(game.grid, curr, stop, j);
                game.current_score+=(trueCount*stop);
                highscore = max(highscore, game.current_score);
                game.grid = check(game.grid, game.power_up, &(game.current_score));

            }

            //If gravity mode is on:
             else {

                //This is the row we will insert the ones in
                int rowPos=0;
                int smallest_distance=game.rows;
                int final_drop_distance=game.rows;
                int sum_all_ones=0;
                //First thing: We need to iterate over every column, Find out how many 1s are in it 
                //And find out which column in the grid they should be placed in:
                for (int col = 0; col < curr[0].size(); col++) {
                    int biggest1_index=0;
                    int onesCount=0;
                     for (int row = 0; row < curr.size(); row++) {
                        // If the element in this column is true (i.e., one), increment the count for this column
                        if (curr[row][col]==1) {
                            biggest1_index=max(biggest1_index,row);
                            onesCount++;
                            sum_all_ones++;
                        }
                    }
                    //This is the column we will insert the ones in
                    int columnPos=j+col;
                    for (int row = curr.size(); row < game.rows; ++row) {
                         if (game.grid[row][columnPos] == 1) {
                            rowPos=row-onesCount; 
                            break;
                        }
                        if (row==game.rows-1){
                            rowPos=game.rows-onesCount; 
                            break;
                        }
                       
                    }
                    std::vector<std::vector<bool>> insertion(onesCount, std::vector<bool>(1, true));
                    game.grid =insert(game.grid,insertion,rowPos,columnPos);
                    int drop_distance=rowPos-biggest1_index+onesCount-1;
                    final_drop_distance=min(drop_distance,final_drop_distance);
                }
                game.current_score+=(final_drop_distance*sum_all_ones);
                highscore = max(highscore, game.current_score);
                                    game.grid=check(game.grid,game.power_up,&(game.current_score));


             }
                i=0;
                j=0;
                if (game.active_rotation->next_block!=nullptr){
                game.active_rotation =game.active_rotation->next_block;
                curr=game.active_rotation->shape;
                bool canPlace = canPlaceBlock(game.grid, curr, 0, 0);

                if (!canPlace) {
                    std::cout << "GAME OVER!" << std::endl;
                    std::cout<<"Next block that couldn't fit:"<<std::endl;
                    print_block(game.active_rotation->shape);
                    std::cout<<std::endl;
                    std::cout<<"Final grid and score:"<<std::endl;
                    std::cout<<std::endl;
                    print_grid(game.grid, game.current_score,highscore);
                    currentplayer->score=game.current_score;

                    game.leaderboard.insert_new_entry(currentplayer);
                    std::cout<<endl;
                    std::cout<<"Leaderboard"<<std::endl;
                    std::cout<<"-----------"<<endl;
                    game.leaderboard.print_leaderboard();
                    game.leaderboard.write_to_file(game.leaderboard_file_name);
                        // std::cout<<endl<<endl<<endl;


                    return false; // End the game
                }

                }
                else{
                    std::cout<<"YOU WIN!"<<std::endl<<"No more blocks."<<std::endl<<"Final grid and score:"<<std::endl;
                    std::cout<<endl;
                    print_grid(game.grid, game.current_score,highscore);
                        currentplayer->score=game.current_score;

                    game.leaderboard.insert_new_entry(currentplayer);
                    std::cout<<endl;
    std::cout<<"Leaderboard"<<std::endl;
    std::cout<<"-----------"<<endl;
    game.leaderboard.print_leaderboard();
    game.leaderboard.write_to_file(game.leaderboard_file_name);
        // std::cout<<endl<<endl<<endl;


                    youwin=true;
                    
                    return true; // End the game
                }
        
        
        };
        if (line=="GRAVITY_SWITCH"){
            game.gravity_mode_on=!(game.gravity_mode_on);
            std::vector<std::vector<int>> gravityGrid = game.grid;
            if (game.gravity_mode_on) {
                std::vector<std::vector<int>> tempGrid = game.grid;

    for (int col = 0; col < game.grid[0].size(); col++) {
        int onesCount = 0;
        std::vector<int> columnValues;

        // Find the number of ones:
        for (int row = 0; row < game.grid.size(); row++) {
            columnValues.push_back(game.grid[row][col]);
            if (game.grid[row][col] == 1) {
                onesCount++;
            }
        }

       
        for (int row = 0; row < game.grid.size(); row++) {
            tempGrid[row][col] = 0;
        }

        // Fill the ones where they should go
        for (int i = game.grid.size() - onesCount; i < game.grid.size(); ++i) {
            tempGrid[i][col] = 1;
        }
    }

    // Update the original game grid with the modified temp grid
    game.grid = tempGrid;
    gravityclear=true;
    game.grid=check(game.grid, game.power_up,&(game.current_score));
    gravityclear=false;
            }

        };
        
    }
    std::cout<<"GAME FINISHED!"<<std::endl<<"No more commands."<<std::endl<<"Final grid and score:"<<std::endl;
    std::cout<<endl;
    print_grid(game.grid,game.current_score,highscore);
    currentplayer->score=game.current_score;
    game.leaderboard.insert_new_entry(currentplayer);
    std::cout<<endl;
    std::cout<<"Leaderboard"<<std::endl;
    std::cout<<"-----------"<<endl;
    game.leaderboard.print_leaderboard();
    game.leaderboard.write_to_file(game.leaderboard_file_name);

    return true;
}
