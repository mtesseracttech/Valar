//
// Created by mtesseract on 6/9/19.
//


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnested-anon-types"
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#ifndef MACH_QUATERNION_HPP
#define MACH_QUATERNION_HPP

#include <auxiliary/exceptions/NotImplemented.hpp>
#include "Rotations.hpp"

namespace mach {
	template<typename T>
	class Quaternion {
	public:
		union {
			struct {
				T w;
				union {
					struct {
						T x, y, z;
					};
					Vector<T, 3> v;
				};
			};
			std::array<T, 4> m_array;
		};

		Quaternion(T p_w, T p_x, T p_y, T p_z) : m_array{p_w, p_x, p_y, p_z} {}

		Quaternion(T p_w, Vector<T, 3> p_v) : w(p_w), v(p_v) {}

		Quaternion(const Quaternion &p_q) : w(p_q.w), v(p_q.v) {}

		Quaternion &operator=(const Quaternion &p_rhs) {
			if (this == &p_rhs) return *this;
			w = p_rhs.w;
			v = p_rhs.v;
			return *this;
		}

		static constexpr Quaternion identity() {
			return Quaternion(1.0, 0.0, 0.0, 0.0);
		}

		inline Quaternion operator*(const Quaternion &p_q) const {
			return Quaternion(
					w * p_q.w - v.dot(p_q.v),
					w * p_q.v + p_q.w * v + (v.cross(p_q.v))
			);
		}

		inline Quaternion operator*(const T &p_s) const {
			return Quaternion(w * p_s, v * p_s);
		}

		inline Quaternion operator+(const Quaternion &p_other) const {
			return Quaternion(w + p_other.w, v + p_other.v);
		}

		inline Quaternion operator-(const Quaternion &p_other) const {
			return Quaternion(w - p_other.w, v - p_other.v);
		}

		inline Quaternion operator-() const {
			return Quaternion(-w, -v);
		}

		friend inline Quaternion operator*(T &p_s, const Quaternion &p_q) {
			return p_q * p_s;
		}

		friend inline Vector<T, 3> operator*(const Vector<T, 3> &p_v, const Quaternion<T> &p_q) {
			Quaternion<T> in(0.0, p_v);
			Quaternion result = p_q * in * p_q.inverse();
			return result.v;
		}

		/*
		 * Compares two quaternions for equality.
		 * Leverages pre-existing code that exists for vectors.
		 */
		inline bool operator==(const Quaternion &p_q) const {
			return (*reinterpret_cast<const Vector<T, 4> *>(this)) == (*reinterpret_cast<const Vector<T, 4> *>(&p_q));
		}

		inline bool operator!=(const Quaternion &p_q) const {
			return !((*this) == p_q);
		}

		friend std::ostream &operator<<(std::ostream &p_os, const Quaternion &p_q) {
			p_os << std::fixed << std::setprecision(3) << "(" << p_q.w << p_q.v << ")";
			return p_os;
		}


		static inline T dot(const Quaternion &p_a, const Quaternion &p_b) {
			return p_a.w * p_b.w + Vector<T, 3>::dot(p_a.v, p_b.v);
		}

		inline T dot(const Quaternion &p_q) const {
			return dot(*this, p_q);
		}

		inline T magnitude_squared() const {
			return dot(*this);
		}

		inline T magnitude() const {
			return std::sqrt(magnitude_squared());
		}

		inline Quaternion conjugate() const {
			return Quaternion(w, -v);
		}

		inline Quaternion inverse() const {
			T inv = 1.0 / magnitude();
			return conjugate() * inv;
		}

		inline Quaternion normalized() const {
			T inv = 1.0 / magnitude();
			return Quaternion(w * inv, v * inv);
		}

		inline void normalize() {
			T inv = 1.0 / magnitude();
			w = w * inv;
			v = v * inv;
		}

		inline bool is_unit() {
			return approx_eq<T>(magnitude_squared(), 1.0);
		}


		inline Quaternion pow(T p_exp) {
			if (std::abs(w) < 0.9999) {
				T alpha = std::acos(w);
				T new_alpha = alpha * p_exp;
				T scalar = std::sin(new_alpha) / std::sin(alpha);
				return Quaternion(std::cos(new_alpha), v * scalar);
			} else {
				return Quaternion(x, y, z, w);
			}
		}

		inline Quaternion exp() {
			T v_l = v.length();
			if (!approx_eq(v_l, 0.0)) {
				return std::exp(w) * Quaternion(std::cos(w), v / v.length() * sin(v.length()));
			} else {
				return Quaternion::identity();
			}
		}

		inline Quaternion log() {
			T v_l = v.length();
			T q_l = magnitude();
			if (!approx_eq(v_l, 0.0) && !approx_eq(q_l, 0.0)) {
				return Quaternion(std::log(magnitude()), (v / v_l) * std::acos(w / q_l));
			} else {
				return Quaternion::identity();
			}
		}

		static inline Quaternion lerp(const Quaternion &p_start, const Quaternion &p_end, T p_t) {
			if (dot(p_start, p_end) < 0.0) {
				return p_start + p_t * (-p_end - p_start);
			} else {
				return p_start + p_t * (p_end - p_start);
			}
		}

		static inline Quaternion lerp_norm(const Quaternion &p_start, const Quaternion &p_end, T p_t) {
			return lerp(p_start, p_end, p_t).normalized();
		}

		static inline Quaternion slerp(const Quaternion &p_start, const Quaternion &p_end, T p_t) {
			Quaternion end = p_end;
			double dot = p_start.dot(p_end);
			if (dot < 0.0f) {
				end = -end;
				dot = -dot;
			}

			if (dot > 0.9995) return lerp(p_start, end, p_t);

			T theta_0 = std::acos(dot);
			T theta = theta_0 * p_t;
			T sin_theta = std::sin(theta);
			T sin_theta_0 = std::sin(theta_0);

			T s_0 = std::cos(theta) - dot * sin_theta / sin_theta_0;
			T s_1 = sin_theta / sin_theta_0;

			return (s_0 * p_start) + (s_1 * end);
		}

		inline AngleAxis<T> to_angle_axis() {
			if (w > 1.0) normalize();

			AngleAxis<T> angle_axis;
			angle_axis.theta = 2.0 * std::acos(w);
			angle_axis.x = x;
			angle_axis.y = y;
			angle_axis.z = z;

			T s = std::sqrt(1.0 - w * w);
			if (s >= 0.001) {
				angle_axis.n / s;
			}

			return angle_axis;
		}

		//Angle Axis
		inline static Quaternion from_angle_axis(T p_theta, const Vector<T, 3> &p_n) {
			Vector<T, 3> n = p_n.normalized();
			//mach_assert(p_n.is_unit(), "You can only construct a quaternion from an angle-axis with a normalized axis");
			T st = std::sin(p_theta / 2.0);
			T ct = std::cos(p_theta / 2.0);
			return Quaternion(ct, n * st);
		}

		//Angle Axis
		inline static Quaternion from_angle_axis(const AngleAxis<T> &p_angle_axis) {
			return from_angle_axis(p_angle_axis.theta, p_angle_axis.n);
		}

		//Euler Angles
		template<RotationOrder O = BPH>
		inline static Quaternion from_euler(const EulerAngles<T, O> &p_angles) {
			T half_pitch = p_angles.pitch / 2.0;
			T half_heading = p_angles.heading / 2.0;
			T half_bank = p_angles.bank / 2.0;

			T sp = std::sin(half_pitch);
			T cp = std::cos(half_pitch);
			T sh = std::sin(half_heading);
			T ch = std::cos(half_heading);
			T sb = std::sin(half_bank);
			T cb = std::cos(half_bank);

			Quaternion p = Quaternion(cp, sp, 0.0, 0.0);
			Quaternion h = Quaternion(ch, 0.0, sh, 0.0);
			Quaternion b = Quaternion(cb, 0.0, 0.0, sb);

			return combine_rotations<O>(p, h, b);
		}

		//From matrix
		inline static Quaternion from_matrix(const Matrix<T, 3, 3> &p_m) {
			T four_w_squared_minus_1 = p_m[0][0] + p_m[1][1] + p_m[2][2];
			T four_x_squared_minus_1 = p_m[0][0] - p_m[1][1] + p_m[2][2];
			T four_y_squared_minus_1 = p_m[1][1] - p_m[0][0] - p_m[2][2];
			T four_z_squared_minus_1 = p_m[2][2] - p_m[0][0] - p_m[1][1];

			size_t biggest_index = 0;
			T four_biggest_squared_minus_1 = four_w_squared_minus_1;
			if (four_x_squared_minus_1 > four_biggest_squared_minus_1) {
				four_biggest_squared_minus_1 = four_x_squared_minus_1;
				biggest_index = 1;
			}
			if (four_y_squared_minus_1 > four_biggest_squared_minus_1) {
				four_biggest_squared_minus_1 = four_y_squared_minus_1;
				biggest_index = 2;
			}
			if (four_z_squared_minus_1 > four_biggest_squared_minus_1) {
				four_biggest_squared_minus_1 = four_z_squared_minus_1;
				biggest_index = 3;
			}

			T biggest_val = std::sqrt(four_biggest_squared_minus_1 + 1.0) * 0.5;
			T f = 0.25 / biggest_val;

			switch (biggest_index) {
				case 0:
					return Quaternion(
							biggest_val,
							(p_m[1][2] - p_m[2][1]) * f,
							(p_m[2][0] - p_m[0][2]) * f,
							(p_m[0][1] - p_m[1][0]) * f
					);
				case 1:
					return Quaternion(
							(p_m[1][2] - p_m[2][1]) * f,
							biggest_val,
							(p_m[0][1] + p_m[1][0]) * f,
							(p_m[2][0] + p_m[0][2]) * f
					);
				case 2:
					return Quaternion(
							(p_m[2][0] - p_m[0][2]) * f,
							(p_m[0][1] + p_m[1][0]) * f,
							biggest_val,
							(p_m[1][2] + p_m[2][1]) * f
					);
				case 3:
					return Quaternion(
							(p_m[0][1] - p_m[1][0]) * f,
							(p_m[2][0] + p_m[0][2]) * f,
							(p_m[1][2] + p_m[2][1]) * f,
							biggest_val
					);
			}
			return Quaternion::identity();
		}


		//Work in progress...
//        static inline Quaternion fast_slerp(const Quaternion &q0, const Quaternion &q1, T t) {
//            constexpr T mu = 1.85298109240830;
//            constexpr T u[8] = {
//                    1.0 / (1 * 3), 1.0 / (2 * 5), 1.0 / (3 * 7), 1.0 / (4 * 9),
//                    1.0 / (5 * 11), 1.0 / (6 * 13), 1.0 / (7 * 15), mu / (8 * 17)
//            };
//            constexpr T v[8] = {
//                    1.0 / 3, 2.0 / 5, 3.0 / 7, 4.0 / 9,
//                    5.0 / 11, 6.0 / 13, 7.0 / 15, mu * 8.0 / 17
//            };
//
//            T xm1 = dot(q0, q1);
//            T d = 1.0 - t;
//            T sqrT = t * t;
//            T sqrD = d * d;
//
//            T bT[8];
//            T bD[8];
//
//            for (int i = 7; i >= 0; --i) {
//                bT[i] = (u[i] * sqrT - v[i]) * xm1;
//                bD[i] = (u[i] * sqrD - v[i]) * xm1;
//            }
//
//            T f0 = t * (
//                    1 + bT[0] * (1 + bT[1] * (1 + bT[2] * (1 + bT[3] * (
//                            1 + bT[4] * (1 + bT[5] * (1 + bT[6] * (1 + bT[7]))))))));
//
//            T f1 = t * (
//                    1 + bD[0] * (1 + bD[1] * (1 + bD[2] * (1 + bD[3] * (
//                            1 + bD[4] * (1 + bD[5] * (1 + bD[6] * (1 + bD[7]))))))));
//
//            //std::cout << f0 << " and " << f1 << std::endl;
//            Quaternion slerp = f0 * q0 + f1 * q1;
//            return slerp;
//        }
	};
}

#endif //MACH_QUATERNION_HPP
#pragma clang diagnostic pop