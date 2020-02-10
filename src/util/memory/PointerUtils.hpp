//
// Created by MTesseracT on 10/02/2020.
//

#ifndef VALCANO_POINTERUTILS_HPP
#define VALCANO_POINTERUTILS_HPP

#include <memory>

namespace mt {
	template<typename T, typename U>
	inline bool equals(const std::weak_ptr<T> &p_t, const std::weak_ptr<U> &p_u) {
		return !p_t.owner_before(p_u) && !p_u.owner_before(p_t);
	}


	template<typename T, typename U>
	inline bool equals(const std::weak_ptr<T> &p_t, const std::shared_ptr<U> &p_u) {
		return !p_t.owner_before(p_u) && !p_u.owner_before(p_t);
	}

	template<class T, class U>
	std::weak_ptr<T> static_pointer_cast(std::weak_ptr<U> const &p_r) {
		return std::static_pointer_cast<T>(p_r.lock());
	}
}

#endif //VALCANO_POINTERUTILS_HPP
