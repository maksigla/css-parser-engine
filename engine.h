#ifndef PROJECT_V2_ENGINE_H
#define PROJECT_V2_ENGINE_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "string.h"

#define T 8

struct Attribute {
    unsigned int index = 1;
    String property;
    String value;
    Attribute* next = nullptr;
};

struct Selector {
    unsigned int index = 1;
    String selector;
    Selector* next = nullptr;
};

struct Section {
    unsigned int index = 1;
    unsigned int numberOfSelectors = 0;
    unsigned int numberOfAttributes = 0;
    Selector* headSelector = nullptr;
    Attribute* headAttribute = nullptr;
};


struct Node {
    unsigned int index = 1;
    unsigned int sectionsCount = 0;
    Section* data[T] {nullptr};
    Node* prev = nullptr;
    Node* next = nullptr;
};

// main class
class DoublyLinkedList {
private:
public:
    int nodesCount = 0;
    Node* head = nullptr;
    Node* tail = nullptr;
    DoublyLinkedList();

    void removeDuplicates(Section* section);


    void addSection(DoublyLinkedList* dll, Section* section);
    static void fixIndexesForSelectors(Section* section);
    static void fixIndexesForSections(Node* node);
    static void removeAttribute(Section* section, int i);
    static void shiftLeft(Node* node);

    //commands
    void numberOfSections(); // command: ?
    void numberOfSelectors(int i); // command: i,S,?
    void numberOfAttributes(int i); // command: i,A,?
    void jthSelector(int i, int j); // command: i,S,j
    void getAttributeValue(int i, String n); // command: i,A,n
    void totalAttributeOccurrences(String n); // command: n,A,?
    void totalSelectorOccurrences(String n); // command: z,S,?
    void attributeValueForSelector(String z, String n); // command: z,E,n
    void removeSection(int i); // command: i,D,*
    void removeAttribute(int i, String n); // command: i,D,n

    Node* createNode();

};



#endif //PROJECT_V2_ENGINE_H
