//
// Created by mtesseract on 28-5-19.
//

#ifndef MACH_VECTOR_HPP
#define MACH_VECTOR_HPP

#include <array>
#include <cmath>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include "math/util/NumberTraits.hpp"
#include "auxiliary/MachAssertion.hpp"
#include "auxiliary/Properties.hpp"

/*
 * Thanks to Gaztin from the Cherno discord, for helping me with the functor things.
 */

namespace mach {
	template<typename T, std::size_t N>
	class Vector {
		std::array<T, N> m_data;
	public:
		Vector() : m_data{0} {}

		explicit Vector(T p_s) { m_data.fill(p_s); }

		Vector(const Vector &p_v) {
			m_data = p_v.m_data;
		}

		template<typename T2, std::size_t N2>
		explicit Vector(const Vector<T2, N2> &p_v) {
			m_data = {0};
			for (std::size_t i = 0; i < std::min(N, N2); ++i) {
				m_data[i] = p_v[i];
			}
		}

		template<typename... Args, typename = typename std::enable_if<sizeof...(Args) == N>::type>
		explicit Vector(Args &&... p_values) : m_data{static_cast<T>(std::forward<Args>(p_values))...} {}

		static constexpr Vector zero() {
			return Vector(0);
		}

		static constexpr Vector one() {
			return Vector(1);
		}

		static constexpr Vector right() {
			Vector res(0);
			if (N > 0) res[0] = 1.0;
			return res;
		}

		static constexpr Vector up() {
			Vector res(0);
			if (N > 1) res[1] = 1.0;
			return res;
		}

		static constexpr Vector forward() {
			Vector res(0);
			if (N > 2) res[2] = 1.0;
			return res;
		}

		static constexpr Vector w_only() {
			Vector res(0);
			if (N > 3) res[3] = 1.0;
			return res;
		}

		constexpr std::size_t size() { return N; }

		T &operator[](std::size_t p_n) { return m_data[p_n]; }

		const T &operator[](std::size_t p_n) const { return m_data[p_n]; }

		Vector &operator=(const Vector &p_rhs) {
			if (this == &p_rhs) return *this;
			for (std::size_t i = 0; i < N; ++i) {
				m_data[i] = p_rhs[i];
			}
			return *this;
		}

		inline const T *get_value_ptr() const {
			return m_data.data();
		}

		constexpr T x() const {
			static_assert(N > 0, "Cannot call x on an lower than 1d vector");
			return m_data[0];
		}

		constexpr void x(T p_x) {
			static_assert(N > 0, "Cannot call x on an lower than 1d vector");
			m_data[0] = p_x;
		}

		constexpr T y() const {
			static_assert(N > 1, "Cannot call y on an lower than 2d vector");
			return m_data[1];
		}

		constexpr void y(T p_y) {
			static_assert(N > 1, "Cannot call y on an lower than 2d vector");
			m_data[1] = p_y;
		}

		constexpr T z() const {
			static_assert(N > 2, "Cannot call z on an lower than 3d vector");
			return m_data[2];
		}

		constexpr void z(T p_z) {
			static_assert(N > 2, "Cannot call z on an lower than 3d vector");
			m_data[2] = p_z;
		}

		constexpr T w() const {
			static_assert(N > 3, "Cannot call w on an lower than 4d vector");
			return m_data[3];
		}

		constexpr void w(T p_w) {
			static_assert(N > 3, "Cannot call w on an lower than 4d vector");
			m_data[3] = p_w;
		}

	protected:

		//Member-wise
		template<typename Functor>
		inline Vector un_op(Functor &&p_op) const {
			Vector output;
			for (std::size_t i = 0; i < N; ++i) {
				output[i] = p_op((*this)[i]);
			}
			return output;
		}

		template<typename Functor>
		inline Vector bin_op(const Vector &p_v, Functor &&p_op) const {
			Vector output;
			for (std::size_t i = 0; i < N; ++i) {
				output[i] = p_op((*this)[i], p_v[i]);
			}
			return output;
		}

		//Member/scalar
		template<typename Functor>
		inline Vector bin_op(const T &p_s, Functor &&p_op) const {
			Vector output;
			for (std::size_t i = 0; i < N; ++i) {
				output[i] = p_op((*this)[i], p_s);
			}
			return output;
		}


	public:
		inline Vector operator+(const Vector &p_v) const {
			return bin_op(p_v, std::plus<T>());
		}

		inline Vector operator-(const Vector &p_v) const {
			return bin_op(p_v, std::minus<T>());
		}

		inline Vector operator*(const Vector &p_v) const {
			return bin_op(p_v, std::multiplies<T>());
		}

		inline Vector operator/(const Vector &p_v) const {
			return bin_op(p_v, std::divides<T>());
		}

		inline Vector operator%(const Vector &p_v) const {
			return bin_op(p_v, std::modulus<T>());
		}

		inline Vector operator-() const {
			return un_op(std::negate<T>());
		}

		inline Vector operator*(const T &p_s) const {
			return bin_op(p_s, std::multiplies<T>());
		}

		inline Vector operator/(const T &p_s) const {
			return bin_op(p_s, std::divides<T>());
		}

		inline Vector operator%(const T &p_s) const {
			return bin_op(p_s, std::modulus<T>());
		}

		friend inline Vector operator*(const T &p_s, const Vector &p_v) {
			return p_v * p_s;
		}

		inline bool operator==(const Vector &p_v) const {
			for (std::size_t i = 0; i < N; ++i) {
				if (!approx_eq<T>(m_data[i], p_v[i])) {
					return false;
				}
			}
			return true;
		}

		inline bool operator!=(const Vector &p_v) {
			return !((*this) == p_v);
		}

		inline void operator+=(const Vector &p_v) {
			for (std::size_t i = 0; i < N; ++i) {
				m_data[i] += p_v[i];
			}
		}

		inline void operator-=(const Vector &p_v) {
			for (std::size_t i = 0; i < N; ++i) {
				m_data[i] -= p_v[i];
			}
		}

		inline void operator*=(const Vector &p_v) {
			for (std::size_t i = 0; i < N; ++i) {
				m_data[i] *= p_v[i];
			}
		}

		inline void operator/=(const Vector &p_v) {
			for (std::size_t i = 0; i < N; ++i) {
				m_data[i] /= p_v[i];
			}
		}

		inline void operator+=(const T &p_s) {
			for (std::size_t i = 0; i < N; ++i) {
				m_data[i] += p_s;
			}
		}

		inline void operator-=(const T &p_s) {
			for (std::size_t i = 0; i < N; ++i) {
				m_data[i] -= p_s;
			}
		}

		inline void operator*=(const T &p_s) {
			for (std::size_t i = 0; i < N; ++i) {
				m_data[i] *= p_s;
			}
		}

		inline void operator/=(const T &p_s) {
			for (std::size_t i = 0; i < N; ++i) {
				m_data[i] /= p_s;
			}
		}

		friend std::ostream &operator<<(std::ostream &p_os, const Vector &p_v) {
			p_os << std::fixed << std::setprecision(3) << "(" << p_v[0];
			for (std::size_t i = 1; i < N; ++i) {
				p_os << "," << p_v[i];
			}
			p_os << ")";
			return p_os;
		}

		static inline T dot(const Vector &p_v1, const Vector &p_v2) {
			T result = 0;
			for (std::size_t i = 0; i < N; ++i) {
				result += p_v1[i] * p_v2[i];
			}
			return result;
		}


		inline T dot(const Vector &p_v) const {
			return Vector::dot((*this), p_v);
		}

		inline T length_squared() const {
			return Vector::dot((*this), (*this));
		}

		inline T length() const {
			return std::sqrt(length_squared());
		}

		static inline T distance(const Vector &p_v1, const Vector &p_v2) {
			return (p_v1 - p_v2).length();
		}

		static inline T angle(const Vector &p_v1, const Vector &p_v2){
			T cos_theta = dot(p_v1, p_v2)/(p_v1.length() * p_v2.length());
			return std::acos(cos_theta);
		}

		inline bool is_unit() const {
			return approx_eq<T>(length_squared(), 1.0);
		}


		inline Vector normalized() const {
			T l = length();
			if (approx_eq<T>(l, 0.0)) return zero();
			return (*this) / length();
		}


		inline void normalize() {
			T l = length();
			if (approx_eq<T>(l, 0.0)) {
				for (size_t i = 0; i < N; ++i) {
					m_data[i] = 0.0;
				}
			}
			(*this) /= l;
		}

		static inline Vector reflect(const Vector &p_incident, const Vector &p_normal) {
			mach_assert(p_normal.is_unit(), "The reflect function only works with normalized normal vectors");
			return p_incident - static_cast<T>(2.0) * dot(p_incident, p_normal) * p_normal;
		}

		static inline Vector refract(const Vector &p_incident, const Vector &p_normal, const T p_eta) {
			mach_assert(p_normal.is_unit(), "The refraction function only works with normalized normal vectors");
			T n_dot_i = dot(p_normal, p_incident);
			T k = 1.0 - p_eta * p_eta * (1.0 - n_dot_i * n_dot_i);
			if (k < 0.0) {
				return zero();
			} else {
				return p_eta * p_incident - (p_eta * n_dot_i + std::sqrt(k)) * p_normal;
			}
		}

		static inline Vector mix(const Vector &p_start, const Vector &p_end, T p_t) {
			return p_start + (p_end - p_start) * p_t;
		}

		static inline Vector mix(const Vector &p_start, const Vector &p_end, const Vector &p_t) {
			return p_start + (p_end - p_start) * p_t;
		}

		static inline Vector clamp(const Vector &p_v, const T p_low, const T p_high) {
			Vector result;
			for (std::size_t i = 0; i < N; ++i) {
				result[i] = std::clamp(p_v[i], p_low, p_high);
			}
			return result;
		}

		static inline Vector clamp(const Vector &p_v, const Vector &p_low, const Vector &p_high) {
			Vector result;
			for (std::size_t i = 0; i < N; ++i) {
				result[i] = std::clamp(p_v[i], p_low[i], p_high[i]);
			}
			return result;
		}

		inline T get_largest() const {
			T largest = m_data[0];
			for (std::size_t i = 1; i < N; ++i) {
				if (m_data[i] > largest) largest = m_data[i];
			}
			return largest;
		}

		inline T get_smallest() const {
			T smallest = m_data[0];
			for (std::size_t i = 1; i < N; ++i) {
				if (m_data[i] < smallest) smallest = m_data[i];
			}
			return smallest;
		}

		inline std::size_t get_largest_index() const {
			std::size_t largest = 0;
			for (std::size_t i = 1; i < N; ++i) {
				if (m_data[i] > largest) largest = i;
			}
			return largest;
		}

		inline std::size_t get_smallest_index() const {
			std::size_t smallest = 0;
			for (std::size_t i = 1; i < N; ++i) {
				if (m_data[i] < smallest) smallest = i;
			}
			return smallest;
		}

		static inline Vector cross(const Vector &p_v1, const Vector &p_v2) {
			static_assert(N == 3, "Cross product is only defined for 3d vectors");
			Vector output(
					p_v1.y() * p_v2.z() - p_v1.z() * p_v2.y(),
					p_v1.z() * p_v2.x() - p_v1.x() * p_v2.z(),
					p_v1.x() * p_v2.y() - p_v1.y() * p_v2.x()
			);
			return output;
		}

		inline Vector cross(const Vector &p_v) const {
			return cross(*this, p_v);
		}
	};
}
#endif //MACH_VECTOR_HPP
