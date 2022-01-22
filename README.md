# Gyaszmat
Gyaszmat is the best game ever created in C++ using SFML. 


## Building on Linux (Debian based)

### Setup environment
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

## Executing
The out file can be executed directly from terminal:
```
./build/gyaszmat
```

## Building on macOs

### Setup environment

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

### Build, clean and execute
The steps are exactly the same as described in the Linux section.