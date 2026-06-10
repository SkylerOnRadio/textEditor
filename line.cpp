#include "line.h"
#include <vector>

void Line::incrementLines() { Line::noOfLines++; }

void Line::decrementLines() { Line::noOfLines--; }

Line::Line() {
  characters = 0;
  start = nullptr;
  end = nullptr;
}

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

void Line::delelteCharacter(const int x) {
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

int Line::noOfLines = 0;

void joinTwoLines(const int y, std::vector<std::unique_ptr<Line>> &lines) {
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
