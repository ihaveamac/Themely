// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#pragma once

#include "global.h"
#include "ui.h"

struct Theme {
	string fileName;
	wstring title;
	wstring description;
	wstring author;

	sf2d_texture* icon;
	sf2d_texture* preview;

	bool isZip;
	bool infoIsloaded;
	bool infoIsFullyLoaded;
	bool previewIsLoaded;

	bool hasBGM;
	bool hasPreview;

	bool toShuffle;
	bool shuffleNoBGM;

	LightLock lock;
};

extern deque<Theme> themes;
extern bool themesScanned;
extern bool isInstalling;
extern wstring installProgress;

extern bool upgradeToAnemone;
extern bool deletePrompt;
extern bool dumpPrompt;

extern bool shuffleMode;

void loadTheme(void*);
void scanThemes(void*);
void loadThemeInfo(void*);
void checkInfosToBeLoaded(int);
void loadPreview(void*);
void installTheme(void*);

void installShuffle(void*);
void exitShuffleMode();

void deleteTheme();
void dumpTheme();

void toggleBGM();
