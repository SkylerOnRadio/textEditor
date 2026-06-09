#include <iostream>
#include <memory>
#include <ncurses.h>
#include <utility>
#include <vector>

#define CTRL(key) (key & 0x1F)

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

class Line {
public:
  static int noOfLines;
  int characters;
  std::unique_ptr<Letter> start;
  Letter *end = nullptr;

  Line() {
    characters = 0;
    start = nullptr;
    end = nullptr;
  }

  static void incrementLines() { noOfLines++; }
  static void decrementLines() { noOfLines--; }

  Letter *getPointerToCharacterAtPos(const int x) {
    int charIteratedOver{1};
    Letter *l = this->start.get();
    while (charIteratedOver < x) {
      l = l->next.get();
      charIteratedOver++;
    }
    return l;
  }

  void addCharacter(const int x, const char letter) {
    std::unique_ptr<Letter> newLetter = std::make_unique<Letter>(letter);
    characters++;

    if (this->start.get() == nullptr) {
      this->start = std::move(newLetter);
      this->end = this->start.get();
      return;
    }

    Letter *charToBeAppendedTo = this->getPointerToCharacterAtPos(x);

    if (charToBeAppendedTo == this->end) {
      newLetter->prev = charToBeAppendedTo;
      charToBeAppendedTo->next = std::move(newLetter);
      this->end = charToBeAppendedTo->next.get();
      return;
    }

    charToBeAppendedTo->next->prev = newLetter.get();
    newLetter->next = std::move(charToBeAppendedTo->next);
    newLetter->prev = charToBeAppendedTo;
    charToBeAppendedTo->next = std::move(newLetter);
  }

  void delelteCharacter(const int x) {
    if (x == 0)
      return;

    characters--;

    if (this->start.get() == this->end) {
      this->end = nullptr;
      this->start.reset();
      return;
    }

    Letter *charToDelete = getPointerToCharacterAtPos(x);

    if (charToDelete == this->end) {
      this->end = charToDelete->prev;
      this->end->next.reset();
      return;
    }

    Letter *prev = charToDelete->prev;
    std::unique_ptr<Letter> charToBeJoined = std::move(charToDelete->next);
    prev->next.reset();
    charToBeJoined->prev = prev;
    prev->next = std::move(charToBeJoined);
  }
};
int Line::noOfLines = 0;

// a vector of unique_ptr of Line to keep track of the different lines
std::vector<std::unique_ptr<Line>> lines;

void joinTwoLines(const int y) {
  Line *toBeAppendedTo = lines.at(y - 2).get();
  Line *lineToAppend = lines.at(y - 1).get();

  if (lineToAppend->start.get() == nullptr) {
    toBeAppendedTo->delelteCharacter(toBeAppendedTo->characters);
    lines.erase(lines.begin() + y - 1);
    Line::decrementLines();
    return;
  }

  toBeAppendedTo->delelteCharacter(toBeAppendedTo->characters);
  toBeAppendedTo->characters += lineToAppend->characters;
  toBeAppendedTo->end->next = std::move(lineToAppend->start);
  toBeAppendedTo->end = lineToAppend->end;

  lines.at(y - 1).reset();
  lines.erase(lines.begin() + y - 1);
  Line::decrementLines();
}

void moveCursorLeft(int &cursorY, int &cursorX) {
  if (cursorX == 1) {
    if (cursorY == 1)
      return;

    cursorY--;
    cursorX = lines.at(cursorY - 1)->characters;
    return;
  }
  cursorX--;
}

void moveCursorRight(int &cursorY, int &cursorX) {
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
void moveCursorUp(int &cursorY, int &cursorX) {
  if (cursorY == 1)
    return;

  cursorY--;
  Line *lineToGoTo = lines.at(cursorY - 1).get();
  int maxXCord = lineToGoTo->end->letter == '\n' ? lineToGoTo->characters
                                                 : lineToGoTo->characters + 1;
  if (cursorX > maxXCord)
    cursorX = maxXCord;
}
void moveCursorDown(int &cursorY, int &cursorX) {
  if (cursorY == Line::noOfLines)
    return;

  cursorY++;
  Line *lineToGoTo = lines.at(cursorY - 1).get();
  int maxXCord = lineToGoTo->end->letter == '\n' ? lineToGoTo->characters
                                                 : lineToGoTo->characters + 1;
  if (cursorX > maxXCord)
    cursorX = maxXCord;
}

void displayText(WINDOW *win) {
  werase(win);
  box(win, 0, 0);
  int y{1};

  for (auto &&line : lines) {
    Letter *l = line->start.get();
    int x{1};
    while (l != nullptr) {
      if (x == COLS - 1)
        break;
      if (l->letter == '\n')
        l->letter = ':';
      // break;
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

int main() {
  WINDOW *mainWindow;
  int cursorPos[2] = {1, 1};

  initscr();
  mainWindow = newwin(LINES, COLS, 0, 0);
  raw();
  noecho();
  curs_set(1);

  keypad(mainWindow, true);
  wmove(mainWindow, cursorPos[1], cursorPos[0]);

  box(mainWindow, 0, 0);

  wrefresh(mainWindow);

  lines.emplace_back(std::make_unique<Line>());
  Line::incrementLines();

  int exit{0};
  while (1) {
    int l = wgetch(mainWindow);
    if (l == CTRL('q')) {
      exit = 1;
    } else if (l == KEY_LEFT) {
      moveCursorLeft(cursorPos[1], cursorPos[0]);
    } else if (l == KEY_RIGHT) {
      moveCursorRight(cursorPos[1], cursorPos[0]);
    } else if (l == KEY_UP) {
      moveCursorUp(cursorPos[1], cursorPos[0]);
    } else if (l == KEY_DOWN) {
      moveCursorDown(cursorPos[1], cursorPos[0]);
    } else if (l == 127 || l == KEY_BACKSPACE) {
      if (cursorPos[0] == 1) {
        if (cursorPos[1] > 1) {
          joinTwoLines(cursorPos[1]);
          cursorPos[1]--;
          cursorPos[0] = lines.at(cursorPos[1] - 1)->characters + 1;
        }
      } else {
        lines.at(cursorPos[1] - 1)->delelteCharacter(cursorPos[0] - 1);
        cursorPos[0]--;
      }
    } else if (l == '\n') {
      lines.at(cursorPos[1] - 1)
          ->addCharacter(cursorPos[0] - 1, static_cast<char>(l));
      lines.emplace_back(std::make_unique<Line>());
      Line::incrementLines();
      cursorPos[0] = 1;
      cursorPos[1]++;
    } else if (l > 31 && l < 127) {
      lines.at(cursorPos[1] - 1)
          ->addCharacter(cursorPos[0] - 1, static_cast<char>(l));
      cursorPos[0]++;
    }
    if (exit == 1)
      break;

    displayText(mainWindow);
    wmove(mainWindow, cursorPos[1], cursorPos[0]);
  }
  delwin(mainWindow);
  return 0;
}
