/**
 * @file route.h
 * @brief Заголовочный файл для класса code (шифр маршрутной перестановки)
 * @author Назарова Софья
 * @date 2025
 */

#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
using namespace std;

/**
 * @class cipher_error
 * @brief Исключение для ошибок шифрования маршрутной перестановкой
 */
class cipher_error: public invalid_argument {
    public:
        /**
         * @brief Конструктор с строкой
         * @param[in] what_arg Сообщение об ошибке
         */
        explicit cipher_error (const string& what_arg):
            invalid_argument(what_arg) {}
            
        /**
         * @brief Конструктор с си-строкой
         * @param[in] what_arg Сообщение об ошибке
         */
        explicit cipher_error (const char* what_arg):
            invalid_argument(what_arg) {}
};

/**
 * @class code
 * @brief Класс для шифрования методом маршрутной перестановки
 * @details Шифрование происходит путем записи текста в таблицу по строкам
 *          и чтения по столбцам в обратном порядке
 */
class code {
    private:
        int key; ///< Ключ шифрования (количество столбцов)
        
        /**
         * @brief Проверка валидности ключа
         * @param[in] key Ключ для проверки
         * @param[in] Text Текст для шифрования
         * @return Валидный ключ
         * @throw cipher_error при невалидном ключе
         */
        inline int getValidKey(int key, const string& Text);
        
        /**
         * @brief Проверка валидности открытого текста
         * @param[in] s Текст для проверки
         * @return Валидированный текст
         * @throw cipher_error при невалидном тексте
         */
        inline string getValidOpenText(const string& s);
        
        /**
         * @brief Проверка валидности зашифрованного текста
         * @param[in] s Зашифрованный текст
         * @param[in] open_text Исходный открытый текст
         * @return Валидированный зашифрованный текст
         * @throw cipher_error при несоответствии длин
         */
        inline string getValidCipherText(const string& s, const string& open_text);
        
    public:
        /**
         * @brief Удаленный конструктор по умолчанию
         */
        code() = delete;
        
        /**
         * @brief Конструктор с ключом и текстом
         * @param[in] skey Ключ шифрования
         * @param[in] text Текст для инициализации
         */
        code(int skey, string text);
        
        /**
         * @brief Шифрование текста
         * @param[in] text Текст для шифрования
         * @return Зашифрованный текст
         */
        string encryption(const string& text);
        
        /**
         * @brief Дешифрование текста
         * @param[in] text Зашифрованный текст
         * @param[in] open_text Исходный открытый текст (для проверки длины)
         * @return Расшифрованный текст
         */
        string transcript(const string& text, const string& open_text);
};