#ifndef UPLOADER_SERVICE_H
#define UPLOADER_SERVICE_H

#include <string>

void uploaderAddScholarship(const std::string &uploaderID);
void uploaderUpdateScholarship(const std::string &uploaderID);
void uploaderDeleteScholarship(const std::string &uploaderID);
void uploaderShowAll(const std::string &uploaderID);
void uploaderMenuLoop(const std::string &uploaderID);

#endif