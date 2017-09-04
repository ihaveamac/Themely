// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include <string>
#include <map>

std::map<std::string, std::wstring> DUTCH = {
	// UI.CPP
	{"scanning",            L"Thema's scannen..."},
	{"error_message",       L"Er is een fout opgetreden. U heeft Themely Versie %s."},
	{"press_start",         L"Druk op start te stoppen."},
	{"themes",              L"%i Thema%ls"}, // %ls is the plural suffix
	{"plural_suffix",       L"s"},
	{"qr_scanner",          L"QR Code scanner"},
	{"by",                  L"door %ls"},
	{"add_to_shuffle",      L"Voeg toe aan shuffle"},
	{"remove_from_shuffle", L"Verwijder van shuffle"},
	{"install",             L"Installeren"},
	{"w_o_bgm",             L"Zonder muziek"},
	{"fullscreen_prev",     L"Volledig scherm preview"},
	{"hold_delete",         L"[Houvast] Verwijderen"},
	{"bgm_prev",            L"Muziek preview"},
	{"installing",          L"Installeren..."},
	{"downloading",         L"Downloaden..."},
	{"new_update",          L"Er is een nieuwe update!"},
	{"delete_prompt_1",     L"Weet je zeker dat je"},
	{"delete_prompt_2",     L"dit thema wil verwijderen?"},
	{"dump_prompt",         L"Dump nu geïnstalleerde thema?"},
	{"dump_prompt_warn",    L"Plaats geen officiële Thema's op themeplaza.eu!"},
	{"playing",             L"Spelen..."},
	{"no_themes_1",         L"Geen thema's gevonden!"},
	{"no_themes_2",         L"Ga naar themeplaza.eu op uw computer, download een paar thema's,"},
	{"no_themes_3",         L"en doe ze in de /Themes map op de SD Card."},
	{"shuffle_count",       L"%i/10 Thema%ls gekozen voor shuffle"},
	{"qr_explain_1",        L"Scan een QR Code van een direct link naar"},
	{"qr_explain_2",        L"een ZIP bestand van een thema om het the downloaden."},
	{"qr_explain_3",        L"Ga naar themeplaza.eu, kies een thema en hou dan de muis"},
	{"qr_explain_4",        L"boven de preview van dat thema om de QR code te zien."},
	{"update_prompt",       L"Installeer deze update?"},
	{"yes",                 L"Ja"},
	{"no",                  L"Nee"},
	{"stop_bgm",            L"Druk op (Y) om te stoppen."},
	// THEME.CPP
	{"no_desc",             L"[Beschrijving niet aanwezig]"},
	{"unknown",             L"Onbekend"},
	{"install_reading",     L"Lezen %s..."},
	{"install_writing",     L"Schrijven naar %s..."},

	// ERRORS
	// AUDIO.CPP
	{"err_bgm_fail",       L"Mislukt om muziek te spelen."},
	// UI.CPP
	{"err_texture",        L"Mislukt om textures te laden."},
	// NETWORK.CPP
	{"err_update_dl_fail", L"Update downloaden mislukt."},
	{"err_update_manual",  L"Als je nog steeds problemen hebt, update handmatig."},
	{"err_update_titledb", L"Als je nog steeds problemen hebt, update via FBI -> TitleDB."},
	{"err_zip_dl_fail",    L"Mislukt om ZIP bestand te downloaden"},
	{"err_zip_invalid",    L"Het ZIP bestand is ongeldig."},
	{"err_zip_no_body",    L"Het ZIP bestand heeft geen body_LZ.bin in de root"},
	// THEME.CPP
	{"err_fail_open",      L"Mislukt om %s te openen."},
	{"err_fail_read",      L"Mislukt om %s te lezen."},
	{"err_fail_write",     L"Mislukt om naar %s te schrijven."},
	{"err_try_default",    L"Probeer om een standaard thema in het homemenu te selecteren voordat je het opnieuw probeert."},
	{"err_bgm_too_big",    L"Het BGM bestand is groter dan 3371008 bytes. BGM bestanden groter dan dit kunnen de 3ds softbricken."},
	{"err_no_dsp",         L"DSP dump niet gevonden! Gebruik DSP1 om de DSP firmware te dumpen voor audio playback."}
};
