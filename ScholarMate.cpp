#include <bits/stdc++.h>
using namespace std;

/* ---------------------------
   Data Structures & Helpers
   --------------------------- */

struct Scholarship
{
    int id;
    string title;
    double amount;
    double minGPA;
    string category;
    string state;
    string uploaderID;
    string provider;
    double annualIncome;
    string degreeOrSchool;
    string contactInfo;
    string description;
    string endDate;

    Scholarship() = default;
    Scholarship(int _id, const string &t, const string &prov, double a, double g, double income,
                const string &deg, const string &c, const string &s, const string &u,
                const string &contact, const string &desc, const string &end)
        : id(_id), title(t), provider(prov), amount(a), minGPA(g), annualIncome(income),
          degreeOrSchool(deg), category(c), state(s), uploaderID(u),
          contactInfo(contact), description(desc), endDate(end) {}
};

struct BSTNode
{
    Scholarship data;
    BSTNode *left, *right;
    BSTNode(const Scholarship &s) : data(s), left(nullptr), right(nullptr) {}
};

struct IDListNode
{
    int scholarshipID;
    IDListNode *next;
    IDListNode(int id) : scholarshipID(id), next(nullptr) {}
};

struct Uploader
{
    string password;
    IDListNode *head;
    Uploader() : head(nullptr) {}
    Uploader(const string &pw) : password(pw), head(nullptr) {}
};

BSTNode *root = nullptr;
unordered_map<string, Uploader> uploaders;
int nextScholarshipID = 1;

/* ---------------------------
   Input helpers
   --------------------------- */

static void clearCin()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static string readLineTrim()
{
    string s;
    getline(cin, s);
    auto l = s.find_first_not_of(" \t\r\n");
    if (l == string::npos)
        return "";
    auto r = s.find_last_not_of(" \t\r\n");
    return s.substr(l, r - l + 1);
}

static string readNonEmptyLine(const string &prompt)
{
    string s;
    while (true)
    {
        cout << prompt << flush;
        s = readLineTrim();
        if (!s.empty())
            return s;
        cout << "Input cannot be empty. Try again.\n";
    }
}

static double readDouble(const string &prompt, bool allowNegative = false)
{
    while (true)
    {
        cout << prompt << flush;
        string s;
        getline(cin, s);
        try
        {
            double v = stod(s);
            if (!allowNegative && v < 0)
            {
                cout << "Value cannot be negative.\n";
                continue;
            }
            return v;
        }
        catch (...)
        {
            cout << "Please enter a valid number.\n";
        }
    }
}

static int readInt(const string &prompt, bool allowNegative = false)
{
    while (true)
    {
        cout << prompt << flush;
        string s;
        getline(cin, s);
        try
        {
            long v = stol(s);
            if (!allowNegative && v < 0)
            {
                cout << "Value cannot be negative.\n";
                continue;
            }
            return static_cast<int>(v);
        }
        catch (...)
        {
            cout << "Please enter a valid integer.\n";
        }
    }
}

static bool isValidDateFormat(const string &date)
{
    // Basic check for YYYY-MM-DD format (e.g., 2025-12-31)
    regex datePattern("\\d{4}-\\d{2}-\\d{2}");
    return regex_match(date, datePattern);
}

/* ---------------------------
   Case-insensitive helper
   --------------------------- */

string toLower(const string &s)
{
    string r = s;
    transform(r.begin(), r.end(), r.begin(), ::tolower);
    return r;
}

/* ---------------------------
   BST operations
   --------------------------- */

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

BSTNode *insertBST(BSTNode *node, const Scholarship &s)
{
    if (!node)
        return new BSTNode(s);
    int c = cmpScholarship(s, node->data);
    if (c < 0)
        node->left = insertBST(node->left, s);
    else
        node->right = insertBST(node->right, s);
    return node;
}

void inorderTraverse(BSTNode *node, function<void(const Scholarship &)> f)
{
    if (!node)
        return;
    inorderTraverse(node->left, f);
    f(node->data);
    inorderTraverse(node->right, f);
}

BSTNode *findByID(BSTNode *node, int id)
{
    if (!node)
        return nullptr;
    if (node->data.id == id)
        return node;
    BSTNode *left = findByID(node->left, id);
    if (left)
        return left;
    return findByID(node->right, id);
}

BSTNode *deleteByKey(BSTNode *node, double amount, int id)
{
    if (!node)
        return nullptr;
    int c = cmpScholarshipKey(amount, id, node->data);
    if (c < 0)
        node->left = deleteByKey(node->left, amount, id);
    else if (c > 0)
        node->right = deleteByKey(node->right, amount, id);
    else
    {
        if (!node->left)
        {
            BSTNode *r = node->right;
            delete node;
            return r;
        }
        else if (!node->right)
        {
            BSTNode *l = node->left;
            delete node;
            return l;
        }
        else
        {
            BSTNode *succParent = node;
            BSTNode *succ = node->right;
            while (succ->left)
            {
                succParent = succ;
                succ = succ->left;
            }
            node->data = succ->data;
            if (succParent == node)
                succParent->right = deleteByKey(succParent->right, succ->data.amount, succ->data.id);
            else
                succParent->left = deleteByKey(succParent->left, succ->data.amount, succ->data.id);
        }
    }
    return node;
}

/* ---------------------------
   Uploader linked list helpers
   --------------------------- */

void addToUploaderList(const string &uploaderID, int scholarshipID)
{
    auto it = uploaders.find(uploaderID);
    if (it == uploaders.end())
        return;
    IDListNode *n = new IDListNode(scholarshipID);
    n->next = it->second.head;
    it->second.head = n;
}

void removeFromUploaderList(const string &uploaderID, int scholarshipID)
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

/* ---------------------------
   Printing helpers
   --------------------------- */

void printScholarshipShort(const Scholarship &s)
{
    cout << "[" << s.id << "] " << s.title << " | Amount: " << s.amount
         << " | MinGPA: " << s.minGPA << " | Cat: " << s.category
         << " | State: " << s.state << " | Degree/School: " << s.degreeOrSchool
         << " | By: " << s.uploaderID << '\n';
}

void printScholarshipDetail(const Scholarship &s)
{
    cout << "ID: " << s.id << '\n';
    cout << "Title: " << s.title << '\n';
    cout << "Provider: " << s.provider << '\n';
    cout << "Amount: " << s.amount << '\n';
    cout << "Minimum GPA: " << s.minGPA << '\n';
    cout << "Maximum Annual Income: " << s.annualIncome << '\n';
    cout << "Degree/School: " << s.degreeOrSchool << '\n';
    cout << "Category: " << s.category << '\n';
    cout << "State: " << s.state << '\n';
    cout << "Uploader: " << s.uploaderID << '\n';
    cout << "Contact Info: " << s.contactInfo << '\n';
    cout << "Description: " << s.description << '\n';
    cout << "End Date: " << s.endDate << '\n';
    cout << "----------------------------------------\n";
}

/* ---------------------------
   Result list
   --------------------------- */

struct ResultList
{
    IDListNode *head;
    ResultList() : head(nullptr) {}
    void pushFront(int id)
    {
        IDListNode *n = new IDListNode(id);
        n->next = head;
        head = n;
    }
    void clear()
    {
        IDListNode *cur = head;
        while (cur)
        {
            IDListNode *nx = cur->next;
            delete cur;
            cur = nx;
        }
        head = nullptr;
    }
    ~ResultList() { clear(); }
};

void printResultList(ResultList &r)
{
    if (!r.head)
    {
        cout << "No results found.\n";
        return;
    }
    cout << "=== Results ===\n";
    IDListNode *cur = r.head;
    while (cur)
    {
        BSTNode *n = findByID(root, cur->scholarshipID);
        if (n)
            printScholarshipDetail(n->data);
        cur = cur->next;
    }
}

/* ---------------------------
   Collect helpers
   --------------------------- */

void collectIf(BSTNode *node, ResultList &res, function<bool(const Scholarship &)> pred)
{
    if (!node)
        return;
    collectIf(node->left, res, pred);
    if (pred(node->data))
        res.pushFront(node->data.id);
    collectIf(node->right, res, pred);
}

/* ---------------------------
   Uploader operations
   --------------------------- */

bool createUploader(const string &userID)
{
    string uid = toLower(userID);
    if (uploaders.find(uid) != uploaders.end())
    {
        cout << "User already exists.\n";
        return false;
    }
    string pw = readNonEmptyLine("Set password for new user: ");
    uploaders[uid] = Uploader(pw);
    cout << "Uploader '" << uid << "' created.\n";
    return true;
}

bool loginUploader(const string &userID)
{
    string uid = toLower(userID);
    auto it = uploaders.find(uid);
    if (it == uploaders.end())
    {
        cout << "No such user found.\n";
        return false;
    }
    string pw = readNonEmptyLine("Enter password: ");
    if (pw == it->second.password)
    {
        cout << "Login successful.\n";
        return true;
    }
    else
    {
        cout << "Incorrect password.\n";
        return false;
    }
}

void uploaderAddScholarship(const string &uploaderID)
{
    cout << "\n--- Add New Scholarship ---\n";
    string title = readNonEmptyLine("Title: ");
    string provider = readNonEmptyLine("Provider Name: ");
    double amount = readDouble("Amount: ");
    double gpa = readDouble("Minimum GPA: ");
    double income = readDouble("Maximum Annual Income: ");
    string degreeOrSchool = readNonEmptyLine("Degree or School (e.g., degree/school): ");
    string category = readNonEmptyLine("Category (e.g., none/SC/OBC/ST): ");
    string state = readNonEmptyLine("State: ");
    string contact = readNonEmptyLine("Contact Info (email/phone/website): ");
    string description = readNonEmptyLine("Description & Website: ");
    string endDate;
    while (true)
    {
        endDate = readNonEmptyLine("End Date (YYYY-MM-DD): ");
        if (isValidDateFormat(endDate))
            break;
        cout << "Invalid date format. Please use YYYY-MM-DD.\n";
    }

    int id = nextScholarshipID++;
    Scholarship s(id, title, provider, amount, gpa, income, degreeOrSchool,
                  category, state, uploaderID, contact, description, endDate);
    root = insertBST(root, s);
    addToUploaderList(uploaderID, id);
    cout << "Scholarship added with ID " << id << ".\n";
}

void uploaderUpdateScholarship(const string &uploaderID)
{
    cout << "\n--- Update Scholarship ---\n";
    int id = readInt("Enter scholarship ID: ");
    BSTNode *node = findByID(root, id);
    if (!node)
    {
        cout << "Scholarship not found.\n";
        return;
    }
    if (node->data.uploaderID != uploaderID)
    {
        cout << "You can only update scholarships uploaded by you.\n";
        return;
    }
    printScholarshipDetail(node->data);

    cout << "Enter new values or leave blank to keep current.\n";
    string title = readLineTrim();
    if (title.empty())
        cout << "Keeping current title.\n";
    string provider = readLineTrim();
    if (provider.empty())
        cout << "Keeping current provider.\n";
    cout << "New Amount (or blank): " << flush;
    string sAmount;
    getline(cin, sAmount);
    cout << "New Min GPA (or blank): " << flush;
    string sGpa;
    getline(cin, sGpa);
    cout << "New Max Annual Income (or blank): " << flush;
    string sIncome;
    getline(cin, sIncome);
    string degreeOrSchool = readLineTrim();
    if (degreeOrSchool.empty())
        cout << "Keeping current degree/school.\n";
    string category = readLineTrim();
    if (category.empty())
        cout << "Keeping current category.\n";
    string state = readLineTrim();
    if (state.empty())
        cout << "Keeping current state.\n";
    string contact = readLineTrim();
    if (contact.empty())
        cout << "Keeping current contact info.\n";
    string description = readLineTrim();
    if (description.empty())
        cout << "Keeping current description.\n";
    string endDate = readLineTrim();
    if (!endDate.empty() && !isValidDateFormat(endDate))
    {
        cout << "Invalid date format. Keeping current end date.\n";
        endDate = node->data.endDate;
    }
    else if (endDate.empty())
    {
        cout << "Keeping current end date.\n";
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
            double newAmount = stod(sAmount);
            if (newAmount < 0)
            {
                cout << "Amount cannot be negative. Keeping old amount.\n";
                isValid = false;
            }
            else
            {
                updated.amount = newAmount;
            }
        }
        catch (...)
        {
            cout << "Invalid amount input; keeping old.\n";
            isValid = false;
        }
    }
    if (!sGpa.empty())
    {
        try
        {
            double newGpa = stod(sGpa);
            if (newGpa < 0)
            {
                cout << "GPA cannot be negative. Keeping old GPA.\n";
                isValid = false;
            }
            else
            {
                updated.minGPA = newGpa;
            }
        }
        catch (...)
        {
            cout << "Invalid GPA input; keeping old.\n";
            isValid = false;
        }
    }
    if (!sIncome.empty())
    {
        try
        {
            double newIncome = stod(sIncome);
            if (newIncome < 0)
            {
                cout << "Annual income cannot be negative. Keeping old income.\n";
                isValid = false;
            }
            else
            {
                updated.annualIncome = newIncome;
            }
        }
        catch (...)
        {
            cout << "Invalid income input; keeping old.\n";
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
        cout << "Updated scholarship (amount changed, reinserted).\n";
    }
    else if (isValid)
    {
        node->data = updated;
        cout << "Updated scholarship in place.\n";
    }
    else
    {
        cout << "Update partially applied due to invalid inputs.\n";
    }

    cout << "\n--- Updated Scholarship Details ---\n";
    printScholarshipDetail(updated);
}

void uploaderDeleteScholarship(const string &uploaderID)
{
    cout << "\n--- Delete Scholarship ---\n";
    int id = readInt("Enter scholarship ID to delete: ");
    BSTNode *node = findByID(root, id);
    if (!node)
    {
        cout << "Scholarship not found.\n";
        return;
    }
    if (node->data.uploaderID != uploaderID)
    {
        cout << "You can only delete scholarships uploaded by you.\n";
        return;
    }
    root = deleteByKey(root, node->data.amount, node->data.id);
    removeFromUploaderList(uploaderID, id);
    cout << "Scholarship deleted.\n";
}

void uploaderShowAll(const string &uploaderID)
{
    cout << "\n--- My Scholarships ---\n";
    auto it = uploaders.find(uploaderID);
    if (it == uploaders.end())
    {
        cout << "Uploader not found.\n";
        return;
    }
    IDListNode *cur = it->second.head;
    if (!cur)
    {
        cout << "You have not uploaded any scholarships yet.\n";
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

/* ---------------------------
   Student operations
   --------------------------- */

void studentViewAll()
{
    cout << "\n--- All Scholarships (sorted by amount ascending) ---\n";
    bool any = false;
    inorderTraverse(root, [&](const Scholarship &s)
                    {
                        printScholarshipDetail(s);
                        any = true; });
    if (!any)
        cout << "No scholarships available.\n";
}

void studentSearchMenu()
{
    cout << "\n--- Search Scholarships ---\n";
    cout << "Enter keyword to search (title, provider, description, category, state, degree/school) or leave blank: ";
    string keyword;
    getline(cin, keyword);
    ResultList res;
    if (!keyword.empty())
    {
        collectIf(root, res, [keyword](const Scholarship &s)
                  {
                      string k = toLower(keyword);
                      return toLower(s.title).find(k) != string::npos ||
                             toLower(s.provider).find(k) != string::npos ||
                             toLower(s.description).find(k) != string::npos ||
                             toLower(s.category).find(k) != string::npos ||
                             toLower(s.state).find(k) != string::npos ||
                             toLower(s.degreeOrSchool).find(k) != string::npos; });
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
    cout << "\n--- Sort Scholarships ---\n";
    cout << "1. Amount (asc)\n2. Amount (desc)\n3. MinGPA (asc)\n4. MinGPA (desc)\n5. Title (A-Z)\n0. Back\n";
    int ch = readInt("Choice: ", true);
    if (ch == 0)
        return;
    vector<Scholarship> v;
    inorderTraverse(root, [&v](const Scholarship &s)
                    { v.push_back(s); });
    if (v.empty())
    {
        cout << "No scholarships available.\n";
        return;
    }
    switch (ch)
    {
    case 1:
        sort(v.begin(), v.end(), [](auto &a, auto &b)
             { return a.amount < b.amount; });
        break;
    case 2:
        sort(v.begin(), v.end(), [](auto &a, auto &b)
             { return a.amount > b.amount; });
        break;
    case 3:
        sort(v.begin(), v.end(), [](auto &a, auto &b)
             { return a.minGPA < b.minGPA; });
        break;
    case 4:
        sort(v.begin(), v.end(), [](auto &a, auto &b)
             { return a.minGPA > b.minGPA; });
        break;
    case 5:
        sort(v.begin(), v.end(), [](auto &a, auto &b)
             { return toLower(a.title) < toLower(b.title); });
        break;
    default:
        cout << "Invalid option.\n";
        return;
    }
    cout << "Sorted result:\n";
    for (auto &s : v)
        printScholarshipDetail(s);
}

/* ---------------------------
   Filter Scholarships
   --------------------------- */

void studentFilterMenu()
{
    cout << "\n--- Filter Scholarships ---\n";
    cout << "For any criterion, press Enter to ignore it.\n";
    cout << "Min GPA (or blank): " << flush;
    string sg;
    getline(cin, sg);
    cout << "Min Amount (or blank): " << flush;
    string sa1;
    getline(cin, sa1);
    cout << "Max Amount (or blank): " << flush;
    string sa2;
    getline(cin, sa2);
    cout << "Category (or blank): " << flush;
    string cat;
    getline(cin, cat);
    cout << "State (or blank): " << flush;
    string st;
    getline(cin, st);
    cout << "Degree/School (or blank): " << flush;
    string deg;
    getline(cin, deg);

    double g = -1, a1 = -1, a2 = -1;
    if (!sg.empty())
        try
        {
            g = stod(sg);
        }
        catch (...)
        {
            g = -1;
        }
    if (!sa1.empty())
        try
        {
            a1 = stod(sa1);
        }
        catch (...)
        {
            a1 = -1;
        }
    if (!sa2.empty())
        try
        {
            a2 = stod(sa2);
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

/* ---------------------------
   Menus & Main
   --------------------------- */

void uploaderMenuLoop(const string &uploaderID)
{
    while (true)
    {
        cout << "\n=== Uploader Menu (" << uploaderID << ") ===\n";
        cout << "1. Add new scholarship\n2. Update scholarship\n3. Delete scholarship\n4. Show all my scholarships\n0. Logout\n";
        int ch = readInt("Choice: ", true);
        if (ch == 0)
        {
            cout << "Logging out...\n";
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
            cout << "Invalid option.\n";
    }
}

void studentMenuLoop()
{
    while (true)
    {
        cout << "\n=== Student Menu ===\n";
        cout << "1. View all scholarships\n2. Search scholarships\n3. Sort scholarships\n4. Filter scholarships\n0. Back\n";
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
            cout << "Invalid choice.\n";
    }
}

/* ---------------------------
   Memory cleanup & seeding
   --------------------------- */

void cleanupBST(BSTNode *node)
{
    if (!node)
        return;
    cleanupBST(node->left);
    cleanupBST(node->right);
    delete node;
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

/* ---------------------------
   Main
   --------------------------- */

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=========================================\n";
    cout << "            ScholarMate      \n";
    cout << "=========================================\n\n";

    seedData();

    while (true)
    {
        cout << "\nMain Menu:\n1. Uploader\n2. Student\n0. Exit\n";
        int ch = readInt("Choice: ", true);
        if (ch == 0)
            break;
        if (ch == 1)
        {
            cout << "\n--- Uploader ---\n";
            string userID = readNonEmptyLine("Enter userID: ");
            string uidLower = toLower(userID);
            if (uploaders.find(uidLower) == uploaders.end())
            {
                cout << "User does not exist. Creating new uploader.\n";
                if (!createUploader(uidLower))
                    continue;
            }
            if (!loginUploader(uidLower))
                continue;
            uploaderMenuLoop(uidLower);
        }
        else if (ch == 2)
        {
            cout << "\n--- Student ---\n";
            studentMenuLoop();
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }

    cleanupBST(root);
    cleanupUploaders();
    cout << "Exiting. Goodbye.\n";
    return 0;
}
