#include "utils/input_helpers.h"
#include <iostream>
#include <limits>
#include <regex>

void clearCin()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string readLineTrim()
{
    std::string s;
    getline(std::cin, s);
    auto l = s.find_first_not_of(" \t\r\n");
    if (l == std::string::npos)
        return "";
    auto r = s.find_last_not_of(" \t\r\n");
    return s.substr(l, r - l + 1);
}

std::string readNonEmptyLine(const std::string &prompt)
{
    std::string s;
    while (true)
    {
        std::cout << prompt << std::flush;
        s = readLineTrim();
        if (!s.empty())
            return s;
        std::cout << "Input cannot be empty. Try again.\n";
    }
}

double readDouble(const std::string &prompt, bool allowNegative)
{
    while (true)
    {
        std::cout << prompt << std::flush;
        std::string s;
        getline(std::cin, s);
        try
        {
            double v = std::stod(s);
            if (!allowNegative && v < 0)
            {
                std::cout << "Value cannot be negative.\n";
                continue;
            }
            return v;
        }
        catch (...)
        {
            std::cout << "Please enter a valid number.\n";
        }
    }
}

int readInt(const std::string &prompt, bool allowNegative)
{
    while (true)
    {
        std::cout << prompt << std::flush;
        std::string s;
        getline(std::cin, s);
        try
        {
            long v = std::stol(s);
            if (!allowNegative && v < 0)
            {
                std::cout << "Value cannot be negative.\n";
                continue;
            }
            return static_cast<int>(v);
        }
        catch (...)
        {
            std::cout << "Please enter a valid integer.\n";
        }
    }
}

bool isValidDateFormat(const std::string &date)
{
    // Basic check for YYYY-MM-DD format (e.g., 2025-12-31)
    std::regex datePattern("\\d{4}-\\d{2}-\\d{2}");
    return std::regex_match(date, datePattern);
}