#include "line.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>

void Line::incrementLines() { Line::noOfLines++; }

void Line::decrementLines() { Line::noOfLines--; }

// constructor for the Line class
Line::Line() {
  characters = 0;
  start = nullptr;
  end = nullptr;
}

// to get the character at position x
Letter *Line::getPointerToCharacterAtPos(const int x) {
  int charIteratedOver{1};
  Letter *l = this->start.get();
  while (charIteratedOver < x) {
    l = l->next.get();
    charIteratedOver++;
  }
  return l;
}

void Line::addCharacter(const int x, const char letter) {
  std::unique_ptr<Letter> newLetter = std::make_unique<Letter>(letter);
  // optimistically update the character count
  characters++;

  // if start if null handling
  if (this->start.get() == nullptr) {
    this->start = std::move(newLetter);
    this->end = this->start.get();
    return;
  }

  // get the character to append the new character too
  Letter *charToBeAppendedTo = this->getPointerToCharacterAtPos(x);

  // if the character is to be inserted after end
  if (charToBeAppendedTo == this->end) {
    newLetter->prev = charToBeAppendedTo;
    charToBeAppendedTo->next = std::move(newLetter);
    this->end = charToBeAppendedTo->next.get();
    return;
  }

  // if the character is to be inserted between two nodes
  charToBeAppendedTo->next->prev = newLetter.get();
  newLetter->next = std::move(charToBeAppendedTo->next);
  newLetter->prev = charToBeAppendedTo;
  charToBeAppendedTo->next = std::move(newLetter);
}

void Line::delelteCharacter(const int x) {
  // return of the x coordinate is the same as that of a border
  if (x == 0)
    return;

  // optimistically update the characters count
  characters--;

  // hadle the start and end being the same node
  if (this->start.get() == this->end) {
    this->end = nullptr;
    this->start.reset();
    return;
  }

  // if the letter being deleted is start and the next letter is end
  if (characters == 1 && x == 1) {
    std::unique_ptr<Letter> l = std::move(start);
    start = std::move(l->next);
    start->prev = nullptr;
    l.reset();
    return;
  }

  Letter *charToDelete = getPointerToCharacterAtPos(x);

  // handle the node being the end node
  if (charToDelete == this->end) {
    this->end = charToDelete->prev;
    this->end->next.reset();
    return;
  }

  // handle the node being in between two
  Letter *prev = charToDelete->prev;
  std::unique_ptr<Letter> charToBeJoined = std::move(charToDelete->next);
  prev->next.reset();
  charToBeJoined->prev = prev;
  prev->next = std::move(charToBeJoined);
}

void Line::updateCharacterCount() {
  Letter *l = start.get();
  int count{0};

  while (l != nullptr) {
    count++;
    l = l->next.get();
  }

  characters = count;
}

void Line::deleteFromAToB(int a, int b) {
  if (a < 1)
    return;
  if (b > characters)
    return;

  int targetCharNo = characters - (b - a + 1);

  while (characters > targetCharNo) {
    delelteCharacter(b);
    b--;
  }
}

std::string Line::getCharsFromAToB(int a, int b) {
  if (a < 1)
    return "";
  if (b > characters)
    return "";

  std::string text{""};

  while (a <= b) {
    Letter *l = getPointerToCharacterAtPos(a);
    text += l->letter;
    a++;
  }
  return text;
}

int Line::noOfLines = 0;

void joinTwoLines(const int y, std::vector<std::unique_ptr<Line>> &lines) {
  // get the two lines to append
  Line *toBeAppendedTo = lines.at(y - 2).get();
  Line *lineToAppend = lines.at(y - 1).get();

  // handle if the line to append is null
  if (lineToAppend->start.get() == nullptr) {
    toBeAppendedTo->delelteCharacter(toBeAppendedTo->characters);
    lines.erase(lines.begin() + y - 1);
    Line::decrementLines();
    return;
  }

  // delete the newLine char, update the character count, and join the two
  // linked lists
  toBeAppendedTo->delelteCharacter(toBeAppendedTo->characters);
  toBeAppendedTo->characters += lineToAppend->characters;
  toBeAppendedTo->end->next = std::move(lineToAppend->start);
  toBeAppendedTo->end = lineToAppend->end;

  // delete the memory of the node
  lines.at(y - 1).reset();
  // remove the line
  lines.erase(lines.begin() + y - 1);
  Line::decrementLines();
}

void insertNewLine(std::vector<std::unique_ptr<Line>> &lines, int &y, int &x,
                   char letter) {
  // add the newLine char
  lines.at(y - 1)->addCharacter(x - 1, letter);
  // insert the new line
  lines.insert(lines.begin() + y, std::make_unique<Line>());
  Line::incrementLines();

  Letter *l = lines.at(y - 1)->getPointerToCharacterAtPos(x);
  // get the character after the newLine character
  if (l->next != nullptr) {
    std::unique_ptr<Letter> charToBeMoved = std::move(l->next);
    l->next.reset();
    lines.at(y)->end = lines.at(y - 1)->end;
    lines.at(y - 1)->end = l;
    charToBeMoved->prev = nullptr;
    lines.at(y)->start = std::move(charToBeMoved);
    lines.at(y - 1)->updateCharacterCount();
    lines.at(y)->updateCharacterCount();
  }
  x = 1;
  y++;
  return;
}

void deleteChar(std::vector<std::unique_ptr<Line>> &lines, int &y, int &x) {
  if (x == 1) {
    if (y > 1) {
      joinTwoLines(y, lines);
      y--;
      x = lines.at(y - 1)->characters + 1;
    }
  } else {
    lines.at(y - 1)->delelteCharacter(x - 1);
    x--;
  }
}

void deleteSelection(std::vector<std::unique_ptr<Line>> &lines,
                     std::array<int, 2> &start, std::array<int, 2> &end,
                     int &cursorY, int &cursorX) {

  if (end[1] > start[1]) {
    lines.at(start[1] - 1)
        ->deleteFromAToB(start[0], lines.at(start[1] - 1)->characters);
    lines.at(end[1] - 1)->deleteFromAToB(0, end[0]);
    int lineToCompletelyDelete = start[1] + 1;
    while (lineToCompletelyDelete < end[1]) {
      lines.at(lineToCompletelyDelete - 1)
          ->deleteFromAToB(0, lines.at(lineToCompletelyDelete - 1)->characters);
      lineToCompletelyDelete++;
    }
    for (int i = 0; i <= end[1] - start[1]; ++i)
      lines.erase(lines.begin() + start[1]);

    Line *lineToAppendTo = lines.at(start[1] - 1).get();
    Line *lineToAppend = lines.at(start[1]).get();

    lineToAppend->start->prev = lineToAppendTo->end;
    lineToAppendTo->end->next = std::move(lineToAppend->start);

    lines.erase(lines.begin() + start[1]);
    cursorX = start[0];
    cursorY = start[1];

    start = {0, 0};
    end = {0, 0};
  } else {
    lines.at(start[1] - 1)->deleteFromAToB(start[0], end[0]);
    cursorX = start[0];
    cursorY = start[1];

    start = {0, 0};
    end = {0, 0};
  }
}

void pasteBuffer(int &y, int &x, std::string &buffer,
                 std::vector<std::unique_ptr<Line>> &lines) {
  Line *line = lines.at(y - 1).get();

  for (char letter : buffer) {
    if (letter == '\n') {
      insertNewLine(lines, y, x, letter);
      line = lines.at(y - 1).get();
      continue;
    }
    line->addCharacter(x - 1, letter);
    x++;
  }
}

void pasteIntoSelection(std::vector<std::unique_ptr<Line>> &lines,
                        std::array<int, 2> &startPos,
                        std::array<int, 2> &endPos, std::string buffer, int &x,
                        int &y) {
  deleteSelection(lines, startPos, endPos, y, x);
  pasteBuffer(y, x, buffer, lines);
}

void insertChar(std::vector<std::unique_ptr<Line>> &lines, int &y, int &x,

                char letter) {
  lines.at(y - 1)->addCharacter(x - 1, letter);
  x++;
}

std::string copySelection(std::vector<std::unique_ptr<Line>> &lines,
                          std::array<int, 2> &start, std::array<int, 2> &end) {
  std::string buffer{""};
  if (end[1] - start[1] > 0) {
    Line *startLine = lines.at(start[1] - 1).get();
    buffer += startLine->getCharsFromAToB(start[0], startLine->characters);

    int lineNo = start[1] + 1;
    while (lineNo < end[1]) {
      Line *line = lines.at(lineNo - 1).get();
      buffer += line->getCharsFromAToB(0, line->characters);
      lineNo++;
    }

    Line *endLine = lines.at(end[1] - 1).get();
    buffer += endLine->getCharsFromAToB(0, end[0]);
  } else {
    Line *line = lines.at(start[1] - 1).get();
    buffer += line->getCharsFromAToB(start[0], end[0]);
  }

  start = {0, 0};
  end = {0, 0};
  return buffer;
}
