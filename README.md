# Gyaszmat
Gyaszmat is the best game ever created in C++ using SFML.


**Roadmap:** https://trello.com/b/wMSF3RLJ/gyaszmat

## Building on Windows

### Setup the environment
1. Download and install Visual Studio 2022 with cmake
2. Also download and install the latest cmake seperately
3. Open project in Visual Studio, it should parse everything.

### Build
Just press the build all button in Visual Studio.

### Execution
Just press the play button

## Coding Guidelines
-----------------------------

### Naming Conventions
Use app hungarian notation for members and statics: https://stackoverflow.com/a/1229360/12531771
Temporary variable names prfered to be short and understandable.
Naming scheme:
 - LongClassName
 - longLocalVariableName
 - m_LongPrivateMemberName
 - longPublicMemberName
 - s_LongStaticVariableName
 - longFunctionMethodName()

### Other rules
- Only create struct if all member is public. In this case you dont have to use the m_ prefix.
- Structure a class in this order: public methods / public variables / private methods / private members.
- Use pragma once instead of include guards

If any question: https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md

