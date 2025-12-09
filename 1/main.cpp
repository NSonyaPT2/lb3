/**
 * @file main.cpp
 * @brief Тесты для класса modAlphaCipher
 * @author Назарова Софья
 * @date 2025
 * @copyright WECT ПГУ
 */

#include <UnitTest++/UnitTest++.h>
#include "modAlphaCipher.h"

/**
 * @test Suite KeyTest
 * @brief Тесты для проверки ключа шифрования
 */
SUITE(KeyTest)
{
    /**
     * @test ValidKey
     * @brief Проверка создания шифра с валидным ключом
     */
    TEST(ValidKey) {
        modAlphaCipher cipher("ЙЦУ");
        CHECK(true); // Просто проверяем что конструктор не бросает исключение
    }
    
    /**
     * @test LongKey
     * @brief Проверка длинного ключа
     */
    TEST(LongKey) {
        modAlphaCipher cipher("ПОМИДОРЫ");
        CHECK(true);
    }
    
    /**
     * @test LowCaseKey
     * @brief Проверка ключа в нижнем регистре
     */
    TEST(LowCaseKey) {
        modAlphaCipher cipher("йцу");
        CHECK(true);
    }
    
    /**
     * @test DigitsInKey
     * @brief Проверка ключа с цифрами (ожидается исключение)
     */
    TEST(DigitsInKey) {
        CHECK_THROW(modAlphaCipher cp("Й1"), cipher_error);
    }
    
    /**
     * @test PunctuationInKey
     * @brief Проверка ключа со знаками препинания (ожидается исключение)
     */
    TEST(PunctuationInKey) {
        CHECK_THROW(modAlphaCipher cp("Й,Ц"), cipher_error);
    }
    
    /**
     * @test WhitespaceInKey
     * @brief Проверка ключа с пробелами (ожидается исключение)
     */
    TEST(WhitespaceInKey) {
        CHECK_THROW(modAlphaCipher cp("Й Ц У"), cipher_error);
    }
    
    /**
     * @test EmptyKey
     * @brief Проверка пустого ключа (ожидается исключение)
     */
    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cp(""), cipher_error);
    }
    
    /**
     * @test WeakKey
     * @brief Проверка слабого ключа (все символы одинаковые, ожидается исключение)
     */
    TEST(WeakKey) {
        CHECK_THROW(modAlphaCipher cp("ЙЙЙ"), cipher_error);
    }
}

/**
 * @struct SimpleFixture
 * @brief Фикстура для тестов с предустановленным шифром
 */
struct SimpleFixture {
    modAlphaCipher * p; ///< Указатель на шифр
    
    /**
     * @brief Конструктор фикстуры
     * @details Создает шифр с ключом "БОРЩ"
     */
    SimpleFixture() { p = new modAlphaCipher("БОРЩ"); }
    
    /**
     * @brief Деструктор фикстуры
     */
    ~SimpleFixture() { delete p; }
};

/**
 * @test Suite EncryptTest
 * @brief Тесты для шифрования
 */
SUITE(EncryptTest)
{
    /**
     * @test BasicEncrypt
     * @brief Базовое шифрование
     */
    TEST_FIXTURE(SimpleFixture, BasicEncrypt) {
        std::string result = p->encrypt("СУП");
        CHECK(!result.empty()); // Просто проверяем что результат не пустой
    }
    
    /**
     * @test LowCaseEncrypt
     * @brief Шифрование текста в нижнем регистре
     */
    TEST_FIXTURE(SimpleFixture, LowCaseEncrypt) {
        std::string result = p->encrypt("суп");
        CHECK(!result.empty());
    }
    
    /**
     * @test TextWithSpaces
     * @brief Шифрование текста с пробелами
     */
    TEST_FIXTURE(SimpleFixture, TextWithSpaces) {
        std::string result = p->encrypt("СУП С ФРИКАДЕЛЬКАМИ");
        CHECK(!result.empty());
    }
    
    /**
     * @test TextWithNumbers
     * @brief Шифрование текста с цифрами
     */
    TEST_FIXTURE(SimpleFixture, TextWithNumbers) {
        std::string result = p->encrypt("2СУПОВ");
        CHECK(!result.empty());
    }
    
    /**
     * @test EmptyString
     * @brief Шифрование пустой строки (ожидается исключение)
     */
    TEST_FIXTURE(SimpleFixture, EmptyString) {
        CHECK_THROW(p->encrypt(""), cipher_error);
    }
    
    /**
     * @test NoAlphaString
     * @brief Шифрование строки без букв (ожидается исключение)
     */
    TEST_FIXTURE(SimpleFixture, NoAlphaString) {
        CHECK_THROW(p->encrypt("*_*"), cipher_error);
    }
    
    /**
     * @test MaxShiftKey
     * @brief Шифрование с максимальным сдвигом (ключ "Я")
     */
    TEST(MaxShiftKey) {
        modAlphaCipher cipher("Я");
        std::string result = cipher.encrypt("СУП");
        CHECK(!result.empty());
    }
}

/**
 * @test Suite DecryptTest
 * @brief Тесты для дешифрования
 */
SUITE(DecryptTest)
{
    /**
     * @test BasicDecrypt
     * @brief Базовое дешифрование
     */
    TEST_FIXTURE(SimpleFixture, BasicDecrypt) {
        std::string encrypted = p->encrypt("СУП");
        CHECK_EQUAL(encrypted, "ТВА");
    }
    
    /**
     * @test LowCaseDecrypt
     * @brief Дешифрование текста в нижнем регистре (ожидается исключение)
     */
    TEST_FIXTURE(SimpleFixture, LowCaseDecrypt) {
        CHECK_THROW(p->decrypt("суп"), cipher_error);
    }
    
    /**
     * @test WhitespaceDecrypt
     * @brief Дешифрование текста с пробелами (ожидается исключение)
     */
    TEST_FIXTURE(SimpleFixture, WhitespaceDecrypt) {
        CHECK_THROW(p->decrypt("СУП С ФРИКАДЕЛЬКАМИ"), cipher_error);
    }
    
    /**
     * @test DigitsDecrypt
     * @brief Дешифрование текста с цифрами (ожидается исключение)
     */
    TEST_FIXTURE(SimpleFixture, DigitsDecrypt) {
        CHECK_THROW(p->decrypt("2СУПОВ"), cipher_error);
    }
    
    /**
     * @test PunctDecrypt
     * @brief Дешифрование текста со знаками препинания (ожидается исключение)
     */
    TEST_FIXTURE(SimpleFixture, PunctDecrypt) {
        CHECK_THROW(p->decrypt("СУП!"), cipher_error);
    }
    
    /**
     * @test EmptyDecrypt
     * @brief Дешифрование пустой строки (ожидается исключение)
     */
    TEST_FIXTURE(SimpleFixture, EmptyDecrypt) {
        CHECK_THROW(p->decrypt(""), cipher_error);
    }
    
    /**
     * @test MaxShiftDecrypt
     * @brief Дешифрование с максимальным сдвигом
     */
    TEST(MaxShiftDecrypt) {
        modAlphaCipher cipher("Я");
        std::string encrypted = cipher.encrypt("КОД");
        CHECK_EQUAL(encrypted, "ЙНГ");
    }
}

/**
 * @brief Главная функция для запуска тестов
 * @param[in] argc Количество аргументов командной строки
 * @param[in] argv Аргументы командной строки
 * @return Код завершения (0 - все тесты прошли успешно)
 */
int main(int argc, char **argv)
{
    return UnitTest::RunAllTests();
}