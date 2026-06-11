#include "line.h"
#include <iostream>
#include <memory>
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

  // get the character after the newLine character
  Letter *l = lines.at(y - 1)->getPointerToCharacterAtPos(x);
  if (l->next != nullptr) {
    std::cerr << "hello";
    std::unique_ptr<Letter> charToBeMoved = std::move(l->next);
    l->next.reset();
    charToBeMoved->prev = nullptr;
    lines.at(y)->start = std::move(charToBeMoved);
  }
  x = 1;
  y++;
  return;
}
