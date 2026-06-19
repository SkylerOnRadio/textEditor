#include "./header-files/ui.h"
#include "./header-files/fileHandling.h"
#include "./header-files/keymaps.h"
#include "./header-files/line.h"
#include <array>
#include <memory>
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
void handleDisplay(WINDOW *win, std::string &filename) {
  // a vector of unique_ptr of Line to keep track of the different lines
  std::string buffer{""};
  std::vector<std::unique_ptr<Line>> lines;

  // to track the cursor position
  int cursorPos[2] = {1, 1};
  // to track the first and last coords to do selection
  std::array<int, 2> startPos = {0, 0};
  std::array<int, 2> endPos = {0, 0};

  keypad(win, true);
  raw();
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
    } else if (l == KEY_SF) { // shift down
      shiftMoveCursorDown(cursorPos[1], cursorPos[0], startPos, endPos, lines);
    } else if (l == KEY_SR) { // shift up
      shiftMoveCursorUp(cursorPos[1], cursorPos[0], startPos, endPos, lines);
    } else if (l == KEY_SLEFT) {
      shiftMoveCursorLeft(cursorPos[1], cursorPos[0], startPos, endPos, lines);
    } else if (l == KEY_SRIGHT) {
      shiftMoveCursorRight(cursorPos[1], cursorPos[0], startPos, endPos, lines);
    } else if (l == CTRL('c')) {
      if (startPos[0] == 0 && endPos[1] == 0)
        continue;
      buffer = copySelection(lines, startPos, endPos);
    } else if (l == CTRL('v')) {
      if (startPos[1] == 0 && startPos[0] == 0)
        pasteBuffer(cursorPos[1], cursorPos[0], buffer, lines);
      else
        pasteIntoSelection(lines, startPos, endPos, buffer, cursorPos[0],
                           cursorPos[1]);
    } else if (l == CTRL('s')) {
      saveFile(lines, filename);
    } else if (l == 127 || l == KEY_BACKSPACE) {
      if (startPos[0] == 0 && startPos[1] == 0)
        deleteChar(lines, cursorPos[1], cursorPos[0]);
      else
        deleteSelection(lines, startPos, endPos, cursorPos[1], cursorPos[0]);
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

std::string askWindow(std::string askMessage) {
  WINDOW *win;
  win = newwin(5, COLS * 0.9, COLS * 0.05, 5);
  raw();
  curs_set(1);
  int y{1};

  box(win, 0, 0);
  mvwaddstr(win, 0, 5, askMessage.c_str());
  wmove(win, y, 1);
  wrefresh(win);
  std::string filename;

  while (1) {
    int c = wgetch(win);
    if (c == CTRL('c'))
      return "";
    if (c == '\n')
      break;
    if (c == KEY_BACKSPACE || c == 127) {
      if (!filename.empty())
        filename.pop_back();
    }
    if (c > 31 && c < 127)
      filename.push_back(c);

    werase(win);
    box(win, 0, 0);
    mvwaddstr(win, 0, 5, askMessage.c_str());
    mvwaddstr(win, y, 1, filename.c_str());
    wmove(win, y, 1 + filename.size());
    wrefresh(win);
  }

  delwin(win);
  return filename;
}
