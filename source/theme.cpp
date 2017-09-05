// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "theme.h"

deque<Theme> themes;
bool themesScanned = false;
bool isInstalling = false;
wstring installProgress = L"";

bool upgradeToAnemone = false;
bool deletePrompt = false;
bool dumpPrompt = false;

bool shuffleMode = false;

void loadTheme(void* entryVP){
	FS_DirectoryEntry* entry = static_cast<FS_DirectoryEntry*>(entryVP);

	if(entry->attributes & FS_ATTRIBUTE_DIRECTORY){
		if(fileExists(u"/Themes/" + u16string((char16_t*)entry->name) + u"/body_LZ.bin")){
			Theme theme = {
				u16tstr(entry->name, 0x106),
				s2ws(u16tstr(entry->name, 0x106)),
				L"no desc",
				i18n("unknown"),
				NULL,
				NULL,
				false,
				true,
				true,
				false,
				fileExists(u"/Themes/" + u16string((char16_t*)entry->name) + u"/bgm.ogg"),
				false
			};

			LightLock_Init(&theme.lock);

			if(fileExists(u"/Themes/" + u16string((char16_t*)entry->name) + u"/Preview.png")){
				vector<char> tmp_buf;

				if(!fileToVector("/Themes/" + u16tstr(entry->name, 0x106) + "/Preview.png", tmp_buf, 24)){
					int width = (tmp_buf[16]<<24) + (tmp_buf[17]<<16) + (tmp_buf[18]<<8) + (tmp_buf[19]<<0);
					int height = (tmp_buf[20]<<24) + (tmp_buf[21]<<16) + (tmp_buf[22]<<8) + (tmp_buf[23]<<0);

					if(width <= 412 && height <= 482)
						theme.hasPreview = true;
				}
			}

			ifstream smdhFile("/Themes/" + theme.fileName + "/info.smdh", ios::in | ios::binary);
			if(smdhFile.is_open()){
				char* buffer = new char[0x520];
				smdhFile.read(buffer, 0x520);

				string tmpTitle = u16tstr((u16*)&buffer[0x8], 64);
				string tmpDescription = u16tstr((u16*)&buffer[0x8 + 128], 128);
				string tmpAuthor = u16tstr((u16*)&buffer[0x8 + 128 + 256], 64);

				if(tmpTitle.size() != 0)
					theme.title = s2ws(tmpTitle);

				if(tmpDescription.size() != 0)
					theme.description = s2ws(tmpDescription);

				if(tmpAuthor.size() != 0 && tmpAuthor.size() < 20)
					theme.author = s2ws(tmpAuthor);

				smdhFile.seekg(0x24C0);
				char* iconBuf = new char[0x1200];
				smdhFile.read(iconBuf, 0x1200);

				// detects default icons
				if(iconBuf[0] != '\x9D' && iconBuf[1] != '\x04' && iconBuf[0] != '\xBF' && iconBuf[1] != '\x0D'){
					theme.icon = sf2d_create_texture(48, 48, TEXFMT_RGB565, SF2D_PLACE_RAM);
					u16* dst = (u16*)(theme.icon->tex.data + 64 * 8 * 2 * sizeof(u16));
					u16* src = (u16*)(iconBuf);
					for (u8 j = 0; j < 48; j += 8){
						memcpy(dst, src, 48 * 8 * sizeof(u16));
						src += 48 * 8;
						dst += 64 * 8;
					}
				}

				delete[] buffer;
				delete[] iconBuf;
			}

			smdhFile.close();

			themes.push_back(theme);

			if(themes.size() < 3){
				themes[themes.size()-1].previewIsLoaded = true;
				queueTask(loadPreview, (void*)(themes.size()-1), true);
			}
		}
	} else if(
		entry->shortExt[0] == 'Z' &&
		entry->shortExt[1] == 'I' &&
		entry->shortExt[2] == 'P'){
		unzFile zipFile = unzOpen((string("/Themes/") + u16tstr(entry->name, 0x106)).c_str());

		if(!zipFile){
			printf("INVALID ZIP:%s\n", u16tstr(entry->name, 0x106).c_str());
			delete entry;
			themesScanned = true;
			return;
		}

		if(unzLocateFile(zipFile, "body_LZ.bin", 0) && unzLocateFile(zipFile, "body_lz.bin", 0)){
			unzClose(zipFile);
			printf("NO BODY:%s\n", u16tstr(entry->name, 0x106).c_str());
			delete entry;
			themesScanned = true;
			return;
		}

		Theme theme = {
			u16tstr(entry->name, 0x106),
			s2ws(u16tstr(entry->name, 0x106)),
			L"no desc",
			i18n("unknown"),
			NULL,
			NULL,
			true,
			true,
			true,
			false,
			false,
			false
		};

		LightLock_Init(&theme.lock);

		// check if bgm exists
		if(!unzLocateFile(zipFile, "bgm.ogg", 0))
			theme.hasBGM = true;

		// check if preview exists
		if((!unzLocateFile(zipFile, "Preview.png", 0) || !unzLocateFile(zipFile, "preview.png", 0)) && !unzOpenCurrentFile(zipFile)){
			char* tmp_buf = new char[24];
			int err = unzReadCurrentFile(zipFile, tmp_buf, 24);
			if(err == 24){
				int width = (tmp_buf[16]<<24) + (tmp_buf[17]<<16) + (tmp_buf[18]<<8) + (tmp_buf[19]<<0);
				int height = (tmp_buf[20]<<24) + (tmp_buf[21]<<16) + (tmp_buf[22]<<8) + (tmp_buf[23]<<0);

				if(width <= 412 && height <= 482)
					theme.hasPreview = true;
			}

			delete[] tmp_buf;
			unzCloseCurrentFile(zipFile);
		}

		if(!unzLocateFile(zipFile, "info.smdh", 0)){
			if(!unzOpenCurrentFile(zipFile)){
				vector<char> smdhData;
				if(zippedFileToVector(zipFile, smdhData)){
					unzCloseCurrentFile(zipFile);
				} else {
					if(smdhData.size() == 0x36C0){
						string tmpTitle = u16tstr((u16*)&smdhData[0x8], 64);
						string tmpDescription = u16tstr((u16*)&smdhData[0x8 + 128], 128);
						string tmpAuthor = u16tstr((u16*)&smdhData[0x8 + 128 + 256], 64);

						if(tmpTitle.size() != 0)
							theme.title = s2ws(tmpTitle);

						if(tmpDescription.size() != 0)
							theme.description = s2ws(tmpDescription);

						if(tmpAuthor.size() != 0 && tmpAuthor.size() < 20)
							theme.author = s2ws(tmpAuthor);

						// detects default icons
						if(smdhData[0x24C0] != '\x9D' && smdhData[0x24C1] != '\x04' && smdhData[0x24C0] != '\xBF' && smdhData[0x24C1] != '\x0D'){
							theme.icon = sf2d_create_texture(48, 48, TEXFMT_RGB565, SF2D_PLACE_RAM);
							u16* dst = (u16*)(theme.icon->tex.data + 64 * 8 * 2 * sizeof(u16));
							u16* src = (u16*)(&smdhData[0x24C0]);
							for (u8 j = 0; j < 48; j += 8){
								memcpy(dst, src, 48 * 8 * sizeof(u16));
								src += 48 * 8;
								dst += 64 * 8;
							}
						}
					}

					unzCloseCurrentFile(zipFile);
				}
			}
		}

		unzCloseCurrentFile(zipFile);
		unzClose(zipFile);

		themes.push_back(theme);

		if(themes.size() < 3){
			themes[themes.size()-1].previewIsLoaded = true;
			queueTask(loadPreview, (void*)(themes.size()-1), true);
		}
	}

	delete entry;

	themesScanned = true;
}

bool sortDirectoryEntriesByName(FS_DirectoryEntry* a, FS_DirectoryEntry* b){
	string first = u16tstr(a->name, 0x106);
	string second = u16tstr(b->name, 0x106);

	transform(first.begin(), first.end(), first.begin(),(int (*)(int))toupper);
	transform(second.begin(), second.end(), second.begin(),(int (*)(int))toupper);

	return first < second;
}

void scanThemes(void*){
	Result ret;

	Handle themeDir;
	if(FSUSER_OpenDirectory(&themeDir, ARCHIVE_SD, fsMakePath(PATH_ASCII, "/Themes/")))
		return throwError(i18n("err_fail_open", "/Themes/"));

	vector<FS_DirectoryEntry*> entries;

	while (true){
		u32 entriesRead;
		FS_DirectoryEntry* entry = new FS_DirectoryEntry;
		FSDIR_Read(themeDir, &entriesRead, 1, entry);
		if(entriesRead)
			entries.push_back(entry);
		else
			break;
	}

	FSDIR_Close(themeDir);

	if(entries.size() == 0)
		themesScanned = true;
	else {
		sort(entries.begin(), entries.end(), sortDirectoryEntriesByName);

		for (size_t i = 0; i < entries.size(); i++)
			queueTask(loadTheme, (void*)entries[i], false);
	}
}

void loadPreview(void* id){
	if(!themes[(int)id].hasPreview)
		return;

	if(!themes[(int)id].isZip){
		while (true){
			if(!LightLock_TryLock(&themes[(int)id].lock))
				break;

			svcSleepThread(0x400000LL);
		}

		sf2d_texture* tmp;
		Result res = load_png((string("/Themes/") + themes[(int)id].fileName + "/Preview.png").c_str(), &tmp);

		if(!res){
			themes[(int)id].preview = tmp;
			C3D_TexSetFilter(&themes[(int)id].preview->tex, GPU_LINEAR, GPU_LINEAR);
		} else
			themes[(int)id].hasPreview = false;

		LightLock_Unlock(&themes[(int)id].lock);
	} else {
		// open zip
		while (true){
			if(!LightLock_TryLock(&themes[(int)id].lock))
				break;

			svcSleepThread(0x400000LL);
		}

		unzFile zipFile = unzOpen(string("/Themes/" + string(themes[(int)id].fileName)).c_str());

		if(!zipFile){
			LightLock_Unlock(&themes[(int)id].lock);
			return;
		}

		vector<char> pngData;

		if(unzLocateFile(zipFile, "Preview.png", 0) && unzLocateFile(zipFile, "preview.png", 0)){
			unzClose(zipFile);
			themes[(int)id].hasPreview = false;
			LightLock_Unlock(&themes[(int)id].lock);
			return;
		}

		if(unzOpenCurrentFile(zipFile)){
			unzClose(zipFile);
			themes[(int)id].hasPreview = false;
			LightLock_Unlock(&themes[(int)id].lock);
			return;
		}

		if(zippedFileToVector(zipFile, pngData)){
			unzCloseCurrentFile(zipFile);
			unzClose(zipFile);
			themes[(int)id].hasPreview = false;
			LightLock_Unlock(&themes[(int)id].lock);
			return;
		}

		unzCloseCurrentFile(zipFile);
		unzClose(zipFile);

		sf2d_texture* tmp;
		Result res = load_png_mem(pngData, &tmp);

		if(!res){
			themes[(int)id].preview = tmp;
			C3D_TexSetFilter(&themes[(int)id].preview->tex, GPU_LINEAR, GPU_LINEAR);
		} else
			themes[(int)id].hasPreview = false;

		LightLock_Unlock(&themes[(int)id].lock);
	}
}

void checkInfosToBeLoaded(int id){
	for (int i = 0; i < themes.size(); i++){
		if(!themes[i].hasPreview)
			continue;

		if(i < (id - 4) || i > (id + 4)){
			// just in case the user is scrolling too fast
			while (true){
				if(!LightLock_TryLock(&taskQueueLock))
					break;

				svcSleepThread(0x400000LL);
			}

			for (size_t j = 0; j < taskQueue.size(); j++){
				if(taskQueue[j].arg == (void*)i){
					if(taskQueue[j].entrypoint == loadPreview){
						printf("DELETING TASK FOR THEME %i\n", i);
						themes[i].previewIsLoaded = false;
						taskQueue.erase(taskQueue.begin() + j);
					}
				}
			}

			LightLock_Unlock(&taskQueueLock);

			if(themes[i].hasPreview && themes[i].preview != NULL && !LightLock_TryLock(&themes[i].lock)){
				try {
					themes[i].previewIsLoaded = false;
					sf2d_free_texture(themes[i].preview);
					themes[i].preview = NULL;
				} catch (...){
					LightLock_Unlock(&themes[i].lock);
					printf("error\n");
				}

				LightLock_Unlock(&themes[i].lock);
			}
		} else {
			if(!LightLock_TryLock(&themes[i].lock)){
				if(!themes[i].previewIsLoaded){
					themes[i].previewIsLoaded = true;
					queueTask(loadPreview, (void*)i, true);
				}

				LightLock_Unlock(&themes[i].lock);
			}
		}
	}
}

void installTheme(void* noBGM){
	installProgress = i18n("install_reading", "body_LZ.bin");

	Result ret = 0;
	vector<char> bodyData;
	vector<char> BGMData;

	// Load data
	if(!themes[currentSelectedItem].isZip){
		if(fileToVector(string("/Themes/") + themes[currentSelectedItem].fileName + "/body_LZ.bin", bodyData))
			return throwError(i18n("err_fail_open", "body_LZ.bin"));

		installProgress += wstring(L"\n") + i18n("install_reading", "BGM.bcstm");

		if(!(bool)noBGM)
			fileToVector(string("/Themes/") + themes[currentSelectedItem].fileName + "/bgm.bcstm", BGMData);
	} else {
		unzFile zipFile = unzOpen(string("/Themes/" + string(themes[currentSelectedItem].fileName)).c_str());

		if(!zipFile)
			return throwError(i18n("err_fail_open", "ZIP"));

		if(unzLocateFile(zipFile, "body_LZ.bin", 0) && unzLocateFile(zipFile, "body_lz.bin", 0))
			return throwError(L"Can't find body_LZ.bin file in ZIP");

		if(unzOpenCurrentFile(zipFile))
			return throwError(L"Can't open body_LZ.bin file in ZIP");

		ret = zippedFileToVector(zipFile, bodyData);
		if(ret)
			return throwError(string(string("Can't read body_LZ.bin from ZIP -- error code ") + to_string(ret)).c_str());

		unzCloseCurrentFile(zipFile);

		installProgress += wstring(L"\n") + i18n("install_reading", "BGM.bcstm");

		if(!(bool)noBGM && !unzLocateFile(zipFile, "bgm.bcstm", 0)){
			if(unzOpenCurrentFile(zipFile))
				return throwError(L"Can't open bgm.bcstm file in ZIP");

			ret = zippedFileToVector(zipFile, BGMData);
			if(ret)
				return throwError(string(string("Can't read bgm.bcstm from ZIP -- error code ") + to_string(ret)).c_str());

			unzCloseCurrentFile(zipFile);
		}

		installProgress += wstring(L"\n") + i18n("install_reading", "SaveData.dat");

		unzClose(zipFile);
	}

	// Update SaveData.dat
	u8* saveDataDat_buf;
	u64 saveDataDat_size;
	Handle saveDataDat_handle;

	ret = FSUSER_OpenFile(&saveDataDat_handle, ARCHIVE_HomeExt, fsMakePath(PATH_ASCII, "/SaveData.dat"), FS_OPEN_READ | FS_OPEN_WRITE, 0);
	if(ret){
		return throwError(i18n("err_fail_open", "SaveData.dat") + wstring(L" ") + i18n("err_try_default"), ret);}

	FSFILE_GetSize(saveDataDat_handle, &saveDataDat_size);

	saveDataDat_buf = new u8[saveDataDat_size];
	ret = FSFILE_Read(saveDataDat_handle, nullptr, 0, saveDataDat_buf, (u32)saveDataDat_size);
	if(ret){
		free(saveDataDat_buf);
		FSFILE_Close(saveDataDat_handle);
		return throwError(i18n("err_fail_read", "SaveData.dat") + wstring(L" ") + i18n("err_try_default"), ret);
	}

	installProgress += wstring(L"\n") + i18n("install_writing", "SaveData.dat");

	saveDataDat_buf[0x141b] = 0; // turn off shuffle
	memset(&saveDataDat_buf[0x13b8], 0, 8); // clear the regular theme structure
	saveDataDat_buf[0x13bd] = 3; // make it persistent
	saveDataDat_buf[0x13b8] = 0xff; // theme index

	ret = FSFILE_Write(saveDataDat_handle, nullptr, 0, saveDataDat_buf, saveDataDat_size, FS_WRITE_FLUSH);
	if(ret){
		free(saveDataDat_buf);
		FSFILE_Close(saveDataDat_handle);
		return throwError(i18n("err_fail_write", "SaveData.dat") + wstring(L" ") + i18n("err_try_default"), ret);
	}

	free(saveDataDat_buf);
	FSFILE_Close(saveDataDat_handle);

	// Inject body_lz.bin into BodyCache.bin
	Handle bodyCacheBin_handle;

	if(FSUSER_OpenFile(&bodyCacheBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache.bin"), FS_OPEN_WRITE, 0)){
		FSUSER_DeleteFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache.bin"));
		ret = FSUSER_CreateFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache.bin"), 0, (u64)0x150000);
		if(ret){
			FSFILE_Close(bodyCacheBin_handle);
			return throwError(L"Failed to create BodyCache.bin." + wstring(L" ") + i18n("err_try_default"), ret);
		}

		ret = FSUSER_OpenFile(&bodyCacheBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache.bin"), FS_OPEN_WRITE, 0);
		if(ret){
			FSFILE_Close(bodyCacheBin_handle);
			return throwError(i18n("err_fail_open", "BodyCache.bin") + wstring(L" ") + i18n("err_try_default"), ret);
		}
	}

	installProgress += wstring(L"\n") + i18n("install_writing", "BodyCache.bin");

	ret = FSFILE_Write(bodyCacheBin_handle, nullptr, 0, &bodyData[0], (u64)bodyData.size(), FS_WRITE_FLUSH);
	if(ret){
		FSFILE_Close(bodyCacheBin_handle);
		return throwError(i18n("err_fail_write", "BodyCache.bin") + wstring(L" ") + i18n("err_try_default"), ret);
	}

	FSFILE_Close(bodyCacheBin_handle);

	// Inject bgm.bcstm into BgmCache.bin
	Handle bgmCacheBin_handle;

	if(FSUSER_OpenFile(&bgmCacheBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BgmCache.bin"), FS_OPEN_WRITE, 0)){
		FSUSER_DeleteFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BgmCache.bin"));
		ret = FSUSER_CreateFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BgmCache.bin"), 0, (u64)3371008);
		if(ret){
			FSFILE_Close(bgmCacheBin_handle);
			return throwError(L"Failed to create BgmCache.bin." + wstring(L" ") + i18n("err_try_default"), ret);
		}

		ret = FSUSER_OpenFile(&bgmCacheBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BgmCache.bin"), FS_OPEN_WRITE, 0);
		if(ret){
			FSFILE_Close(bgmCacheBin_handle);
			return throwError(i18n("err_fail_open", "BgmCache.bin") + wstring(L" ") + i18n("err_try_default"), ret);
		}
	}

	installProgress += wstring(L"\n") + i18n("install_writing", "BgmCache.bin");

	if(BGMData.size() != 0)
		if(BGMData.size() > 3371008){
			FSFILE_Close(bgmCacheBin_handle);
			return throwError(i18n("err_bgm_too_big"));
		} else
			ret = FSFILE_Write(bgmCacheBin_handle, nullptr, 0, &BGMData[0], BGMData.size(), FS_WRITE_FLUSH);
	else {
		char* empty = new char[3371008]();
		ret = FSFILE_Write(bgmCacheBin_handle, nullptr, 0, empty, (u64)3371008, FS_WRITE_FLUSH);
		delete[] empty;
	}

	if(ret){
		FSFILE_Close(bgmCacheBin_handle);
		return throwError(i18n("err_fail_write", "BgmCache.bin") + wstring(L" ") + i18n("err_try_default"), ret);
	}

	FSFILE_Close(bgmCacheBin_handle);
	// Update ThemeManage.bin
	u8* themeManageBin_buf = new u8[0x800];
	Handle themeManageBin_handle;

	//FSUSER_DeleteFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/ThemeManage.bin"));
	//if(FSUSER_CreateFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/ThemeManage.bin"), 0, 0x800))
	//	return throwError(L"Failed to create ThemeManage.bin." + wstring(L" ") + i18n("err_try_default"));

	ret = FSUSER_OpenFile(&themeManageBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/ThemeManage.bin"), FS_OPEN_WRITE, 0);
	if(ret){
		FSFILE_Close(themeManageBin_handle);
		return throwError(i18n("err_fail_open", "ThemeManage.bin") + wstring(L" ") + i18n("err_try_default"), ret);
	}

	ret = FSFILE_Read(themeManageBin_handle, nullptr, 0, themeManageBin_buf, (u32)0x800);
	if(ret){
		FSFILE_Close(themeManageBin_handle);
		return throwError(i18n("err_fail_read", "ThemeManage.bin") + wstring(L" ") + i18n("err_try_default"), ret);
	}

	installProgress += wstring(L"\n") + i18n("install_writing", "ThemeManage.bin");

	themeManageBin_buf[0x00] = 1;
	themeManageBin_buf[0x01] = 0;
	themeManageBin_buf[0x02] = 0;
	themeManageBin_buf[0x03] = 0;
	themeManageBin_buf[0x04] = 0;
	themeManageBin_buf[0x05] = 0;
	themeManageBin_buf[0x06] = 0;
	themeManageBin_buf[0x07] = 0;
	u32* bodySize_loc = (u32*)(&themeManageBin_buf[0x08]);
	u32* BGMSize_loc = (u32*)(&themeManageBin_buf[0x0C]);
	*bodySize_loc = (u32)bodyData.size();
	*BGMSize_loc = (u32)BGMData.size();
	themeManageBin_buf[0x10] = 0xFF;
	themeManageBin_buf[0x14] = 0x01;
	themeManageBin_buf[0x18] = 0xFF;
	themeManageBin_buf[0x1D] = 0x02;
	memset(&themeManageBin_buf[0x338], 0, 4);
	memset(&themeManageBin_buf[0x340], 0, 4);
	memset(&themeManageBin_buf[0x360], 0, 4);
	memset(&themeManageBin_buf[0x368], 0, 4);

	ret = FSFILE_Write(themeManageBin_handle, nullptr, 0, themeManageBin_buf, 0x800, FS_WRITE_FLUSH);
	if(ret){
		FSFILE_Close(themeManageBin_handle);
		return throwError(i18n("err_fail_write", "ThemeManage.bin") + wstring(L" ") + i18n("err_try_default"), ret);
	}

	FSFILE_Close(themeManageBin_handle);

	isInstalling = false;
	installProgress = L"";
}

void installShuffle(void*){
	installProgress = i18n("install_reading", "body_LZ.bin\n& BGM.bcstm");

	Result ret = 0;
	vector<int> themesToBeShuffled;
	vector<int> themesToBeShuffled_bodySize;
	vector<int> themesToBeShuffled_BGMSize;

	for (size_t i = 0; i < themes.size(); i++)
		if(themes[i].toShuffle)
			themesToBeShuffled.push_back(i);

	// Update SaveData.dat
	u8* saveDataDat_buf;
	u64 saveDataDat_size;
	Handle saveDataDat_handle;

	ret = FSUSER_OpenFile(&saveDataDat_handle, ARCHIVE_HomeExt, fsMakePath(PATH_ASCII, "/SaveData.dat"), FS_OPEN_READ | FS_OPEN_WRITE, 0);
	if(ret)
		return throwError(i18n("err_fail_open", "SaveData.dat") + wstring(L" ") + i18n("err_try_default"), ret);

	FSFILE_GetSize(saveDataDat_handle, &saveDataDat_size);

	saveDataDat_buf = new u8[saveDataDat_size];
	ret = FSFILE_Read(saveDataDat_handle, nullptr, 0, saveDataDat_buf, (u32)saveDataDat_size);
	if(ret){
		FSFILE_Close(saveDataDat_handle);
		return throwError(i18n("err_fail_read", "SaveData.dat") + wstring(L" ") + i18n("err_try_default"), ret);
	}

	installProgress += wstring(L"\n") + i18n("install_writing", "SaveData.dat");

	saveDataDat_buf[0x141b] = 1; // turn on shuffle
	memset(&saveDataDat_buf[0x13b8], 0, 8); // clear the regular theme structure
	saveDataDat_buf[0x13bd] = 3; // make it persistent
	saveDataDat_buf[0x13b8] = 0xff; // theme index

	for (size_t i = 0; i < 10; i++) {
		memset(&saveDataDat_buf[0x13C0 + 0x8 * i], 0, 8);
		if(themesToBeShuffled.size() > i){
			saveDataDat_buf[0x13C0 + 0x8 * i] = i;
			saveDataDat_buf[0x13C0 + 0x8 * i + 5] = 3;
		}
	}

	ret = FSFILE_Write(saveDataDat_handle, nullptr, 0, saveDataDat_buf, saveDataDat_size, FS_WRITE_FLUSH);
	if(ret){
		FSFILE_Close(saveDataDat_handle);
		return throwError(i18n("err_fail_write", "SaveData.dat") + wstring(L" ") + i18n("err_try_default"), ret);
	}

	delete[] saveDataDat_buf;
	FSFILE_Close(saveDataDat_handle);

	// Inject body_lz.bin into BodyCache.bin
	Handle bodyCacheBin_handle;
	if(FSUSER_OpenFile(&bodyCacheBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache_rd.bin"), FS_OPEN_WRITE, 0)){
		FSUSER_DeleteFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache_rd.bin"));
		ret = FSUSER_CreateFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache_rd.bin"), 0, 0x150000*10);
		if(ret)
			return throwError(L"Failed to create BodyCache_rd.bin." + wstring(L" ") + i18n("err_try_default"), ret);

		ret = FSUSER_OpenFile(&bodyCacheBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache_rd.bin"), FS_OPEN_WRITE, 0);
		if(ret)
			return throwError(i18n("err_fail_open", "BodyCache_rd.bin") + wstring(L" ") + i18n("err_try_default"), ret);
	}

	installProgress += wstring(L"\n") + i18n("install_writing", "BodyCache_rd.bin");

	for (size_t i = 0; i < 10; i++) {
		if(themesToBeShuffled.size() > i){
			vector<char> tmpBodyData;
			if(!themes[themesToBeShuffled[i]].isZip){
				if(fileToVector(string("/Themes/") + themes[themesToBeShuffled[i]].fileName + "/body_LZ.bin", tmpBodyData)){
					FSFILE_Close(bodyCacheBin_handle);
					return throwError(L"Failed to open body_LZ.bin file");
				}
			} else {
				unzFile zipFile = unzOpen(string("/Themes/" + string(themes[themesToBeShuffled[i]].fileName)).c_str());

				if(!zipFile){
					FSFILE_Close(bodyCacheBin_handle);
					return throwError(L"Failed to open ZIP file");
				}

				if(unzLocateFile(zipFile, "body_LZ.bin", 0) && unzLocateFile(zipFile, "body_lz.bin", 0)){
					FSFILE_Close(bodyCacheBin_handle);
					return throwError(L"Can't find body_LZ.bin file in ZIP");
				}

				if(unzOpenCurrentFile(zipFile)){
					FSFILE_Close(bodyCacheBin_handle);
					return throwError(L"Can't open body_LZ.bin file in ZIP");
				}

				ret = zippedFileToVector(zipFile, tmpBodyData);
				if(ret){
					FSFILE_Close(bodyCacheBin_handle);
					return throwError(string(string("Can't read body_LZ.bin from ZIP -- error code ") + to_string(ret)).c_str());
				}

				unzCloseCurrentFile(zipFile);
				unzClose(zipFile);
			}

			themesToBeShuffled_bodySize.push_back(tmpBodyData.size());

			ret = FSFILE_Write(bodyCacheBin_handle, nullptr, 0x150000 * i, &tmpBodyData[0], tmpBodyData.size(), FS_WRITE_FLUSH);
			if(ret){
				FSFILE_Close(bodyCacheBin_handle);
				return throwError(i18n("err_fail_write", "BodyCache_rd.bin") + wstring(L" ") + i18n("err_try_default"), ret);
			}
		} else {
			char* empty = new char[0x150000]();
			FSFILE_Write(bodyCacheBin_handle, nullptr, 0x150000 * i, empty, 0x150000, FS_WRITE_FLUSH);
			delete[] empty;
		}
	}

	FSFILE_Close(bodyCacheBin_handle);

	installProgress += wstring(L"\n") + i18n("install_writing", "BgmCache_**.bin");

	// Inject bgm.bcstm into BgmCache.bin
	for (size_t i = 0; i < 10; i++){
		Handle bgmCacheBin_handle;
		if(FSUSER_OpenFile(&bgmCacheBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, ("/BgmCache_0" + to_string(i) + ".bin").c_str()), FS_OPEN_WRITE, 0)){
			FSUSER_DeleteFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, ("/BgmCache_0" + to_string(i) + ".bin").c_str()));
			ret = FSUSER_CreateFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, ("/BgmCache_0" + to_string(i) + ".bin").c_str()), 0, (u64)3371008);
			if(ret)
				return throwError(L"Failed to create BgmCache_0" + to_wstring(i) + L".bin." + wstring(L" ") + i18n("err_try_default"), ret);

			ret = FSUSER_OpenFile(&bgmCacheBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, ("/BgmCache_0" + to_string(i) + ".bin").c_str()), FS_OPEN_WRITE, 0);
			if(ret)
				return throwError(i18n("err_fail_open", "BgmCache_0" + to_string(i) + ".bin") + wstring(L" ") + i18n("err_try_default"), ret);
		}

		if(themesToBeShuffled.size() > i && !themes[themesToBeShuffled[i]].shuffleNoBGM){
			vector<char> tmpBGMData;
			if(!themes[themesToBeShuffled[i]].isZip){
				fileToVector(string("/Themes/") + themes[themesToBeShuffled[i]].fileName + "/bgm.bcstm", tmpBGMData);
			} else {
				unzFile zipFile = unzOpen(string("/Themes/" + string(themes[themesToBeShuffled[i]].fileName)).c_str());

				if(!zipFile)
					return throwError(L"Failed to open ZIP file");

				if(!unzLocateFile(zipFile, "bgm.bcstm", 0)){
					if(unzOpenCurrentFile(zipFile))
						return throwError(L"Can't open bgm.bcstm file in ZIP");

					ret = zippedFileToVector(zipFile, tmpBGMData);
					if(ret)
						return throwError(string(string("Can't read bgm.bcstm from ZIP -- error code ") + to_string(ret)).c_str());

					unzCloseCurrentFile(zipFile);
				}

				unzClose(zipFile);
			}

			themesToBeShuffled_BGMSize.push_back(tmpBGMData.size());

			if(tmpBGMData.size() == 0){
				char* empty = new char[3371008]();
				ret = FSFILE_Write(bgmCacheBin_handle, nullptr, 0, empty, (u64)3371008, FS_WRITE_FLUSH);
				delete[] empty;
			} else {
				if(tmpBGMData.size() > 3371008)
					return throwError(i18n("err_bgm_too_big"));
				else
					ret = FSFILE_Write(bgmCacheBin_handle, nullptr, 0, &tmpBGMData[0], tmpBGMData.size(), FS_WRITE_FLUSH);
			}
		} else {
			char* empty = new char[3371008]();
			ret = FSFILE_Write(bgmCacheBin_handle, nullptr, 0, empty, (u64)3371008, FS_WRITE_FLUSH);
			delete[] empty;
		}

		if(ret)
			return throwError(i18n("err_fail_write", "BgmCache.bin") + wstring(L" ") + i18n("err_try_default"), ret);

		FSFILE_Close(bgmCacheBin_handle);
	}

	// Update ThemeManage.bin
	u8* themeManageBin_buf = new u8[0x800];
	Handle themeManageBin_handle;

	//FSUSER_DeleteFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/ThemeManage.bin"));
	//if(FSUSER_CreateFile(ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/ThemeManage.bin"), 0, 0x800))
	//	return throwError(L"Failed to create ThemeManage.bin." + wstring(L" ") + i18n("err_try_default"));

	ret = FSUSER_OpenFile(&themeManageBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/ThemeManage.bin"), FS_OPEN_WRITE, 0);
	if(ret)
		return throwError(i18n("err_fail_open", "ThemeManage.bin") + wstring(L" ") + i18n("err_try_default"), ret);

	ret = FSFILE_Read(themeManageBin_handle, nullptr, 0, themeManageBin_buf, (u32)0x800);
	if(ret)
		return throwError(i18n("err_fail_read", "ThemeManage.bin") + wstring(L" ") + i18n("err_try_default"), ret);

	installProgress += wstring(L"\n") + i18n("install_writing", "ThemeManage.bin");

	themeManageBin_buf[0x00] = 1;
	themeManageBin_buf[0x01] = 0;
	themeManageBin_buf[0x02] = 0;
	themeManageBin_buf[0x03] = 0;
	themeManageBin_buf[0x04] = 0;
	themeManageBin_buf[0x05] = 0;
	themeManageBin_buf[0x06] = 0;
	themeManageBin_buf[0x07] = 0;
	u32* bodySize_loc = (u32*)(&themeManageBin_buf[0x08]);
	u32* BGMSize_loc = (u32*)(&themeManageBin_buf[0x0C]);
	*bodySize_loc = (u32)0;
	*BGMSize_loc = (u32)0;
	themeManageBin_buf[0x10] = 0xFF;
	themeManageBin_buf[0x14] = 0x01;
	themeManageBin_buf[0x18] = 0xFF;
	themeManageBin_buf[0x1D] = 0x02;
	for (size_t i = 0; i < 10; i++) {
		u32* bodySize_loc = (u32*)(&themeManageBin_buf[0x338 + 4 * i]);
		u32* BGMSize_loc = (u32*)(&themeManageBin_buf[0x360 + 4 * i]);
		if(themesToBeShuffled.size() > i){
			*bodySize_loc = (u32)themesToBeShuffled_bodySize[i];
			*BGMSize_loc = (u32)themesToBeShuffled_BGMSize[i];
		} else {
			*bodySize_loc = (u32)0;
			*BGMSize_loc = (u32)0;
		}
	}

	ret = FSFILE_Write(themeManageBin_handle, nullptr, 0, themeManageBin_buf, 0x800, FS_WRITE_FLUSH);
	if(ret)
		return throwError(i18n("err_fail_write", "ThemeManage.bin") + wstring(L" ") + i18n("err_try_default"), ret);

	FSFILE_Close(themeManageBin_handle);

	exitShuffleMode();

	isInstalling = false;
	installProgress = L"";
}

void exitShuffleMode(){
	for (size_t i = 0; i < themes.size(); i++){
		themes[i].toShuffle = false;
		themes[i].shuffleNoBGM = false;
	}

	shuffleMode = false;
}

void deleteTheme(){
	if(!themes[currentSelectedItem].isZip)
		FSUSER_DeleteDirectoryRecursively(ARCHIVE_SD, fsMakePath(PATH_UTF16, (u"/Themes/" + strtu16str(themes[currentSelectedItem].fileName)).data()));
	else
		FSUSER_DeleteFile(ARCHIVE_SD, fsMakePath(PATH_UTF16, (u"/Themes/" + strtu16str(themes[currentSelectedItem].fileName)).data()));

	themes.erase(themes.begin() + currentSelectedItem);
	selectTheme(max(0, currentSelectedItem - 1));
	deletePrompt = false;
}

void dumpTheme(){
	Result ret;

	string num;
	for (size_t i = 0; i < 100; i++){
		char str[3];
		snprintf(str, 3, "%02d", i);
		if(!FSUSER_CreateDirectory(ARCHIVE_SD, fsMakePath(PATH_ASCII, (string("/Themes/Themely_Dump") + str).c_str()), FS_ATTRIBUTE_DIRECTORY)){
			num = string(str);
			break;
		}
	}

	if(num.size() == 0)
		return throwError(L"Ran out of numbers for the dump name. Clean that shit");

	u8* themeManageBin_buf = new u8[0x800];
	Handle themeManageBin_handle;

	u8* bodyCacheBin_buf;
	u64 bodyCacheBin_size;
	Handle bodyCacheBin_handle;

	u8* bgmCacheBin_buf;
	u64 bgmCacheBin_size;
	Handle bgmCacheBin_handle;

	Handle bodyOutput_handle;
	Handle bgmOutput_handle;

	// get size of body and bgm
	ret = FSUSER_OpenFile(&themeManageBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/ThemeManage.bin"), FS_OPEN_WRITE, 0);
	if(ret)
		return throwError(L"Failed to open ThemeManage.bin", ret);

	ret = FSFILE_Read(themeManageBin_handle, nullptr, 0, themeManageBin_buf, (u32)0x800);
	if(ret)
		return throwError(L"Failed to read ThemeManage.bin", ret);

	bodyCacheBin_size = (u64)(*((u32*)&themeManageBin_buf[0x08]));
	bgmCacheBin_size = (u64)(*((u32*)&themeManageBin_buf[0x0C]));

	delete[] themeManageBin_buf;
	FSFILE_Close(themeManageBin_handle);

	// get body
	ret = FSUSER_OpenFile(&bodyCacheBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BodyCache.bin"), FS_OPEN_READ, 0);
	if(ret)
		return throwError(L"Failed to open BodyCache.bin. Perhaps you don't have a theme set?", ret);

	bodyCacheBin_buf = new u8[bodyCacheBin_size];
	ret = FSFILE_Read(bodyCacheBin_handle, nullptr, 0, bodyCacheBin_buf, (u32)bodyCacheBin_size);
	if(ret)
		return throwError(L"Failed to read BodyCache.bin. Perhaps you don't have a theme set?", ret);

	ret = FSUSER_CreateFile(ARCHIVE_SD, fsMakePath(PATH_ASCII, ("/Themes/Themely_Dump" + num + "/body_LZ.bin").c_str()), 0, (u64)bodyCacheBin_size);
	if(ret)
		return throwError(L"Failed to create body_LZ.bin", ret);

	ret = FSUSER_OpenFile(&bodyOutput_handle, ARCHIVE_SD, fsMakePath(PATH_ASCII, ("/Themes/Themely_Dump" + num + "/body_LZ.bin").c_str()), FS_OPEN_WRITE, 0);
	if(ret)
		return throwError(L"Failed to open body_LZ.bin", ret);

	ret = FSFILE_Write(bodyOutput_handle, nullptr, 0, bodyCacheBin_buf, bodyCacheBin_size, FS_WRITE_FLUSH);
	if(ret)
		return throwError(L"Failed to write to body_LZ.bin", ret);

	delete[] bodyCacheBin_buf;

	// get music
	if(!FSUSER_OpenFile(&bgmCacheBin_handle, ARCHIVE_ThemeExt, fsMakePath(PATH_ASCII, "/BgmCache.bin"), FS_OPEN_READ, 0)){
		bgmCacheBin_buf = new u8[bgmCacheBin_size];
		if(!FSFILE_Read(bgmCacheBin_handle, nullptr, 0, bgmCacheBin_buf, (u32)bgmCacheBin_size)){
			if(!FSUSER_CreateFile(ARCHIVE_SD, fsMakePath(PATH_ASCII, ("/Themes/Themely_Dump" + num + "/bgm.bcstm").c_str()), 0, (u64)bgmCacheBin_size)){
				if(!FSUSER_OpenFile(&bgmOutput_handle, ARCHIVE_SD, fsMakePath(PATH_ASCII, ("/Themes/Themely_Dump" + num + "/bgm.bcstm").c_str()), FS_OPEN_WRITE | FS_OPEN_CREATE, 0)){
					FSFILE_Write(bgmOutput_handle, nullptr, 0, bgmCacheBin_buf, bgmCacheBin_size, FS_WRITE_FLUSH);

					FSFILE_Close(bgmOutput_handle);
				}
			}
		}

		delete[] bgmCacheBin_buf;
		FSFILE_Close(bgmCacheBin_handle);
	}

	FSFILE_Close(bodyCacheBin_handle);
	FSFILE_Close(bodyOutput_handle);

	Handle themeDir;
	if(FSUSER_OpenDirectory(&themeDir, ARCHIVE_SD, fsMakePath(PATH_ASCII, "/Themes/"))){
		downloading = -1;
		return throwError("Failed to open /Themes/");
	}

	FS_DirectoryEntry* foundEntry;

	while (true){
		u32 entriesRead;
		FS_DirectoryEntry* entry = new FS_DirectoryEntry;
		FSDIR_Read(themeDir, &entriesRead, 1, entry);
		if(entriesRead){
			if(!u16tstr(entry->name, 0x106).compare("Themely_Dump" + num)){
				foundEntry = entry;
				break;
			}
		} else
			break;
	}

	FSDIR_Close(themeDir);

	if(foundEntry){
		loadTheme((void*)foundEntry);
		//loadPreview((void*)themes.size() - 1);
	}

	dumpPrompt = false;
	selectTheme(themes.size() - 1);
}

void toggleBGM(){
	if(!themes[currentSelectedItem].hasBGM)
		return;

	if(!fileExists(u"/3ds/dspfirm.cdc"))
		return throwError(i18n("err_no_dsp"));

	string path;

	if(themes[currentSelectedItem].isZip){
		unzFile zipFile = unzOpen(string("/Themes/" + string(themes[currentSelectedItem].fileName)).c_str());

		if(!zipFile)
			return throwError(L"Failed to open ZIP file");

		if(unzLocateFile(zipFile, "bgm.ogg", 0))
			return throwError(L"Can't find bgm.ogg file in ZIP");

		if(unzOpenCurrentFile(zipFile))
			return throwError(L"Can't open bgm.ogg file in ZIP");

		std::vector<char> oggData;
		int ret = zippedFileToVector(zipFile, oggData);
		if(ret)
			return throwError(string(string("Can't read bgm.ogg from ZIP -- error code ") + to_string(ret)).c_str());

		unzCloseCurrentFile(zipFile);
		unzClose(zipFile);

		remove("/3ds/Themely/tmp/bgm.ogg");
		ofstream tmpOggFile("/3ds/Themely/tmp/bgm.ogg", ios::out | ios::binary);
		tmpOggFile.write(&oggData[0], oggData.size());
		tmpOggFile.close();

		path = "/3ds/Themely/tmp/bgm.ogg";
	} else
		path = string("/Themes/") + themes[currentSelectedItem].fileName + "/bgm.ogg";

	if(!audioIsPlaying && currentPlayingAudio == NULL){
		audioIsPlaying = true;
		AUDIO_load(path);
	} else
		AUDIO_stop();
}
