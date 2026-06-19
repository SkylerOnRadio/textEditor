#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include "line.h"
#include <memory>
#include <string_view>
#include <vector>

void saveFile(std::vector<std::unique_ptr<Line>> &lines, std::string &filename);

#endif // !DEBUG
