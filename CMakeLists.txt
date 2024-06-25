cmake_minimum_required(VERSION 3.5)
project (snaze)

# Currently CMake (since 2.8.5) supports generation of compilation databases
# for Unix Makefile builds (Ninja builds in the works) with the option
# CMAKE_EXPORT_COMPILE_COMMANDS.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#=== SETTING VARIABLES ===#
# Compiling flags
set( GCC_COMPILE_FLAGS "-Wall" )
#set( PREPROCESSING_FLAGS  "-D PRINT")
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS} ${PREPROCESSING_FLAGS} -g3" )

#=== Main App ===
add_executable(snaze  src/main.cpp
                        src/data.cpp
                        src/level.cpp
                        src/snake.cpp
                        src/snakeGame.cpp
                        src/player.cpp)

#define C++17 as the standard.
# set_target_properties( snaze PROPERTIES CXX_STANDARD 17 )
target_compile_features( snaze PUBLIC cxx_std_17 )