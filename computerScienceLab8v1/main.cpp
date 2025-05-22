#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// Проверка, начинается ли предложение с тире, перед которым могут быть только пробельные символы
bool isDashSentence(const std::string& sentence) {
    size_t i = 0;

    // Если предложение пустое — false
    if (sentence.empty()) return false;

    // Пропускаем пробельные символы в начале
    while (i < sentence.size() && std::isspace(static_cast<unsigned char>(sentence[i]))) {
        ++i;
    }

    // После пробелов должен идти символ тире '-' или EM DASH (UTF-8: 0xE2 0x80 0x94)
    if (i >= sentence.size()) return false;

    // Проверяем обычное тире
    if (sentence[i] == '-') return true;

    // Проверяем EM DASH (UTF-8 3 байта)
    if ((sentence.size() - i) >= 3) {
        if (static_cast<unsigned char>(sentence[i]) == 0xE2 &&
            static_cast<unsigned char>(sentence[i + 1]) == 0x80 &&
            static_cast<unsigned char>(sentence[i + 2]) == 0x94) {
            return true;
        }
    }

    return false;
}

// Разбивает текст на предложения по символам окончания: '.', '!', '?', '\n'
std::vector<std::string> splitIntoSentences(const std::string& text) {
    std::vector<std::string> sentences;
    std::string current;
    for (char c : text) {
        current += c;
        if (c == '.' || c == '!' || c == '?' || c == '\n') {
            sentences.push_back(current);
            current.clear();
        }
    }
    if (!current.empty()) {
        sentences.push_back(current);
    }
    return sentences;
}

// Считывает весь текст из файла
std::string readTextFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        throw std::runtime_error("Failed to open input file: " + filename);
    }

    std::stringstream buffer;
    buffer << inFile.rdbuf();
    return buffer.str();
}

// Записывает предложения в файл построчно, каждое предложение с символом переноса строки
void writeSentencesToFile(const std::vector<std::string>& sentences, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw std::runtime_error("Failed to open output file: " + filename);
    }

    for (auto sentence : sentences) {
        // Удаляем все \r и \n из строки
        sentence.erase(std::remove(sentence.begin(), sentence.end(), '\n'), sentence.end());
        sentence.erase(std::remove(sentence.begin(), sentence.end(), '\r'), sentence.end());

        outFile << sentence << '\n';
    }
}

// Добавляет .txt, если расширения нет
std::string ensureTxtExtension(const std::string& filename) {
    if (filename.find('.') == std::string::npos)
        return filename + ".txt";
    return filename;
}

void requestFilenames(std::string& inputFile, std::string& outputFile) {
    while (true) {
        std::string rawInput;
        std::cout << "Введите имя входного файла (без расширения или с .txt): ";
        std::getline(std::cin, rawInput);
        inputFile = ensureTxtExtension(rawInput);

        std::ifstream in(inputFile);
        if (in) {
            break;
        }
        else {
            std::cerr << "Ошибка: не удалось открыть файл '" << inputFile << "'. Попробуйте снова.\n";
        }
    }

    while (true) {
        std::string rawOutput;
        std::cout << "Введите имя выходного файла (без расширения или с .txt): ";
        std::getline(std::cin, rawOutput);
        outputFile = ensureTxtExtension(rawOutput);

        std::ofstream out(outputFile, std::ios::out); // открываем для записи
        if (out) {
            break;
        }
        else {
            std::cerr << "Ошибка: не удалось создать файл '" << outputFile << "'. Попробуйте снова.\n";
        }
    }
}

// Обрабатывает файл: фильтрует предложения и записывает результат
void processFiles(const std::string& inputFile, const std::string& outputFile) {
    const std::string fullText = readTextFromFile(inputFile);
    const std::vector<std::string> allSentences = splitIntoSentences(fullText);

    std::vector<std::string> filtered;
    for (const auto& sentence : allSentences) {
        if (isDashSentence(sentence)) {
            filtered.push_back(sentence);
        }
    }
        
    writeSentencesToFile(filtered, outputFile);
}

int main() {
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    std::string inputFile, outputFile;
    requestFilenames(inputFile, outputFile);

    try {
        processFiles(inputFile, outputFile);
        std::cout << "Готово! Результат записан в файл: " << outputFile << "\n";

        // Открытие выходного файла в Windows
        std::string command = "start \"\" \"" + outputFile + "\"";
        system(command.c_str());
    }
    catch (const std::exception& ex) {
        std::cerr << "Ошибка: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
