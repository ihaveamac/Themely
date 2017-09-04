// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include <string>
#include <map>

std::map<std::string, std::wstring> ENGLISH = {
	// UI.CPP
	{"scanning",            L"Scanning themes..."},
	{"error_message",       L"An error has occurred. You have Themely version %s."},
	{"press_start",         L"Press START to quit."},
	{"themes",              L"%i theme%ls"}, // %ls is the plural suffix
	{"plural_suffix",       L"s"},
	{"qr_scanner",          L"QR Code scanner"},
	{"by",                  L"by %ls"},
	{"add_to_shuffle",      L"Add to shuffle"},
	{"remove_from_shuffle", L"Remove from shuffle"},
	{"install",             L"Install"},
	{"w_o_bgm",             L"w/o BGM"},
	{"fullscreen_prev",     L"Fullscreen preview"},
	{"hold_delete",         L"[hold] delete"},
	{"bgm_prev",            L"BGM preview"},
	{"installing",          L"Installing..."},
	{"downloading",         L"Downloading..."},
	{"new_update",          L"There's a new update!"},
	{"delete_prompt_1",     L"Are you sure you want to"},
	{"delete_prompt_2",     L"delete this theme?"},
	{"dump_prompt",         L"Dump currently installed theme?"},
	{"dump_prompt_warn",    L"Please don't submit official themes to themeplaza.eu!"},
	{"playing",             L"Playing..."},
	{"no_themes_1",         L"No themes found!"},
	{"no_themes_2",         L"Go to themeplaza.eu on your computer, download some themes,"},
	{"no_themes_3",         L"and put them into the /Themes folder in your SD Card."},
	{"shuffle_count",       L"%i/10 theme%ls selected for shuffle"},
	{"qr_explain_1",        L"Scan a QR Code of a direct link to"},
	{"qr_explain_2",        L"a ZIP file of a theme to download it."},
	{"qr_explain_3",        L"Go to themeplaza.eu, select a theme, then click on the"},
	{"qr_explain_4",        L"QR Code button to see the QR Code for that particular theme."},
	{"update_prompt",       L"Install this update?"},
	{"yes",                 L"Yes"},
	{"no",                  L"No"},
	{"stop_bgm",            L"Press (Y) to stop."},
	// THEME.CPP
	{"no_desc",             L"[description not available]"},
	{"unknown",             L"Unknown"},
	{"install_reading",     L"Reading %s..."},
	{"install_writing",     L"Writing to %s..."},

	// ERRORS
	// AUDIO.CPP
	{"err_bgm_fail",       L"Failed to play BGM"},
	// UI.CPP
	{"err_texture",        L"Failed to load textures."},
	// NETWORK.CPP
	{"err_update_dl_fail", L"Failed to download update."},
	{"err_update_manual",  L"If you're still having problems, update manually."},
	{"err_update_titledb", L"If you're still having problems, update through FBI -> TitleDB."},
	{"err_zip_dl_fail",    L"Failed to download ZIP file"},
	{"err_zip_invalid",    L"The ZIP file is invalid"},
	{"err_zip_no_body",    L"The ZIP file doesn't contain a body_LZ.bin in the root"},
	// THEME.CPP
	{"err_fail_open",      L"Failed to open %s."},
	{"err_fail_read",      L"Failed to read %s."},
	{"err_fail_write",     L"Failed to write to %s."},
	{"err_try_default",    L"Try selecting one of the default themes on the home menu settings before trying again"},
	{"err_bgm_too_big",    L"The BGM file exceeds 3371008 bytes. BGM files bigger than this size can softbrick your device."},
	{"err_no_dsp",         L"DSP dump not found! Use DSP1 to dump the DSP firmware required to playback audio."}
};
