#include "keymaps.h"
#include "line.h"
#include <memory>
#include <vector>

void moveCursorDown(int &cursorY, int &cursorX,
                    std::vector<std::unique_ptr<Line>> &lines) {
  if (cursorY == Line::noOfLines)
    return;

  cursorY++;
  Line *lineToGoTo = lines.at(cursorY - 1).get();
  int maxXCord = lineToGoTo->end->letter == '\n' ? lineToGoTo->characters
                                                 : lineToGoTo->characters + 1;
  if (cursorX > maxXCord)
    cursorX = maxXCord;
}

void moveCursorLeft(int &cursorY, int &cursorX,
                    std::vector<std::unique_ptr<Line>> &lines) {
  if (cursorX == 1) {
    if (cursorY == 1)
      return;

    cursorY--;
    cursorX = lines.at(cursorY - 1)->characters;
    return;
  }
  cursorX--;
}

void moveCursorRight(int &cursorY, int &cursorX,
                     std::vector<std::unique_ptr<Line>> &lines) {
  Line *line = lines.at(cursorY - 1).get();
  int maxXCord =
      line->end->letter == '\n' ? line->characters : line->characters + 1;
  if (cursorX >= maxXCord) {
    if (Line::noOfLines > cursorY - 1) {
      cursorY++;
      cursorX = 1;
      return;
    } else
      return;
  }
  cursorX++;
}

void moveCursorUp(int &cursorY, int &cursorX,
                  std::vector<std::unique_ptr<Line>> &lines) {
  if (cursorY == 1)
    return;

  cursorY--;
  Line *lineToGoTo = lines.at(cursorY - 1).get();
  int maxXCord = lineToGoTo->end->letter == '\n' ? lineToGoTo->characters
                                                 : lineToGoTo->characters + 1;
  if (cursorX > maxXCord)
    cursorX = maxXCord;
}
