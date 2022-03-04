# Gyaszmat
Gyaszmat is the best game ever created in C++ using SFML.


**Roadmap:** https://trello.com/b/wMSF3RLJ/gyaszmat

## Building on Linux (Debian based)

### Setup the environment
Install libsfml-dev from the official repositories.

```
sudo apt-get install libsfml-dev
```
### Build
Run `build.sh` script from the project source directory.
```
./build.sh
```

### Clean
To clean the build call the build script with the `clean` parameter.
```
./build.sh clean
```

### Executing
The out file can be executed directly from terminal:
```
cd build
./gyaszmat
```

## Building on macOs

### (1st option) Setup the environment manually

Download SFML SDK from here:
https://www.sfml-dev.org/files/SFML-2.5.1-macOS-clang.tar.gz

Unarchive the downloaded file:
```
tar -xvf SFML-2.5.1-macOS-clang.tar.gz
```

Copy the content of SFML-2.5.1-macos-clang/Frameworks to /Library/Frameworks:
```
cp -R SFML-2.5.1-macos-clang/Frameworks /Library/Frameworks
```

Copy the content of FML-2.5.1-macos-clang/lib to /usr/local/lib and copy the content of SFML-2.5.1-macos-clang/include to /usr/local/include:
```
cp -R SFML-2.5.1-macos-clang/lib /usr/local/lib
cp -R SFML-2.5.1-macos-clang/include /usr/local/include
```

Copy the content of SFML-2.5.1-macos-clang/extlibs to /Library/Frameworks:
```
cp -R SFML-2.5.1-macos-clang/extlibs /Library/Frameworks
```

### (2nd option) Setup the environment with brew

Download Homebrew and install as said on the official site: https://brew.sh/

Install CMake and SFML with it:
```
brew install cmake
brew install sfml
```

### Build, clean and execute
The steps are exactly the same as described in the Linux section.

## Building on Windows (Technical Alpha Preview 0.1)

### Setup the environment
1. Download and install Visual Studio 2022 with cmake
2. Also download and install cmake seperately (don't know if needed but who cares)
3. Download the most recent 64 bit Visual C++ SFML from: https://www.sfml-dev.org/download/sfml/2.5.1/
4. Unzip SFML-2.5.1 folder from the zip file and place the folder to the Gyaszmat/include folder and rename it to SFML.
5. Open project in Visual Studio, it should parse everything.

### Build
Just press the build all button in Visual Studio.

### Execution
1. Copy all the dll-s from Gyaszmat/include/SFML/bin to Gyaszmat/build
