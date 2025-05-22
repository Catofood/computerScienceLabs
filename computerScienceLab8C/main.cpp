#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define CHUNK_SIZE 1024
#define INITIAL_SENT_CAPACITY 100

char* read_text_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    size_t capacity = CHUNK_SIZE;
    size_t length = 0;
    char* buffer = (char*)malloc(capacity);
    if (!buffer) return NULL;

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        buffer[length++] = (char)ch;

        if (length >= capacity) {
            capacity *= 2;
            char* tmp = (char*)realloc(buffer, capacity);
            if (!tmp) {
                free(buffer);
                fclose(file);
                return NULL;
            }
            buffer = tmp;
        }
    }

    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

int is_dash_sentence(const char* sentence) {
    int i = 0;

    while (sentence[i] && isspace((unsigned char)sentence[i])) i++;

    if (sentence[i] == '-') return 1;

    if ((unsigned char)sentence[i] == 0xE2 &&
        (unsigned char)sentence[i + 1] == 0x80 &&
        (unsigned char)sentence[i + 2] == 0x94) {
        return 1;
    }

    return 0;
}

char** split_into_sentences(const char* text, int* out_count) {
    int capacity = INITIAL_SENT_CAPACITY;
    int count = 0;
    char** sentences = (char**)malloc(sizeof(char*) * capacity);
    if (!sentences) return NULL;

    size_t len = strlen(text);
    char* current = (char*)malloc(len + 1);
    if (!current) {
        free(sentences);
        return NULL;
    }

    int j = 0;
    for (size_t i = 0; i <= len; ++i) {
        char c = text[i];
        current[j++] = c;

        if (c == '.' || c == '!' || c == '?' || c == '\n' || c == '\0') {
            current[j] = '\0';

            char* sentence = (char*)malloc(j + 1);
            if (!sentence) {
                // очистка
                for (int k = 0; k < count; ++k) free(sentences[k]);
                free(sentences);
                free(current);
                return NULL;
            }
            strcpy(sentence, current);
            sentences[count++] = sentence;

            if (count >= capacity) {
                capacity *= 2;
                char** tmp = (char**)realloc(sentences, sizeof(char*) * capacity);
                if (!tmp) {
                    for (int k = 0; k < count; ++k) free(sentences[k]);
                    free(sentences);
                    free(current);
                    return NULL;
                }
                sentences = tmp;
            }

            j = 0;
        }
    }

    free(current);
    *out_count = count;
    return sentences;
}

int write_sentences_to_file(const char* filename, char** sentences, int count) {
    FILE* file = fopen(filename, "w");
    if (!file) return 0;

    for (int i = 0; i < count; i++) {
        char* s = sentences[i];
        for (int j = 0; s[j]; j++) {
            if (s[j] != '\r' && s[j] != '\n') fputc(s[j], file);
        }
        fputc('\n', file);
    }

    fclose(file);
    return 1;
}

void ensure_txt_extension(char* filename) {
    if (!strchr(filename, '.')) {
        strcat(filename, ".txt");
    }
}

char* prompt_filename(const char* message, int for_reading) {
    char raw[256];
    FILE* file;
    char* filename = NULL;

    while (1) {
        printf("%s", message);
        if (!fgets(raw, sizeof(raw), stdin)) continue;

        raw[strcspn(raw, "\n")] = '\0';

        filename = (char*)malloc(strlen(raw) + 5);
        if (!filename) return NULL;

        strcpy(filename, raw);
        ensure_txt_extension(filename);

        file = fopen(filename, for_reading ? "r" : "w");
        if (file) {
            fclose(file);
            break;
        }

        printf("Ошибка при открытии файла '%s'\n", filename);
        free(filename);
        filename = NULL;
    }

    return filename;
}

int main() {
    char* input_file = prompt_filename("Введите имя входного файла: ", 1);
    char* output_file = prompt_filename("Введите имя выходного файла: ", 0);

    if (!input_file || !output_file) {
        printf("Ошибка выделения памяти\n");
        free(input_file);
        free(output_file);
        return 1;
    }

    char* text = read_text_from_file(input_file);
    if (!text) {
        printf("Ошибка при чтении файла '%s'\n", input_file);
        free(input_file);
        free(output_file);
        return 1;
    }

    int sentence_count = 0;
    char** sentences = split_into_sentences(text, &sentence_count);
    if (!sentences) {
        printf("Ошибка при разбиении текста\n");
        free(text);
        free(input_file);
        free(output_file);
        return 1;
    }

    // Фильтрация
    char** filtered = (char**)malloc(sizeof(char*) * sentence_count);
    if (!filtered) {
        printf("Ошибка выделения памяти\n");
        for (int i = 0; i < sentence_count; ++i) free(sentences[i]);
        free(sentences);
        free(text);
        free(input_file);
        free(output_file);
        return 1;
    }

    int filtered_count = 0;
    for (int i = 0; i < sentence_count; i++) {
        if (is_dash_sentence(sentences[i])) {
            filtered[filtered_count++] = sentences[i];
        }
        else {
            free(sentences[i]);
        }
    }

    free(sentences);

    if (!write_sentences_to_file(output_file, filtered, filtered_count)) {
        printf("Ошибка при записи файла '%s'\n", output_file);
    }
    else {
        printf("Готово! Результат записан в файл: %s\n", output_file);
    }

    for (int i = 0; i < filtered_count; ++i) {
        free(filtered[i]);
    }
    free(filtered);
    free(text);
    free(input_file);
    free(output_file);

    return 0;
}
