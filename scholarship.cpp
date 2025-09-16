#include "core/scholarship.h"
#include <iostream>

Scholarship::Scholarship(int _id, const std::string &t, const std::string &prov, double a, double g, double income,
                         const std::string &deg, const std::string &c, const std::string &s, const std::string &u,
                         const std::string &contact, const std::string &desc, const std::string &end)
    : id(_id), title(t), provider(prov), amount(a), minGPA(g), annualIncome(income),
      degreeOrSchool(deg), category(c), state(s), uploaderID(u),
      contactInfo(contact), description(desc), endDate(end) {}

int cmpScholarshipKey(double amountA, int idA, const Scholarship &b)
{
    if (amountA < b.amount)
        return -1;
    if (amountA > b.amount)
        return 1;
    if (idA < b.id)
        return -1;
    if (idA > b.id)
        return 1;
    return 0;
}

int cmpScholarship(const Scholarship &a, const Scholarship &b)
{
    if (a.amount < b.amount)
        return -1;
    if (a.amount > b.amount)
        return 1;
    if (a.id < b.id)
        return -1;
    if (a.id > b.id)
        return 1;
    return 0;
}

void printScholarshipShort(const Scholarship &s)
{
    std::cout << "[" << s.id << "] " << s.title << " | Amount: " << s.amount
              << " | MinGPA: " << s.minGPA << " | Cat: " << s.category
              << " | State: " << s.state << " | Degree/School: " << s.degreeOrSchool
              << " | By: " << s.uploaderID << '\n';
}

void printScholarshipDetail(const Scholarship &s)
{
    std::cout << "ID: " << s.id << '\n';
    std::cout << "Title: " << s.title << '\n';
    std::cout << "Provider: " << s.provider << '\n';
    std::cout << "Amount: " << s.amount << '\n';
    std::cout << "Minimum GPA: " << s.minGPA << '\n';
    std::cout << "Maximum Annual Income: " << s.annualIncome << '\n';
    std::cout << "Degree/School: " << s.degreeOrSchool << '\n';
    std::cout << "Category: " << s.category << '\n';
    std::cout << "State: " << s.state << '\n';
    std::cout << "Uploader: " << s.uploaderID << '\n';
    std::cout << "Contact Info: " << s.contactInfo << '\n';
    std::cout << "Description: " << s.description << '\n';
    std::cout << "End Date: " << s.endDate << '\n';
    std::cout << "----------------------------------------\n";
}