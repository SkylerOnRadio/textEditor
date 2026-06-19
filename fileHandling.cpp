#include "./header-files/fileHandling.h"
#include "./header-files/ui.h"
#include "header-files/line.h"
#include <filesystem>
#include <fstream>
#include <ios>

void saveFile(std::vector<std::unique_ptr<Line>> &lines,
              std::string &filename) {
  if (filename == "") {
    filename = askWindow("Enter the name for the file");
    if (filename == "")
      return;
  }

  std::fstream file(filename, file.out);
  for (int i = 0; i < lines.size(); ++i) {
    Line *line = lines.at(i).get();
    int charPos = 0;
    while (charPos <= line->characters) {
      file << line->getPointerToCharacterAtPos(charPos)->letter;
      charPos++;
    }
  }
}

void loadFile(std::string &filename, std::vector<std::unique_ptr<Line>> &lines,
              int &y, int &x) {
  if (!std::filesystem::exists(filename))
    return;

  std::fstream file{filename, std::ios_base::in};

  char letter;
  while (file.get(letter)) {
    if (letter == '\n') {
      insertNewLine(lines, y, x, letter);
      continue;
    }
    insertChar(lines, y, x, letter);
  }
}
