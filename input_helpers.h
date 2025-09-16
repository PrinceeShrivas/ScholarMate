#ifndef INPUT_HELPERS_H
#define INPUT_HELPERS_H

#include <string>

void clearCin();
std::string readLineTrim();
std::string readNonEmptyLine(const std::string &prompt);
double readDouble(const std::string &prompt, bool allowNegative = false);
int readInt(const std::string &prompt, bool allowNegative = false);
bool isValidDateFormat(const std::string &date);

#endif