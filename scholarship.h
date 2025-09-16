#ifndef SCHOLARSHIP_H
#define SCHOLARSHIP_H

#include <string>

struct Scholarship
{
    int id;
    std::string title;
    double amount;
    double minGPA;
    std::string category;
    std::string state;
    std::string uploaderID;
    std::string provider;
    double annualIncome;
    std::string degreeOrSchool;
    std::string contactInfo;
    std::string description;
    std::string endDate;

    Scholarship() = default;
    Scholarship(int _id, const std::string &t, const std::string &prov, double a, double g, double income,
                const std::string &deg, const std::string &c, const std::string &s, const std::string &u,
                const std::string &contact, const std::string &desc, const std::string &end);
};

// Comparison functions
int cmpScholarshipKey(double amountA, int idA, const Scholarship &b);
int cmpScholarship(const Scholarship &a, const Scholarship &b);

// Printing functions
void printScholarshipShort(const Scholarship &s);
void printScholarshipDetail(const Scholarship &s);

#endif