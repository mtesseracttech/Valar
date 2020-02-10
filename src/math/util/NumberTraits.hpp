//
// Created by mtesseract on 6/9/19.
//

#ifndef MACH_NUMBERTRAITS_HPP
#define MACH_NUMBERTRAITS_HPP

#include <cmath>
#include <limits>

namespace mach {

	template<typename T>
	struct NumberTraits {
		static constexpr T default_epsilon = 0;
	};

	template<>
	struct NumberTraits<float> {
		static constexpr float default_epsilon = 0.000001;
	};

	template<>
	struct NumberTraits<double> {
		static constexpr double default_epsilon = 0.00000000001;
	};

	template<typename T>
	static bool fp_comp(T p_a, T p_b, T p_epsilon) {
		const float abs_a = std::abs(p_a);
		const float abs_b = std::abs(p_b);
		const float diff = std::abs(p_a - p_b);

		if (p_a == p_b) { // shortcut, handles infinities
			return true;
		} else if (p_a == 0 || p_b == 0 || diff < std::numeric_limits<T>::min()) {
			return diff < p_epsilon;
		} else { // use relative error, don't think anything else could use this case
			return diff / std::min((abs_a + abs_b), std::numeric_limits<T>::max()) < p_epsilon;
		}
	}

	template<typename T>
	static bool int_comp(T p_a, T p_b) {
		return p_a == p_b;
	}

	template<typename T>
	static bool approx_eq(T p_a, T p_b, T p_epsilon = NumberTraits<T>::default_epsilon) {
		if constexpr (std::is_floating_point_v<T>) {
			return fp_comp(p_a, p_b, p_epsilon);
		} else if constexpr (std::is_integral_v<T>) {
			return int_comp(p_a, p_b);
		} else {
			return std::abs(p_a - p_b) <= p_epsilon;
		}
	}
}

#endif //MACH_NUMBERTRAITS_HPP
