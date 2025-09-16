#include "services/uploader_service.h"
#include "core/bst.h"
#include "core/scholarship.h"
#include "core/uploader.h"
#include "utils/input_helpers.h"
#include "utils/string_utils.h"
#include <iostream>

void uploaderAddScholarship(const std::string &uploaderID)
{
    std::cout << "\n--- Add New Scholarship ---\n";
    std::string title = readNonEmptyLine("Title: ");
    std::string provider = readNonEmptyLine("Provider Name: ");
    double amount = readDouble("Amount: ");
    double gpa = readDouble("Minimum GPA: ");
    double income = readDouble("Maximum Annual Income: ");
    std::string degreeOrSchool = readNonEmptyLine("Degree or School (e.g., degree/school): ");
    std::string category = readNonEmptyLine("Category (e.g., none/SC/OBC/ST): ");
    std::string state = readNonEmptyLine("State: ");
    std::string contact = readNonEmptyLine("Contact Info (email/phone/website): ");
    std::string description = readNonEmptyLine("Description & Website: ");
    std::string endDate;
    while (true)
    {
        endDate = readNonEmptyLine("End Date (YYYY-MM-DD): ");
        if (isValidDateFormat(endDate))
            break;
        std::cout << "Invalid date format. Please use YYYY-MM-DD.\n";
    }

    int id = nextScholarshipID++;
    Scholarship s(id, title, provider, amount, gpa, income, degreeOrSchool,
                  category, state, uploaderID, contact, description, endDate);
    root = insertBST(root, s);
    addToUploaderList(uploaderID, id);
    std::cout << "Scholarship added with ID " << id << ".\n";
}

void uploaderUpdateScholarship(const std::string &uploaderID)
{
    std::cout << "\n--- Update Scholarship ---\n";
    int id = readInt("Enter scholarship ID: ");
    BSTNode *node = findByID(root, id);
    if (!node)
    {
        std::cout << "Scholarship not found.\n";
        return;
    }
    if (node->data.uploaderID != uploaderID)
    {
        std::cout << "You can only update scholarships uploaded by you.\n";
        return;
    }
    printScholarshipDetail(node->data);

    std::cout << "Enter new values or leave blank to keep current.\n";
    std::string title = readLineTrim();
    if (title.empty())
        std::cout << "Keeping current title.\n";
    std::string provider = readLineTrim();
    if (provider.empty())
        std::cout << "Keeping current provider.\n";
    std::cout << "New Amount (or blank): " << std::flush;
    std::string sAmount;
    getline(std::cin, sAmount);
    std::cout << "New Min GPA (or blank): " << std::flush;
    std::string sGpa;
    getline(std::cin, sGpa);
    std::cout << "New Max Annual Income (or blank): " << std::flush;
    std::string sIncome;
    getline(std::cin, sIncome);
    std::string degreeOrSchool = readLineTrim();
    if (degreeOrSchool.empty())
        std::cout << "Keeping current degree/school.\n";
    std::string category = readLineTrim();
    if (category.empty())
        std::cout << "Keeping current category.\n";
    std::string state = readLineTrim();
    if (state.empty())
        std::cout << "Keeping current state.\n";
    std::string contact = readLineTrim();
    if (contact.empty())
        std::cout << "Keeping current contact info.\n";
    std::string description = readLineTrim();
    if (description.empty())
        std::cout << "Keeping current description.\n";
    std::string endDate = readLineTrim();
    if (!endDate.empty() && !isValidDateFormat(endDate))
    {
        std::cout << "Invalid date format. Keeping current end date.\n";
        endDate = node->data.endDate;
    }
    else if (endDate.empty())
    {
        std::cout << "Keeping current end date.\n";
    }

    Scholarship updated = node->data;
    bool isValid = true;

    if (!title.empty())
        updated.title = title;
    if (!provider.empty())
        updated.provider = provider;
    if (!sAmount.empty())
    {
        try
        {
            double newAmount = std::stod(sAmount);
            if (newAmount < 0)
            {
                std::cout << "Amount cannot be negative. Keeping old amount.\n";
                isValid = false;
            }
            else
            {
                updated.amount = newAmount;
            }
        }
        catch (...)
        {
            std::cout << "Invalid amount input; keeping old.\n";
            isValid = false;
        }
    }
    if (!sGpa.empty())
    {
        try
        {
            double newGpa = std::stod(sGpa);
            if (newGpa < 0)
            {
                std::cout << "GPA cannot be negative. Keeping old GPA.\n";
                isValid = false;
            }
            else
            {
                updated.minGPA = newGpa;
            }
        }
        catch (...)
        {
            std::cout << "Invalid GPA input; keeping old.\n";
            isValid = false;
        }
    }
    if (!sIncome.empty())
    {
        try
        {
            double newIncome = std::stod(sIncome);
            if (newIncome < 0)
            {
                std::cout << "Annual income cannot be negative. Keeping old income.\n";
                isValid = false;
            }
            else
            {
                updated.annualIncome = newIncome;
            }
        }
        catch (...)
        {
            std::cout << "Invalid income input; keeping old.\n";
            isValid = false;
        }
    }
    if (!degreeOrSchool.empty())
        updated.degreeOrSchool = degreeOrSchool;
    if (!category.empty())
        updated.category = category;
    if (!state.empty())
        updated.state = state;
    if (!contact.empty())
        updated.contactInfo = contact;
    if (!description.empty())
        updated.description = description;
    if (!endDate.empty())
        updated.endDate = endDate;

    if (isValid && updated.amount != node->data.amount)
    {
        root = deleteByKey(root, node->data.amount, node->data.id);
        root = insertBST(root, updated);
        std::cout << "Updated scholarship (amount changed, reinserted).\n";
    }
    else if (isValid)
    {
        node->data = updated;
        std::cout << "Updated scholarship in place.\n";
    }
    else
    {
        std::cout << "Update partially applied due to invalid inputs.\n";
    }

    std::cout << "\n--- Updated Scholarship Details ---\n";
    printScholarshipDetail(updated);
}

void uploaderDeleteScholarship(const std::string &uploaderID)
{
    std::cout << "\n--- Delete Scholarship ---\n";
    int id = readInt("Enter scholarship ID to delete: ");
    BSTNode *node = findByID(root, id);
    if (!node)
    {
        std::cout << "Scholarship not found.\n";
        return;
    }
    if (node->data.uploaderID != uploaderID)
    {
        std::cout << "You can only delete scholarships uploaded by you.\n";
        return;
    }
    root = deleteByKey(root, node->data.amount, node->data.id);
    removeFromUploaderList(uploaderID, id);
    std::cout << "Scholarship deleted.\n";
}

void uploaderShowAll(const std::string &uploaderID)
{
    std::cout << "\n--- My Scholarships ---\n";
    auto it = uploaders.find(uploaderID);
    if (it == uploaders.end())
    {
        std::cout << "Uploader not found.\n";
        return;
    }
    IDListNode *cur = it->second.head;
    if (!cur)
    {
        std::cout << "You have not uploaded any scholarships yet.\n";
        return;
    }
    while (cur)
    {
        BSTNode *n = findByID(root, cur->scholarshipID);
        if (n)
            printScholarshipDetail(n->data);
        cur = cur->next;
    }
}

void uploaderMenuLoop(const std::string &uploaderID)
{
    while (true)
    {
        std::cout << "\n=== Uploader Menu (" << uploaderID << ") ===\n";
        std::cout << "1. Add new scholarship\n2. Update scholarship\n3. Delete scholarship\n4. Show all my scholarships\n0. Logout\n";
        int ch = readInt("Choice: ", true);
        if (ch == 0)
        {
            std::cout << "Logging out...\n";
            break;
        }
        if (ch == 1)
            uploaderAddScholarship(uploaderID);
        else if (ch == 2)
            uploaderUpdateScholarship(uploaderID);
        else if (ch == 3)
            uploaderDeleteScholarship(uploaderID);
        else if (ch == 4)
            uploaderShowAll(uploaderID);
        else
            std::cout << "Invalid option.\n";
    }
}