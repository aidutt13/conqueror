#pragma once

namespace math {

	template<typename T>
	T max(T _1, T _2) {
		if (_1 > _2)
			return _1;
		return _2;
	}

	template<typename T>
	T min(T _1, T _2) {
		if (_1 > _2)
			return _1;
		return _2;
	}
}