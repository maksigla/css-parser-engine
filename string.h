#ifndef PROJECT_V2_STRING_H
#define PROJECT_V2_STRING_H

#include <iostream>

class String
{
private:
    int length = 0;
    char* buffer = nullptr;

public:
// Constructors
    String();
    String(const char* str);

// Deconstructor
    ~String();

// Operators
    String& operator=(const String& otherStr);
    String operator+(const String& otherStr) const;
    String& operator+=(char c);
    bool operator==(const String& otherStr) const;
    bool operator!=(const String& otherStr) const;
    char& operator[](int index);
    const char& operator[](int index) const;

    friend std::ostream& operator<<(std::ostream& os, const String& s);
    friend std::istream& operator>>(std::istream& is, String& str);

// Getters
    int lengthOfStr() const;
    static int stringToInt(const String& str); // convert string to an int
    const char* c_str() const; // return buffer


//methods
    void removeWhiteCharacters();
    bool isNumeric() const; // check if value of a string is numeric

};

#endif //PROJECT_V2_STRING_H
