#include <iostream>
#include <sstream>

const int MAX_LEN = 2048;

int getValidatedInt(const std::string& prompt) {
    int value;
    std::string line;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, line)) {
            std::cerr << "Ошибка ввода." << std::endl;
            exit(EXIT_FAILURE);
        }
        std::stringstream ss(line);
        if (ss >> value && ss.eof() && value > 0) {
            return value;
        }
        std::cout << "Неверный ввод. Введите положительное целое число.\n";
    }
}

int utf8CharLen(unsigned char c) {
    if ((c & 0b10000000) == 0) return 1;
    if ((c & 0b11100000) == 0b11000000) return 2;
    if ((c & 0b11110000) == 0b11100000) return 3;
    if ((c & 0b11111000) == 0b11110000) return 4;
    return 1; // fallback
}

int countWords(const char* str) {
    bool inWord = false;
    int count = 0;
    for (int i = 0; str[i] != '\0'; ) {
        if ((unsigned char)str[i] == 0x20) {
            inWord = false;
            ++i;
        }
        else {
            if (!inWord) {
                inWord = true;
                ++count;
            }
            i += utf8CharLen((unsigned char)str[i]);
        }
    }
    return count;
}

int byteLength(const char* str) {
    int i = 0;
    while (str[i] != '\0') ++i;
    return i;
}

void copyString(const char* src, char* dest) {
    int i = 0;
    while ((dest[i] = src[i]) != '\0') ++i;
}

void stretchString(char* str, int targetLen) {
    int origLen = byteLength(str);

    int spaceCount = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        if ((unsigned char)str[i] == 0x20 && (i == 0 || (unsigned char)str[i - 1] != 0x20)) {
            ++spaceCount;
        }
    }

    if (spaceCount == 0) return;
    int extraSpaces = targetLen - origLen;
    if (extraSpaces <= 0) return;

    int newLen = origLen + extraSpaces;
    char* result = new char[newLen + 1];

    int i = 0, j = 0, remainingSpaces = spaceCount;
    while (str[i] != '\0') {
        if ((unsigned char)str[i] == 0x20 && (i == 0 || (unsigned char)str[i - 1] != 0x20)) {
            result[j++] = str[i++];
            int toInsert = extraSpaces / spaceCount;
            if (remainingSpaces <= extraSpaces % spaceCount) {
                ++toInsert;
            }
            for (int s = 0; s < toInsert; ++s) result[j++] = ' ';
            --remainingSpaces;
        }
        else {
            int chLen = utf8CharLen((unsigned char)str[i]);
            for (int k = 0; k < chLen; ++k) {
                result[j++] = str[i++];
            }
        }
    }

    result[j] = '\0';
    copyString(result, str);
    delete[] result;
}

void readLine(char* buffer, int size) {
    std::cin.getline(buffer, size);
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(size, '\n');
        std::cerr << "Ошибка ввода строки." << std::endl;
        exit(EXIT_FAILURE);
    }
}

int main() {
    char input[MAX_LEN];

    while (true) {
        std::cout << "Введите строку (Максимальная длина " << MAX_LEN << " бит): ";
        readLine(input, MAX_LEN);

        int wordCount = countWords(input);
        if (wordCount < 2) {
            std::cout << "Ошибка: введите строку с более чем одним словом.\n";
            continue;
        }

        int inputBytes = byteLength(input);
        std::cout << "Количество байт во введённой строке: " << inputBytes << "\n";

        int desiredLength = 0;
        while (true) {
            desiredLength = getValidatedInt("Введите желаемую длину строки (в байтах): ");
            if (desiredLength <= inputBytes) {
                std::cout << "Ошибка: длина должна быть больше текущей (" << inputBytes << "). Попробуйте снова.\n";
            }
            else {
                break;
            }
        }

        stretchString(input, desiredLength);

        std::cout << "Результат: \"" << input << "\"\n";
        break;
    }

#ifdef _MSC_VER
    _CrtDumpMemoryLeaks();
#endif

    return 0;
}
