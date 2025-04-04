#include <iostream>
#include "iomanip.hpp"
#include "checks.hpp"
#include "structs.hpp"
#include "movement.hpp"
using namespace std;

int main()
{
	string command;
	Config gameCfg;
	while (cin >> command)
	{
		if (command == "LOAD_GAME_BOARD")
			LoadGameBoard(gameCfg);
		else if (command == "PRINT_GAME_BOARD")
			PrintGameBoard(gameCfg);
		else if (command == "DO_MOVE")
			DoMove(gameCfg);
		else if (command == "GEN_ALL_POS_MOV_NUM")
			PrintPossibleMoves(gameCfg);
		else if (command == "GEN_ALL_POS_MOV")
			PrintPossibleMoves(gameCfg, false);
		else if (command == "GEN_ALL_POS_MOV_EXT_NUM")
			PrintEndingMoves(gameCfg);
		else if (command == "GEN_ALL_POS_MOV_EXT")
			PrintEndingMoves(gameCfg, false);
		else if (command == "IS_GAME_OVER")
			IsGameOver(gameCfg);
		else if (command == "PRINT_GAME_STATE")
			PrintStatus(gameCfg);
	}
	return 0;
}