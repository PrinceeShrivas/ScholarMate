#ifndef BST_H
#define BST_H

#include "core/scholarship.h"
#include <functional>

struct BSTNode
{
    Scholarship data;
    BSTNode *left, *right;
    BSTNode(const Scholarship &s);
};

struct IDListNode
{
    int scholarshipID;
    IDListNode *next;
    IDListNode(int id);
};

struct ResultList
{
    IDListNode *head;
    ResultList();
    void pushFront(int id);
    void clear();
    ~ResultList();
};

// Global BST root and ID counter
extern BSTNode *root;
extern int nextScholarshipID;

// BST operations
BSTNode *insertBST(BSTNode *node, const Scholarship &s);
void inorderTraverse(BSTNode *node, std::function<void(const Scholarship &)> f);
BSTNode *findByID(BSTNode *node, int id);
BSTNode *deleteByKey(BSTNode *node, double amount, int id);

// Collection and result operations
void collectIf(BSTNode *node, ResultList &res, std::function<bool(const Scholarship &)> pred);
void printResultList(ResultList &r);

// Cleanup
void cleanupBST(BSTNode *node);

#endif