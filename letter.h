#ifndef LETTER_H
#define LETTER_H
#include <memory>
class Letter {
public:
  char letter;
  std::unique_ptr<Letter> next;
  Letter *prev;
  Letter(char Letter);
};

#endif
