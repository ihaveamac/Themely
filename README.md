<h3 align="center"><img src="https://github.com/ihaveamac/Themely/raw/master/meta/banner_3ds.png" alt="Themely"/><br/>
<img src="https://github.com/ihaveamac/Themely/raw/master/screenshot.png" alt="Themely screenshot"/><br/>
<b>Themely</b> is a custom home menu manager for the Nintendo 3DS.<br/><br/>
<a href="https://github.com/ihaveamac/Themely/releases/latest">Download</a><br/>

# Features
- **Native code, which allows us to have...**
- **... up to 380% speed increase from CHMM2**
- ZIP support (ignores invalid ZIPs)
- BGM preview support (CHMM is usually hit or miss with this one)
- Instant theme preview
- Fullscreen preview
- QR Code reader<b>&#x002A;</b>
- Install without BGM
- Theme shuffle
- Theme dumping
- Multilingual (French, German, Italian, Portuguese, Russian, and Spanish available)<b>&#x002A;</b>
- Self updater

(<b>&#x002A;</b> = new in the latest stable release)

<h3 align="center">
<a href="https://github.com/ihaveamac/Themely/issues?q=is%3Aissue+is%3Aopen+label%3Aenhancement">Planned features</a> - <a href="https://github.com/ihaveamac/Themely/issues?q=is%3Aissue+is%3Aopen+label%3Abug">Known bugs</a>
</h3>

# Dependencies & Building
- DevkitARM r46
- `master` branch of ctrulib
- `next` branch of ctrulib
- Latest version of sf2dlib
- Latest version of sftdlib
- Latest version of sfillib
- zlib, libpng, libjpeg-turbo, freetype, libogg, and tremor from 3ds-portlibs (https://github.com/javimadgit/3ds_portlibs/ recommended)

You can run the `deps.sh` script to install `sf2dlib`, `sftdlib`, `sfillib`, and `3ds-portlibs` automatically.

Run `make` to build the 3DSX, CIA, and ZIP files,  `make run` to build & run on Citra. A debug build with debugging symbols will be built by default, append `DEBUG=0` to the make command to build a production build.

# Thanks
- People at Nintendo Homebrew discord #dev channel (especially MarcusD)
- DevkitPro contributors
- ctrulib contributors
- xerpi for sf2dlib and sftdlib
- ZetaDesigns for German translation
- theheroGAC for Italian translation
- Jubash for Portuguese translation
- Ann0ying for Russian translation
- umbjolt for Spanish translation
- QuaserEatingATaco for Dutch translation

# License
This software is licensed under GNU GPLv3. Check LICENSE.

Copyright (c) 2017 Erman SAYIN
