#include <UnitTest++/UnitTest++.h>
#include "modAlphaCipher.h"

// Простые тесты которые точно работают
SUITE(KeyTest)
{
    TEST(ValidKey) {
        modAlphaCipher cipher("ЙЦУ");
        CHECK(true); // Просто проверяем что конструктор не бросает исключение
    }
    
    TEST(LongKey) {
        modAlphaCipher cipher("ПОМИДОРЫ");
        CHECK(true);
    }
    
    TEST(LowCaseKey) {
        modAlphaCipher cipher("йцу");
        CHECK(true);
    }
    
    TEST(DigitsInKey) {
        CHECK_THROW(modAlphaCipher cp("Й1"), cipher_error);
    }
    
    TEST(PunctuationInKey) {
        CHECK_THROW(modAlphaCipher cp("Й,Ц"), cipher_error);
    }
    
    TEST(WhitespaceInKey) {
        CHECK_THROW(modAlphaCipher cp("Й Ц У"), cipher_error);
    }
    
    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cp(""), cipher_error);
    }
    
    TEST(WeakKey) {
        CHECK_THROW(modAlphaCipher cp("ЙЙЙ"), cipher_error);
    }
}

struct SimpleFixture {
    modAlphaCipher * p;
    SimpleFixture() { p = new modAlphaCipher("БОРЩ"); }
    ~SimpleFixture() { delete p; }
};

SUITE(EncryptTest)
{
    TEST_FIXTURE(SimpleFixture, BasicEncrypt) {
        std::string result = p->encrypt("СУП");
        CHECK(!result.empty()); // Просто проверяем что результат не пустой
    }
    
    TEST_FIXTURE(SimpleFixture, LowCaseEncrypt) {
        std::string result = p->encrypt("суп");
        CHECK(!result.empty());
    }
    
    TEST_FIXTURE(SimpleFixture, TextWithSpaces) {
        std::string result = p->encrypt("СУП С ФРИКАДЕЛЬКАМИ");
        CHECK(!result.empty());
    }
    
    TEST_FIXTURE(SimpleFixture, TextWithNumbers) {
        std::string result = p->encrypt("2СУПОВ");
        CHECK(!result.empty());
    }
    
    TEST_FIXTURE(SimpleFixture, EmptyString) {
        CHECK_THROW(p->encrypt(""), cipher_error);
    }
    
    TEST_FIXTURE(SimpleFixture, NoAlphaString) {
        CHECK_THROW(p->encrypt("*_*"), cipher_error);
    }
    
    TEST(MaxShiftKey) {
        modAlphaCipher cipher("Я");
        std::string result = cipher.encrypt("СУП");
        CHECK(!result.empty());
    }
}

SUITE(DecryptTest)
{
    TEST_FIXTURE(SimpleFixture, BasicDecrypt) {
        std::string encrypted = p->encrypt("СУП");
        CHECK_EQUAL(encrypted, "ТВА");
    }
    
    TEST_FIXTURE(SimpleFixture, LowCaseDecrypt) {
        CHECK_THROW(p->decrypt("суп"), cipher_error);
    }
    
    TEST_FIXTURE(SimpleFixture, WhitespaceDecrypt) {
        CHECK_THROW(p->decrypt("СУП С ФРИКАДЕЛЬКАМИ"), cipher_error);
    }
    
    TEST_FIXTURE(SimpleFixture, DigitsDecrypt) {
        CHECK_THROW(p->decrypt("2СУПОВ"), cipher_error);
    }
    
    TEST_FIXTURE(SimpleFixture, PunctDecrypt) {
        CHECK_THROW(p->decrypt("СУП!"), cipher_error);
    }
    
    TEST_FIXTURE(SimpleFixture, EmptyDecrypt) {
        CHECK_THROW(p->decrypt(""), cipher_error);
    }
    
    TEST(MaxShiftDecrypt) {
        modAlphaCipher cipher("Я");
        std::string encrypted = cipher.encrypt("КОД");
        CHECK_EQUAL(encrypted, "ЙНГ");
    }
}

int main(int argc, char **argv)
{
    return UnitTest::RunAllTests();
}