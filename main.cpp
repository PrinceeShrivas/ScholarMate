#include "core/bst.h"
#include "core/uploader.h"
#include "core/scholarship.h"
#include "services/student_service.h"
#include "services/uploader_service.h"
#include "utils/input_helpers.h"
#include "utils/string_utils.h"
#include <iostream>

void seedData()
{
    uploaders["admin"] = Uploader("adminpw");
    Scholarship a(nextScholarshipID++, "STEM Excellence", "STEM Foundation", 5000.0, 3.5, 100000, "degree", "STEM", "California", "admin", "contact@stem.org", "Support for STEM students.", "2025-12-31");
    Scholarship b(nextScholarshipID++, "Arts Support", "Arts Council", 2000.0, 3.0, 100000, "school", "Arts", "New York", "admin", "arts@support.org", "Grants for arts students.", "2025-11-30");
    Scholarship c(nextScholarshipID++, "Merit Grant", "Merit Foundation", 3000.0, 3.7, 120000, "degree", "General", "Texas", "admin", "info@merit.org", "Merit-based scholarship.", "2025-10-15");
    root = insertBST(root, a);
    root = insertBST(root, b);
    root = insertBST(root, c);
    addToUploaderList("admin", a.id);
    addToUploaderList("admin", b.id);
    addToUploaderList("admin", c.id);
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cout << "=========================================\n";
    std::cout << "            ScholarMate      \n";
    std::cout << "=========================================\n\n";

    seedData();

    while (true)
    {
        std::cout << "\nMain Menu:\n1. Uploader\n2. Student\n0. Exit\n";
        int ch = readInt("Choice: ", true);
        if (ch == 0)
            break;
        if (ch == 1)
        {
            std::cout << "\n--- Uploader ---\n";
            std::string userID = readNonEmptyLine("Enter userID: ");
            std::string uidLower = toLower(userID);
            if (uploaders.find(uidLower) == uploaders.end())
            {
                std::cout << "User does not exist. Creating new uploader.\n";
                if (!createUploader(uidLower))
                    continue;
            }
            if (!loginUploader(uidLower))
                continue;
            uploaderMenuLoop(uidLower);
        }
        else if (ch == 2)
        {
            std::cout << "\n--- Student ---\n";
            studentMenuLoop();
        }
        else
        {
            std::cout << "Invalid choice.\n";
        }
    }

    cleanupBST(root);
    cleanupUploaders();
    std::cout << "Exiting. Goodbye.\n";
    return 0;
}