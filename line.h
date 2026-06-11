#ifndef LINE_H
#define LINE_H

#include "letter.h"
#include <memory>
#include <vector>

class Line {
public:
  static int noOfLines;
  int characters;
  std::unique_ptr<Letter> start;
  Letter *end = nullptr;

  static void incrementLines();
  static void decrementLines();

  Line();
  Letter *getPointerToCharacterAtPos(const int x);
  void addCharacter(const int x, const char letter);
  void delelteCharacter(const int x);
};

void joinTwoLines(const int y, std::vector<std::unique_ptr<Line>> &lines);
void insertNewLine(std::vector<std::unique_ptr<Line>> &lines, int &y, int &x,
                   char letter);

#endif // !DEBUG
