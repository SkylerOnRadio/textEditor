#ifndef KEYMAPS_H
#define KEYMAPS_H

#include "./line.h"
#include <vector>

void moveCursorLeft(int &cursorY, int &cursorX,
                    std::vector<std::unique_ptr<Line>> &lines);
void moveCursorRight(int &cursorY, int &cursorX,
                     std::vector<std::unique_ptr<Line>> &lines);
void moveCursorUp(int &cursorY, int &cursorX,
                  std::vector<std::unique_ptr<Line>> &lines);
void moveCursorDown(int &cursorY, int &cursorX,
                    std::vector<std::unique_ptr<Line>> &lines);

void shiftMoveCursorLeft(int &cursorY, int &cursorX,
                         std::array<int, 2> &startPos,
                         std::array<int, 2> &endPos,
                         std::vector<std::unique_ptr<Line>> &lines);
void shiftMoveCursorRight(int &cursorY, int &cursorX,
                          std::array<int, 2> &startPos,
                          std::array<int, 2> &endPos,
                          std::vector<std::unique_ptr<Line>> &lines);
void shiftMoveCursorUp(int &cursorY, int &cursorX, std::array<int, 2> &startPos,
                       std::array<int, 2> &endPos,
                       std::vector<std::unique_ptr<Line>> &lines);
void shiftMoveCursorDown(int &cursorY, int &cursorX,
                         std::array<int, 2> &startPos,
                         std::array<int, 2> &endPos,
                         std::vector<std::unique_ptr<Line>> &lines);

#endif // KEYMAPS_H
