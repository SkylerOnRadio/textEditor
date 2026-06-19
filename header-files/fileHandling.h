#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include "line.h"
#include <memory>
#include <string>
#include <string_view>
#include <vector>

void saveFile(std::vector<std::unique_ptr<Line>> &lines, std::string &filename);
void loadFile(std::string &filename, std::vector<std::unique_ptr<Line>> &lines,
              int &y, int &x);

#endif // !DEBUG
