#include "engine.h"

// Default Constructor
DoublyLinkedList::DoublyLinkedList()
{
    head = nullptr;
    tail = nullptr;
}

// Add a section to a node
void DoublyLinkedList::addSection(DoublyLinkedList *dll, Section *section)
{
    removeDuplicates(section); // remove duplicates of attributes in a section
    fixIndexesForSelectors(section); // fix indexes for selectors;


    Node* currentNode = dll->head;
    if (currentNode == nullptr) {
        Node* newNode = createNode(); // create a new node
        newNode->sectionsCount++;
        newNode->data[0] = section; // assign a new section for a first index
        //newNode->data[0]->index = 1;
        fixIndexesForSections(newNode); // fix indexes for sections;
        newNode->index = dll->nodesCount;
        return;
    }
    else {
        Node* currNode = dll->tail;
        if (currNode == nullptr || currNode->sectionsCount == T) {
            Node* newNode = createNode(); // create a new node
            newNode->sectionsCount++;
            newNode->data[0] = section; // assign a new section for a first index
            shiftLeft(newNode);
            newNode->index = dll->nodesCount;
        }
        else {
            if (currNode != nullptr && currNode->sectionsCount < T) {
                for (int i=0; i<T; i++) {
                    if (currNode->data[i] == nullptr) {
                        currNode->data[i] = section;
                        currNode->sectionsCount++;
                        fixIndexesForSections(currNode);
                        return;
                    }
                }
            }
        }
    }
}


// Function for removing dupliactes of attributes
void DoublyLinkedList::removeDuplicates(Section* section) {
    if(section != nullptr) {
        int attributesToRemove[32] = { -1 }; // array for indexes of attributes to remove
        Attribute* pIterator = section->headAttribute;
        while(pIterator != nullptr) {
            for(int k=0; k<32; k++) attributesToRemove[k] = -1;
            Attribute* pSecIterator = section->headAttribute;
            int counter = 0;
            while(pSecIterator != nullptr) {
                if(pIterator->property == pSecIterator->property) {
                    attributesToRemove[counter] = pSecIterator->index;
                    counter++;
                }
                pSecIterator = pSecIterator->next;
            }
            pIterator = pIterator->next;
            for(int i=0; i<counter-1; i++) {
                if(attributesToRemove[i] != -1) removeAttribute(section, attributesToRemove[i]);
            }
        }
    }
    //fixIndexesForAttributes(section);
}

void DoublyLinkedList::fixIndexesForSelectors(Section* section)
{
    Selector* currentSelector = section->headSelector;
    int newIndex = 1;
    while(currentSelector != nullptr) {
        currentSelector->index = newIndex;
        newIndex++;
        currentSelector = currentSelector->next;
    }
}


void DoublyLinkedList::fixIndexesForSections(Node *node)
{
    int index = 1;
    for(int i=0; i<T; i++) {
        if(node->data[i] != nullptr) {
            node->data[i]->index = index;
            index++;
        }
    }
}

void DoublyLinkedList::removeAttribute(Section* section, int i)
{
    Attribute* currentAttribute = section->headAttribute;
    Attribute* previousAttribute = nullptr;

    while (currentAttribute != nullptr && currentAttribute->index != i) {
        previousAttribute = currentAttribute;
        currentAttribute = currentAttribute->next;
    }

    if (currentAttribute != nullptr) {
        if (previousAttribute == nullptr) {
            section->headAttribute = currentAttribute->next;
        }
        else {
            previousAttribute->next = currentAttribute->next;
        }
    }
    section->numberOfAttributes--;
}


void DoublyLinkedList::shiftLeft(Node *node)
{
    for (int i=1; i<T; i++) {
        if (node->data[i] != nullptr && node->data[i - 1] == nullptr) {
            node->data[i - 1] = node->data[i];
            node->data[i] = nullptr;
        }
    }
}

// COMMANDS

//command: ?
void DoublyLinkedList::numberOfSections()
{
    Node* currentNode = head;
    int count = 0;

    for (int i=0; i<nodesCount; i++){
        count += currentNode->sectionsCount;
        if (currentNode->next == nullptr) break;
        else currentNode = currentNode->next;
    }
    std::cout << "? == " << count << std::endl;
}


//command: i,S,?
void DoublyLinkedList::numberOfSelectors(int i)
{
    int ithSection = i;
    Node* currentNode = head;

    while (ithSection > 8 && currentNode != nullptr) {
        ithSection -= currentNode->sectionsCount;
        currentNode = currentNode->next;
    }

    while (currentNode != nullptr && currentNode->sectionsCount < ithSection) {
        ithSection -= currentNode->sectionsCount;
        currentNode = currentNode->next;
    }
    if (currentNode == nullptr) return;
    for (int j=0; j<T; j++) {
        if(currentNode->data[j] != nullptr && currentNode->data[j]->index == ithSection) {
            std::cout << i << ",S,? == " << currentNode->data[j]->numberOfSelectors << std::endl;
            return;
        }
    }
}


// command i,S,j
void DoublyLinkedList::jthSelector(int i, int j)
{
    int ithSection = i;
    Node* currentNode = head;

    while (ithSection > 8 && currentNode != nullptr) {
        ithSection -= currentNode->sectionsCount;
        currentNode = currentNode->next;
    }

    while (currentNode != nullptr && currentNode->sectionsCount < ithSection) {
        ithSection -= currentNode->sectionsCount;
        currentNode = currentNode->next;
    }
    if (currentNode == nullptr) return;
    for (int k=0; k<T; k++) {
        if(currentNode->data[k] != nullptr && currentNode->data[k]->index == ithSection) {
            if(j > currentNode->data[k]->numberOfSelectors) return;
            Selector* currentSelector = currentNode->data[k]->headSelector;
            while(currentSelector != nullptr && currentSelector->index != j) currentSelector = currentSelector->next;
            if(currentSelector != nullptr && currentSelector->index == j && currentSelector->selector.lengthOfStr() > 0) {
                std::cout << i << ",S," << j << " == " << currentSelector->selector.c_str() << std::endl;
                return;
            }
            return;
        }
    }
}

//command: i,A,?

void DoublyLinkedList::numberOfAttributes(int i)
{
    int ithSection = i;
    Node* currentNode = head;

    while (ithSection > 8 && currentNode != nullptr) {
        ithSection -= currentNode->sectionsCount;
        currentNode = currentNode->next;
    }

    while (currentNode != nullptr && currentNode->sectionsCount < ithSection) {
        ithSection -= currentNode->sectionsCount;
        currentNode = currentNode->next;
    }
    if (currentNode == nullptr) return;
    for (int j=0; j<T; j++) {
        if (currentNode->data[j] != nullptr && currentNode->data[j]->index == ithSection) {
            std::cout << i << ",A,? == " << currentNode->data[j]->numberOfAttributes << std::endl;
            return;
        }
    }
}


//command: i,A,n
void DoublyLinkedList::getAttributeValue(int i, String n)
{
    int ithSection = i;
    Node* currentNode = head;

    while (ithSection > 8 && currentNode != nullptr) {
        ithSection -= currentNode->sectionsCount;
        currentNode = currentNode->next;
    }

    while (currentNode != nullptr && currentNode->sectionsCount < ithSection) {
        ithSection -= currentNode->sectionsCount;
        currentNode = currentNode->next;
    }
    if (currentNode == nullptr) return;
    for (int j=0; j<T; j++) {
        if (currentNode->data[j] != nullptr && currentNode->data[j]->index == ithSection) {
            Attribute* currentAttribute = currentNode->data[j]->headAttribute;
            if (currentAttribute == nullptr) return;
            while (currentAttribute != nullptr && currentAttribute->property != n) currentAttribute = currentAttribute->next;
            if (currentAttribute != nullptr && currentAttribute->property == n) {
                std::cout << i << ",A," << n.c_str() << " == " << currentAttribute->value.c_str() << std::endl;
            }

        }
    }
}


//command: n,A,?
void DoublyLinkedList::totalAttributeOccurrences(String n)
{
    Node* currentNode = head;
    int count = 0;

    for (int i=0; i<nodesCount; i++) {
        for (int j=0; j<T; j++) {
            if (currentNode->data[j] != nullptr) {
                Attribute* currentAttribute = currentNode->data[j]->headAttribute;
                while (currentAttribute != nullptr && currentAttribute->property != n) currentAttribute = currentAttribute->next;
                if (currentAttribute != nullptr && currentAttribute->property == n) count++;
            }
        }
        if (currentNode->next == nullptr) break;
        else currentNode = currentNode->next;
    }
    std::cout << n.c_str() << ",A,? == " << count << std::endl;
}

//command: z,S,?
void DoublyLinkedList::totalSelectorOccurrences(String n)
{
    Node* currentNode = head;
    int count = 0;

    for (int i=0; i<nodesCount; i++) {
        for (int j=0; j<T; j++) {
            if (currentNode->data[j] != nullptr) {
                Selector* currentSelector = currentNode->data[j]->headSelector;
                while (currentSelector != nullptr && currentSelector->selector != n) currentSelector = currentSelector->next;
                if (currentSelector != nullptr && currentSelector->selector == n) {
                    count++;
                }
            }
        }
        currentNode = currentNode->next;
    }

    std::cout << n.c_str() << ",S,? == " << count << std::endl;
}

//command: z,E,n
void DoublyLinkedList::attributeValueForSelector(String z, String n)
{
    Node* currentNode = tail;
    while (currentNode != nullptr) {
        for (int i=T-1; i>=0; i--) {
            if (currentNode->data[i] != nullptr) {
                Selector* currentSelector = currentNode->data[i]->headSelector;
                while (currentSelector != nullptr && currentSelector->selector != z) {
                    currentSelector = currentSelector->next;
                }
                if (currentSelector != nullptr && currentSelector->selector == z) {
                    Attribute* currentAttribute = currentNode->data[i]->headAttribute;
                    while (currentAttribute != nullptr && currentAttribute->property != n) currentAttribute = currentAttribute->next;
                    if (currentAttribute != nullptr && currentAttribute->property == n) {
                        std::cout << z.c_str() << ",E," << n.c_str() << " == " << currentAttribute->value << std::endl;
                        return;
                    }
                }
            }
        }
        currentNode = currentNode->prev;
    }
}

//command: i,D,*
void DoublyLinkedList::removeSection(int i)
{
    int ithSection = i;
    Node* currentNode = head;

    while (ithSection > 8 && currentNode != nullptr) {
        ithSection -= currentNode->sectionsCount;
        currentNode = currentNode->next;
    }
    while (currentNode != nullptr && currentNode->sectionsCount < ithSection) {
        ithSection -= currentNode->sectionsCount;
        currentNode = currentNode->next;
    }
    if (currentNode == nullptr) return;
    for (int j=0; j<T; j++) {
        if (currentNode->data[j] != nullptr && currentNode->data[j]->index == ithSection) {
            delete currentNode->data[j]->headAttribute;
            currentNode->data[j]->headAttribute = nullptr;
            delete currentNode->data[j]->headSelector;
            currentNode->data[j]->headSelector = nullptr;
            delete currentNode->data[j];
            currentNode->data[j] = nullptr;
            currentNode->sectionsCount--;
            fixIndexesForSections(currentNode);
            std::cout << i << ",D,* == deleted" << std::endl;
            shiftLeft(currentNode);
            return;
        }
    }
}

//command: i,D,n
void DoublyLinkedList::removeAttribute(int i, String n)
{
    int ithSection = i;
    Node* currentNode = head;

    while (ithSection > 8 && currentNode != nullptr) {
        ithSection -= currentNode->sectionsCount;
        currentNode = currentNode->next;
    }

    while (currentNode != nullptr && currentNode->sectionsCount < ithSection) {
        ithSection -= currentNode->sectionsCount;
        currentNode = currentNode->next;
    }
    if (currentNode == nullptr) return;
    for (int j=0; j<T; j++) {
        if (currentNode->data[j] != nullptr && currentNode->data[j]->index == ithSection) {
            Attribute* currentAttribute = currentNode->data[j]->headAttribute;
            Attribute* prev = nullptr;
            while (currentAttribute != nullptr) {
                if (currentAttribute->property == n) {
                    if (prev != nullptr) prev->next = currentAttribute->next;
                    else currentNode->data[j]->headAttribute = currentAttribute->next;
                    delete currentAttribute;
                    currentAttribute = nullptr;
                    currentNode->data[j]->numberOfAttributes--;
                    if(currentNode->data[j]->numberOfAttributes == 0) {
                        delete currentNode->data[j]->headSelector;
                        currentNode->data[j]->headSelector = nullptr;
                        delete currentNode->data[j];
                        currentNode->data[j] = nullptr;
                        currentNode->sectionsCount--;
                        fixIndexesForSections(currentNode);
                    }
                    std::cout << i << ",D," << n.c_str() << " == deleted" << std::endl;
                    return;
                }
                prev = currentAttribute;
                currentAttribute = currentAttribute->next;
            }
        }
    }
}

// Create a new node
Node* DoublyLinkedList::createNode()
{
    Node* newNode = new Node;
    nodesCount++;
    if (head == nullptr) {
        head = tail = newNode;
        return newNode;
    }
    tail->next = newNode;
    newNode->prev = tail;
    tail = newNode;
    return newNode;
}