#include "letter.h"

Letter::Letter(char letter) {
  this->letter = letter;
  next = nullptr;
  prev = nullptr;
}
