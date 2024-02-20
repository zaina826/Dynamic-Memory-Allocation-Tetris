#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H
#include <iostream>
#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay
    void print_grid(std::vector<std::vector<int>> grid, int score, int highscore);
    void print_block(std::vector<std::vector<bool>> grid);
    bool canPlaceBlock(const std::vector<std::vector<int>>& grid, const std::vector<std::vector<bool>>& block, int startI, int startJ) ;
    bool youwin,gameover;
    bool gravityclear=false;
    unsigned long highscore=0;
    std::vector<std::vector<int>> insert(const std::vector<std::vector<int>>& grid, const std::vector<std::vector<bool>>& block, int startI, int startJ);
    std::vector<std::vector<int>> check(const std::vector<std::vector<int>>& grid,std::vector<std::vector<bool>> powerup,unsigned long* scorePtr);

};

#endif //PA2_GAMECONTROLLER_H
