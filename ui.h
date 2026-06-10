#ifndef UI_H
#define UI_H

#include "line.h"
#include <memory>
#include <ncurses.h>
#include <vector>

void displayText(WINDOW *win, std::vector<std::unique_ptr<Line>> &lines);
void handleDisplay(WINDOW *win);

#endif // !DEBUG
