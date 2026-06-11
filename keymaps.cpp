#include "keymaps.h"
#include "line.h"
#include <memory>
#include <vector>

void moveCursorDown(int &cursorY, int &cursorX,
                    std::vector<std::unique_ptr<Line>> &lines) {
  // if the cursor is at the last line or beyond that (somehow) return
  if (cursorY >= Line::noOfLines)
    return;

  cursorY++;
  Line *lineToGoTo = lines.at(cursorY - 1).get();
  // check if the line has newLine, if yes then set the maximum X coord such
  // that it ignore the newLine character
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
    // put the cursor's X co-ordinate right where the newLine character is, so
    // writing new characters still means that the newLine is at the last
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
  // if the next x position is greater or equal to the maximum X co-ordinate
  if (cursorX + 1 > maxXCord) {
    if (cursorY < Line::noOfLines) {
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
