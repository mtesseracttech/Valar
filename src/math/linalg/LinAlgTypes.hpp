//
// Created by mtesseract on 6/4/19.
//

#ifndef MACH_LINALGTYPES_HPP
#define MACH_LINALGTYPES_HPP


#include "math/linalg/Matrix/Matrix.hpp"
#include "Quaternion.hpp"

namespace mach {
	//Common Vector Templates
	template<typename T>
	using Vector2 = Vector<T, 2>;

	template<typename T>
	using Vector3 = Vector<T, 3>;

	template<typename T>
	using Vector4 = Vector<T, 4>;

	//Standard Precision
	typedef Vector2<float> Vec2;
	typedef Vector3<float> Vec3;
	typedef Vector4<float> Vec4;

	//High Precision
	typedef Vector2<double> Vec2h;
	typedef Vector3<double> Vec3h;
	typedef Vector4<double> Vec4h;

	//Signed Integer Precision
	typedef Vector2<int> IVec2;
	typedef Vector3<int> IVec3;
	typedef Vector4<int> IVec4;

	//Unsigned Integer Precision
	typedef Vector2<unsigned int> UVec2;
	typedef Vector3<unsigned int> UVec3;
	typedef Vector4<unsigned int> UVec4;

	//Common Square Matrix Templates
	template<typename T>
	using Matrix2 = Matrix<T, 2, 2>;

	template<typename T>
	using Matrix3 = Matrix<T, 3, 3>;

	template<typename T>
	using Matrix4 = Matrix<T, 4, 4>;

	//Standard Precision, short form
	typedef Matrix2<float> Mat2;
	typedef Matrix3<float> Mat3;
	typedef Matrix4<float> Mat4;

	//High Precision, short form
	typedef Matrix2<double> Mat2h;
	typedef Matrix3<double> Mat3h;
	typedef Matrix4<double> Mat4h;


	//Common Matrix Templates
	template<typename T>
	using Matrix2x2 = Matrix<T, 2, 2>;

	template<typename T>
	using Matrix2x3 = Matrix<T, 2, 3>;

	template<typename T>
	using Matrix2x4 = Matrix<T, 2, 4>;

	template<typename T>
	using Matrix3x2 = Matrix<T, 3, 2>;

	template<typename T>
	using Matrix3x3 = Matrix<T, 3, 3>;

	template<typename T>
	using Matrix3x4 = Matrix<T, 3, 4>;

	template<typename T>
	using Matrix4x2 = Matrix<T, 4, 2>;

	template<typename T>
	using Matrix4x3 = Matrix<T, 4, 3>;

	template<typename T>
	using Matrix4x4 = Matrix<T, 4, 4>;


	//Standard Precision, full form
	typedef Matrix2x2<float> Mat2x2;
	typedef Matrix2x3<float> Mat2x3;
	typedef Matrix2x4<float> Mat2x4;
	typedef Matrix3x2<float> Mat3x2;
	typedef Matrix3x3<float> Mat3x3;
	typedef Matrix3x4<float> Mat3x4;
	typedef Matrix4x2<float> Mat4x2;
	typedef Matrix4x3<float> Mat4x3;
	typedef Matrix4x4<float> Mat4x4;

	//High Precision, full form
	typedef Matrix2x2<double> Mat2x2h;
	typedef Matrix2x3<double> Mat2x3h;
	typedef Matrix2x4<double> Mat2x4h;
	typedef Matrix3x2<double> Mat3x2h;
	typedef Matrix3x3<double> Mat3x3h;
	typedef Matrix3x4<double> Mat3x4h;
	typedef Matrix4x2<double> Mat4x2h;
	typedef Matrix4x3<double> Mat4x3h;
	typedef Matrix4x4<double> Mat4x4h;

	//Quaternions
	typedef Quaternion<float> Quat;
	typedef Quaternion<double> Quath;
}

#endif //MACH_LINALGTYPES_HPP
