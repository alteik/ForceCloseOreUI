cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 26)
project(ForceCloseOreUI)

# force /MD for both debug & release builds
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

# compiler check
if (NOT MSVC)
    message(FATAL_ERROR "ForceCloseOreUI can only be built using MSVC.")
endif ()

# architecture check
if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "ForceCloseOreUI can only be built as x64.")
endif ()

add_compile_options(/MP)      # multi-threaded compilation
add_compile_options(/FS)      # force sync of writes to PDB files
add_compile_options(/utf-8)   # source files in UTF-8 encoding (needed mostly for spdlog)
add_compile_options(/bigobj)  # support for large object files
add_compile_options(/GR-)     # disable RTTI

# collect all source and header files
file(GLOB_RECURSE sourceFiles CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp")
# disable modules scanning
set_source_files_properties(${sourceFiles} PROPERTIES CXX_SCAN_FOR_MODULES OFF)

# includes setup
include_directories(.)                   # project root
include_directories(deps/include)  # third-party includes
link_directories   (deps/lib)      # third-party libs dir

# link library
add_library(ForceCloseOreUI SHARED ${sourceFiles})

# link third-party libs
target_link_libraries(ForceCloseOreUI PRIVATE libhat.lib minhook.lib)

# define precompiled header for .cpp files
target_precompile_headers(ForceCloseOreUI PRIVATE src/headers.hpp)