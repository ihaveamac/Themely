// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include <string>
#include <map>

// Translated by theheroGAC

std::map<std::string, std::wstring> ITALIAN = {
	// UI.CPP
	{"scanning",            L"Scansione dei temi..."},
	{"error_message",       L"Si è verificato un errore. Hai la versione di Themely %s."},
	{"press_start",         L"Premere START per abbandonare."},
	{"themes",              L"%i theme%ls"}, // %ls è un suffisso plurale
	{"plural_suffix",       L"s"},
	{"qr_scanner",          L"QR Code scanner"},
	{"by",                  L"di %ls"},
	{"add_to_shuffle",      L"Aggiungere"},
	{"remove_from_shuffle", L"Rimuovi"},
	{"install",             L"Installa"},
	{"w_o_bgm",             L"senza BGM"},
	{"fullscreen_prev",     L"Anteprima in Fullscreen"},
	{"hold_delete",         L"[tenere] cancella"},
	{"bgm_prev",            L"Anteprima del BGM"},
	{"installing",          L"Installazione..."},
	{"downloading",         L"Scaricamento..."},
	{"new_update",          L"E' disponibile un nuovo aggiornamento!"},
	{"delete_prompt_1",     L"Sei sicuro di volere"},
	{"delete_prompt_2",     L"cancellare questo tema?"},
	{"dump_prompt",         L"Scollegare il tema attualmente installato?"},
	{"dump_prompt_warn",    L"Per favore non inviare temi ufficiali a themeplaza.eu!"},
	{"playing",             L"Riproduzione..."},
	{"no_themes_1",         L"Nessun tema trovato!"},
	{"no_themes_2",         L"Vai all'indirizzo themeplaza.eu sul tuo computer, scarica alcuni temi,"},
	{"no_themes_3",         L"e inserirli nella cartella /Themes nella tua SD Card."},
	{"shuffle_count",       L"%i/10 temi%ls selezionati"},
	{"qr_explain_1",        L"Effettua la scansione di un codice QR per un collegamento"},
	{"qr_explain_2",        L"diretto a un file ZIP di un tema per scaricarlo."},
	{"qr_explain_3",        L"Vai all'indirizzo themeplaza.eu, seleziona, quindi fare clic sul"},
	{"qr_explain_4",        L"pulsante del codice QR per visualizzare il codice QR."},
	{"update_prompt",       L"Installare questo aggiornamento?"},
	{"yes",                 L"Si"},
	{"no",                  L"No"},
	{"stop_bgm",            L"Premere (Y) per fermare."},
	// THEME.CPP
	{"no_desc",             L"[descrizione non disponibile]"},
	{"unknown",             L"Sconosciuto"},
	{"install_reading",     L"Lettura in corso del %s..."},
	{"install_writing",     L"Scrittura in corso del %s..."},

	// ERRORS
	// AUDIO.CPP
	{"err_bgm_fail",       L"Impossibile riprodurre il BGM"},
	// UI.CPP
	{"err_texture",        L"Impossibile caricare le texture."},
	// NETWORK.CPP
	{"err_update_dl_fail", L"Scaricamento dell'aggiornamento fallito."},
	{"err_update_manual",  L"Se hai ancora problemi,prova ad aggiornare manualmente."},
	{"err_update_titledb", L"Se hai ancora problemi, prova ad aggiornare FBI -> TitleDB."},
	{"err_zip_dl_fail",    L"Scaricamento del file ZIP fallito"},
	{"err_zip_invalid",    L"Il file ZIP non è valido"},
	{"err_zip_no_body",    L"Il file ZIP non contiene il file body_LZ.bin nella cartella principale"},
	// THEME.CPP
	{"err_fail_open",      L"Apertura fallita di %s."},
	{"err_fail_read",      L"Lettura fallita di %s."},
	{"err_fail_write",     L"Scrittura fallita di %s."},
	{"err_try_default",    L"Provare a selezionare uno dei temi predefiniti nelle impostazioni del menu prima di riprovare"},
	// err_bgm_too_big
	{"err_no_dsp",         L"DSP dump non trovato! Utilizzare DSP1 per scaricare il firmware DSP richiesto per riprodurre l'audio."}
};
