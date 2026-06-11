#include "ui.h"
#include <ncurses.h>

int main() {

  // initialize the main window
  WINDOW *mainWindow;

  initscr();
  // set the size and start position of the window
  mainWindow = newwin(LINES, COLS, 0, 0);
  raw();
  noecho();
  curs_set(1);

  handleDisplay(mainWindow);
  // delete the window to remove from the heap
  delwin(mainWindow);
  return 0;
}
