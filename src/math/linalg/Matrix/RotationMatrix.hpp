//
// Created by mtesseract on 6/11/19.
//

#ifndef MACH_ROTATIONMATRIX_HPP
#define MACH_ROTATIONMATRIX_HPP

#include <math/linalg/Quaternion.hpp>
#include <auxiliary/exceptions/NotImplemented.hpp>
#include <math/linalg/LinAlgTypes.hpp>
#include "Matrix.hpp"


namespace mach {
	template<typename T>
	class RotationMatrix {
		Matrix3 <T> m_matrix;

		explicit RotationMatrix(Matrix3 <T> p_matrix) : m_matrix(p_matrix) {}

		template<typename... Args, typename = typename std::enable_if<sizeof...(Args) == 9>::type>
		explicit RotationMatrix(Args &&... p_values) : m_matrix(static_cast<T>(std::forward<Args>(p_values))...) {
		}

	public:

		RotationMatrix() : m_matrix(Matrix3<T>::identity()) {}

		static constexpr RotationMatrix identity() {
			return RotationMatrix(Matrix3<T>::identity());
		}

		friend std::ostream &operator<<(std::ostream &p_os, const RotationMatrix &p_m) {
			p_os << p_m.m_matrix;
			return p_os;
		}

		template<RotationOrder O = BPH>
		inline static RotationMatrix from_euler(const EulerAngles <T, O> &p_angles) {
			T sp = std::sin(p_angles.pitch);
			T cp = std::cos(p_angles.pitch);
			T sh = std::sin(p_angles.heading);
			T ch = std::cos(p_angles.heading);
			T sb = std::sin(p_angles.bank);
			T cb = std::cos(p_angles.bank);

			Matrix3<T> b(cb, sb, 0.0,
			             -sb, cb, 0.0,
			             0.0, 0.0, 1.0);
			Matrix3<T> h(ch, 0.0, -sh,
			             0.0, 1.0, 0.0,
			             sh, 0.0, ch);
			Matrix3<T> p(1.0, 0.0, 0.0,
			             0.0, cp, sp,
			             0.0, -sp, cp);

			RotationMatrix result;
			result.m_matrix = combine_rotations<O>(p, h, b);
			return result;
		}

		static inline RotationMatrix from_quat(const Quaternion <T> &p_q) {
			T xx = p_q.x * p_q.x;
			T yy = p_q.y * p_q.y;
			T zz = p_q.z * p_q.z;
			T xy = p_q.x * p_q.y;
			T xz = p_q.x * p_q.z;
			T xw = p_q.x * p_q.w;
			T yz = p_q.y * p_q.z;
			T yw = p_q.y * p_q.w;
			T zw = p_q.z * p_q.w;

			return RotationMatrix(1 - 2 * (yy + zz), 2 * (xy - zw), 2 * (xz + yw),
			                      2 * (xy + zw), 1 - 2 * (xx + zz), 2 * (yz - xw),
			                      2 * (xz - yw), 2 * (yz + xw), 1 - 2 * (xx + yy));
		}

		//Angle Axis
		static inline RotationMatrix from_angle_axis(T p_theta, const Vector3 <T> &p_n) {
			mach_assert(p_n.is_unit(),
			            "You can only construct a RotationMatrix from an angle-axis with a normalized axis");
			T c = std::cos(p_theta);
			T s = std::sin(p_theta);
			T o_m_c = 1.0 - c;
			return RotationMatrix(p_n.x * p_n.x * o_m_c + c,
			                      p_n.x * p_n.y * o_m_c + p_n.z * s,
			                      p_n.x * p_n.z * o_m_c - p_n.y * s,
			                      p_n.x * p_n.y * o_m_c - p_n.z * s,
			                      p_n.y * p_n.y * o_m_c + c,
			                      p_n.y * p_n.z * o_m_c + p_n.x * s,
			                      p_n.x * p_n.z * o_m_c + p_n.y * s,
			                      p_n.y * p_n.z * o_m_c - p_n.x * s,
			                      p_n.z * p_n.z * o_m_c + c
			);
		}

		//Angle Axis
		static inline RotationMatrix from_angle_axis(const AngleAxis <T> &p_angle_axis) {
			return from_angle_axis(p_angle_axis.theta, p_angle_axis.n);
		}


		inline RotationMatrix transpose() {
			RotationMatrix matrix(m_matrix.transpose());
			return matrix;
		}

		//Happens to be the same as the transpose for rotation matrices
		inline RotationMatrix inverse() {
			return transpose();
		}

		template<RotationOrder O = BPH>
		inline RotationMatrix rotate(const RotationMatrix &p_m) {
			m_matrix *= p_m.m_matrix;
		}

		inline RotationMatrix rotate(const RotationMatrix &p_m) {
			m_matrix *= p_m.m_matrix;
		}

		inline RotationMatrix rotate(const Quaternion <T> &p_q) {
			m_matrix *= RotationMatrix(p_q);
		}

		inline static RotationMatrix from_mat3(const Matrix3 <T> &p_matrix) {
			mach_assert(approx_eq<T>(p_matrix[0].dot(p_matrix[1]), 0.0) &&
			            approx_eq<T>(p_matrix[1].dot(p_matrix[2]), 0.0) &&
			            approx_eq<T>(p_matrix[2].dot(p_matrix[0]), 0.0),
			            "This is not a valid matrix that describes a rotation");
			if (!p_matrix[0].is_unit() &&
			    !p_matrix[1].is_unit() &&
			    !p_matrix[2].is_unit()) {
				return RotationMatrix(p_matrix.normalized());
			}
			return RotationMatrix(p_matrix);
		}

		inline Matrix4 <T> to_mat4() const {
			Matrix4<T> result(m_matrix[0][0], m_matrix[0][1], m_matrix[0][2], 0.0,
			                  m_matrix[1][0], m_matrix[1][1], m_matrix[1][2], 0.0,
			                  m_matrix[2][0], m_matrix[2][1], m_matrix[2][2], 0.0,
			                  0.0, 0.0, 0.0, 1.0);
			return result;
		}

		inline Matrix3 <T> to_mat3() const {
			return m_matrix;
		}

		inline Vector3 <T> right() {
			return m_matrix[0];
		}

		inline Vector3 <T> up() {
			return m_matrix[1];
		}

		inline Vector3 <T> forward() {
			return m_matrix[2];
		}

		inline Vector3 <T> left() {
			return -m_matrix[0];
		}

		inline Vector3 <T> down() {
			return -m_matrix[1];
		}

		inline Vector3 <T> back() {
			return -m_matrix[2];
		}
	};
}

#endif //MACH_ROTATIONMATRIX_HPP
