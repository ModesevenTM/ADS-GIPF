#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "structs.hpp"
#include "checks.hpp"
#include "iomanip.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

void DoMove(Config& cfg);
void TryMovement(Config& cfg, int destLeft, int destRight, int startLeft, int startRight, string& state);
void RemovePawns(Config& cfg, vector<string> stringBoard, vector<RemovalResult>& possibilities);
void PrintPossibleMoves(Config& cfg, bool countOnly = true);
void FillAmbiguousPossibilities(Config& boardCopy, struct MoveContainer& cont);
void RemovePawnsFromAmbiguousSituation(Config& boardCopy, struct MoveContainer& cont, Players whosePawns, pair<int, int> startPawnIndex, pair<int, int> destPawnIndex, string& state);
void AcceptMovement(Config& cfg, Config& boardCopy, string startPawn, string destPawn, Players whosePawns, string& state);
bool GenerateMove(Config& cfg, int destLeft, int destRight, int startLeft, int startRight, vector<vector<string>>& moves, bool countOnly = false, bool ending = false);
void PrintEndingMoves(Config& cfg, bool countOnly = true);
void LoadError(Config& cfg, const string& moveCoords);
void ApplyMovement(Config& cfg, int destLeft, int destRight);