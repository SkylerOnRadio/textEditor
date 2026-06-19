#ifndef UI_H
#define UI_H

#include "line.h"
#include <array>
#include <memory>
#include <ncurses.h>
#include <string>
#include <string_view>
#include <vector>

void displayText(WINDOW *win, std::vector<std::unique_ptr<Line>> &lines,
                 std::array<int, 2> &startPos, std::array<int, 2> &endPos);
void handleDisplay(WINDOW *win, std::string &filename);

std::string askWindow(std::string askMessage);

#endif // !DEBUG
