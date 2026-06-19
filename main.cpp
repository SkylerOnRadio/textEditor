#include "./header-files/ui.h"
#include "header-files/line.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>

void parseArguments(char *arguments[], int argCount, std::string &filename) {
  for (int i = 1; i < argCount; ++i) {
    if (strcmp(arguments[i], "-h") == 0) {
      std::cout << "This is a rudimentary attempt at creating a text editor.\n"
                << "Current flags are: \n";
      exit(0);
    } else
      filename = arguments[i];
  }
}

int main(int argc, char *argv[]) {
  std::string filename{""};

  if (argc > 1)
    parseArguments(argv, argc, filename);

  // initialize the main window
  WINDOW *mainWindow;

  initscr();
  // set the size and start position of the window
  mainWindow = newwin(LINES, COLS, 0, 0);
  raw();
  noecho();
  curs_set(1);

  std::vector<std::unique_ptr<Line>> lines;
  handleDisplay(mainWindow, filename);
  // delete the window
  delwin(mainWindow);
  endwin();
  return 0;
}
