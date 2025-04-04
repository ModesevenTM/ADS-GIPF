#include "checks.hpp"

bool CheckIfInside(Config& cfg, int left, int right)
{
	if (left < -1 || left > 2 * cfg.boardSize - 1 || right < 1 || right > 2 * cfg.boardSize + 1)
		return false;
	else if ((left == -1 || left == 2 * cfg.boardSize - 1) && right > cfg.boardSize + 1)
		return false;
	else if (left > -1 && left < 2 * cfg.boardSize - 1 && right >(int)cfg.board[left].size() + 2)
		return false;
	return true;
}

bool CheckStartCandidate(Config& cfg, int left, int right)
{
	return (left == -1 || left == 2 * cfg.boardSize - 1 || right == 1 || right == (int)cfg.board[left].size() + 2);
}

bool CheckIfMoveExists(const vector<string>& move, vector<vector<string>> moves)
{
	for (int i = 0; i < (int)moves.size(); i++)
	{
		if (move == moves[i]) return true;
	}
	return false;
}

int CountWrongRows(Config& cfg, vector<string> stringBoard)
{
	int wrongRows = 0;
	for (int i = 0; i < (int)stringBoard.size(); i++)
	{
		for (int j = abs(cfg.boardSize - i - 1); j < (int)stringBoard[i].size(); j += 2)
		{
			if (stringBoard[i][j] != '_'
				&& (i + 1 < (int)stringBoard.size() && j - 1 >= abs(cfg.boardSize - i - 2) && stringBoard[i + 1][j - 1] == stringBoard[i][j])
				&& (i == 0 || (i <= cfg.boardSize - 1 && j == (int)stringBoard[i].size() - 1) || (i - 1 >= 0 && j + 1 < (int)stringBoard[i - 1].size() && stringBoard[i - 1][j + 1] != stringBoard[i][j])))
			{
				int rowLength = 1, dI = i, dJ = j;
				while (dI + 1 < (int)stringBoard.size() && dJ - 1 >= abs(cfg.boardSize - i) && stringBoard[dI][dJ] == stringBoard[dI + 1][dJ - 1])
				{
					rowLength++;
					dI++;
					dJ--;
				}
				if (rowLength >= cfg.collectionTrigger) wrongRows++;
			}
			if (stringBoard[i][j] != '_'
				&& (i + 1 < (int)stringBoard.size() && j + 1 < (int)stringBoard[i + 1].size() && stringBoard[i + 1][j + 1] == stringBoard[i][j])
				&& (i == 0 || (i <= cfg.boardSize - 1 && j == abs(cfg.boardSize - i - 1)) || (i - 1 >= 0 && j - 1 >= abs(cfg.boardSize - i) && stringBoard[i - 1][j - 1] != stringBoard[i][j])))
			{
				int rowLength = 1, dI = i, dJ = j;
				while (dI + 1 < (int)stringBoard.size() && dJ + 1 < (int)stringBoard[dI + 1].size() && stringBoard[dI][dJ] == stringBoard[dI + 1][dJ + 1])
				{
					rowLength++;
					dI++;
					dJ++;
				}
				if (rowLength >= cfg.collectionTrigger) wrongRows++;
			}
			if (stringBoard[i][j] != '_'
				&& (j + 2 < (int)stringBoard[i].size() && stringBoard[i][j + 2] == stringBoard[i][j])
				&& (j == abs(cfg.boardSize - i - 1) || (j - 2 >= abs(cfg.boardSize - i - 1) && stringBoard[i][j - 2] != stringBoard[i][j])))
			{
				int rowLength = 1, dJ = j;
				while (dJ + 2 < (int)stringBoard[i].size() && stringBoard[i][dJ] == stringBoard[i][dJ + 2])
				{
					rowLength++;
					dJ += 2;
				}
				if (rowLength >= cfg.collectionTrigger) wrongRows++;
			}
		}
	}
	return wrongRows;
}