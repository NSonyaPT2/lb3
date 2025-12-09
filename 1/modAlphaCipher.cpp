/**
 * @file modAlphaCipher.cpp
 * @brief Реализация класса modAlphaCipher
 * @author Назарова Софья
 * @date 2025
 * @copyright WECT ПГУ
 */

#include "modAlphaCipher.h"
#include <locale>
#include <codecvt>
#include <iostream>

std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> codec; ///< Конвертер UTF-8

/**
 * @brief Конструктор с ключом
 * @param[in] skey Ключ шифрования в виде строки
 * @throw cipher_error при слабом или невалидном ключе
 * @details Инициализирует таблицу преобразований и проверяет ключ на слабость
 */
modAlphaCipher::modAlphaCipher(const std::string& skey) {
    // Инициализация таблицы преобразований
    for (unsigned i=0; i < numAlpha.size(); i++)
        alphaNum[numAlpha[i]]=i;
    key = convert(getValidKey(skey));
    
    // Проверка на слабый ключ (все символы одинаковые)
    if (key.size() > 1) {
        bool allSame = true;
        for (size_t i = 1; i < key.size(); i++) {
            if (key[i] != key[0]) {
                allSame = false;
                break;
            }
        }
        if (allSame)
            throw cipher_error("WeakKey");
    }
}

/**
 * @brief Шифрование текста
 * @param[in] open_text Открытый текст для шифрования
 * @return Зашифрованный текст
 * @throw cipher_error при ошибках валидации
 * @details Алгоритм: C_i = (P_i + K_{i mod len(K)}) mod N
 */
std::string modAlphaCipher::encrypt(const std::string& open_text) {
    std::vector<int> work = convert(getValidOpenText(open_text));
    for(unsigned i=0; i < work.size(); i++)
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
    return convert(work);
}

/**
 * @brief Дешифрование текста
 * @param[in] cipher_text Зашифрованный текст
 * @return Расшифрованный текст
 * @throw cipher_error при ошибках валидации
 * @details Алгоритм: P_i = (C_i - K_{i mod len(K)} + N) mod N
 */
std::string modAlphaCipher::decrypt(const std::string& cipher_text) {
    std::vector<int> work = convert(getValidCipherText(cipher_text));
    for(unsigned i=0; i < work.size(); i++)
        work[i] = (work[i] + alphaNum.size() - key[i % key.size()]) % alphaNum.size();
    return convert(work);
}

/**
 * @brief Преобразование строки в вектор числовых индексов
 * @param[in] s Входная строка
 * @return Вектор индексов символов
 */
std::vector<int> modAlphaCipher::convert(const std::string& s) {
    std::wstring ws = codec.from_bytes(s);
    std::vector<int> result;
    for(auto c:ws)
        result.push_back(alphaNum[c]);
    return result;
}

/**
 * @brief Преобразование вектора индексов в строку
 * @param[in] v Вектор индексов
 * @return Результирующая строка
 */
std::string modAlphaCipher::convert(const std::vector<int>& v) {
    std::wstring ws;
    for(auto i:v)
        ws.push_back(numAlpha[i]);
    std::string result = codec.to_bytes(ws);
    return result;
}

/**
 * @brief Проверка и нормализация ключа
 * @param[in] s Ключ в виде строки
 * @return Валидированный ключ (все символы заглавные)
 * @throw cipher_error при пустом ключе или не-буквенных символах
 */
std::string modAlphaCipher::getValidKey(const std::string & s) {
    std::wstring ws = codec.from_bytes(s);
    if (ws.empty())
        throw cipher_error("Пустой ключ");
    
    std::wstring tmp = ws;
    for (auto& c : tmp) {
        if(c < L'А' || c > L'я')
            throw cipher_error("Неверный ключ: содержит не-буквенные символы");
        if (c >= L'а' && c <= L'я')
            c -= 32;  // преобразование в заглавные
    }
    
    std::string mp = codec.to_bytes(tmp);
    return mp;
}

/**
 * @brief Проверка и нормализация открытого текста
 * @param[in] s Открытый текст
 * @return Валидированный текст (только заглавные русские буквы)
 * @throw cipher_error при пустом тексте
 */
std::string modAlphaCipher::getValidOpenText(const std::string & s) {
    std::wstring ws = codec.from_bytes(s);
    std::wstring tmp;
    
    for (auto c:ws) {
        if (c >= L'А' && c <= L'я') {
            if (c >= L'а' && c <= L'я')
                tmp.push_back(c - 32);  // преобразование в заглавные
            else
                tmp.push_back(c);
        }
    }
    
    if (tmp.empty())
        throw cipher_error("Отсутствует открытый текст!"); 
    
    std::string mp = codec.to_bytes(tmp);
    return mp;
}

/**
 * @brief Проверка зашифрованного текста
 * @param[in] s Зашифрованный текст
 * @return Валидированный текст
 * @throw cipher_error при пустом тексте или недопустимых символах
 */
std::string modAlphaCipher::getValidCipherText(const std::string & s) {
    std::wstring ws = codec.from_bytes(s);
    
    if (ws.empty())
        throw cipher_error("Empty cipher text");
    
    for (auto c:ws) {
        if ((c < L'А' || c > L'Я') && c != L'Ё')
            throw cipher_error("Неправильный зашифрованный текст!");
    }
    
    std::string mp = codec.to_bytes(ws);
    return mp;
}