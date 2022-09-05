#pragma once

#include <iostream>

template <typename T>
struct Iterator {

	T const* item;

	Iterator<T>(T const* start) : item{ start } {}

	bool operator==(Iterator<T> const& oth) const {
		return item == oth.item;
	}

	bool operator==(Iterator<T> const&& oth) const noexcept {
		return item == oth.item;
	}

	bool operator!=(Iterator<T> const& oth) const {
		return !(*this == oth);
	}

	bool operator!=(Iterator<T> const&& oth) const noexcept {
		return !(*this == oth);
	}

	virtual Iterator<T>& operator++() = 0;

	Iterator<T> operator++(int) {
		const auto iterator = *this;
		++(*this);
		return iterator;
	}

	virtual Iterator<T>& operator--() = 0;

	Iterator<T> operator--(int) {
		const auto iterator = *this;
		--(*this);
		return iterator;
	}

	T const* operator*() const {
		return item;
	}
};
