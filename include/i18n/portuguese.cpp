// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include <string>
#include <map>

// Translated by Jubash

std::map<std::string, std::wstring> PORTUGUESE = {
	// UI.CPP
	{"scanning",            L"Buscando temas..."},
	{"error_message",       L"Ocorreu um erro. Sua versão do Themely é %s."},
	{"press_start",         L"Pressione START para sair."},
	{"themes",              L"%i tema%ls"}, // %ls is the plural suffix
	{"plural_suffix",       L"s"},
	{"qr_scanner",          L"Escanear código QR"},
	{"by",                  L"por %ls"},
	{"add_to_shuffle",      L"Adicionar aos aleatórios"},
	{"remove_from_shuffle", L"Remover dos aleatórios"},
	{"install",             L"Instalar"},
	{"w_o_bgm",             L"sem BGM"},
	{"fullscreen_prev",     L"Prévia em tela cheia"},
	{"hold_delete",         L"[pressione] excluir"},
	{"bgm_prev",            L"Prévia da BGM"},
	{"installing",          L"Instalando..."},
	{"downloading",         L"Baixando..."},
	{"new_update",          L"Foi encontrada uma nova atualização!"},
	{"delete_prompt_1",     L"Tem certeza que você quer"},
	{"delete_prompt_2",     L"excluir este tema?"},
	{"dump_prompt",         L"Extrair o tema instalado atualmente?"},
	{"dump_prompt_warn",    L"Por favor, não envie temas oficiais ao themeplaza.eu!"},
	{"playing",             L"Reproduzindo..."},
	{"no_themes_1",         L"Nenhum tema encontrado!"},
	{"no_themes_2",         L"Vá ao themeplaza.eu em seu computador, baixe alguns temas"},
	{"no_themes_3",         L"e coloque-os na pasta /Themes em seu cartão SD."},
	{"shuffle_count",       L"%i/10 tema%ls selecionados para exibição aleatória"},
	{"qr_explain_1",        L"Escaneie o código QR de um link direto para"},
	{"qr_explain_2",        L"o arquivo ZIP de um tema para baixá-lo."},
	{"qr_explain_3",        L"Vá ao themeplaza.eu, selecione um tema e então clique no"},
	{"qr_explain_4",        L"botão QR para ver o código QR daquele tema em particular."},
	{"update_prompt",       L"Instalar este update?"},
	{"yes",                 L"Sim"},
	{"no",                  L"Não"},
	{"stop_bgm",            L"Pressione (Y) para parar."},
	// THEME.CPP
	{"no_desc",             L"[descrição indisponível]"},
	{"unknown",             L"Desconhecido"},
	{"install_reading",     L"Lendo %s..."},
	{"install_writing",     L"Escrevendo em %s..."},

	// ERRORS
	// AUDIO.CPP
	{"err_bgm_fail",       L"Falha ao reproduzir BGM"},
	// UI.CPP
	{"err_texture",        L"Falha ao carregar texturas."},
	// NETWORK.CPP
	{"err_update_dl_fail", L"Falha ao baixar a atualização."},
	{"err_update_manual",  L"Se você continua encontrando problemas, atualize manualmente."},
	{"err_update_titledb", L"Se você continua encontrando problemas, atualize através de FBI -> TitleDB."},
	{"err_zip_dl_fail",    L"Falha ao baixar o arquivo ZIP"},
	{"err_zip_invalid",    L"O arquivo ZIP é inválido"},
	{"err_zip_no_body",    L"O arquivo ZIP não contém o arquivo body_LZ.bin na raiz"},
	// THEME.CPP
	{"err_fail_open",      L"Falha ao abrir %s."},
	{"err_fail_read",      L"Falha ao ler %s."},
	{"err_fail_write",     L"Falha ao escrever em %s."},
	{"err_try_default",    L"Tente selecionar um dos temas padrões nas configurações do menu HOME antes de tentar novamente"},
	// err_bgm_too_big
	{"err_no_dsp",         L"DSP não encontrado! Use DSP1 para extrair o firmware DSP, necessário para a reprodução de áudio."}
};
