#pragma once
#include <vector>
#include <string>
using namespace std;

const int PLAYERS = 2;

enum Players
{
	white,
	black,
	none
};

class Config
{
public:
	int boardSize = 0, collectionTrigger = 0;
	int pawns[PLAYERS] = { 0 }, reserved[PLAYERS] = {0};
	Players currentPlayer = none;
	vector<vector<Players>> board;
	string state = "in_progress";

	Config() {};

	Config(const Config& oth)
	{
		boardSize = oth.boardSize;
		collectionTrigger = oth.collectionTrigger;
		for (int i = 0; i < PLAYERS; i++)
		{
			pawns[i] = oth.pawns[i];
			reserved[i] = oth.reserved[i];
		}
		currentPlayer = oth.currentPlayer;
		board.clear();
		for (int i = 0; i < (int)oth.board.size(); i++)
		{
			board.push_back(vector<Players>(oth.board[i]));
		}
	}

	Config& operator=(const Config& oth)
	{
		if (this == &oth) return *this;
		boardSize = oth.boardSize;
		collectionTrigger = oth.collectionTrigger;
		for (int i = 0; i < PLAYERS; i++)
		{
			pawns[i] = oth.pawns[i];
			reserved[i] = oth.reserved[i];
		}
		currentPlayer = oth.currentPlayer;
		board.clear();
		for (int i = 0; i < (int)oth.board.size(); i++)
		{
			board.push_back(vector<Players>(oth.board[i]));
		}
		return *this;
	}
};

struct RemovalResult
{
	vector<pair<int, int>> coordsToDelete;
	Players whichHasMore;
	pair<int, int> start, end;
};

struct MoveContainer
{
	vector<RemovalResult> possibilities;
	vector<vector<vector<RemovalResult>>> occurences;
	vector<pair<int, int>> ambiguous;
};