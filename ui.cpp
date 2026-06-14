#include "ui.h"
#include "keymaps.h"
#include "line.h"
#include <array>
#include <ncurses.h>
#include <string>
#include <vector>

// macro to perform bitwise and, which lets us detect CTRL characters since
// CTRL chars have their first 3 bits 0, then the normal bits,
#define CTRL(key) (key & 0x1F)

void displayText(WINDOW *win, std::vector<std::unique_ptr<Line>> &lines,
                 std::array<int, 2> &startPos, std::array<int, 2> &endPos) {
  werase(win);
  box(win, 0, 0);
  int y{1};

  // loop through all the lines, and traverse through each character and display
  // that character
  for (auto &&line : lines) {
    Letter *l = line->start.get();
    int x{1};
    while (l != nullptr) {
      if (x == startPos[0] && y == startPos[1])
        wattron(win, A_REVERSE);
      if (x == COLS - 1)
        break;
      if (l->letter == '\n')
        // l->letter = ':';
        break;
      if (y == LINES)
        break;

      mvwaddch(win, y, x, l->letter);
      if (x == endPos[0] && y == endPos[1])
        wattroff(win, A_REVERSE);
      l = l->next.get();
      x++;
    }
    y++;
  }
  wrefresh(win);
}

void resetHighlightPos(std::array<int, 2> &start, std::array<int, 2> &end) {
  start = {0, 0};
  end = {0, 0};
}

// the main function that handles everything
void handleDisplay(WINDOW *win) {
  // a vector of unique_ptr of Line to keep track of the different lines
  std::vector<std::unique_ptr<Line>> lines;
  std::string buffer{"Test Word!\n THis is another line"};

  // to track the cursor position
  int cursorPos[2] = {1, 1};
  // to track the first and last coords to do selection
  std::array<int, 2> startPos = {0, 0};
  std::array<int, 2> endPos = {0, 0};

  keypad(win, true);
  wmove(win, cursorPos[1], cursorPos[0]);

  box(win, 0, 0);

  wrefresh(win);

  // create the first line
  lines.emplace_back(std::make_unique<Line>());
  Line::incrementLines();

  int exit{0};
  while (1) {
    int l = wgetch(win);
    if (l == CTRL('q')) { // Quit if CTRL+Q is pressed
      exit = 1;
    } else if (l == KEY_LEFT) {
      resetHighlightPos(startPos, endPos);
      moveCursorLeft(cursorPos[1], cursorPos[0], lines);
    } else if (l == KEY_RIGHT) {
      resetHighlightPos(startPos, endPos);
      moveCursorRight(cursorPos[1], cursorPos[0], lines);
    } else if (l == KEY_UP) {
      resetHighlightPos(startPos, endPos);
      moveCursorUp(cursorPos[1], cursorPos[0], lines);
    } else if (l == KEY_DOWN) {
      resetHighlightPos(startPos, endPos);
      moveCursorDown(cursorPos[1], cursorPos[0], lines);
    } else if (l == KEY_SF) // shift down
    {
      shiftMoveCursorDown(cursorPos[1], cursorPos[0], startPos, endPos, lines);
    } else if (l == KEY_SR) // shift up
    {
      shiftMoveCursorUp(cursorPos[1], cursorPos[0], startPos, endPos, lines);
    } else if (l == KEY_SLEFT) {
      shiftMoveCursorLeft(cursorPos[1], cursorPos[0], startPos, endPos, lines);
    } else if (l == KEY_SRIGHT) {
      shiftMoveCursorRight(cursorPos[1], cursorPos[0], startPos, endPos, lines);
    } else if (l == CTRL('v')) {
      pasteBuffer(cursorPos[1], cursorPos[0], buffer, lines);
    } else if (l == 127 || l == KEY_BACKSPACE) {
      if (startPos[0] == 0 && startPos[1] == 0)
        deleteChar(lines, cursorPos[1], cursorPos[0]);
      else
        deleteSelection(lines, startPos, endPos);
    } else if (l == '\n') {
      insertNewLine(lines, cursorPos[1], cursorPos[0], static_cast<char>(l));
    } else if (l > 31 && l < 127) {
      insertChar(lines, cursorPos[1], cursorPos[0], static_cast<char>(l));
    }
    if (exit == 1)
      break;

    displayText(win, lines, startPos, endPos);
    wmove(win, cursorPos[1], cursorPos[0]);
  }
}
