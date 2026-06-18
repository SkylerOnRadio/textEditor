cmake_minimum_required(VERSION 4.3)

#project name
project(app)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses)

# add the executable
add_executable(${PROJECT_NAME} main.cpp line.cpp letter.cpp keymaps.cpp ui.cpp fileHandling.cpp)
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
