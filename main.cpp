#include "engine.h"


// Function for adding selector into the section
void addSelector(Section* section, Selector* selector)
{
    if (section == nullptr) return;
    if (selector->selector.lengthOfStr() < 1) return;
    if (section->headSelector == nullptr) { // check for head selector
        section->headSelector = selector;
    }
    else {
        Selector* currSelector = section->headSelector;
        while (currSelector->next != nullptr) {
            currSelector = currSelector->next; // go to the next selector
        }
        currSelector->next = selector;
    }
    section->numberOfSelectors++; // increase number of selectors
}

// Function for adding attribute into the section
void addAttribute(Section* section, Attribute* attribute)
{
    attribute->property.removeWhiteCharacters();
    attribute->value.removeWhiteCharacters();
    if (section == nullptr) return;
    if (section->headAttribute == nullptr) { // check for head attribute
        section->headAttribute = attribute;
    }
    else {
        Attribute* currAttribute = section->headAttribute;
        while (currAttribute->next != nullptr) {
            currAttribute = currAttribute->next; // go to the next attribute
        }
        currAttribute->next = attribute;
        attribute->index = section->numberOfAttributes + 1;
    }
    section->numberOfAttributes++; // increase number of attributes
}

void addCss(DoublyLinkedList* nodes, char css)
{
    static bool isSelector = true; // create static bools
    static bool isAttribute = false;
    static bool isProperty = true;
    static bool isValue = false;
    static bool attributeAdded = false;

    static auto* section = new Section; // create struct and allocate memory for it
    static auto* selector = new Selector;
    static auto* attribute = new Attribute;

    if (isSelector) { // check if program is handling selectors
        if(css != ',' && css != '{' && css != '\n') {
            selector->selector += css; // add char to the selector name
        }
        else if (css == ',') {
            addSelector(section, selector); // add a selector
            selector->selector.removeWhiteCharacters();
            nodes->fixIndexesForSelectors(section); // fix indexes
            selector = new Selector; // create a new selector
        }
        else if (css == '{') {
            addSelector(section, selector); // add a selector
            selector->selector.removeWhiteCharacters(); // remove white characters
            nodes->fixIndexesForSelectors(section); // fix indexes
            selector = new Selector; // create a selector
            isSelector = false;
            isAttribute = true;
            isProperty = true;
            isValue = false;
        }
    }
    else if (isAttribute) { // check if program is handling attributes
        if (isProperty) { // check if program is handling property of attribute
            if (css != ':' && css != '\n' && css != '}' && css != '\t') attribute->property += css;
            else if (css == ':') {
                attributeAdded = false;
                isProperty = false;
                isValue = true;
            }
        }
        else if (isValue) { // check if program is handling value of attribute
            if (css != ';' && css != '}' && css != '\n' && css != '\t') attribute->value += css;
            else {
                if (css == ';') {
                    attributeAdded = true;
                    addAttribute(section, attribute); // add attribute to the section
                    attribute->property.removeWhiteCharacters(); // remove white characters
                    attribute->value.removeWhiteCharacters(); // remove white characters
                    attribute = new Attribute; // create new attribute
                    isProperty = true;
                    isValue = false;
                }
            }
        }
        if (css == '}') {
            if (!attributeAdded) {
                addAttribute(section, attribute); // add an attriute
                attribute->property.removeWhiteCharacters(); // remove white characters
                attribute->value.removeWhiteCharacters(); // remove white characters
            }
            nodes->addSection(nodes, section); // add section to the node
            selector = new Selector; // create new selector
            attribute = new Attribute; // create new attribute
            section = new Section; // create new section
            isSelector = true;
            isAttribute = false;
            isProperty = true;
            isValue = false;
        }
    }
}

void addCommands(DoublyLinkedList* nodes, char command)
{
    if (command == '\n') return;
    if (command == '?') nodes->numberOfSections(); // execute command for number of sections
    else {
        String fToken; // create tokens for separate commands
        String sToken;
        String tToken;
        fToken += command;
        char c = getchar();
        if (c == EOF) return;
        while (c != ','){
            fToken += c;// add char to the first token
            c = getchar();
            if (c == EOF) return;
        }
        c = getchar();
        if (c == EOF) return;
        while (c != ',') {
            sToken += c; // add char to the second token
            c = getchar();
            if (c == EOF) return;
        }
        c = getchar();
        if (c == EOF) return;
        while (c != '\n') {
            tToken += c; // add char to the third token
            c = getchar();
            if (c == EOF) return;
        }
        if (fToken.isNumeric() && sToken == "S" && tToken == "?") nodes->numberOfSelectors(fToken.stringToInt(fToken)); // execute command for number of selectors
        else if (fToken.isNumeric() && sToken == "A" && tToken == "?") nodes->numberOfAttributes(fToken.stringToInt(fToken)); // execute command for number of attributes
        else if (fToken.isNumeric() && sToken == "S" && tToken.isNumeric()) nodes->jthSelector(fToken.stringToInt(fToken), tToken.stringToInt(tToken)); // execute command for jth selector in ith section
        else if (fToken.isNumeric() && sToken == "A") nodes->getAttributeValue(fToken.stringToInt(fToken), tToken); // execute command for getting the attribute value
        else if (!fToken.isNumeric() && sToken == "A" && tToken == "?") nodes->totalAttributeOccurrences(fToken); // execute command for total attribute occurrences
        else if (!fToken.isNumeric() && sToken == "S" && tToken == "?") nodes->totalSelectorOccurrences(fToken); // execute command for total selector occurrences
        else if (!fToken.isNumeric() && sToken == "E" && !tToken.isNumeric()) nodes->attributeValueForSelector(fToken, tToken); // execute command for attribute value for a certain selector
        else if (fToken.isNumeric() && sToken == "D" && tToken == "*") nodes->removeSection(fToken.stringToInt(fToken)); // execute command for removing section
        else if (fToken.isNumeric() && sToken == "D" && !tToken.isNumeric()) nodes->removeAttribute(fToken.stringToInt(fToken), tToken); // execute command for removing attribute
    }
}


// main function for the program
void engine (DoublyLinkedList* nodes)
{
    bool isCss = true;
    bool isCommands = false;

    char c = getchar();
    while (c != EOF) {
        if (c == '?' && !isCommands) { // check for change of input status
            char next1 = getchar(); // get next chars
            char next2 = getchar();
            char next3 = getchar();
            if (next1 == '?' && next2 == '?' && next3 == '?') {
                isCss = false;
                isCommands = true;
            }
            else {
                if (isCss) {
                    addCss(nodes, next1); // return chars if they are not for changing the status of the program
                    addCss(nodes, next2);
                    addCss(nodes, next3);
                }
            }
        }
        else if (c == '\t');
        else if (c == '*' && !isCss) { // check for change of input status
            char next1 = getchar(); // get chars for possible change of the program status
            char next2 = getchar();
            char next3 = getchar();
            if (next1 == '*' && next2 == '*' && next3 == '*') {
                isCss = true;
                isCommands = false;
            }
            else {
                if (isCommands) {
                    addCommands(nodes, next1); // return chars if they are not for changing the status
                    addCommands(nodes, next1);
                    addCommands(nodes, next1);
                }
            }
        }
        else if (isCss) addCss(nodes, c); // adding char to css parser
        else if (isCommands) addCommands(nodes, c); // adding char to commands handler
        c = getchar(); // get char at the end of loop
    }
}


int main()
{
    DoublyLinkedList* nodes = new DoublyLinkedList; // main object of DLL class

    engine(nodes); // calling the function for the program

    return 0;
}

