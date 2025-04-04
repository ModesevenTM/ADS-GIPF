#include "iomanip.hpp"

void InitializeBoard(Config& cfg)
{
	cfg.board.resize(2 * cfg.boardSize - 1);
	for (int i = 0; i < (int)cfg.board.size(); i++)
		cfg.board[i].resize(cfg.board.size() - abs(cfg.boardSize - i - 1));
}

void ClearLastGame(Config& cfg)
{
	for (int i = 0; i < (int)cfg.board.size(); i++)
		cfg.board[i].clear();
	cfg.board.clear();
}

void LoadGameBoard(Config& cfg)
{
	ClearLastGame(cfg);
	cin >> cfg.boardSize >> cfg.collectionTrigger;
	for (int i = 0; i < PLAYERS; i++)
	{
		cin >> cfg.pawns[i];
	}
	for (int i = 0; i < PLAYERS; i++)
	{
		cin >> cfg.reserved[i];
	}
	string player;
	cin >> player;
	if (player == "W")
		cfg.currentPlayer = white;
	else if (player == "B")
		cfg.currentPlayer = black;
	else
	{
		cout << "WRONG_PLAYER_" << player << "_DOES_NOT_EXIST\n";
		return;
	}

	getchar();

	vector<vector<string>> boardLines;
	vector<string> stringBoard;
	int usedPawns[PLAYERS] = { cfg.reserved[white], cfg.reserved[black] };
	boardLines.resize(2 * cfg.boardSize - 1);
	InitializeBoard(cfg);
	int* sizeArr = new int[2 * cfg.boardSize - 1];
	for (int i = 0; i < 2 * cfg.boardSize - 1; i++)
		sizeArr[i] = 2 * cfg.boardSize - 1 - abs(cfg.boardSize - i - 1);
	int firstPawnInLine = 0;

	for (int i = 0; i < (int)boardLines.size(); i++)
	{
		stringBoard.push_back(string());
		for (int sp = 0; sp < abs(cfg.boardSize - i - 1) - 1; sp++)
			stringBoard[i] += " ";
		string line, character;
		getline(cin, line);
		stringstream lineStrm(line);
		while (lineStrm >> character)
		{
			boardLines[i].push_back(character);
			if (abs(cfg.boardSize - i - 1) || stringBoard[i].size())
				stringBoard[i] += " ";
			if (character == "W")
			{
				usedPawns[white]++;
				stringBoard[i] += "W";
			}
			else if (character == "B")
			{
				usedPawns[black]++;
				stringBoard[i] += "B";
			}
			else if (character == "_")
			{
				stringBoard[i] += "_";
			}
		}
		if (boardLines[i].size() != boardLines.size() - abs(cfg.boardSize - i - 1))
		{
			cout << "WRONG_BOARD_ROW_LENGTH\n";
			ClearLastGame(cfg);
			delete[] sizeArr;
			return;
		}
		else
		{
			for (int j = 0; j < (int)boardLines[i].size(); j++)
			{
				if (boardLines[i][j] == "W")
					cfg.board[j + firstPawnInLine][--sizeArr[j + firstPawnInLine]] = white;
				else if (boardLines[i][j] == "B")
					cfg.board[j + firstPawnInLine][--sizeArr[j + firstPawnInLine]] = black;
				else
					cfg.board[j + firstPawnInLine][--sizeArr[j + firstPawnInLine]] = none;
			}
			if (i >= cfg.boardSize - 1) firstPawnInLine++;
		}
	}

	int wrongRows = CountWrongRows(cfg, stringBoard);
	if (wrongRows)
	{
		if (wrongRows == 1)
		{
			cout << "ERROR_FOUND_1_ROW_OF_LENGTH_K\n";
			ClearLastGame(cfg);
			delete[] sizeArr;
			return;
		}
		else
		{
			cout << "ERROR_FOUND_" << wrongRows << "_ROWS_OF_LENGTH_K\n";
			ClearLastGame(cfg);
			delete[] sizeArr;
			return;
		}
	}

	for (int i = 0; i < PLAYERS; i++)
	{
		if (usedPawns[i] > cfg.pawns[i])
		{
			cout << "WRONG_";
			switch (i)
			{
			case white:
				cout << "WHITE";
				break;
			case black:
				cout << "BLACK";
				break;
			}
			cout << "_PAWNS_NUMBER\n";
			ClearLastGame(cfg);
			delete[] sizeArr;
			return;
		}
	}

	cout << "BOARD_STATE_OK\n";
	delete[] sizeArr;
}

void PrintGameBoard(Config& cfg)
{
	if (!cfg.board.size())
		cout << "EMPTY_BOARD\n";
	else
	{
		vector<string> board;
		GenerateStringFromBoardData(board, cfg, true);
		for (int i = 0; i < (int)board.size(); i++)
			cout << board[i] << endl;
		cout << endl;
	}
}

void GenerateStringFromBoardData(vector<string>& stringBoard, Config& cfg, bool withAdditionalData)
{
	int lineOffset = 0;
	if (withAdditionalData)
	{
		lineOffset = 2;
		stringBoard.push_back(string());
		stringBoard[0] += to_string(cfg.boardSize);
		stringBoard[0] += " ";
		stringBoard[0] += to_string(cfg.collectionTrigger);
		for (int i = 0; i < PLAYERS; i++)
		{
			stringBoard[0] += " ";
			stringBoard[0] += to_string(cfg.pawns[i]);
		}
		stringBoard.push_back(string());
		for (int i = 0; i < PLAYERS; i++)
		{
			stringBoard[1] += to_string(cfg.reserved[i]);
			stringBoard[1] += " ";
		}
		switch (cfg.currentPlayer)
		{
		case white:
			stringBoard[1] += "W";
			break;
		case black:
			stringBoard[1] += "B";
			break;
		}
	}

	int* sizeArr = new int[2 * cfg.boardSize - 1];
	for (int i = 0; i < 2 * cfg.boardSize - 1; i++)
		sizeArr[i] = 2 * cfg.boardSize - 1 - abs(cfg.boardSize - i - 1);
	int firstPawnInLine = 0;

	for (int i = 0; i < 2 * cfg.boardSize - 1; i++)
	{
		stringBoard.push_back(string());
		for (int sp = 0; sp < abs(cfg.boardSize - i - 1) - 1; sp++)
			stringBoard[i + lineOffset] += " ";
		for (int j = 0; j < 2 * cfg.boardSize - 1 - abs(cfg.boardSize - i - 1); j++)
		{
			if (abs(cfg.boardSize - i - 1) || (int)stringBoard[i + lineOffset].size())
				stringBoard[i + lineOffset] += " ";
			Players pl = cfg.board[j + firstPawnInLine][--sizeArr[j + firstPawnInLine]];
			switch (pl)
			{
			case white:
				stringBoard[i + lineOffset] += "W";
				break;
			case black:
				stringBoard[i + lineOffset] += "B";
				break;
			case none:
				stringBoard[i + lineOffset] += "_";
				break;
			}
		}
		if (!sizeArr[firstPawnInLine]) firstPawnInLine++;
	}
	delete[] sizeArr;
}

void PrintStatus(Config& cfg)
{
	stringstream str(cfg.state);
	string ssinput;
	str >> ssinput;
	if (ssinput == "bad_move")
		cout << cfg.state << endl;
	else
		cout << GameState(cfg) << endl;
}

void IsGameOver(Config& cfg)
{
	Players opponent;
	cfg.currentPlayer == white ? opponent = black : opponent = white;
	if (!cfg.reserved[cfg.currentPlayer])
	{
		cout << "THE_WINNER_IS_";
		switch (opponent)
		{
		case white:
			cout << "WHITE\n";
			break;
		case black:
			cout << "BLACK\n";
			break;
		}
	}
	else
	{
		for (int i = 0; i < (int)cfg.board.size(); i++)
		{
			for (int j = 0; j < (int)cfg.board[i].size(); j++)
			{
				if (cfg.board[i][j] == none)
				{
					cout << "GAME_IN_PROGRESS\n";
					return;
				}
			}
		}
		cout << "THE_WINNER_IS_";
		switch (opponent)
		{
		case white:
			cout << "WHITE\n";
			break;
		case black:
			cout << "BLACK\n";
			break;
		}
	}
}

string GameState(Config& cfg)
{
	string state;
	Players opponent;
	cfg.currentPlayer == white ? opponent = black : opponent = white;
	if (!cfg.reserved[cfg.currentPlayer])
	{
		switch (opponent)
		{
		case white:
			state = "white";
			break;
		case black:
			state = "black";
			break;
		}
		state += "_win";
	}
	else
	{
		for (int i = 0; i < (int)cfg.board.size(); i++)
		{
			for (int j = 0; j < (int)cfg.board[i].size(); j++)
			{
				if (cfg.board[i][j] == none)
				{
					return "in_progress";
				}
			}
		}
		state = "dead_lock ";
		switch (cfg.currentPlayer)
		{
		case white:
			state += "white";
			break;
		case black:
			state += "black";
			break;
		}
	}
	return state;
}