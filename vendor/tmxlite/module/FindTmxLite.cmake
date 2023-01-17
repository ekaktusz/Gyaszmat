# Search for the header file
find_path(TMXLITE_INCLUDE_DIR NAMES tmxlite/Config.hpp PATH_SUFFIXES include)

# Search for the library
find_library(TMXLITE_LIBRARIES NAMES TMXLITE PATH_SUFFIXES lib)