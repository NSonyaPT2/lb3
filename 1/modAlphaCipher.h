#pragma once
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <locale>
#include <codecvt>

/**
 * @class cipher_error
 * @brief Исключение для ошибок шифрования
 * @details Наследуется от std::invalid_argument, используется для обработки ошибок
 *          при работе с шифром
 */
class cipher_error: public std::invalid_argument {
    public:
        /**
         * @brief Конструктор с строкой
         * @param[in] what_arg Сообщение об ошибке
         */
        explicit cipher_error (const std::string& what_arg):
        std::invalid_argument(what_arg) {}
        
        /**
         * @brief Конструктор с си-строкой
         * @param[in] what_arg Сообщение об ошибке
         */
        explicit cipher_error (const char* what_arg):
        std::invalid_argument(what_arg) {}
};

/**
 * @class modAlphaCipher
 * @brief Класс для шифрования методом модифицированного алфавитного шифра
 * @details Реализует шифрование с использованием ключа на основе русского алфавита.
 *          Поддерживает только русские буквы, автоматически преобразует регистр.
 */
class modAlphaCipher {
    private:
        std::wstring numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; ///< Русский алфавит
        std::map<wchar_t,int> alphaNum; ///< Отображение символа в его индекс
        std::vector<int> key; ///< Ключ шифрования в числовом формате
        
        /**
         * @brief Преобразование строки в вектор числовых индексов
         * @param[in] s Входная строка
         * @return Вектор индексов символов
         */
        std::vector<int> convert(const std::string& s);
        
        /**
         * @brief Преобразование вектора индексов в строку
         * @param[in] v Вектор индексов
         * @return Результирующая строка
         */
        std::string convert(const std::vector<int>& v);
        
        /**
         * @brief Проверка и нормализация ключа
         * @param[in] s Ключ в виде строки
         * @return Валидированный ключ
         * @throw cipher_error при невалидном ключе
         */
        std::string getValidKey(const std::string & s);
        
        /**
         * @brief Проверка и нормализация открытого текста
         * @param[in] s Открытый текст
         * @return Валидированный текст
         * @throw cipher_error при невалидном тексте
         */
        std::string getValidOpenText(const std::string & s);
        
        /**
         * @brief Проверка зашифрованного текста
         * @param[in] s Зашифрованный текст
         * @return Валидированный текст
         * @throw cipher_error при невалидном тексте
         */
        std::string getValidCipherText(const std::string & s);
        
    public:
        /**
         * @brief Удаленный конструктор по умолчанию
         */
        modAlphaCipher()=delete;
        
        /**
         * @brief Конструктор с ключом
         * @param[in] skey Ключ шифрования
         * @throw cipher_error при слабом или невалидном ключе
         */
        modAlphaCipher(const std::string& skey);
        
        /**
         * @brief Шифрование текста
         * @param[in] open_text Открытый текст для шифрования
         * @return Зашифрованный текст
         * @throw cipher_error при ошибках валидации
         */
        std::string encrypt(const std::string& open_text);
        
        /**
         * @brief Дешифрование текста
         * @param[in] cipher_text Зашифрованный текст
         * @return Расшифрованный текст
         * @throw cipher_error при ошибках валидации
         */
        std::string decrypt(const std::string& cipher_text);
};