#include <UnitTest++/UnitTest++.h>
#include "route.h"
#include <string>

SUITE(KeyTest) {
    TEST(ValidKey) {
        code cipher(3, "PRIVET");
        CHECK_EQUAL("ITREPV", cipher.encryption("PRIVET"));
    }
    TEST(InvalidKeyTooSmall) {
        CHECK_THROW(code(1, "DOG"), cipher_error);
    }
    TEST(InvalidKeyTooLarge) {
        CHECK_THROW(code(10, "SHORT"), cipher_error);
    }
}

struct KeyThree_fixture {
    code * t;
    KeyThree_fixture() {
        t = new code(3, "PRIVET");
    }
    ~KeyThree_fixture() {
        delete t;
    }
};

SUITE(EncryptTest) {
    TEST_FIXTURE(KeyThree_fixture, UpCaseString) {
        std::string input = "PRIVET";
        CHECK_EQUAL("ITREPV", t->encryption(input));
    }
    TEST_FIXTURE(KeyThree_fixture, LowCaseString) {
        std::string input = "privet";
        CHECK_EQUAL("itrepv", t->encryption(input));
    }
    TEST_FIXTURE(KeyThree_fixture, StringWithWhitspace) {
        std::string input = "PRI VET";
        CHECK_EQUAL("ITREPV", t->encryption(input));
    }
    TEST_FIXTURE(KeyThree_fixture, StringWithNumbers) {
        std::string input = "PR1VET";
        CHECK_THROW(t->encryption(input), cipher_error);
    }
    TEST_FIXTURE(KeyThree_fixture, EmptyString) {
        std::string input = "";
        CHECK_THROW(t->encryption(input), cipher_error);
    }
    TEST_FIXTURE(KeyThree_fixture, NoAlphaString) {
        std::string input = ":)";
        CHECK_THROW(t->encryption(input), cipher_error);
    }
    TEST(SquareTable) {
        code cipher(4, "HEDGEHOG");
        std::string input = "HEDGEHOG";
        CHECK_EQUAL("GGDOEHHE", cipher.encryption(input));
    }
}

SUITE(DecryptTest) {
    TEST_FIXTURE(KeyThree_fixture, UpCaseString) {
        std::string encrypted = "ITREPV";
        std::string original = "PRIVET";
        CHECK_EQUAL("PRIVET", t->transcript(encrypted, original));
    }
    TEST_FIXTURE(KeyThree_fixture, LowCaseString) {
        std::string encrypted = "itrepv";
        std::string original = "privet";
        CHECK_EQUAL("privet", t->transcript(encrypted, original));
    }
    TEST_FIXTURE(KeyThree_fixture, WhitespaceInCipher) {
        std::string encrypted = "ITR EPV";
        std::string original = "PRIVET";
        CHECK_THROW(t->transcript(encrypted, original), cipher_error);
    }
    TEST_FIXTURE(KeyThree_fixture, DigitsInCipher) {
        std::string encrypted = "1TREPV";
        std::string original = "PRIVET";
        CHECK_THROW(t->transcript(encrypted, original), cipher_error);
    }
    TEST_FIXTURE(KeyThree_fixture, DifferentLength) {
        std::string encrypted = "SHORT";
        std::string original = "LONGER";
        CHECK_THROW(t->transcript(encrypted, original), cipher_error);
    }
    TEST_FIXTURE(KeyThree_fixture, EmptyStrings) {
        std::string encrypted = "";
        std::string original = "";
        CHECK_THROW(t->transcript(encrypted, original), cipher_error);
    }
    TEST(SquareTableDecrypt) {
        code cipher(4, "PROGRAM");
        std::string encrypted = "GGDOEHHE";
        std::string original = "HEDGEHOG";
        CHECK_EQUAL("HEDGEHOG", cipher.transcript(encrypted, original));
    }
}

int main(int argc, char **argv) {
    return UnitTest::RunAllTests();
}