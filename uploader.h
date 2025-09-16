#ifndef UPLOADER_H
#define UPLOADER_H

#include "core/bst.h"
#include <string>
#include <unordered_map>

struct Uploader
{
    std::string password;
    IDListNode *head;
    Uploader();
    Uploader(const std::string &pw);
};

// Global uploaders map
extern std::unordered_map<std::string, Uploader> uploaders;

// Uploader operations
bool createUploader(const std::string &userID);
bool loginUploader(const std::string &userID);
void addToUploaderList(const std::string &uploaderID, int scholarshipID);
void removeFromUploaderList(const std::string &uploaderID, int scholarshipID);
void cleanupUploaders();

#endif