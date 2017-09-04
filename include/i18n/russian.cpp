// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include <string>
#include <map>

// Translated by Ann0ying

std::map<std::string, std::wstring> RUSSIAN = {
	// UI.CPP
	{"scanning",            L"Сканирую темы..."},
	{"error_message",       L"Возникла ошибка. У вас Themely версии %s."},
	{"press_start",         L"Нажмите START чтобы выйти."},
	{"themes",              L"%i тем%ls"}, // %ls is the plural suffix
	{"plural_suffix",       L"ы"},
	{"qr_scanner",          L"Сканер QR кодов"},
	{"by",                  L"от %ls"},
	{"add_to_shuffle",      L"Добавить в перемешку"},
	{"remove_from_shuffle", L"Удалить из перемешки"},
	{"install",             L"Установить"},
	{"w_o_bgm",             L"без фоновой музыки"},
	{"fullscreen_prev",     L"Предпросмотр"},
	{"hold_delete",         L"[Зажать] Удаление"},
	{"bgm_prev",            L"Прослушать Фоновую музыку"},
	{"installing",          L"Установка..."},
	{"downloading",         L"Загрузка..."},
	{"new_update",          L"Доступно обновление!"},
	{"delete_prompt_1",     L"Вы уверенны что хотите"},
	{"delete_prompt_2",     L"удалить эту тему?"},
	{"dump_prompt",         L"Сделать дамп текущей темы?"},
	{"dump_prompt_warn",    L"Пожалуйста, не заливайте официальные темы на themeplaza.eu!"},
	{"playing",             L"Проигрываю..."},
	{"no_themes_1",         L"Темы не найдены!"},
	{"no_themes_2",         L"Перейдите на themeplaza.eu на вашем ПК, скайчайте темы,"},
	{"no_themes_3",         L"и добавьте их в папку /Themes на вашей карте памяти."},
	{"shuffle_count",       L"%i/10 тем%ls выбрано для перемешки"},
	{"qr_explain_1",        L"Просканируйте QR-код с прямой ссылкой на"},
	{"qr_explain_2",        L"ZIP файл темы чтобы скачать ее."},
	{"qr_explain_3",        L"Перейдите на themeplaza.eu, Выбирите тему, затем нажмите"},
	{"qr_explain_4",        L"на кнопку QR-кода чтобы увидеть QR-код этой темы"},
	{"update_prompt",       L"Установить обновление?"},
	{"yes",                 L"Да"},
	{"no",                  L"Нет"},
	{"stop_bgm",            L"Нажмите (Y) для остановки."},
	// THEME.CPP
	{"no_desc",             L"[Описание отсутствует]"},
	{"unknown",             L"Неизвестно"},
	{"install_reading",     L"Чтение %s..."},
	{"install_writing",     L"Запись to %s..."},

	// ERRORS
	// AUDIO.CPP
	{"err_bgm_fail",       L"Ошибка проигрывания фоновой музыки"},
	// UI.CPP
	{"err_texture",        L"Ошибка загрузки текстур."},
	// NETWORK.CPP
	{"err_update_dl_fail", L"Ошибка загрузки обновления."},
	{"err_update_manual",  L"Если вы все еще испытываете проблемы, обновитесь вручную"},
	{"err_update_titledb", L"Если вы все еще испытываете проблемы, обновитесь через FBI -> TitleDB."},
	{"err_zip_dl_fail",    L"Ошибка загрузки ZIP файла"},
	{"err_zip_invalid",    L"Неправильный ZIP файл"},
	{"err_zip_no_body",    L"ZIP файл не содержит body_LZ.bin в корне"},
	// THEME.CPP
	{"err_fail_open",      L"Ошибка открытия %s."},
	{"err_fail_read",      L"Ошибка чтения %s."},
	{"err_fail_write",     L"Ошибка записи в %s."},
	{"err_try_default",    L"Попробуйте установить одну из стандартных тем в Home Menu, и попытайтесь еще раз."},
	// err_bgm_too_big
	{"err_no_dsp",         L"DSP файл не найден! Используйте DSP1 чтобы сделать дамп DSP файла необходимого для проигрывания музыки."}
};
