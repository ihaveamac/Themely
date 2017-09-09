// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "network.h"

string update;
bool is_upgrading = false;
string updateDownloadURL;
int downloading = -1;

Result HTTPGet(vector<char>& returnedVec, string url, string* fileName, int* progress){
	printf("URL:%s\n", url.c_str());

	if(progress)
		*progress = 0;

	// thx https://github.com/devkitPro/3ds-examples/blob/master/network/http/source/main.c
	Result ret = 0;
	httpcContext context;
	string newurl;
	u8* buf, *lastbuf;
	u32 statuscode = 0,
		contentsize = 0,
		readsize = 0,
		size = 0;

	do {
		ret = httpcOpenContext(&context, HTTPC_METHOD_GET, url.c_str(), 1);
		ret = httpcSetSSLOpt(&context, SSLCOPT_DisableVerify);
		ret = httpcAddRequestHeaderField(&context, "User-Agent", (string("Themely/") + VERSION).c_str());
		ret = httpcAddRequestHeaderField(&context, "Accept-Language", "en");
		ret = httpcAddRequestHeaderField(&context, "Connection", "Keep-Alive");

		if(closing) return -1;

		ret = httpcBeginRequest(&context);
		if(ret != 0){
			httpcCloseContext(&context);
			return ret;
		}

		if(closing) return -1;

		ret = httpcGetResponseStatusCode(&context, &statuscode);
		if(ret != 0){
			httpcCloseContext(&context);
			return ret;
		}

		if((statuscode >= 301 && statuscode <= 303) || (statuscode >= 307 && statuscode <= 308)){
			newurl.clear();
			newurl.resize(0x1000);
			ret = httpcGetResponseHeader(&context, "Location", &newurl[0], 0x1000);
			url = newurl;
			httpcCloseContext(&context);
		}

		if(closing) return -1;
	} while ((statuscode >= 301 && statuscode <= 303) || (statuscode >= 307 && statuscode <= 308));

	if(statuscode != 200){
		httpcCloseContext(&context);
		return -1;
	}

	if(closing) return -1;

	ret = httpcGetDownloadSizeState(&context, NULL, &contentsize);
	if(ret){
		httpcCloseContext(&context);
		return ret;
	}

	if(closing) return -1;

	buf = (u8*)malloc(0x1000);
	if(buf == NULL){
		httpcCloseContext(&context);
		return -2;
	}

	if(fileName){
		char* tmpFileName = new char[1024];
		if(!httpcGetResponseHeader(&context, "Content-Disposition", tmpFileName, 1024)){
			string tmpFileNameStr = string(tmpFileName);
			regex filenameRegex("filename[^;=\\n]*=((['\"]).*?\\2|[^;\\n]*)");
			smatch regexMatch;

			if(regex_search(tmpFileNameStr, regexMatch, filenameRegex)){
				if(regexMatch.size() >= 2){
					tmpFileNameStr = regexMatch[1].str();

					string illegalChars = "\\/:?\"<>|*^";
					for (size_t i = 0; i < illegalChars.size(); i++)
						tmpFileNameStr.erase(remove(tmpFileNameStr.begin(), tmpFileNameStr.end(), illegalChars[i]), tmpFileNameStr.end());

					if(tmpFileNameStr.size() > 0)
						*fileName = tmpFileNameStr;
				}
			}
		}

		delete[] tmpFileName;
	}

	do {
		if(closing) return -1;

		ret = httpcDownloadData(&context, buf + size, 0x1000, &readsize);
		size += readsize;
		if(progress)
			*progress = min((int)floor((float)size / (float)contentsize * 100.f), 100);

		if (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING){
				lastbuf = buf;
				buf = (u8*)realloc(buf, size + 0x1000);
				if(buf == NULL){
					httpcCloseContext(&context);
					free(lastbuf);
					return ret;
				}
			}
	} while (ret == (s32)HTTPC_RESULTCODE_DOWNLOADPENDING);

	if(closing) return -1;

	if(ret != 0){
		httpcCloseContext(&context);
		free(buf);
		return ret;
	}

	if(closing) return -1;

	lastbuf = buf;
	buf = (u8*)realloc(buf, size);
	if(buf == NULL){
		httpcCloseContext(&context);
		free(lastbuf);
		return ret;
	}

	if(closing) return -1;

	httpcCloseContext(&context);

	for (int i = 0; i < size; i++)
		returnedVec.push_back(buf[i]);

	return 0;
}

void installAnemone(){
	// get the latest release
	vector<char> httpData;
	Result ret = HTTPGet(httpData, string("https://api.github.com/repos/astronautlevel2/Anemone3DS/releases"));
	if(ret){
		printf("Failed to check for an update.\n");
		return;
	}

	json releases;

	try {
		releases = json::parse(httpData);
	} catch (...) {
		printf("Failed to check for an update.\n");
		return;
	}

	for (size_t i = 0; i < releases[0]["assets"].size(); i++){
		string name = releases[0]["assets"][i]["name"];
		if(
			(envIsHomebrew() && name.substr(name.find(".") + 1) == "3dsx") ||
			(!envIsHomebrew() && name.substr(name.find(".") + 1) == "cia")
		)
			updateDownloadURL = releases[0]["assets"][i]["browser_download_url"];
	}

	// install Anemone3DS
	if(envIsHomebrew()){
		vector<char> threedsxData;
		Result ret = HTTPGet(threedsxData, updateDownloadURL);
		if(ret)
			return throwError(i18n("err_update_dl_fail") + L" " + i18n("err_update_manual"), ret, true);

		Handle threedsxHandle;
		FSUSER_DeleteFile(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Anemone3DS.3dsx"));
		FSUSER_DeleteFile(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Anemone3DS/Anemone3DS.3dsx"));
		if(FSUSER_CreateFile(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Anemone3DS.3dsx"), 0, (u64)threedsxData.size()))
			return throwError("Failed to create Anemone3DS.3dsx", 0, true);

		if(FSUSER_OpenFile(&threedsxHandle, ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Anemone3DS.3dsx"), FS_OPEN_WRITE, 0))
			return throwError("Failed to open Anemone3DS.3dsx", 0, true);

		if(FSFILE_Write(threedsxHandle, nullptr, 0, &threedsxData[0], (u64)threedsxData.size(), FS_WRITE_FLUSH))
			return throwError("Failed to write to Anemone3DS.3dsx.", 0, true);

		FSFILE_Close(threedsxHandle);
	} else {
		vector<char> ciaData;
		Result ret = HTTPGet(ciaData, updateDownloadURL);
		if(ret)
			return throwError(i18n("err_update_dl_fail") + L" " + i18n("err_update_titledb"), ret, true);

		amInit();
		Handle handle;
		AM_QueryAvailableExternalTitleDatabase(NULL);
		AM_StartCiaInstall(MEDIATYPE_SD, &handle);
		FSFILE_Write(handle, nullptr, 0, &ciaData[0], (u64)ciaData.size(), 0);
		AM_FinishCiaInstall(handle);
		// AM_DeleteAppTitle(MEDIATYPE_SD, 0x000400000374E500LL);
		amExit();
	}

	closing = true;
}

void checkForUpdate(void*){
	vector<char> httpData;
	bool bleeding = string(VERSION).substr(string(VERSION).find("-") + 1) == "git";
	Result ret = HTTPGet(httpData, string("https://api.github.com/repos/ihaveamac/Themely/releases"));
	if(ret){
		printf("Failed to check for an update.\n");
		return;
	}

	json releases;

	try {
		releases = json::parse(httpData);
	} catch (...) {
		printf("Failed to check for an update.\n");
		return;
	}

	if((!bleeding && releases[0]["tag_name"] != string("v") + VERSION) || (bleeding && releases[0]["tag_name"] != string(VERSION).substr(0, string(VERSION).find("-git")))){
		update = string("You have Themely version ") + VERSION + ".\n\n";
		update += releases[0]["tag_name"];
		update += ":\n";
		update += releases[0]["body"];
		update = update.substr(0, update.find("Issue/Suggestion tracker:"));
		if(bleeding)
			update += "\n\n";

		for (size_t i = 0; i < releases[0]["assets"].size(); i++){
			string name = releases[0]["assets"][i]["name"];
			if(
				(envIsHomebrew() && name.substr(name.find(".") + 1) == "3dsx") ||
				(!envIsHomebrew() && name.substr(name.find(".") + 1) == "cia")
			)
				updateDownloadURL = releases[0]["assets"][i]["browser_download_url"];
		}

		if(updateDownloadURL.size() == 0){
			update = "";
			return;
		}

		update += "--------------------------------------------------CHANGELOG--------------------------------------------------\n\n";

		for (size_t i = 1; i < releases.size(); i++){
			if((!bleeding && releases[i]["tag_name"] == string("v") + VERSION) || (bleeding && releases[i]["tag_name"] == string(VERSION).substr(0, string(VERSION).find("-git"))))
				update += "[CURRENT] ";

			update += releases[i]["tag_name"];
			update += ":\n";
			update += releases[i]["body"];
			update = update.substr(0, update.find("Issue/Suggestion tracker:"));
			if(bleeding)
				update += "\n\n";
		}
	}
}

void installUpdate(){
	if(envIsHomebrew()){
		vector<char> threedsxData;
		Result ret = HTTPGet(threedsxData, updateDownloadURL);
		if(ret)
			return throwError(i18n("err_update_dl_fail") + L" " + i18n("err_update_manual"), ret, true);

		Handle threedsxHandle;
		FSUSER_DeleteFile(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Themely.3dsx"));
		FSUSER_DeleteFile(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Themely/Themely.3dsx"));
		if(FSUSER_CreateFile(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Themely/Themely.3dsx"), 0, (u64)threedsxData.size()))
			return throwError("Failed to create Themely.3dsx", 0, true);

		if(FSUSER_OpenFile(&threedsxHandle, ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Themely/Themely.3dsx"), FS_OPEN_WRITE, 0))
			return throwError("Failed to open Themely.3dsx", 0, true);

		if(FSFILE_Write(threedsxHandle, nullptr, 0, &threedsxData[0], (u64)threedsxData.size(), FS_WRITE_FLUSH))
			return throwError("Failed to write to Themely.3dsx.", 0, true);

		FSFILE_Close(threedsxHandle);
	} else {
		vector<char> ciaData;
		Result ret = HTTPGet(ciaData, updateDownloadURL);
		if(ret)
			return throwError(i18n("err_update_dl_fail") + L" " + i18n("err_update_titledb"), ret, true);

		amInit();
		Handle handle;
		AM_QueryAvailableExternalTitleDatabase(NULL);
		AM_StartCiaInstall(MEDIATYPE_SD, &handle);
		FSFILE_Write(handle, nullptr, 0, &ciaData[0], (u64)ciaData.size(), 0);
		AM_FinishCiaInstall(handle);
		amExit();
	}

	closing = true;
}

void downloadThemeFromURL(void* url){
	vector<char> zipData;
	string fileName;
	Result ret = HTTPGet(zipData, string((char*)url), &fileName, &downloading);
	if(ret){
		downloading = -1;
		return throwError(i18n("err_zip_dl_fail"), ret);
	}

	if(fileName.size() == 0){
		downloading = -1;
		return throwError("Content-Disposition header not in response");
	}

	if(fileName.size() > 0x106){
		downloading = -1;
		return throwError("Filename must not exceed 262 characters");
	}

	if(!hasSuffix(fileName, ".zip")){
		downloading = -1;
		return throwError("Filename doesn't end with .zip");
	}

	printf("%s\n", fileName.c_str());

	// store it temporarily
	Handle tmpZip_handle;
	FSUSER_DeleteFile(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Themely/tmp.zip"));
	ret = FSUSER_CreateFile(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Themely/tmp.zip"), 0, (u64)zipData.size());
	if(ret){
		downloading = -1;
		return throwError("Failed to create temporary ZIP file", ret);
	}

	ret = FSUSER_OpenFile(&tmpZip_handle, ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Themely/tmp.zip"), FS_OPEN_WRITE, 0);
	if(ret){
		downloading = -1;
		return throwError("Failed to open temporary ZIP file", ret);
	}

	ret = FSFILE_Write(tmpZip_handle, nullptr, 0, &zipData[0], (u64)zipData.size(), FS_WRITE_FLUSH);
	if(ret){
		downloading = -1;
		return throwError("Failed to write to the temporary ZIP file", ret);
	}

	FSFILE_Close(tmpZip_handle);

	// verify if the zip is correct
	unzFile zipFile = unzOpen("/3ds/Themely/tmp.zip");
	if(!zipFile){
		downloading = -1;
		return throwError(i18n("err_zip_invalid"));
	}

	if(unzLocateFile(zipFile, "body_LZ.bin", 0) && unzLocateFile(zipFile, "body_lz.bin", 0)){
		unzClose(zipFile);
		downloading = -1;
		return throwError(i18n("err_zip_no_body"));
	}

	unzCloseCurrentFile(zipFile);
	unzClose(zipFile);

	// move file
	FSUSER_DeleteFile(ARCHIVE_SD, fsMakePath(PATH_UTF16, (u"/Themes/" + strtu16str(fileName)).c_str()));
	ret = FSUSER_RenameFile(ARCHIVE_SD, fsMakePath(PATH_ASCII, "/3ds/Themely/tmp.zip"), ARCHIVE_SD, fsMakePath(PATH_UTF16, (u"/Themes/" + strtu16str(fileName)).c_str()));
	if(ret){
		downloading = -1;
		return throwError("Failed to move from temporary path to /Themes", ret);
	}

	// scan theme
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
			if(!u16tstr(entry->name, 0x106).compare(fileName)){
				foundEntry = entry;
				break;
			}
		} else {
			downloading = -1;
			throwError("Can't find ZIP in /Themes");
			break;
		}
	}

	FSDIR_Close(themeDir);

	if(foundEntry){
		loadTheme((void*)foundEntry);
		loadPreview((void*)themes.size() - 1);
	}

	downloading = -1;
	selectTheme(themes.size() - 1);
}
