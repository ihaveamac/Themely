// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "i18n.h"

#include "i18n/english.cpp"
#include "i18n/french.cpp"
#include "i18n/german.cpp"
#include "i18n/italian.cpp"
#include "i18n/spanish.cpp"
#include "i18n/portuguese.cpp"
#include "i18n/russian.cpp"
#include "i18n/dutch.cpp"

u8 lang = 1;

wstring i18n(const string& token){
	if((currentPlayingAudio || audioIsPlaying) && !NEW3DS && token != "playing" && token != "stop_bgm")
		return wstring(L"");

	map<string, wstring> langMap;

	/*
		CFG_LANGUAGE_JP = 0,  ///< Japanese
		CFG_LANGUAGE_EN = 1,  ///< English
		CFG_LANGUAGE_FR = 2,  ///< French
		CFG_LANGUAGE_DE = 3,  ///< German
		CFG_LANGUAGE_IT = 4,  ///< Italian
		CFG_LANGUAGE_ES = 5,  ///< Spanish
		CFG_LANGUAGE_ZH = 6,  ///< Simplified Chinese
		CFG_LANGUAGE_KO = 7,  ///< Korean
		CFG_LANGUAGE_NL = 8,  ///< Dutch
		CFG_LANGUAGE_PT = 9,  ///< Portuguese
		CFG_LANGUAGE_RU = 10, ///< Russian
		CFG_LANGUAGE_TW = 11, ///< Traditional Chinese
	*/

	switch(lang){
		case CFG_LANGUAGE_FR:
			langMap = FRENCH;
			break;
		case CFG_LANGUAGE_DE:
			langMap = GERMAN;
			break;
		case CFG_LANGUAGE_IT:
			langMap = ITALIAN;
			break;
		case CFG_LANGUAGE_ES:
			langMap = SPANISH;
			break;
		case CFG_LANGUAGE_PT:
			langMap = PORTUGUESE;
			break;
		case CFG_LANGUAGE_RU:
			langMap = RUSSIAN;
			break;
		case CFG_LANGUAGE_NL:
			langMap = DUTCH;
			break;
		default:
			langMap = ENGLISH;
			break;
	}

	if(langMap.find(token) == langMap.end())
		if(langMap == ENGLISH){
			string toBeConverted = string(token);
			return wstring(toBeConverted.begin(), toBeConverted.end());
		} else
			if(ENGLISH.find(token) == ENGLISH.end()){
				string toBeConverted = string(token);
				return wstring(toBeConverted.begin(), toBeConverted.end());
			} else
				return ENGLISH[token];
	else
		return langMap[token];
}
