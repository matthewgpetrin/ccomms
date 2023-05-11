// Copyright(c) 2023, Matthew Petrin, All rights reserved.

#ifndef CCOMMS_MODULES_TENSOR_VECTOR_HPP_
#define CCOMMS_MODULES_TENSOR_VECTOR_HPP_

#include <array>
#include <vector>
#include <ostream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <type_traits>

namespace ccomms {

// TODO: implement complex inner product and ensure complex functionality
/**
 * @class vector
 *
 * @brief A templated vector class inheriting from std::array/std::vector featuring expanded functionality.
 *
 * @tparam T: Container element type
 * @tparam N: Static container length
 *
 * @ingroup tensor
 *
 * @details The vector class is a 1D vector of potentially infinite length capable of storing any data type. It wraps
 * both std::vector and optionally std::array. Can be used as a fixed-length array when speed is required by specifying
 * the N template parameter (defaults to 0 for std::vector). It also features overloaded methods allowing for element
 * insertion in a way that automatically handles copying and moving. For vectors of arithmetic types, automatic type
 * conversion is implemented. Mathematical operators are also overloaded for both element-wise and scalar operations as
 * well as inner and cross product.
 */
template<typename T, std::size_t N = 0>
class vector : public std::conditional<N == 0, std::vector<T>, std::array<T, N> >::type {

    using container = typename std::conditional<N == 0, std::vector<T>, std::array<T, N> >::type;

    bool is_row;
    bool is_col;

public:

    //************************************************** CONSTRUCTORS **************************************************

    explicit vector(const char &vectype = 'c') :
            container(),
            is_row(vectype == 'r'),
            is_col(vectype == 'c') {}

    template<typename U>
    vector(const size_t &len, const U &fill, const char &vectype = 'c') :
            container(),
            is_row(vectype == 'r'),
            is_col(vectype == 'c') {
        if constexpr (!N)
            container::assign(len, fill);

        if constexpr (N)
            std::fill(container::begin(), std::next(container::begin(), len), fill);

        if constexpr (!std::is_same_v<U, T>)
            std::cerr << "\nWARNING: vector fill constructor performing type conversion\n";
    }

    template<typename U>
    vector(std::initializer_list<U> list, const char &vectype = 'c') :
            container(),
            is_row(vectype == 'r'),
            is_col(vectype == 'c') {
        if (N && list.size() != N)
            throw std::invalid_argument("\nERR: fixed vector requires number of elements equal to its length");

        if constexpr (!N)
            container::insert(container::end(), list.begin(), list.end());

        if constexpr (N)
            std::copy(list.begin(), list.end(), container::begin());

        if constexpr (!std::is_same_v<U, T>)
            std::cerr << "\nWARNING: vector list constructor performing type conversion\n";
    }

    template<typename V, typename U = typename V::value_type>
    explicit vector(const V &other, const char &vectype = 'c') :
            container(),
            is_row(vectype == 'r'),
            is_col(vectype == 'c') {
        init_copy(other, "constructor");

        if constexpr (!std::is_same_v<U, T>)
            std::cerr << "\nWARNING: vector copy constructor performing type conversion\n";
    }

    template<typename V, typename U = typename V::value_type>
    explicit vector(V &&other, const char &vectype = 'c') :
            container(),
            is_row(vectype == 'r'),
            is_col(vectype == 'c') {
        init_move(other, "constructor");

        if constexpr (!std::is_same_v<U, T>)
            std::cerr << "\nWARNING: vector move constructor performing type conversion\n";
    }

    //*************************************************** ASSIGNMENT ***************************************************

    template<typename V, typename U = typename V::value_type>
    vector<T, N> &operator=(const V &other) {
        init_copy(other, "assignment");

        if constexpr (!std::is_same_v<U, T>)
            std::cerr << "\nWARNING: vector copy assignment performed on vectors of different types\n";

        return *this;
    }

    template<typename V, typename U = typename V::value_type>
    vector<T, N> &operator=(const V &&other) {
        init_move(other, "assignment");

        if constexpr (!std::is_same_v<U, T>)
            std::cerr << "\nWARNING: vector move assignment performed on vectors of different types\n";

        return *this;
    }

    //************************************************** VECTOR MATH ***************************************************

    template<typename V, typename U = typename V::value_type>
    vector<std::common_type_t<T, U>, N> operator&(const V &other) const {
        if (other.size() != 3 || this->size() != 3)
            throw std::invalid_argument("\nERR: cross product requires two 3D vectors\n");

        auto x = (*this)[1] * other[2] - (*this)[2] * other[1];
        auto y = (*this)[2] * other[0] - (*this)[0] * other[2];
        auto z = (*this)[0] * other[1] - (*this)[1] * other[0];
        return {x, y, z};
    }

    template<typename V, typename U = typename V::value_type>
    std::common_type_t<T, U> operator|(const V &other) const {
        if (other.size() != this->size())
            throw std::invalid_argument("\nERR: inner product requires vectors of the same length\n");

        std::common_type_t<T, U> result = 0;
        for (std::size_t i = 0; i < this->size(); i++)
            result += (*this)[i] * other[i];

        return result;
    }

    //********************************************** ELEMENTWISE OVERLOADS *********************************************

    template<typename V, typename U = typename V::value_type>
    vector<std::common_type_t<T, U>, N> operator+(const V &other) const {
        if (other.size() != this->size())
            throw std::invalid_argument("\nERR: elementwise addition requires vectors of equal length\n");

        vector<std::common_type_t<T, U>, N> result(this->size(), 0);
        for (std::size_t i = 0; i < this->size(); i++)
            result[i] = (*this)[i] + other[i];
        return result;
    }

    template<typename V, typename U = typename V::value_type>
    vector<std::common_type_t<T, U>, N> operator-(const V &other) const {
        if (other.size() != this->size())
            throw std::invalid_argument("\nERR: elementwise subtraction requires vectors of equal length\n");

        vector<std::common_type_t<T, U>, N> result(this->size(), 0);
        for (std::size_t i = 0; i < this->size(); i++)
            result[i] = (*this)[i] - other[i];
        return result;
    }

    template<typename V, typename U = typename V::value_type>
    vector<std::common_type_t<T, U>, N> operator*(const V &other) const {
        if (other.size() != this->size() && other.size() != 1 && this->size() != 1)
            throw std::invalid_argument("\nERR: elementwise multiplication requires vectors of equal length\n");

        vector<std::common_type_t<T, U>, N> result(this->size(), 0);
        for (std::size_t i = 0; i < this->size(); i++)
            result[i] = (*this)[i] * other[i];
        return result;
    }

    template<typename V, typename U = typename V::value_type>
    vector<std::common_type_t<T, U>, N> operator/(const V &other) const {
        if (other.size() != this->size() && other.size() != 1 && this->size() != 1)
            throw std::invalid_argument("\nERR: elementwise division requires vectors of equal length\n");

        vector<std::common_type_t<T, U>, N> result(this->size(), 0);
        for (std::size_t i = 0; i < this->size(); i++)
            result[i] = (*this)[i] / other[i];
        return result;
    }

    //*********************************************** SCALARS OVERLOADS ************************************************

    template<typename U>
    vector<std::common_type_t<T, U>, N> operator+(const U &scalar) const {
        vector<std::common_type_t<T, U>, N> result(this->size());

        for (std::size_t i = 0; i < this->size(); i++)
            result[i] = (*this)[i] + scalar;

        return result;
    }

    template<typename U>
    vector<std::common_type_t<T, U>, N> operator-(const U &scalar) const {
        vector<std::common_type_t<T, U>, N> result(this->size());

        for (std::size_t i = 0; i < this->size(); i++)
            result[i] = (*this)[i] - scalar;

        return result;
    }

    template<typename U>
    vector<std::common_type_t<T, U>, N> operator*(const U &scalar) const {
        vector<std::common_type_t<T, U>, N> result(this->size());

        for (std::size_t i = 0; i < this->size(); i++)
            result[i] = (*this)[i] * scalar;

        return result;
    }

    template<typename U>
    vector<std::common_type_t<T, U>, N> operator/(const U &scalar) const {
        vector<std::common_type_t<T, U>, N> result(this->size());

        for (std::size_t i = 0; i < this->size(); i++)
            result[i] = (*this)[i] / scalar;

        return result;
    }

private:

    //***************************************************** INITS ******************************************************

    template<typename V>
    void init_copy(const V &other, const std::string &logic) {
        if (N && other.size() != N)
            throw std::invalid_argument(
                    "\nERR: fixed vector" + logic + " requires # of elements equal to its length");

        if constexpr (!N) {
            container::clear();
            container::insert(container::end(), other.begin(), other.end());
        }
        if constexpr (N) {
            std::copy(other.begin(), other.end(), container::begin());
        }
    }

    template<typename V>
    void init_move(const V &other, const std::string &logic) {
        if (N && other.size() != N)
            throw std::invalid_argument(
                    "\nERR: fixed vector" + logic + " requires # of elements equal to its length");

        if constexpr (!N) {
            container::clear();
            container::insert(
                    container::end(),
                    std::make_move_iterator(other.begin()),
                    std::make_move_iterator(other.end()));
        }
        if constexpr (N) {
            std::copy(
                    std::make_move_iterator(other.begin()),
                    std::make_move_iterator(other.end()),
                    container::begin());
        }
    }
};

template<typename T, size_t N>
std::ostream &operator<<(std::ostream &os, const vector<T, N> &vec) {
    os << "[";
    for (std::size_t i = 0; i < vec.size(); i++) {
        os << vec[i];
        if (i != vec.size() - 1) os << ", ";
    }
    os << "]";
    return os;
}

}

#endif // CCOMMS_MODULES_TENSOR_VECTOR_HPP_