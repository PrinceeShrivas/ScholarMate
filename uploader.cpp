#include "core/uploader.h"
#include "utils/input_helpers.h"
#include "utils/string_utils.h"
#include <iostream>

std::unordered_map<std::string, Uploader> uploaders;

Uploader::Uploader() : head(nullptr) {}
Uploader::Uploader(const std::string &pw) : password(pw), head(nullptr) {}

bool createUploader(const std::string &userID)
{
    std::string uid = toLower(userID);
    if (uploaders.find(uid) != uploaders.end())
    {
        std::cout << "User already exists.\n";
        return false;
    }
    std::string pw = readNonEmptyLine("Set password for new user: ");
    uploaders[uid] = Uploader(pw);
    std::cout << "Uploader '" << uid << "' created.\n";
    return true;
}

bool loginUploader(const std::string &userID)
{
    std::string uid = toLower(userID);
    auto it = uploaders.find(uid);
    if (it == uploaders.end())
    {
        std::cout << "No such user found.\n";
        return false;
    }
    std::string pw = readNonEmptyLine("Enter password: ");
    if (pw == it->second.password)
    {
        std::cout << "Login successful.\n";
        return true;
    }
    else
    {
        std::cout << "Incorrect password.\n";
        return false;
    }
}

void addToUploaderList(const std::string &uploaderID, int scholarshipID)
{
    auto it = uploaders.find(uploaderID);
    if (it == uploaders.end())
        return;
    IDListNode *n = new IDListNode(scholarshipID);
    n->next = it->second.head;
    it->second.head = n;
}

void removeFromUploaderList(const std::string &uploaderID, int scholarshipID)
{
    auto it = uploaders.find(uploaderID);
    if (it == uploaders.end())
        return;
    IDListNode *cur = it->second.head;
    IDListNode *prev = nullptr;
    while (cur)
    {
        if (cur->scholarshipID == scholarshipID)
        {
            if (prev)
                prev->next = cur->next;
            else
                it->second.head = cur->next;
            delete cur;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void cleanupUploaders()
{
    for (auto &p : uploaders)
    {
        IDListNode *cur = p.second.head;
        while (cur)
        {
            IDListNode *nx = cur->next;
            delete cur;
            cur = nx;
        }
        p.second.head = nullptr;
    }
}