// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#pragma once

#include "global.h"
#include "theme.h"

#include "lib/json.hpp"
using nlohmann::json;

extern string update;
extern bool is_upgrading;
extern int downloading;

Result HTTPGet(vector<char>&, string, string* fileName = NULL, int* progress = NULL);

void installAnemone();
void checkForUpdate(void*);
void installUpdate();

void downloadThemeFromURL(void*);
