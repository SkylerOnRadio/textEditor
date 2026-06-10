#include "ui.h"
#include <ncurses.h>

int main() {

  WINDOW *mainWindow;

  initscr();
  mainWindow = newwin(LINES, COLS, 0, 0);
  raw();
  noecho();
  curs_set(1);

  handleDisplay(mainWindow);
  delwin(mainWindow);
  return 0;
}
