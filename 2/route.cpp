/**
 * @file route.cpp
 * @brief Реализация класса code (шифр маршрутной перестановки)
 * @author Назарова Софья
 * @date 2025
 * @copyright WECT ПГУ
 */

#include "route.h"

/**
 * @brief Конструктор с ключом и текстом
 * @param[in] skey Ключ шифрования
 * @param[in] text Текст для инициализации
 * @details Проверяет валидность ключа относительно длины текста
 */
code::code(int skey, string text) {
    key = getValidKey(skey, text);
}

/**
 * @brief Шифрование текста методом маршрутной перестановки
 * @param[in] text Текст для шифрования
 * @return Зашифрованный текст
 * @details Алгоритм:
 *          1. Запись текста в таблицу по строкам
 *          2. Чтение таблицы по столбцам справа налево
 */
string code::encryption(const string& text) {
    string t = getValidOpenText(text);
    int k = 0;
    int simvoli = t.size();
    int stroki = simvoli / key;
    char** tabl = new char* [stroki];
    for (int i = 0; i < stroki; i++)
        tabl[i] = new char [key];

    // Запись в таблицу по строкам
    for (int i = 0; i < stroki; i++)
        for (int j = 0; j < key; j++) {
            if(k < simvoli) {
                tabl[i][j] = t[k];
                k++;
            }
        }

    k = 0;
    // Чтение из таблицы по столбцам справа налево
    for (int j = key - 1; j >= 0 ; j--)
        for (int i = 0; i < stroki; i++) {
            t[k] = tabl[i][j];
            k++;
        }

    // Освобождение памяти
    for (int i = 0; i < stroki; i++)
        delete[] tabl[i];
    delete[] tabl;

    return t;
}

/**
 * @brief Дешифрование текста
 * @param[in] text Зашифрованный текст
 * @param[in] open_text Исходный открытый текст (для проверки длины)
 * @return Расшифрованный текст
 * @throw cipher_error при несоответствии длин или невалидных символах
 */
string code::transcript(const string& text, const string& open_text) {
    if (text.empty() || open_text.empty()) {
        throw cipher_error("Один из текстов пуст!");
    }

    for (char c : text) {
        if (!isalpha(c)) {
            throw cipher_error("Некорректные символы в зашифрованном тексте!");
        }
    }

    for (char c : open_text) {
        if (!isalpha(c)) {
            throw cipher_error("Некорректные символы в открытом тексте!");
        }
    }

    string t = getValidCipherText(text, open_text);
    int k = 0;
    int simvoli = t.size();
    int stroki = simvoli / key;
    char** tabl = new char* [stroki];
    for (int i = 0; i < stroki; i++)
        tabl[i] = new char [key];

    // Запись в таблицу по столбцам справа налево
    for (int j = key - 1; j >= 0 ; j--)
        for (int i = 0; i < stroki; i++) {
            tabl[i][j] = t[k];
            k++;
        }

    k = 0;
    // Чтение из таблицы по строкам
    for (int i = 0; i < stroki; i++)
        for (int j = 0; j < key; j++) {
            t[k] = tabl[i][j];
            k++;
        }

    // Освобождение памяти
    for (int i = 0; i < stroki; i++)
        delete[] tabl[i];
    delete[] tabl;

    return t;
}

/**
 * @brief Проверка валидности зашифрованного текста
 * @param[in] s Зашифрованный текст
 * @param[in] open_text Исходный открытый текст
 * @return Валидированный зашифрованный текст
 * @throw cipher_error при несоответствии длин
 */
inline string code::getValidCipherText(const string& s, const string& open_text) {
    int r1 = s.size();
    int r2 = open_text.size();
    if (r1 != r2) {
        throw cipher_error("Неправильный зашифрованный текст: " + s);
    }
    return s;
}

/**
 * @brief Проверка валидности открытого текста
 * @param[in] s Текст для проверки
 * @return Валидированный текст (без пробелов, только буквы)
 * @throw cipher_error при пустом тексте или недопустимых символах
 */
inline string code::getValidOpenText(const string& s) {
    string text = s;

    if (text.empty()) {
        throw cipher_error("Отсутствует открытый текст!");
    }

    text.erase(remove_if(text.begin(), text.end(), [](char c) {
        if ((c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && c != ' ') {
            throw cipher_error("В тексте встречены некорректные символы!");
        }
        return c == ' ';
    }), text.end());

    return text;
}

/**
 * @brief Проверка валидности ключа
 * @param[in] key Ключ для проверки
 * @param[in] Text Текст для шифрования
 * @return Валидный ключ
 * @throw cipher_error если ключ меньше 2 или больше длины текста
 */
inline int code::getValidKey(int key, const string& Text) {
    if (key < 2 || key > Text.length()) {
        throw cipher_error("Ключ некорректного размера");
    }
    return key;
}