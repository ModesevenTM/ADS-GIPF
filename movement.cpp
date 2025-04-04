#include "movement.hpp"

void DoMove(Config& cfg)
{
	string moveInput, command, startPawn = "", destPawn = "";
	Players whosePawns = none;
	vector<string> commandVector;
	getline(cin, moveInput);
	stringstream moveStrm(moveInput);
	while (moveStrm >> command)
		commandVector.push_back(command);
	char moveCoords[16];
	strcpy(moveCoords, commandVector[0].c_str());
	if ((int)commandVector.size() > 1)
	{
		if (commandVector[1] == "w:")
			whosePawns = white;
		else if (commandVector[1] == "b:")
			whosePawns = black;
		else
			whosePawns = none;
		startPawn = commandVector[2];
		destPawn = commandVector[3];
	}
	char* startCoord = strtok(moveCoords, "-");
	char* destCoord = strtok(NULL, "-");


	int startNum = atoi(&startCoord[1]);

	if (!CheckIfInside(cfg, startCoord[0] - 'b', startNum))
	{
		cout << "BAD_MOVE_" << startCoord << "_IS_WRONG_INDEX\n";
		LoadError(cfg, commandVector[0]);
		return;
	}
	else if (!CheckStartCandidate(cfg, startCoord[0] - 'b', startNum))
	{
		cout << "BAD_MOVE_" << startCoord << "_IS_WRONG_STARTING_FIELD\n";
		LoadError(cfg, commandVector[0]);
		return;
	}


	int destNum = atoi(&destCoord[1]);
	if (!CheckIfInside(cfg, destCoord[0] - 'b', destNum))
	{
		cout << "BAD_MOVE_" << destCoord << "_IS_WRONG_INDEX\n";
		LoadError(cfg, commandVector[0]);
		return;
	}
	else if (CheckStartCandidate(cfg, destCoord[0] - 'b', destNum))
	{
		cout << "BAD_MOVE_" << destCoord << "_IS_WRONG_DESTINATION_FIELD\n";
		LoadError(cfg, commandVector[0]);
		return;
	}
	else if (abs(destCoord[0] - startCoord[0]) == 1)
	{
		if (destNum - startNum > 1)
		{
			cout << "UNKNOWN_MOVE_DIRECTION\n";
			LoadError(cfg, commandVector[0]);
			return;
		}
	}
	else if (destCoord[0] - startCoord[0] == 0)
	{
		if (abs(destNum - startNum) != 1)
		{
			cout << "UNKNOWN_MOVE_DIRECTION\n";
			LoadError(cfg, commandVector[0]);
			return;
		}
	}
	else
	{
		cout << "UNKNOWN_MOVE_DIRECTION\n";
		LoadError(cfg, commandVector[0]);
		return;
	}

	string state = "";
	Config boardCopy(cfg);
	TryMovement(boardCopy, destCoord[0] - 'b', destNum - 2, startCoord[0] - 'b', startNum - 2, state);

	
	if (state == "")
		AcceptMovement(cfg, boardCopy, startPawn, destPawn, whosePawns, state);
	cout << state;
	if (state == "MOVE_COMMITTED\n")
		cfg.state = GameState(cfg);
	else
		LoadError(cfg, commandVector[0]);
}

void TryMovement(Config& cfg, int destLeft, int destRight, int startLeft, int startRight, string& state)
{
	string gameState = GameState(cfg);
	if (cfg.board[destLeft][destRight] == none)
	{
		ApplyMovement(cfg, destLeft, destRight);
	}
	else
	{
		if (destLeft - startLeft == 0)
		{
			if (destRight - startRight == -1)
			{
				int i = destRight;
				while (i >= 0 && cfg.board[destLeft][i] != none) i--;
				if (i < 0) state = "BAD_MOVE_ROW_IS_FULL\n";
				else
				{
					if (gameState == "white_win" || gameState == "black_win" || gameState == "dead_lock white" || gameState == "dead_lock black")
						return;
					for (int j = i; j < destRight; j++)
						swap(cfg.board[destLeft][j], cfg.board[destLeft][j + 1]);
					ApplyMovement(cfg, destLeft, destRight);
				}
			}
			else if (destRight - startRight == 1)
			{
				int i = destRight;
				while (i < (int)cfg.board[destLeft].size() && cfg.board[destLeft][i] != none) i++;
				if (i == (int)cfg.board[destLeft].size()) state = "BAD_MOVE_ROW_IS_FULL\n";
				else
				{
					if (gameState == "white_win" || gameState == "black_win" || gameState == "dead_lock white" || gameState == "dead_lock black")
						return;
					for (int j = i; j > destRight; j--)
						swap(cfg.board[destLeft][j], cfg.board[destLeft][j - 1]);
					ApplyMovement(cfg, destLeft, destRight);
				}
			}
		}
		else if (destLeft - startLeft == 1)
		{
			if (destRight - startRight == 0)
			{
				int i = destLeft, j = destRight;
				while (i < (int)cfg.board.size() && j >= 0 && cfg.board[i][j] != none)
				{
					if (i >= cfg.boardSize - 1) j--;
					i++;
				}
				if (i == (int)cfg.board.size() || j < 0) state = "BAD_MOVE_ROW_IS_FULL\n";
				else
				{
					if (gameState == "white_win" || gameState == "black_win" || gameState == "dead_lock white" || gameState == "dead_lock black")
						return;
					for (int k = i; k > destLeft; k--)
					{
						int offset = 0;
						if (k <= cfg.boardSize - 1) offset++;
						swap(cfg.board[k][j], cfg.board[k - 1][j + 1 - offset]);
						if (k > cfg.boardSize - 1) j++;
					}
					ApplyMovement(cfg, destLeft, destRight);
				}
			}
			else if (destRight - startRight == 1)
			{
				int i = destLeft, j = destRight;
				while (i < (int)cfg.board.size() && j < (int)cfg.board[i].size() && cfg.board[i][j] != none)
				{
					if (i < cfg.boardSize - 1) j++;
					i++;
				}
				if (i == (int)cfg.board.size() || j == (int)cfg.board[i].size()) state = "BAD_MOVE_ROW_IS_FULL\n";
				else
				{
					if (gameState == "white_win" || gameState == "black_win" || gameState == "dead_lock white" || gameState == "dead_lock black")
						return;
					for (int k = i; k > destLeft; k--)
					{
						int offset = 0;
						if (k > cfg.boardSize - 1) offset++;
						swap(cfg.board[k][j], cfg.board[k - 1][j - 1 + offset]);
						if (k <= cfg.boardSize - 1) j--;
					}
					ApplyMovement(cfg, destLeft, destRight);
				}
			}
		}
		else if (destLeft - startLeft == -1)
		{
			if (destRight - startRight == 1)
			{
				int i = destLeft, j = destRight;
				while (i >= 0 && j < (int)cfg.board[i].size() && cfg.board[i][j] != none)
				{
					if (i > cfg.boardSize - 1) j++;
					i--;
				}
				if(i < 0 || j == (int)cfg.board[i].size()) state = "BAD_MOVE_ROW_IS_FULL\n";
				else
				{
					if (gameState == "white_win" || gameState == "black_win" || gameState == "dead_lock white" || gameState == "dead_lock black")
						return;
					for (int k = i; k < destLeft; k++)
					{
						int offset = 0;
						if (k < cfg.boardSize - 1) offset++;
						swap(cfg.board[k][j], cfg.board[k + 1][j - 1 + offset]);
						if (k >= cfg.boardSize - 1) j--;
					}
					ApplyMovement(cfg, destLeft, destRight);
				}
			}
			else if (destRight - startRight == 0)
			{
				int i = destLeft, j = destRight;
				while (i >= 0 && j >= 0 && cfg.board[i][j] != none)
				{
					if (i <= cfg.boardSize - 1) j--;
					i--;
				}
				if (i < 0 || j < 0) state = "BAD_MOVE_ROW_IS_FULL\n";
				else
				{
					for (int k = i; k < destLeft; k++)
					{
						if (gameState == "white_win" || gameState == "black_win" || gameState == "dead_lock white" || gameState == "dead_lock black")
							return;
						int offset = 0;
						if (k >= cfg.boardSize - 1) offset++;
						swap(cfg.board[k][j], cfg.board[k + 1][j + 1 - offset]);
						if (k < cfg.boardSize - 1) j++;
					}
					ApplyMovement(cfg, destLeft, destRight);
				}
			}
		}
	}
}

void RemovePawns(Config& cfg, vector<string> stringBoard, vector<RemovalResult>& possibilities)
{
	vector<int> firstPawnInLine;
	for (int i = 0; i < (int)stringBoard.size(); i++)
	{
		if (i >= cfg.boardSize)
			firstPawnInLine.push_back(firstPawnInLine[i - 1] + 1);
		else
			firstPawnInLine.push_back(0);
	}

	vector<vector<int>> rightValues;
	for (int i = 0; i < (int)stringBoard.size(); i++)
	{
		rightValues.push_back(vector<int>());
		for (int j = 0; j < (int)cfg.board[i].size(); j++)
		{
			if (j <= cfg.boardSize - 1 - firstPawnInLine[i])
				rightValues[i].push_back(cfg.boardSize - 1 - i + j + firstPawnInLine[i]);
			else
				rightValues[i].push_back(rightValues[i][j - 1]);
		}
	}

	for (int i = 0; i < (int)stringBoard.size(); i++)
	{
		for (int j = abs(cfg.boardSize - i - 1); j < (int)stringBoard[i].size(); j += 2)
		{
			if (stringBoard[i][j] != '_'
				&& (i + 1 < (int)stringBoard.size() && j - 1 >= abs(cfg.boardSize - i - 2) && stringBoard[i + 1][j - 1] != '_')
				&& (i == 0 || (i <= cfg.boardSize - 1 && j == (int)stringBoard[i].size() - 1) || (i - 1 >= 0 && j + 1 < (int)stringBoard[i - 1].size() && stringBoard[i - 1][j + 1] == '_')))
			{
				int rowLength = 1, maxRL = 1, dI = i, dJ = j;
				Players whichHasMore;
				if (stringBoard[i][j] == 'W')
					whichHasMore = white;
				else
					whichHasMore = black;
				vector<pair<int, int>> coordsToDelete;
				pair<int, int> start;
				coordsToDelete.push_back({ firstPawnInLine[i] + ((j - abs(cfg.boardSize - i - 1)) / 2), rightValues[i][(j - abs(cfg.boardSize - i - 1)) / 2] });
				start = { firstPawnInLine[i] + ((j - abs(cfg.boardSize - i - 1)) / 2), rightValues[i][(j - abs(cfg.boardSize - i - 1)) / 2] };
				pair<int, int> end = start, nowStart = start;
				while (dI + 1 < (int)stringBoard.size() && dJ - 1 >= abs(cfg.boardSize - dI - 2) && stringBoard[dI + 1][dJ - 1] != '_')
				{
					coordsToDelete.push_back({ firstPawnInLine[dI + 1] + ((dJ - 1 - abs(cfg.boardSize - dI - 2)) / 2), rightValues[dI + 1][(dJ - 1 - abs(cfg.boardSize - dI - 2)) / 2] });
					if (stringBoard[dI][dJ] == stringBoard[dI + 1][dJ - 1])
					{
						rowLength++;
						if (rowLength > maxRL)
						{
							start = nowStart;
							end = { firstPawnInLine[dI + 1] + ((dJ - 1 - abs(cfg.boardSize - dI - 2)) / 2), rightValues[dI + 1][(dJ - 1 - abs(cfg.boardSize - dI - 2)) / 2] };
							maxRL++;
							if (stringBoard[dI + 1][dJ - 1] == 'W')
								whichHasMore = white;
							else
								whichHasMore = black;
						}
					}
					else
					{
						nowStart = { firstPawnInLine[dI + 1] + ((dJ - 1 - abs(cfg.boardSize - dI - 2)) / 2), rightValues[dI + 1][(dJ - 1 - abs(cfg.boardSize - dI - 2)) / 2] };
						rowLength = 1;
					}
					dI++;
					dJ--;
				}
				if (maxRL >= cfg.collectionTrigger)
					possibilities.push_back({ vector<pair<int, int>>(coordsToDelete), whichHasMore, start, end});
			}
			if (stringBoard[i][j] != '_'
				&& (i + 1 < (int)stringBoard.size() && j + 1 < (int)stringBoard[i + 1].size() && stringBoard[i + 1][j + 1] != '_')
				&& (i == 0 || (i <= cfg.boardSize - 1 && j == abs(cfg.boardSize - i - 1)) || (i - 1 >= 0 && j - 1 >= abs(cfg.boardSize - i) && stringBoard[i - 1][j - 1] == '_')))
			{
				int rowLength = 1, maxRL = 1, dI = i, dJ = j;
				Players whichHasMore;
				if (stringBoard[i][j] == 'W')
					whichHasMore = white;
				else
					whichHasMore = black;
				vector<pair<int, int>> coordsToDelete;
				pair<int, int> start;
				coordsToDelete.push_back({ firstPawnInLine[i] + ((j - abs(cfg.boardSize - i - 1)) / 2), rightValues[i][(j - abs(cfg.boardSize - i - 1)) / 2] });
				start = { firstPawnInLine[i] + ((j - abs(cfg.boardSize - i - 1)) / 2), rightValues[i][(j - abs(cfg.boardSize - i - 1)) / 2] };
				pair<int, int> end = start, nowStart = start;
				while (dI + 1 < (int)stringBoard.size() && dJ + 1 < (int)stringBoard[dI + 1].size() && stringBoard[dI + 1][dJ + 1] != '_')
				{
					coordsToDelete.push_back({ firstPawnInLine[dI + 1] + ((dJ + 1 - abs(cfg.boardSize - dI - 2)) / 2), rightValues[dI + 1][(dJ + 1 - abs(cfg.boardSize - dI - 2)) / 2] });
					if (stringBoard[dI][dJ] == stringBoard[dI + 1][dJ + 1])
					{
						rowLength++;
						if (rowLength > maxRL)
						{
							start = nowStart;
							end = { firstPawnInLine[dI + 1] + ((dJ + 1 - abs(cfg.boardSize - dI - 2)) / 2), rightValues[dI + 1][(dJ + 1 - abs(cfg.boardSize - dI - 2)) / 2] };
							maxRL++;
							if (stringBoard[dI + 1][dJ + 1] == 'W')
								whichHasMore = white;
							else
								whichHasMore = black;
						}
					}
					else
					{
						nowStart = { firstPawnInLine[dI + 1] + ((dJ + 1 - abs(cfg.boardSize - dI - 2)) / 2), rightValues[dI + 1][(dJ + 1 - abs(cfg.boardSize - dI - 2)) / 2] };
						rowLength = 1;
					}
					dI++;
					dJ++;
				}
				if (maxRL >= cfg.collectionTrigger)
					possibilities.push_back({ vector<pair<int, int>>(coordsToDelete), whichHasMore, start, end });
			}
			if (stringBoard[i][j] != '_'
				&& (j + 2 < (int)stringBoard[i].size() && stringBoard[i][j + 2] != '_')
				&& (j == abs(cfg.boardSize - i - 1) || (j - 2 >= abs(cfg.boardSize - i - 1) && stringBoard[i][j - 2] == '_')))
			{
				int rowLength = 1, maxRL = 1, dJ = j;
				Players whichHasMore;
				if (stringBoard[i][j] == 'W')
					whichHasMore = white;
				else
					whichHasMore = black;
				vector<pair<int, int>> coordsToDelete;
				pair<int, int> start;
				coordsToDelete.push_back({ firstPawnInLine[i] + ((j - abs(cfg.boardSize - i - 1)) / 2), rightValues[i][(j - abs(cfg.boardSize - i - 1)) / 2] });
				start = { firstPawnInLine[i] + ((j - abs(cfg.boardSize - i - 1)) / 2), rightValues[i][(j - abs(cfg.boardSize - i - 1)) / 2] };
				pair<int, int> end = start, nowStart = start;
				while (dJ + 2 < (int)stringBoard[i].size() && stringBoard[i][dJ + 2] != '_')
				{
					coordsToDelete.push_back({ firstPawnInLine[i] + ((dJ + 2 - abs(cfg.boardSize - i - 1)) / 2), rightValues[i][(dJ + 2 - abs(cfg.boardSize - i - 1)) / 2] });
					if (stringBoard[i][dJ] == stringBoard[i][dJ + 2])
					{
						rowLength++;
						if (rowLength > maxRL)
						{
							start = nowStart;
							end = { firstPawnInLine[i] + ((dJ + 2 - abs(cfg.boardSize - i - 1)) / 2), rightValues[i][(dJ + 2 - abs(cfg.boardSize - i - 1)) / 2] };
							maxRL++;
							if (stringBoard[i][dJ + 2] == 'W')
								whichHasMore = white;
							else
								whichHasMore = black;
						}
					}
					else
					{
						nowStart = { firstPawnInLine[i] + ((dJ + 2 - abs(cfg.boardSize - i - 1)) / 2), rightValues[i][(dJ + 2 - abs(cfg.boardSize - i - 1)) / 2] };
						rowLength = 1;
					}
					dJ+=2;
				}
				if (maxRL >= cfg.collectionTrigger)
					possibilities.push_back({ vector<pair<int, int>>(coordsToDelete), whichHasMore, start, end });
			}
		}
	}
}

void PrintPossibleMoves(Config& cfg, bool countOnly)
{
	vector<vector<string>> moves;
	for (int i = -1; i <= (int)cfg.board.size(); i++)
	{
		if (i == -1)
		{
			for (int j = -1; j < cfg.boardSize; j++)
			{
				GenerateMove(cfg, i + 1, j, i, j, moves);
				GenerateMove(cfg, i + 1, j + 1, i, j, moves);
			}
		}
		else if (i == (int)cfg.board.size())
		{
			for (int j = -1; j < cfg.boardSize; j++)
			{
				GenerateMove(cfg, i - 1, j, i, j, moves);
				GenerateMove(cfg, i - 1, j + 1, i, j, moves);
			}
		}
		else
		{
			GenerateMove(cfg, i, 0, i, -1, moves);

			if (i < cfg.boardSize - 1)
				GenerateMove(cfg, i + 1, 0, i, -1, moves);
			else if (i >= cfg.boardSize)
				GenerateMove(cfg, i - 1, 0, i, -1, moves);

			GenerateMove(cfg, i, 2 * cfg.boardSize - 2 - abs(cfg.boardSize - i - 1), i, 2 * cfg.boardSize - 1 - abs(cfg.boardSize - i - 1), moves);

			if (i < cfg.boardSize - 1)
				GenerateMove(cfg, i + 1, 2 * cfg.boardSize - 1 - abs(cfg.boardSize - i - 1), i, 2 * cfg.boardSize - 1 - abs(cfg.boardSize - i - 1), moves);
			else if (i >= cfg.boardSize)
				GenerateMove(cfg, i - 1, 2 * cfg.boardSize - 1 - abs(cfg.boardSize - i - 1), i, 2 * cfg.boardSize - 1 - abs(cfg.boardSize - i - 1), moves);
		}
	}

	if (countOnly)
		cout << (int)moves.size() << "_UNIQUE_MOVES\n";
	else
	{
		for (int i = 0; i < (int)moves.size(); i++)
		{
			for (int j = 0; j < (int)moves[i].size(); j++)
				cout << moves[i][j] << endl;
			cout << endl;
		}
	}
}

void FillAmbiguousPossibilities(Config& boardCopy, struct MoveContainer& cont)
{
	for (int i = 0; i < (int)boardCopy.board.size(); i++)
	{
		cont.occurences.push_back(vector<vector<RemovalResult>>());
		for (int j = 0; j < (int)boardCopy.board[i].size(); j++)
			cont.occurences[i].push_back(vector<RemovalResult>());
	}
	for (int i = 0; i < (int)cont.possibilities.size(); i++)
	{
		for (int j = 0; j < (int)cont.possibilities[i].coordsToDelete.size(); j++)
		{
			cont.occurences[cont.possibilities[i].coordsToDelete[j].first][cont.possibilities[i].coordsToDelete[j].second].push_back(cont.possibilities[i]);
			if ((int)cont.occurences[cont.possibilities[i].coordsToDelete[j].first][cont.possibilities[i].coordsToDelete[j].second].size() == 2)
				cont.ambiguous.push_back(cont.possibilities[i].coordsToDelete[j]);
		}
	}
}

void RemovePawnsFromAmbiguousSituation(Config& boardCopy, struct MoveContainer& cont, Players whosePawns, pair<int, int> startPawnIndex, pair<int, int> destPawnIndex, string& state)
{
	int anyAmbiguousMoves = false;
	for (int i = 0; i < (int)cont.possibilities.size(); i++)
	{
		bool isAmbiguous = false;
		for (int j = 0; j < (int)cont.occurences[cont.ambiguous[0].first][cont.ambiguous[0].second].size(); j++)
		{
			if (cont.possibilities[i].coordsToDelete == cont.occurences[cont.ambiguous[0].first][cont.ambiguous[0].second][j].coordsToDelete)
			{
				isAmbiguous = true;
				if ((cont.possibilities[i].start == startPawnIndex || cont.possibilities[i].start == destPawnIndex) &&
					(cont.possibilities[i].end == startPawnIndex || cont.possibilities[i].end == destPawnIndex))
				{
					if (cont.possibilities[i].whichHasMore == whosePawns)
					{
						isAmbiguous = false;
						anyAmbiguousMoves = true;
					}
					else
					{
						state = "WRONG_COLOR_OF_CHOSEN_ROW\n";
						return;
					}
				}
			}
		}
		if (!isAmbiguous)
		{
			for (int j = 0; j < (int)cont.possibilities[i].coordsToDelete.size(); j++)
			{
				if (boardCopy.board[cont.possibilities[i].coordsToDelete[j].first][cont.possibilities[i].coordsToDelete[j].second] == cont.possibilities[i].whichHasMore)
					boardCopy.reserved[cont.possibilities[i].whichHasMore]++;
				boardCopy.board[cont.possibilities[i].coordsToDelete[j].first][cont.possibilities[i].coordsToDelete[j].second] = none;
			}
		}
	}
	if (!anyAmbiguousMoves)
	{
		state = "WRONG_INDEX_OF_CHOSEN_ROW\n";
	}
}

void AcceptMovement(Config& cfg, Config& boardCopy, string startPawn, string destPawn, Players whosePawns, string& state)
{
	vector<string> stringBoard;

	MoveContainer cont;
	GenerateStringFromBoardData(stringBoard, boardCopy);
	RemovePawns(boardCopy, stringBoard, cont.possibilities);

	FillAmbiguousPossibilities(boardCopy, cont);

	if (!((int)cont.ambiguous.size()))
	{
		for (int i = 0; i < (int)cont.possibilities.size(); i++)
		{
			for (int j = 0; j < (int)cont.possibilities[i].coordsToDelete.size(); j++)
			{
				if (boardCopy.board[cont.possibilities[i].coordsToDelete[j].first][cont.possibilities[i].coordsToDelete[j].second] == cont.possibilities[i].whichHasMore)
					boardCopy.reserved[cont.possibilities[i].whichHasMore]++;
				boardCopy.board[cont.possibilities[i].coordsToDelete[j].first][cont.possibilities[i].coordsToDelete[j].second] = none;
			}
		}
	}
	else if ((int)cont.ambiguous.size() > 1)
	{
		return;
	}
	else
	{
		if (startPawn == "" || destPawn == "")
			return;
		else
		{
			string state2 = "";
			pair <int, int> startPawnIndex = { startPawn[0] - 'b', atoi(&startPawn[1]) - 2 };
			pair <int, int> destPawnIndex = { destPawn[0] - 'b', atoi(&destPawn[1]) - 2 };
			RemovePawnsFromAmbiguousSituation(boardCopy, cont, whosePawns, startPawnIndex, destPawnIndex, state2);
			if (state2 != "")
			{
				cout << state2;
				return;
			}
		}
	}
	cfg = boardCopy;
	state = "MOVE_COMMITTED\n";
}

bool GenerateMove(Config& cfg, int destLeft, int destRight, int startLeft, int startRight, vector<vector<string>>& moves, bool countOnly, bool ending)
{
	Config cfgCopy(cfg);
	string state = "";
	if (CheckIfInside(cfgCopy, destLeft, destRight + 2) && !CheckStartCandidate(cfgCopy, destLeft, destRight + 2))
	{
		TryMovement(cfgCopy, destLeft, destRight, startLeft, startRight, state);
		MoveContainer cont;
		vector<string> move, brd;
		GenerateStringFromBoardData(move, cfgCopy, true);
		GenerateStringFromBoardData(brd, cfgCopy);
		RemovePawns(cfgCopy, brd, cont.possibilities);
		Config boardCopy(cfgCopy);
		FillAmbiguousPossibilities(boardCopy, cont);
		if (!((int)cont.ambiguous.size()))
		{
			if (state == "" && !CheckIfMoveExists(move, moves))
				moves.push_back(move);
		}
		else
		{
			for (int i = 0; i < (int)cont.ambiguous.size(); i++)
			{
				for (int j = 0; j < (int)cont.occurences[cont.ambiguous[i].first][cont.ambiguous[i].second].size(); j++)
				{
					boardCopy = cfgCopy;
					string state2;
					RemovePawnsFromAmbiguousSituation(boardCopy, cont, cont.occurences[cont.ambiguous[i].first][cont.ambiguous[i].second][j].whichHasMore,
						cont.occurences[cont.ambiguous[i].first][cont.ambiguous[i].second][j].start,
						cont.occurences[cont.ambiguous[i].first][cont.ambiguous[i].second][j].end, state2);
					move.clear();
					GenerateStringFromBoardData(move, boardCopy, true);
					if (state2 == "" && !CheckIfMoveExists(move, moves))
						moves.push_back(move);
				}
			}
		}
	}

	if (ending)
	{
		if (GameState(cfgCopy) == "white_win" || GameState(cfgCopy) == "black_win" || GameState(cfgCopy) == "dead_lock white" || GameState(cfgCopy) == "dead_lock black")
		{
			if (countOnly)
				cout << "1_UNIQUE_MOVES\n";
			else
			{
				for (int j = 0; j < (int)moves[(int)moves.size() - 1].size(); j++)
					cout << moves[(int)moves.size() - 1][j] << endl;
			}
			return true;
		}
	}

	return false;
}

void PrintEndingMoves(Config& cfg, bool countOnly)
{
	vector<vector<string>> moves;
	Config newCfg(cfg);
	for (int i = -1; i <= (int)cfg.board.size(); i++)
	{
		if (i == -1)
		{
			for (int j = -1; j < cfg.boardSize; j++)
			{
				if (GenerateMove(cfg, i + 1, j, i, j, moves, countOnly, true)) return;
				if (GenerateMove(cfg, i + 1, j + 1, i, j, moves, countOnly, true)) return;
			}
		}
		else if (i == (int)cfg.board.size())
		{
			for (int j = -1; j < cfg.boardSize; j++)
			{
				if (GenerateMove(cfg, i - 1, j, i, j, moves, countOnly, true)) return;
				if (GenerateMove(cfg, i - 1, j + 1, i, j, moves, countOnly, true)) return;
			}
		}
		else
		{
			if (GenerateMove(cfg, i, 0, i, -1, moves, countOnly, true)) return;

			if (i < cfg.boardSize - 1)
				if (GenerateMove(cfg, i + 1, 0, i, -1, moves, countOnly, true)) return;
				
			else if (i >= cfg.boardSize)
				if (GenerateMove(cfg, i - 1, 0, i, -1, moves, countOnly, true)) return;

			if (GenerateMove(cfg, i, 2 * cfg.boardSize - 2 - abs(cfg.boardSize - i - 1), i, 2 * cfg.boardSize - 1 - abs(cfg.boardSize - i - 1), moves, countOnly, true)) return;

			if (i < cfg.boardSize - 1)
				if (GenerateMove(cfg, i + 1, 2 * cfg.boardSize - 1 - abs(cfg.boardSize - i - 1), i, 2 * cfg.boardSize - 1 - abs(cfg.boardSize - i - 1), moves, countOnly, true)) return;
			else if (i >= cfg.boardSize)
				if (GenerateMove(cfg, i - 1, 2 * cfg.boardSize - 1 - abs(cfg.boardSize - i - 1), i, 2 * cfg.boardSize - 1 - abs(cfg.boardSize - i - 1), moves, countOnly, true)) return;
		}
	}

	if (countOnly)
		cout << (int)moves.size() << "_UNIQUE_MOVES\n";
	else
	{
		for (int i = 0; i < (int)moves.size(); i++)
		{
			for (int j = 0; j < (int)moves[i].size(); j++)
				cout << moves[i][j] << endl;
			cout << endl;
		}
	}
}

void LoadError(Config& cfg, const string& moveCoords)
{
	cfg.state = "bad_move ";
	switch (cfg.currentPlayer)
	{
	case white:
		cfg.state += "white ";
		break;
	case black:
		cfg.state += "black ";
		break;
	}
	cfg.state += moveCoords;
}

void ApplyMovement(Config& cfg, int destLeft, int destRight)
{
	cfg.board[destLeft][destRight] = cfg.currentPlayer;
	cfg.reserved[cfg.currentPlayer]--;
	if (cfg.currentPlayer == white)
		cfg.currentPlayer = black;
	else
		cfg.currentPlayer = white;
}