#pragma once
#include "structs.hpp"

bool CheckIfInside(Config& cfg, int left, int right);
bool CheckStartCandidate(Config& cfg, int left, int right);
bool CheckIfMoveExists(const vector<string>& move, vector<vector<string>> moves);
int CountWrongRows(Config& cfg, vector<string> stringBoard);