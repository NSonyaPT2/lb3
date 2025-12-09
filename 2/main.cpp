/**
 * @file main.cpp
 * @brief Тесты для класса code (шифр маршрутной перестановки)
 * @author Назарова Софья
 * @date 2025
 */

#include <UnitTest++/UnitTest++.h>
#include "route.h"
#include <string>

/**
 * @test Suite KeyTest
 * @brief Тесты для проверки ключа шифрования
 */
SUITE(KeyTest) {
    /**
     * @test ValidKey
     * @brief Проверка валидного ключа
     */
    TEST(ValidKey) {
        code cipher(3, "PRIVET");
        CHECK_EQUAL("ITREPV", cipher.encryption("PRIVET"));
    }
    