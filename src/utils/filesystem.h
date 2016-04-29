#ifndef UTILS_FILESYSTEM_H
#define UTILS_FILESYSTEM_H

#include <string>

bool directoryExists(const std::string& dir);
bool createDirectory(const std::string& dir);

#endif
