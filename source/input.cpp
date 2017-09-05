// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#include "input.h"

touchPosition initialTouch;
touchPosition lastTouch;

int XHeldLength = 0;
int UpHeldLength = 0;
int DownHeldLength = 0;

bool checkTouch(int ax1, int ay1, int ax2, int ay2){
	int ix = initialTouch.px;
	int iy = initialTouch.py;
	int lx = lastTouch.px;
	int ly = lastTouch.py;

	if(ax1 <= ix && ix <= ax2 + ax1 && ay1 <= iy && iy <= ay2 + ay1 &&
	   ax1 <= lx && lx <= ax2 + ax1 && ay1 <= ly && ly <= ay2 + ay1)
		return true;

	return false;
}

int INPUT_handle(){
	hidScanInput();

	u32 kDown = hidKeysDown();
	u32 kHeld = hidKeysHeld();
	u32 kUp = hidKeysUp();
	touchPosition touchPos;
	hidTouchRead(&touchPos);
	circlePosition circlePos;
	hidCircleRead(&circlePos);

	if(kDown & KEY_START)
		return -1;

	if(isError){
		if(!errorIsFatal && kUp & KEY_A){
			error = L"";
			isError = false;

			isInstalling = false;
		}

		return 0;
	}

	if(isInstalling || !themesScanned || downloading > -1)
		return 0;

	if(kDown & KEY_TOUCH){
		lastTouch = touchPos;
		initialTouch = touchPos;
	}

	if(update.size() != 0){
		if(kDown & KEY_A){
			update = "Updating...";
			updateTextOffset = 0;
			UI_update();
			installUpdate();
		}

		if(kDown & KEY_B)
			update = "";

		if(kHeld & KEY_CPAD_DOWN)
			updateTextOffset = fmax(((count(update.begin(), update.end(), '\n') + 1) * 13 - 200) * -1, updateTextOffset + circlePos.dy / 10);

		if(kHeld & KEY_CPAD_UP && updateTextOffset != 0)
			updateTextOffset = fmin(0, updateTextOffset + circlePos.dy / 10);

		return 0;
	}

	if(upgradeToAnemone){
		if(kUp & KEY_A){
			is_upgrading = true;
			UI_update();
			installAnemone();
		}
		else if(kUp & KEY_B)
			upgradeToAnemone = false;

		return 0;
	}

	if(deletePrompt){
		if(kUp & KEY_A)
			deleteTheme();
		else if(kUp & KEY_B)
			deletePrompt = false;

		return 0;
	}

	if(dumpPrompt){
		if(kUp & KEY_A)
			dumpTheme();
		else if(kUp & KEY_B)
			dumpPrompt = false;

		return 0;
	}

	if(kUp & KEY_B){
		if(shuffleMode)
			exitShuffleMode();

		if(QRMode)
			stopQRMode();
	}

	if(themesScanned){
		if(previewX == 8.f){
			if(!QRMode && themes.size() != 0){
				if(kDown & KEY_Y)
					toggleBGM();

				if(currentPlayingAudio || audioIsPlaying)
					return 0;

				if(kDown & KEY_X || kHeld & KEY_X)
					XHeldLength++;
				else
					XHeldLength = 0;

				if(XHeldLength >= 15)
					deletePrompt = true;

				if(kDown & KEY_DOWN || (DownHeldLength > 10 && DownHeldLength % 10 == 0))
					selectTheme(currentSelectedItem + 1);

				if(kDown & KEY_DOWN || kHeld & KEY_DOWN)
					DownHeldLength++;
				else
					DownHeldLength = 0;

				if(kDown & KEY_UP || (UpHeldLength > 10 && UpHeldLength % 10 == 0))
					selectTheme(currentSelectedItem - 1);

				if(kDown & KEY_UP || kHeld & KEY_UP)
					UpHeldLength++;
				else
					UpHeldLength = 0;

				if(kDown & KEY_LEFT)
					selectTheme(max(0, currentSelectedItem - 5));

				if(kDown & KEY_RIGHT)
					selectTheme(min((int)themes.size() - 1, currentSelectedItem + 5));

				if(kUp & KEY_A){
					if(shuffleMode){
						int selected = 0;
						for (size_t i = 0; i < themes.size(); i++)
							if(themes[i].toShuffle)
								selected++;

						if(!themes[currentSelectedItem].toShuffle && selected < 10){
							themes[currentSelectedItem].toShuffle = true;

							if(kHeld & KEY_R)
								themes[currentSelectedItem].shuffleNoBGM = true;
						} else {
							themes[currentSelectedItem].toShuffle = false;
							themes[currentSelectedItem].shuffleNoBGM = false;
						}
					} else {
						isInstalling = true;
						if(kHeld & KEY_R)
							queueTask(installTheme, (void*)true, true);
						else
							queueTask(installTheme, (void*)false, true);
					}
				}
			}

			if(kUp & KEY_TOUCH){
				if(shuffleMode){
					if(checkTouch(320 - 3*2 - 24*2, 3, 24, 24)){
						int selected = 0;
						for (size_t i = 0; i < themes.size(); i++)
							if(themes[i].toShuffle)
								selected++;

						if(selected > 1){
							isInstalling = true;
							queueTask(installShuffle, (void*)0, true);
						}
					}

					if(checkTouch(320 - 3 - 24, 3, 24, 24))
						exitShuffleMode();
				} else if(QRMode) {
					if(checkTouch(320 - 3 - 24, 3, 24, 24))
						stopQRMode();
				} else {
					if(checkTouch(320 - 3 - 24, 3, 24, 24))
						startQRMode();

					if(checkTouch(320 - 3*2 - 24*2, 3, 24, 24))
						dumpPrompt = true;

					if(checkTouch(85, 0, 150, 30))
						upgradeToAnemone = true;

					if(checkTouch(3, 3, 24, 24))
						shuffleMode = true;
				}
			}
		}

		if(kUp & KEY_X && !QRMode && themes.size() != 0)
			if((previewX == 8.f || previewX == 0.f) && themes[currentSelectedItem].hasPreview)
				toggleFullscreen();
	}

	if(kHeld & KEY_TOUCH)
		lastTouch = touchPos;

	return 0;
}
