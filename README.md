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

## Building on Windows

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

## Coding Guidelines
-----------------------------

### Naming Conventions
Use app hungarian notation: https://stackoverflow.com/a/1229360/12531771
Never use system hungarian notation.
Temporary variable names prfered to be short and understandable.
Naming scheme:
 - LongClassName
 - longLocalVariableName
 - m_LongPrivateMemberName
 - longPublicMemberName
 - s_LongStaticVariableName
 - longFunctionMethodName()

### Best Practices
Only create struct if all member is public. In this case you dont have to use the m_ prefix.

Only use pointers when neccessary. It's not neccesarly bad, but it can lead to nasty problems so if it's possible use smart pointers.
 - std::unique_pointer is preferred. It means ownership, so use it if the lifetime is depended on the context, eg. class member
 - std::shared_pointer when you are not owner
 - if you use raw pointer make sure that you include delete in the destructor

Use const ref in parameter passing and in return values, when the objects is relatively complex. It's not a big deal if you miss it in theory today compilers are smart enough to replace the code where neccessary, but the semi-official guideline still advise to use it. For std::strings you can use std::string_view.

Prefer long ifs with returns/continues. Dont use else after return.

Order class in tihs order: public methods / public variables / private methods / private members.

Dont use using namespace std.

Use const where the method can be const.

Use constexpr where you can.

If you override a method always use override keyword.

If any question: https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md

