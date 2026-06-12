#include "ui.h"
#include "keymaps.h"
#include "line.h"
#include <ncurses.h>
#include <string>

// macro to perform bitwise and, which lets us detect CTRL characters since
// CTRL chars have their first 3 bits 0, then the normal bits,
#define CTRL(key) (key & 0x1F)

void displayText(WINDOW *win, std::vector<std::unique_ptr<Line>> &lines) {
  werase(win);
  box(win, 0, 0);
  int y{1};

  // loop through all the lines, and traverse through each character and display
  // that character
  for (auto &&line : lines) {
    Letter *l = line->start.get();
    int x{1};
    while (l != nullptr) {
      if (x == COLS - 1)
        break;
      if (l->letter == '\n')
        // l->letter = ':';
        break;
      if (y == LINES)
        break;
      mvwaddch(win, y, x, l->letter);
      l = l->next.get();
      x++;
    }
    y++;
  }
  wrefresh(win);
}

// the main function that handles everything
void handleDisplay(WINDOW *win) {
  // a vector of unique_ptr of Line to keep track of the different lines
  std::vector<std::unique_ptr<Line>> lines;
  std::string buffer{"Test Word!\n THis is another line"};

  // to track the cursor position
  int cursorPos[2] = {1, 1};

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
      moveCursorLeft(cursorPos[1], cursorPos[0], lines);
    } else if (l == KEY_RIGHT) {
      moveCursorRight(cursorPos[1], cursorPos[0], lines);
    } else if (l == KEY_UP) {
      moveCursorUp(cursorPos[1], cursorPos[0], lines);
    } else if (l == KEY_DOWN) {
      moveCursorDown(cursorPos[1], cursorPos[0], lines);
    } else if (l == CTRL('v')) {
      pasteBuffer(cursorPos[1], cursorPos[0], buffer, lines);
    } else if (l == 127 || l == KEY_BACKSPACE) {
      if (cursorPos[0] == 1) {
        if (cursorPos[1] > 1) {
          joinTwoLines(cursorPos[1], lines);
          cursorPos[1]--;
          cursorPos[0] = lines.at(cursorPos[1] - 1)->characters + 1;
        }
      } else {
        lines.at(cursorPos[1] - 1)->delelteCharacter(cursorPos[0] - 1);
        cursorPos[0]--;
      }
    } else if (l == '\n') {
      insertNewLine(lines, cursorPos[1], cursorPos[0], static_cast<char>(l));
    } else if (l > 31 && l < 127) {
      lines.at(cursorPos[1] - 1)
          ->addCharacter(cursorPos[0] - 1, static_cast<char>(l));
      cursorPos[0]++;
    }
    if (exit == 1)
      break;

    displayText(win, lines);
    wmove(win, cursorPos[1], cursorPos[0]);
  }
}
