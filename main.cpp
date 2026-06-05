#include <iostream>
#include <memory>
#include <ncurses.h>
#include <utility>

#define CTRL(key) (key & 0x1F)

int charNumber{0};
int lastCharPos[2] = {1, 1};

class Letter {
public:
  char letter;
  std::unique_ptr<Letter> next;
  Letter *prev;
  Letter(char letter) {
    this->letter = letter;
    next = nullptr;
    prev = nullptr;
  }
};

std::unique_ptr<Letter> start = nullptr;
Letter *end = nullptr;

Letter *getPointerToCharacterAtPos(int y, int x) {

  int tempX{1}, tempY{1};
  Letter *l = start.get();
  while (tempX != x || tempY != y) {
    if (tempX == COLS - 1) {
      tempY++;
      tempX = 1;
    }
    if (l->letter == '\n') {
      tempY++;
      tempX = 1;
      l = l->next.get();
      continue;
    }
    l = l->next.get();
    tempX++;
  }
  return l;
}

void addLetter(char letter, int y, int x) {
  std::unique_ptr<Letter> newLetter = std::make_unique<Letter>(letter);

  if (start == nullptr) {
    start = std::move(newLetter);
    end = start.get();
    return;
  }

  // we get the position of the character just behind the cursor
  if (x == 1) {
    x = COLS;
    y--;
  }
  Letter *l = getPointerToCharacterAtPos(y, x - 1);

  if (l == end) {
    newLetter->prev = end;
    end->next = std::move(newLetter);
    end = end->next.get();
    return;
  }

  l->next->prev = newLetter.get();
  newLetter->next = std::move(l->next);
  newLetter->prev = l;
  l->next = std::move(newLetter);
}

void delelteCharacter(int y, int x) {
  if (end == nullptr)
    return;
  if (start.get() == end) {
    end = nullptr;
    start.reset();
    return;
  }

  // get the position of the character just behind the cursor
  if (x == 1) {
    x = COLS;
    y--;
  }
  Letter *l = getPointerToCharacterAtPos(y, x - 1);

  if (l == end) {
    end = end->prev;
    end->next.reset();
    return;
  }

  Letter *prev = l->prev;
  std::unique_ptr<Letter> charToBeJoined = std::move(l->next);
  prev->next.reset();
  charToBeJoined->prev = prev;
  prev->next = std::move(charToBeJoined);
}

void displayText(WINDOW *win) {
  werase(win);
  box(win, 0, 0);
  int x{1}, y{1};

  Letter *l = start.get();
  while (l != nullptr) {
    charNumber++;
    if (x == COLS - 1) {
      y++;
      x = 1;
    }
    if (l->letter == '\n') {
      y++;
      x = 1;
      l = l->next.get();
      continue;
    }
    if (y == LINES - 1)
      return;
    mvwaddch(win, y, x, l->letter);
    l = l->next.get();
    x++;
  }
  wrefresh(win);
}

int main() {
  WINDOW *mainWindow;
  int cursorPos[2] = {1, 1};

  initscr();
  raw();
  keypad(mainWindow, true);
  noecho();
  curs_set(1);

  mainWindow = newwin(LINES, COLS, 0, 0);
  wmove(mainWindow, cursorPos[1], cursorPos[0]);

  box(mainWindow, 0, 0);

  wrefresh(mainWindow);

  int exit{0};
  while (1) {
    int l = wgetch(mainWindow);
    switch (l) {
    case CTRL('q'):
      exit = 1;
      break;

    case 127:
      delelteCharacter(cursorPos[1], cursorPos[0]);
      if (cursorPos[0] == 1) {
        cursorPos[0] = COLS - 1;
        cursorPos[1]--;
      } else
        cursorPos[0]--;
      break;

    case '\n':
      addLetter(static_cast<char>(l), cursorPos[1], cursorPos[0]);
      cursorPos[0] = 1;
      cursorPos[1]++;
      break;

    default:
      addLetter(static_cast<char>(l), cursorPos[1], cursorPos[0]);
      if (cursorPos[0] == COLS - 1) {
        cursorPos[1]++;
        cursorPos[0] = 1;
      } else
        cursorPos[0]++;
    }
    if (exit == 1)
      break;

    wmove(mainWindow, cursorPos[1], cursorPos[0]);
    displayText(mainWindow);
  }
  delwin(mainWindow);
}
