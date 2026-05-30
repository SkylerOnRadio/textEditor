#include <memory>
#include <ncurses.h>
#include <utility>

#define CTRL(key) (key & 0x1F)

class Letter {
public:
  char letter;
  std::unique_ptr<Letter> next;
  Letter(char letter) {
    this->letter = letter;
    next = nullptr;
  }
};

std::unique_ptr<Letter> start = nullptr;
Letter *end = nullptr;

void addLetter(char letter) {
  std::unique_ptr<Letter> newLetter = std::make_unique<Letter>(letter);
  Letter *rawLetter = newLetter.get();

  if (start == nullptr) {
    start = std::move(newLetter);
    end = rawLetter;
  } else {
    end->next = std::move(newLetter);
    end = rawLetter;
  }
}

void displayText(WINDOW *win) {
  box(win, 0, 0);
  int maxy, maxx;
  getmaxyx(win, maxy, maxx);
  int y{1};
  int x{1};

  Letter *l = start.get();
  while (l != nullptr) {
    if (x == maxx) {
      y++;
      x = 1;
    }
    if (y == maxy)
      return;
    mvwaddch(win, y, x, l->letter);
    l = l->next.get();
    x++;
  }
  wmove(win, y, x);
  wrefresh(win);
}

int main() {
  WINDOW *mainWindow;

  initscr();
  raw();
  keypad(mainWindow, true);
  noecho();
  curs_set(1);
  wmove(mainWindow, 1, 1);

  mainWindow = newwin(LINES, COLS, 0, 0);

  box(mainWindow, 0, 0);
  // std::cerr << "made the box";
  wrefresh(mainWindow);
  int exit{0};
  while (1) {
    char l = wgetch(mainWindow);
    switch (l) {
    case CTRL('q'):
      exit = 1;

    default:
      addLetter(l);
    }
    if (exit == 1)
      break;

    displayText(mainWindow);
  }
}
