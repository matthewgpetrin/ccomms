// Copyright(c) 2023, Matthew Petrin, All rights reserved.

#ifndef CCOMMS_COORDS_TYPES_HPP
#define CCOMMS_COORDS_TYPES_HPP

#include "../tensor/types.hpp"

#include <numbers>

namespace ccomms {

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    class cartesian : public vector<T> {
    public:
        T x = (*this)[0];
        T y = (*this)[1];
        T z = (*this)[2];

        cartesian() : vector<T>(3) {}

        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
        cartesian(const U &x, const U &y, const U &z) : vector<T>({x, y, z}) {
            if constexpr (!std::is_same_v<U, T>)
                std::cerr << "\nWARNING: cartesian list constructor performing type conversion\n";
        }

        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
        explicit cartesian(const std::vector<U> &vec) : vector<T>(vec) {
            if (vec.size() != 3)
                throw std::invalid_argument("cartesian copy constructor source must have exactly 3 elements");

            if constexpr (!std::is_same_v<U, T>)
                std::cerr << "\nWARNING: cartesian copy constructor performing type conversion\n";
        }

        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
        explicit cartesian(std::vector<U> &&vec) : vector<T>(std::move(vec)) {
            if (this->size() != 3)
                throw std::invalid_argument("cartesian move constructor source must have exactly 3 elements");

            if constexpr (!std::is_same_v<U, T>)
                std::cerr << "\nWARNING: cartesian move constructor performing type conversion\n";
        }

        //*********************************************** ASSIGN AND MOVE **********************************************

        template<template<typename> class V, typename U>
        cartesian<T> &operator=(const V<U> &other) {
            static_assert(std::is_arithmetic_v<U>, "cartesian assignment operator source must have arithmetic type");

            if (other.size() != 3)
                throw std::invalid_argument("cartesian assignment operator source must have exactly 3 elements");

            if constexpr (std::is_same_v<T, U>)
                vector<T>::operator=(cartesian<U>{other[0], other[1], other[2]});
            else {
                std::cerr << "\nWARNING: cartesian copy operator performing type conversion\n";
                (*this)[0] = static_cast<T>(other[0]);
                (*this)[1] = static_cast<T>(other[1]);
                (*this)[2] = static_cast<T>(other[2]);
            }

            return *this;
        }

        template<template<typename> class V, typename U>
        cartesian<T> &operator=(V<U> &&other) {
            static_assert(std::is_arithmetic_v<U>,
                          "cartesian move operator source type must be convertible to destination type");

            if (other.size() != 3)
                throw std::invalid_argument("cartesian move operator source must have exactly 3 elements");

            if constexpr (std::is_same_v<T, U>)
                vector<T>::operator=(std::move(cartesian<U>{other[0], other[1], other[2]}));
            else {
                std::cerr << "\nWARNING: cartesian move operator performing type conversion\n";
                (*this)[0] = static_cast<T>(std::move(other[0]));
                (*this)[1] = static_cast<T>(std::move(other[1]));
                (*this)[2] = static_cast<T>(std::move(other[2]));
            }

            return *this;
        }

        //******************************************** DISALLOWED OPERATIONS *******************************************

        explicit cartesian(const std::size_t &size) = delete;

        cartesian(const std::size_t &size, const T &vals) = delete;
    };

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    class spherical : public vector<T> {
    public:
        T az = (*this)[0];
        T el = (*this)[1];

        //************************************************* CONSTRUCTORS ***********************************************

        spherical() : vector<T>(2) {}

        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
        spherical(const U &az, const U &el) : vector<T>({az, el}) {
            if constexpr (!std::is_same_v<U, T>)
                std::cerr << "\nWARNING: spherical list constructor performing type conversion\n";
        }

        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
        explicit spherical(const std::vector<U> &vec) : vector<T>(vec) {
            if (vec.size() != 2)
                throw std::invalid_argument("\nERR: spherical copy constructor source must have exactly 2 elements\n");

            if constexpr (!std::is_same_v<U, T>)
                std::cerr << "\nWARNING: spherical copy constructor performing type conversion\n";
        }

        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
        explicit spherical(std::vector<U> &&vec) : vector<T>(std::move(vec)) {
            if (this->size() != 2)
                throw std::invalid_argument("\nERR: spherical move constructor source must have exactly 2 elements\n");

            if constexpr (!std::is_same_v<U, T>)
                std::cerr << "\nWARNING: spherical move constructor performing type conversion\n";
        }

        //*********************************************** ASSIGN AND MOVE **********************************************

        template<template<typename> class V, typename U>
        spherical<T> &operator=(const V<U> &other) {
            static_assert(std::is_arithmetic_v<U>,
                          "\nERR: spherical assignment operator source must have arithmetic type\n");

            if (other.size() != 2)
                throw std::invalid_argument(
                        "\nERR: spherical assignment operator source must have exactly 2 elements\n");

            if constexpr (std::is_same_v<T, U>)
                vector<T>::operator=(spherical<U>{other[0], other[1]});
            else {
                std::cerr << "\nWARNING: spherical copy operator performing type conversion\n";
                (*this)[0] = static_cast<T>(other[0]);
                (*this)[1] = static_cast<T>(other[1]);
            }

            return *this;
        }

        template<template<typename> class V, typename U>
        spherical<T> &operator=(V<U> &&other) {
            static_assert(std::is_arithmetic_v<U>,
                          "\nERR: spherical move operator source type must be convertible to destination type\n");

            if (other.size() != 2)
                throw std::invalid_argument("\nERR: spherical move operator source must have exactly 2 elements\n");

            if constexpr (std::is_same_v<T, U>)
                vector<T>::operator=(std::move(spherical<U>{other[0], other[1]}));
            else {
                std::cerr << "\nWARNING: spherical move operator performing type conversion\n";
                (*this)[0] = static_cast<T>(std::move(other[0]));
                (*this)[1] = static_cast<T>(std::move(other[1]));
            }

            return *this;
        }

        //******************************************** DISALLOWED OPERATIONS *******************************************

        explicit spherical(const std::size_t &size) = delete;

        spherical(const std::size_t &size, const T &vals) = delete;
    };

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    class geodetic : public vector<T> {
    public:
        T lat = (*this)[0];
        T lon = (*this)[1];

        //************************************************* CONSTRUCTORS ***********************************************

        geodetic() : vector<T>(2) {}

        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
        geodetic(const U &lat, const U &lon) : vector<T>({lat, lon}) {
            if (lat > 90 || lat < -90)
                throw std::invalid_argument("ERR: geodetic lat must be between -90 and 90");
            if (lon > 180 || lon < -180)
                throw std::invalid_argument("ERR: geodetic lon must be between -180 and 180");

            if constexpr (!std::is_same_v<U, T>)
                    std::cerr << "\nWARNING: geodetic list constructor performing type conversion\n";
        }

        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
        explicit geodetic(const std::vector<U> &vec) : vector<T>(vec) {
            if (vec.size() != 2)
                throw std::invalid_argument("\nERR: geodetic copy constructor source must have exactly 2 elements\n");
            if (lat > 90 || lat < -90)
                throw std::invalid_argument("ERR: geodetic lat must be between -90 and 90");
            if (lon > 180 || lon < -180)
                throw std::invalid_argument("ERR: geodetic lon must be between -180 and 180");

            if constexpr (!std::is_same_v<U, T>)
                std::cerr << "\nWARNING: geodetic copy constructor performing type conversion\n";
        }

        template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
        explicit geodetic(std::vector<U> &&vec) : vector<T>(std::move(vec)) {
            if (this->size() != 2)
                throw std::invalid_argument("\nERR: geodetic move constructor source must have exactly 2 elements\n");
            if (lat > 90 || lat < -90)
                throw std::invalid_argument("ERR: geodetic lat must be between -90 and 90");
            if (lon > 180 || lon < -180)
                throw std::invalid_argument("ERR: geodetic lon must be between -180 and 180");

            if constexpr (!std::is_same_v<U, T>)
                std::cerr << "\nWARNING: geodetic move constructor performing type conversion\n";
        }

        //*********************************************** ASSIGN AND MOVE **********************************************

        template<template<typename> class V, typename U>
        geodetic<T> &operator=(const V<U> &other) {
            static_assert(std::is_arithmetic_v<U>,
                          "\nERR: geodetic assignment operator source must have arithmetic type\n");
            static_assert(other.size() == 2,
                          "\nERR: geodetic assignment operator source must have exactly 2 elements\n");

            if (other[0] > 90 || other[0] < -90)
                throw std::invalid_argument("ERR: geodetic lat must be between -90 and 90");
            if (other[1] > 180 || other[1] < -180)
                throw std::invalid_argument("ERR: geodetic lon must be between -180 and 180");

            if constexpr (std::is_same_v<T, U>)
                vector<T>::operator=(geodetic<U>{other[0], other[1]});
            else {
                std::cerr << "\nWARNING: geodetic copy operator performing type conversion\n";
                (*this)[0] = static_cast<T>(other[0]);
                (*this)[1] = static_cast<T>(other[1]);
            }

            return *this;
        }

        template<template<typename> class V, typename U>
        geodetic<T> &operator=(V<U> &&other) {
            static_assert(std::is_arithmetic_v<U>,
                          "\nERR: geodetic move operator source type must be convertible to destination type\n");
            static_assert(other.size() == 2,
                          "\nERR: geodetic assignment operator source must have exactly 2 elements\n");

            if (other[0] > 90 || other[0] < -90)
                throw std::invalid_argument("ERR: geodetic lat must be between -90 and 90");
            if (other[1] > 180 || other[1] < -180)
                throw std::invalid_argument("ERR: geodetic lon must be between -180 and 180");

            if constexpr (std::is_same_v<T, U>)
                vector<T>::operator=(std::move(geodetic<U>{other[0], other[1]}));
            else {
                std::cerr << "\nWARNING: geodetic move operator performing type conversion\n";
                (*this)[0] = static_cast<T>(std::move(other[0]));
                (*this)[1] = static_cast<T>(std::move(other[1]));
            }

            return *this;
        }

        //******************************************** DISALLOWED OPERATIONS *******************************************

        explicit geodetic(const std::size_t &size) = delete;

        geodetic(const std::size_t &size, const T &vals) = delete;
    };
}

#endif //CCOMMS_COORDS_TYPES_HPP
