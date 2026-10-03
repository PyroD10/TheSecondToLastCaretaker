cmake_minimum_required(VERSION 3.22)
project(TheSecondToLastCaretaker LANGUAGES CXX)

# RE-UE4SS must be cloned into src/RE-UE4SS at the commit matching the installed UE4SS build
# (see docs/04-dev-setup.md §4). It is not part of this repo.
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/RE-UE4SS/CMakeLists.txt")
    message(FATAL_ERROR "src/RE-UE4SS is missing. Clone RE-UE4SS there and check out the pinned commit — docs/04-dev-setup.md §4")
endif()

add_subdirectory(RE-UE4SS)
add_subdirectory(GhostNet)
