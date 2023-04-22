#include "string.h"
#include <cstring>

String::String() : length(0), buffer(new char[1]) {
    buffer[0] = '\0';
}

String::String(const char *otherStr) : length(0) {
    while (otherStr[length] != '\0') {
        length++;
    }
    buffer = new char[length + 1]; // create a new buffer
    for (int i = 0; i < length; i++) {
        buffer[i] = otherStr[i]; // copy values of the old buffer to the new one
    }
    buffer[length] = '\0';
}


String::~String() {
    //if (buffer) delete[] buffer;
}

// = operator
String& String::operator=(const String& otherStr) {
    if (this != &otherStr) {
        char* tempBuffer = new char[otherStr.length + 1]; // allocate temp buffer
        for (int i = 0; i < otherStr.length; i++) {
            tempBuffer[i] = otherStr.buffer[i]; // copy the values of the old buffer into the new one
        }
        tempBuffer[otherStr.length] = '\0';
        delete[] buffer; // delete old buffer
        length = otherStr.length;
        buffer = tempBuffer;
    }
    return *this;
}

// + operator
String String::operator+(const String& otherStr) const {
    String result;
    result.length = length + otherStr.length;
    result.buffer = new char[result.length + 1]; // allocate new buffer
    for (int i = 0; i < length; i++) {
        result.buffer[i] = buffer[i]; // copy the values
    }
    for (int i = 0; i < otherStr.length; i++) {
        result.buffer[length + i] = otherStr.buffer[i];
    }
    result.buffer[result.length] = '\0';
    return result;
}

// += operator
String& String::operator+=(char c) {
    char* newBuffer = new char[length + 2]; // allocate new buffer
    for (int i = 0; i < length; i++) {
        char temp = buffer[i];
        newBuffer[i] = temp; // copy old values
    }
    newBuffer[length] = c;
    newBuffer[length + 1] = '\0';
    delete[] buffer; // delete the old buffer
    buffer = newBuffer;
    length++;
    return *this;
}

// == operator
bool String::operator==(const String& otherStr) const {
    if (length != otherStr.length) return false;
    for (int i = 0; i < length; i++) {
        if (buffer[i] != otherStr.buffer[i]) return false;
    }
    return true;
}

// != operator
bool String::operator!=(const String& otherStr) const {
    return !(*this == otherStr);
}

// [] opertor
char& String::operator[](int index) {
    return buffer[index];
}

// [] opertor
const char& String::operator[](int index) const {
    return buffer[index];
}

// return the length of the string
int String::lengthOfStr() const {
    return length;
}

// return the buffer
const char* String::c_str() const {
    return buffer;
}

// operator for output
std::ostream& operator<<(std::ostream& os, const String& s) {
    return os << s.buffer;
}

// operator for input
std::istream& operator>>(std::istream& is, String& str) {
    const int bufferSize = 100;
    char tempStr[bufferSize+1];
    is >> tempStr;
    int inputLength = strlen(tempStr);
    char* newBuffer = new char[inputLength+1];
    for (int i = 0; i < inputLength; i++) {
        newBuffer[i] = tempStr[i];
    }
    newBuffer[inputLength] = '\0';
    str = String(newBuffer);
    delete[] newBuffer;
    return is;
}

// convert string to an int
int String::stringToInt(const String &str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    if (str[i] == '-') {
        sign = -1;
        i++;
    }

    for (; i < str.lengthOfStr(); i++) {
        result = result * 10 + (str[i] - '0');
    }

    return result * sign;
}

// remove white characters at the start of the string and at the end
void String::removeWhiteCharacters() {
    int count = 0;
    int start = 0;
    int end = length - 1;

    while (start < length && (buffer[start] == ' ' || buffer[start] == '\t' || buffer[start] == '\n' || buffer[start] == '\r')) start++;
    while (end >= start && (buffer[end] == ' ' || buffer[end] == '\t' || buffer[end] == '\n' || buffer[end] == '\r')) end--;
    for (int i = start; i <= end; i++) buffer[count++] = buffer[i];
    if (count > 0) {
        buffer[count] = '\0';
        length = count;
    }
}

// check if string is a numerical one
bool String::isNumeric() const {
    if (length == 0) return false;
    for (int i = 0; i < length; i++) {
        char c = buffer[i];
        if (c < '0' || c > '9') return false;
    }
    return true;
}


