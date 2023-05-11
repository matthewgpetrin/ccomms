#include <iostream>
#include <vector>
#include <array>
#include <cassert>
#include "../../include/tensor.hpp"

int main() {
    using namespace ccomms;
    {
        // Test default constructor for dynamically-sized vector
        vector<int> v1;
        assert(v1.size() == 0);

        // Test default constructor for fixed-size vector
        vector<int, 3> v2;
        assert(v2.size() == 3);
    }

    {
        // test initialization of fixed-size vector with default size
        vector<int, 5> v1(5, 42);
        for (const auto& val : v1) {
            assert(val == 42);
        }

        // test initialization of dynamic-size vector with specified size
        vector<double> v2(4, 3.14);
        for (const auto& val : v2) {
            assert(val == 3.14);
        }
        assert(v2.size() == 4);

        // test initialization of dynamic-size vector with default size
        vector<std::string> v3(1, "hello");
        for (const auto& val : v3) {
            assert(val == "hello");
        }
        assert(v3.size() == 1);
    }

    {
        // Test initializer list constructor for dynamically-sized vector
        vector<int> v1{1, 2, 3};
        assert(v1.size() == 3);
        assert(v1[0] == 1);
        assert(v1[1] == 2);
        assert(v1[2] == 3);

        // Test initializer list constructor for fixed-size vector
        vector<int, 3> v2{4, 5, 6};
        assert(v2.size() == 3);
        assert(v2[0] == 4);
        assert(v2[1] == 5);
        assert(v2[2] == 6);

        // Test initializer list constructor for fixed-size vector with wrong number of elements
        bool caught_exception = false;
        try {
            vector<int, 3> v3{7, 8};
        } catch (const std::invalid_argument &e) {
            caught_exception = true;
        }
        assert(caught_exception);

        // Test initializer list constructor with type conversion
        vector<int> v4{1, 2, 3};
        assert(v4.size() == 3);
        assert(v4[0] == 1.0f);
        assert(v4[1] == 2.0f);
        assert(v4[2] == 3.0f);
    }

    {
        // Test copy constructor for dynamically-sized vector
        vector<int> v1{1, 2, 3};
        vector<int> v2(v1);
        assert(v2.size() == 3);
        assert(v2[0] == 1);
        assert(v2[1] == 2);
        assert(v2[2] == 3);

        // Test copy constructor for fixed-size vector
        vector<int, 3> v3{4, 5, 6};
        vector<int, 3> v4(v3);
        assert(v4.size() == 3);
        assert(v4[0] == 4);
        assert(v4[1] == 5);
        assert(v4[2] == 6);

        // Test copy constructor with type conversion
        std::vector<float> vec{1, 2, 3};
        vector<int> v5(vec);
        assert(v5.size() == 3);
        assert(v5[0] == 1);
        assert(v5[1] == 2);
        assert(v5[2] == 3);

        std::vector<int> c;
        c.assign(3, 4);

        // Test copy constructor with incompatible size
        bool caught_exception = false;
        try {
            vector<int, 2> v6(vec);
        } catch (const std::invalid_argument &e) {
            caught_exception = true;
        }
        assert(caught_exception);
    }

    {
        // Test dynamic vector move constructor
        std::vector<int> v1 = {1, 2, 3};
        vector<int> v2(std::move(v1));
        //assert(v1.empty());
        assert(v2.size() == 3);
        assert(v2[0] == 1 && v2[1] == 2 && v2[2] == 3);

        // Test fixed vector move constructor
        std::vector<double> v3 = {1.0, 2.0, 3.0};
        vector<double, 3> v4(std::move(v3));
        //assert(v3.empty());
        assert(v4.size() == 3);
        assert(v4[0] == 1.0 && v4[1] == 2.0 && v4[2] == 3.0);

        // Test type conversion in move constructor
        std::vector<float> v5 = {1.0f, 2.0f, 3.0f};
        vector<double, 3> v6(std::move(v5));
        //assert(v5.empty());
        assert(v6.size() == 3);
        assert(v6[0] == 1.0 && v6[1] == 2.0 && v6[2] == 3.0);
    }

    {
        // Test dynamic vector copy constructor from array
        std::array<int, 3> arr1 = {1, 2, 3};
        vector<int> v1(arr1);
        assert(arr1[0] == 1 && arr1[1] == 2 && arr1[2] == 3);
        assert(v1.size() == 3);
        assert(v1[0] == 1 && v1[1] == 2 && v1[2] == 3);

        // Test fixed vector copy constructor from array
        std::array<double, 3> arr2 = {1.0, 2.0, 3.0};
        vector<double, 3> v2(arr2);
        assert(arr2[0] == 1.0 && arr2[1] == 2.0 && arr2[2] == 3.0);
        assert(v2.size() == 3);
        assert(v2[0] == 1.0 && v2[1] == 2.0 && v2[2] == 3.0);

        // Test type conversion in copy constructor
        std::array<float, 3> arr3 = {1.0f, 2.0f, 3.0f};
        vector<double, 3> v3(arr3);
        assert(arr3[0] == 1.0f && arr3[1] == 2.0f && arr3[2] == 3.0f);
        assert(v3.size() == 3);
        assert(v3[0] == 1.0 && v3[1] == 2.0 && v3[2] == 3.0);
    }

    {
        // Test dynamic vector move constructor from array
        std::array<int, 3> arr1 = {1, 2, 3};
        vector<int> v1(std::move(arr1));
        //assert(arr1.empty());
        assert(v1.size() == 3);
        assert(v1[0] == 1 && v1[1] == 2 && v1[2] == 3);

        // Test fixed vector move constructor from array
        std::array<double, 3> arr2 = {1.0, 2.0, 3.0};
        vector<double, 3> v2(std::move(arr2));
        //assert(arr2.empty());
        assert(v2.size() == 3);
        assert(v2[0] == 1.0 && v2[1] == 2.0 && v2[2] == 3.0);

        // Test type conversion in move constructor
        std::array<float, 3> arr3 = {1.0f, 2.0f, 3.0f};
        vector<double, 3> v3(std::move(arr3));
        //assert(arr3.empty());
        assert(v3.size() == 3);
        assert(v3[0] == 1.0 && v3[1] == 2.0 && v3[2] == 3.0);
    }

    {
        // Test dynamic vector copy assignment from std::vector
        std::vector<int> vec1 = {1, 2, 3};
        vector<int> v1;
        v1 = vec1;
        assert(v1.size() == 3);
        assert(v1[0] == 1 && v1[1] == 2 && v1[2] == 3);

        // Test fixed vector copy assignment from std::vector with same size
        std::vector<double> vec2 = {1.0, 2.0, 3.0};
        vector<double, 3> v2;
        v2 = vec2;
        assert(v2.size() == 3);
        assert(v2[0] == 1.0 && v2[1] == 2.0 && v2[2] == 3.0);

        // Test fixed vector copy assignment from std::vector with different size (not allowed)
        bool caught_exception = false;
        try {
            std::vector<float> vec3 = {1.0f, 2.0f};
            vector<float, 3> v3;
            v3 = vec3;
        } catch (const std::invalid_argument &e) {
            caught_exception = true;
        }
        assert(caught_exception);

        // Test type conversion in copy assignment
        std::vector<int> vec4 = {1, 2, 3};
        vector<double> v4;
        v4 = vec4;
        assert(v4.size() == 3);
        assert(v4[0] == 1.0 && v4[1] == 2.0 && v4[2] == 3.0);
    }

    {
        std::vector<int> vec1 = {1, 2, 3};
        vector<int> v1;
        v1 = std::move(vec1);
        //assert(vec1.empty());
        assert(v1.size() == 3);
        assert(v1[0] == 1 && v1[1] == 2 && v1[2] == 3);

        // Test fixed vector move assignment from std::vector with same size
        std::vector<double> vec2 = {1.0, 2.0, 3.0};
        vector<double, 3> v2;
        v2 = std::move(vec2);
        //assert(vec2.empty());
        assert(v2.size() == 3);
        assert(v2[0] == 1.0 && v2[1] == 2.0 && v2[2] == 3.0);

        // Test fixed vector move assignment from std::vector with different size (not allowed)
        bool caught_exception = false;
        try {
            std::vector<float> vec3 = {1.0f, 2.0f};
            vector<float, 3> v3;
            v3 = std::move(vec3);
        } catch (const std::invalid_argument &e) {
            caught_exception = true;
        }
        assert(caught_exception);

        // Test type conversion in move assignment
        std::vector<int> vec4 = {1, 2, 3};
        vector<double> v4;
        v4 = std::move(vec4);
        //assert(vec4.empty());
        assert(v4.size() == 3);
        assert(v4[0] == 1.0 && v4[1] == 2.0 && v4[2] == 3.0);
    }

    {
        // Test dynamic vector copy assignment from std::array
        std::array<int, 3> arr1 = {1, 2, 3};
        vector<int> v1;
        v1 = arr1;
        assert(v1.size() == 3);
        assert(v1[0] == 1 && v1[1] == 2 && v1[2] == 3);

        // Test fixed vector copy assignment from std::array with same size
        std::array<double, 3> arr2 = {1.0, 2.0, 3.0};
        vector<double, 3> v2;
        v2 = arr2;
        assert(v2.size() == 3);
        assert(v2[0] == 1.0 && v2[1] == 2.0 && v2[2] == 3.0);

        // Test fixed vector copy assignment from std::array with different size (not allowed)
        bool caught_exception = false;
        try {
            std::array<float, 2> arr3 = {1.0f, 2.0f};
            vector<float, 3> v3;
            v3 = arr3;
        } catch (const std::invalid_argument &e) {
            caught_exception = true;
        }
        assert(caught_exception);

        // Test type conversion in copy assignment
        std::array<int, 3> arr4 = {1, 2, 3};
        vector<double> v4;
        v4 = arr4;
        assert(v4.size() == 3);
        assert(v4[0] == 1.0 && v4[1] == 2.0 && v4[2] == 3.0);
    }

    {
        // Test dynamic vector move assignment from std::array
        std::array<int, 3> arr1 = {1, 2, 3};
        vector<int> v1;
        v1 = std::move(arr1);
        //assert(arr1.empty());
        assert(v1.size() == 3);
        assert(v1[0] == 1 && v1[1] == 2 && v1[2] == 3);

        // Test fixed vector move assignment from std::array with same size
        std::array<double, 3> arr2 = {1.0, 2.0, 3.0};
        vector<double, 3> v2;
        v2 = std::move(arr2);
        //assert(arr2.empty());
        assert(v2.size() == 3);
        assert(v2[0] == 1.0 && v2[1] == 2.0 && v2[2] == 3.0);

        // Test fixed vector move assignment from std::array with different size (not allowed)
        bool caught_exception = false;
        try {
            std::array<float, 2> arr3 = {1.0f, 2.0f};
            vector<float, 3> v3;
            v3 = std::move(arr3);
        } catch (const std::invalid_argument &e) {
            caught_exception = true;
        }
        assert(caught_exception);

        // Test type conversion in move assignment
        std::array<int, 3> arr4 = {1, 2, 3};
        vector<double> v4;
        v4 = std::move(arr4);
        //assert(arr4.empty());
        assert(v4.size() == 3);
        assert(v4[0] == 1.0 && v4[1] == 2.0 && v4[2] == 3.0);
    }

    {
        // Dynamic vector cross product test
        vector<int, 3> v1 = {1, 2, 3};
        std::array<int, 3> v2 = {4, 5, 6};
        vector<int, 3> result = v1 & v2;
        assert(result[0] == -3);
        assert(result[1] == 6);
        assert(result[2] == -3);

        // Fixed vector cross product test
        vector<int, 3> v3 = {1, 2, 3};
        std::vector<int> v4 = {4, 5, 6};
        auto result2 = v3 & v4;
        assert(result2[0] == -3);
        assert(result2[1] == 6);
        assert(result2[2] == -3);

        // Invalid argument test
        std::vector<int> v5 = {1, 2, 3, 4};
        try {
            v3 & v5;
            assert(false);
        } catch (std::invalid_argument &e) {
            assert(true);
        }
    }

    // Test operator| with fixed-size vectors
    {
        vector<int, 3> a{1, 2, 3};
        vector<double, 3> b{4.5, 6.7, 8.9};

        // Valid usage
        auto c = a | b;
        assert(std::abs(c - 44.6) < 1e-9);

        // Invalid usage: vectors of different length
        std::vector<int> d{1, 2, 3, 4};
        try {
            auto e = a | d;
            assert(false);  // This line should not be reached
        } catch (const std::invalid_argument &e) {
            assert(std::string(e.what()) == "\nERR: inner product requires vectors of the same length\n");
        }
    }

    // Test operator| with dynamic-size vectors
    {
        vector<int> a{1, 2, 3};
        vector<double> b{4.5, 6.7, 8.9};

        // Valid usage
        auto c = a | b;
        assert(std::abs(c - 44.6) < 1e-9);

        // Invalid usage: vectors of different length
        std::vector<int> d{1, 2, 3, 4};
        try {
            auto e = a | d;
            assert(false);  // This line should not be reached
        } catch (const std::invalid_argument &e) {
            assert(std::string(e.what()) == "\nERR: inner product requires vectors of the same length\n");
        }
    }

    {
        // Test fixed-size vectors
        vector<int, 3> v1{1, 2, 3};
        vector<int, 3> v2{4, 5, 6};
        auto result1 = v1 + v2;
        assert(result1[0] == 5 && result1[1] == 7 && result1[2] == 9);

        // Test dynamic-size vectors
        vector<int> v3{1, 2, 3};
        vector<int> v4{4, 5, 6};
        auto result2 = v3 + v4;
        assert(result2[0] == 5 && result2[1] == 7 && result2[2] == 9);

        // Test unequal vector lengths
        vector<int, 3> v5{1, 2, 3};
        vector<int, 4> v6{4, 5, 6, 7};
        try {
            auto result3 = v5 + v6;
        } catch (std::invalid_argument &e) {
            assert(std::string(e.what()) == "\nERR: elementwise addition requires vectors of equal length\n");
        }
    }

    {
        // Test fixed-size vectors
        vector<int, 3> v1{1, 2, 3};
        vector<int, 3> v2{4, 5, 6};
        auto result1 = v1 - v2;
        assert(result1[0] == -3 && result1[1] == -3 && result1[2] == -3);

        // Test dynamic-size vectors
        vector<int> v3{1, 2, 3};
        vector<int> v4{4, 5, 6};
        auto result2 = v3 - v4;
        assert(result2[0] == -3 && result2[1] == -3 && result2[2] == -3);

        // Test unequal vector lengths
        vector<int, 3> v5{1, 2, 3};
        vector<int, 4> v6{4, 5, 6, 7};
        try {
            auto result3 = v5 - v6;
        } catch (std::invalid_argument &e) {
            assert(std::string(e.what()) == "\nERR: elementwise subtraction requires vectors of equal length\n");
        }
    }

    {
        // Test fixed-size vectors
        vector<int, 3> v1{1, 2, 3};
        vector<int, 3> v2{4, 5, 6};
        auto result1 = v1 * v2;
        assert(result1[0] == 4 && result1[1] == 10 && result1[2] == 18);

        // Test dynamic-size vectors
        vector<int> v3{1, 2, 3};
        vector<int> v4{4, 5, 6};
        auto result2 = v3 * v4;
        assert(result2[0] == 4 && result2[1] == 10 && result2[2] == 18);

        // Test scalar multiplication
        vector<int, 3> v5{1, 2, 3};
        auto result3 = v5 * 2;
        assert(result3[0] == 2 && result3[1] == 4 && result3[2] == 6);

        // Test unequal vector lengths
        vector<int, 3> v6{1, 2, 3};
        vector<int, 4> v7{4, 5, 6, 7};
    }

    {
        // Test fixed-size vectors
        vector<int, 3> v1{1, 2, 3};
        vector<int, 3> v2{4, 5, 6};
        auto result1 = v2 / v1;
        assert(result1[0] == 4 && result1[1] == 2 && result1[2] == 2);

        // Test dynamic-size vectors
        vector<int> v3{1, 2, 3};
        vector<int> v4{4, 5, 6};
        auto result2 = v4 / v3;
        assert(result2[0] == 4 && result2[1] == 2 && result2[2] == 2);

        // Test scalar multiplication
        vector<int, 3> v5{1, 2, 3};
        auto result3 = v5 / 2;
        assert(result3[0] == 0 && result3[1] == 1 && result3[2] == 1);

        // Test unequal vector lengths
        vector<int, 3> v6{1, 2, 3};
        vector<int, 4> v7{4, 5, 6, 7};
    }

    return 0;
}


