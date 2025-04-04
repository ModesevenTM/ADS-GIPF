#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "structs.hpp"
#include "checks.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

void InitializeBoard(Config& cfg);
void ClearLastGame(Config& cfg);
void LoadGameBoard(Config& cfg);
void PrintGameBoard(Config& cfg);
void GenerateStringFromBoardData(vector<string>& stringBoard, Config& cfg, bool withAdditionalData = false);
void PrintStatus(Config& cfg);
void IsGameOver(Config& cfg);
string GameState(Config& cfg);