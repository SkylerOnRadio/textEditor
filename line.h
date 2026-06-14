#ifndef LINE_H
#define LINE_H

#include "letter.h"
#include <array>
#include <memory>
#include <string>
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
  void updateCharacterCount();
  void deleteFromAToB(int a, int b);
};

void joinTwoLines(const int y, std::vector<std::unique_ptr<Line>> &lines);
void insertNewLine(std::vector<std::unique_ptr<Line>> &lines, int &y, int &x,
                   char letter);
void pasteBuffer(int &y, int &x, std::string &buffer,
                 std::vector<std::unique_ptr<Line>> &lines);
void insertChar(std::vector<std::unique_ptr<Line>> &lines, int &y, int &x,
                char letter);
void deleteChar(std::vector<std::unique_ptr<Line>> &lines, int &y, int &x);
void deleteSelection(std::vector<std::unique_ptr<Line>> &lines,
                     std::array<int, 2> &start, std::array<int, 2> &end);

#endif // !DEBUG
