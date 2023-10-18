#include <clib/header.hpp>
#include <doctest/doctest.h>
#include <iostream>
#include <cstring>

bool check_bitwise_eq(float a, float b)
{
    return !strncmp(reinterpret_cast<char*>(&a), reinterpret_cast<char*>(&b), 4);
}

TEST_CASE("Simple tests for clib")
{
    float f1 = 3.0001f;
    float f2 = 6.0002f;
    // std::cout << f1 * f2 << std::endl;

    auto fp1 = clib::fp32(f1);
    auto fp2 = clib::fp32(f2);
    // std::cout << fp1.get_float() << std::endl;
    // std::cout << fp2.get_float() << std::endl;
    auto fp3 = fp1 * fp2;

    // std::cout << fp3.get_float() << std::endl;

    std::cout << "fp32 mult:  ";
    fp3.print_bit_interpr();
    std::cout << "float mult: ";
    clib::fp32(f1*f2).print_bit_interpr();
    
    CHECK(check_bitwise_eq(f1*f2, (fp1*fp2).get_float()));
}
