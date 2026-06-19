#include "./header-files/fileHandling.h"
#include "./header-files/ui.h"
#include <fstream>

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
