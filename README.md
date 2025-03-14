# Algorithmic Textiles
Repository for 15-867 Algorithmic Textiles

These programs output knitout readable text files that need to be converted to\
.dat for machine usability via knitout-to-dat.js

```bash
node knitout-to-dat.js output.txt outputDat.dat
```


## Usage
CMake text files are included for terminal execution:

```bash
mkdir build
cd build
cmake ../projectFolderDir
cmake --build . --config Release
/Release/project.exe
```


## Credit
[Soji Yamakawa](https://github.com/captainys) for his simple cpp png reader and decoder\
[ixchow](https://github.com/textiles-lab/knitout-backend-swg) for knitout to dat conversion