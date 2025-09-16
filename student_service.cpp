#include "services/student_service.h"
#include "core/bst.h"
#include "core/scholarship.h"
#include "utils/input_helpers.h"
#include "utils/string_utils.h"
#include <iostream>
#include <vector>
#include <algorithm>

void studentViewAll()
{
    std::cout << "\n--- All Scholarships (sorted by amount ascending) ---\n";
    bool any = false;
    inorderTraverse(root, [&](const Scholarship &s)
                    {
                        printScholarshipDetail(s);
                        any = true; });
    if (!any)
        std::cout << "No scholarships available.\n";
}

void studentSearchMenu()
{
    std::cout << "\n--- Search Scholarships ---\n";
    std::cout << "Enter keyword to search (title, provider, description, category, state, degree/school) or leave blank: ";
    std::string keyword;
    getline(std::cin, keyword);
    ResultList res;
    if (!keyword.empty())
    {
        collectIf(root, res, [keyword](const Scholarship &s)
                  {
                      std::string k = toLower(keyword);
                      return toLower(s.title).find(k) != std::string::npos ||
                             toLower(s.provider).find(k) != std::string::npos ||
                             toLower(s.description).find(k) != std::string::npos ||
                             toLower(s.category).find(k) != std::string::npos ||
                             toLower(s.state).find(k) != std::string::npos ||
                             toLower(s.degreeOrSchool).find(k) != std::string::npos; });
    }
    else
    {
        collectIf(root, res, [](const Scholarship &)
                  { return true; });
    }
    printResultList(res);
}

void studentSortMenu()
{
    std::cout << "\n--- Sort Scholarships ---\n";
    std::cout << "1. Amount (asc)\n2. Amount (desc)\n3. MinGPA (asc)\n4. MinGPA (desc)\n5. Title (A-Z)\n0. Back\n";
    int ch = readInt("Choice: ", true);
    if (ch == 0)
        return;
    std::vector<Scholarship> v;
    inorderTraverse(root, [&v](const Scholarship &s)
                    { v.push_back(s); });
    if (v.empty())
    {
        std::cout << "No scholarships available.\n";
        return;
    }
    switch (ch)
    {
    case 1:
        std::sort(v.begin(), v.end(), [](auto &a, auto &b)
                  { return a.amount < b.amount; });
        break;
    case 2:
        std::sort(v.begin(), v.end(), [](auto &a, auto &b)
                  { return a.amount > b.amount; });
        break;
    case 3:
        std::sort(v.begin(), v.end(), [](auto &a, auto &b)
                  { return a.minGPA < b.minGPA; });
        break;
    case 4:
        std::sort(v.begin(), v.end(), [](auto &a, auto &b)
                  { return a.minGPA > b.minGPA; });
        break;
    case 5:
        std::sort(v.begin(), v.end(), [](auto &a, auto &b)
                  { return toLower(a.title) < toLower(b.title); });
        break;
    default:
        std::cout << "Invalid option.\n";
        return;
    }
    std::cout << "Sorted result:\n";
    for (auto &s : v)
        printScholarshipDetail(s);
}

void studentFilterMenu()
{
    std::cout << "\n--- Filter Scholarships ---\n";
    std::cout << "For any criterion, press Enter to ignore it.\n";
    std::cout << "Min GPA (or blank): " << std::flush;
    std::string sg;
    getline(std::cin, sg);
    std::cout << "Min Amount (or blank): " << std::flush;
    std::string sa1;
    getline(std::cin, sa1);
    std::cout << "Max Amount (or blank): " << std::flush;
    std::string sa2;
    getline(std::cin, sa2);
    std::cout << "Category (or blank): " << std::flush;
    std::string cat;
    getline(std::cin, cat);
    std::cout << "State (or blank): " << std::flush;
    std::string st;
    getline(std::cin, st);
    std::cout << "Degree/School (or blank): " << std::flush;
    std::string deg;
    getline(std::cin, deg);

    double g = -1, a1 = -1, a2 = -1;
    if (!sg.empty())
        try
        {
            g = std::stod(sg);
        }
        catch (...)
        {
            g = -1;
        }
    if (!sa1.empty())
        try
        {
            a1 = std::stod(sa1);
        }
        catch (...)
        {
            a1 = -1;
        }
    if (!sa2.empty())
        try
        {
            a2 = std::stod(sa2);
        }
        catch (...)
        {
            a2 = -1;
        }

    ResultList res;
    collectIf(root, res, [g, a1, a2, cat, st, deg](const Scholarship &s)
              {
                  if (g >= 0 && !(g >= s.minGPA)) return false;
                  if (a1 >= 0 && !(s.amount >= a1)) return false;
                  if (a2 >= 0 && !(s.amount <= a2)) return false;
                  if (!cat.empty() && toLower(s.category) != toLower(cat)) return false;
                  if (!st.empty() && toLower(s.state) != toLower(st)) return false;
                  if (!deg.empty() && toLower(s.degreeOrSchool) != toLower(deg)) return false;
                  return true; });
    printResultList(res);
}

void studentMenuLoop()
{
    while (true)
    {
        std::cout << "\n=== Student Menu ===\n";
        std::cout << "1. View all scholarships\n2. Search scholarships\n3. Sort scholarships\n4. Filter scholarships\n0. Back\n";
        int ch = readInt("Choice: ", true);
        if (ch == 0)
            break;
        if (ch == 1)
            studentViewAll();
        else if (ch == 2)
            studentSearchMenu();
        else if (ch == 3)
            studentSortMenu();
        else if (ch == 4)
            studentFilterMenu();
        else
            std::cout << "Invalid choice.\n";
    }
}