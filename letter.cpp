#include "letter.h"

// constructor for the Letter class
Letter::Letter(char letter) {
  this->letter = letter;
  next = nullptr;
  prev = nullptr;
}
