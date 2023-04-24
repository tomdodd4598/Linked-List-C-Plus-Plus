#pragma once

#include "iterator.h"
#include "to_string.h"

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

template <typename T>
struct Item {

	T value;
	std::unique_ptr<Item<T>> next;

	template <typename U>
	Item<T>(U&& val, std::unique_ptr<Item<T>>&& nxt) : value{ std::forward<U>(val) }, next{ std::move(nxt) } {
		std::cout << "Creating item: " << to_string(value) << '\n';
	}

	~Item<T>() {
		std::cout << "Deleting item: " << to_string(value) << '\n';
	}

	Item<T>(Item<T> const& oth) = delete;

	Item<T>(Item<T>&& oth) noexcept : value{ std::move(oth.value) }, next{ std::move(oth.next) } {}

	Item<T>& operator=(Item<T> const& oth) = delete;

	Item<T>& operator=(Item<T>&& oth) noexcept {
		if (this != &oth) {
			value = std::move(oth.value);
			next = std::move(oth.next);
		}
		return *this;
	}

	Item<T> const& operator[](const int n) const {
		auto item = this;
		for (int i = 0; i < n; ++i) {
			item = item->next.get();
		}
		return *item;
	}

	Item<T>* print_get_next() const {
		std::cout << to_string(value) << (next != nullptr ? ", " : "\n");
		return next.get();
	}

	struct ItemIterator : Iterator<Item<T>> {

		ItemIterator(Item<T> const* start) : Iterator<Item<T>>(start) {}

		Iterator<Item<T>>& operator++() override {
			this->item = this->item->next.get();
			return *this;
		}

		Iterator<Item<T>>& operator--() override {
			throw std::runtime_error("Decrement is unsupported!");
		}
	};

	ItemIterator begin() const {
		return ItemIterator(this);
	}

	ItemIterator end() const {
		return ItemIterator(nullptr);
	}
};

template <typename T, typename A, typename R>
R item_fold(const std::function<A(Item<T> const*, Item<T> const*, A&&)> f_some, const std::function<R(Item<T> const*, A&&)> f_last, const std::function<R(A&&)> f_empty, A&& accumulator, Item<T> const* item) {
	if (item != nullptr) {
		const auto next = item->next.get();
		if (next != nullptr) {
			return item_fold(f_some, f_last, f_empty, f_some(item, next, std::forward<A>(accumulator)), next);
		}
		else {
			return f_last(item, std::forward<A>(accumulator));
		}
	}
	else {
		return f_empty(std::forward<A>(accumulator));
	}
}

template <typename T, typename A, typename R>
R item_foldback(const std::function<A(Item<T> const*, Item<T> const*, A&&)> f_some, const std::function<A(Item<T> const*)> f_last, const std::function<A()> f_empty, const std::function<R(A&&)> generator, Item<T> const* item) {
	if (item != nullptr) {
		const auto next = item->next.get();
		if (next != nullptr) {
			return item_foldback(f_some, f_last, f_empty, static_cast<std::function<R(A&&)>>([=](A&& inner_val) { return generator(f_some(item, next, std::forward<A>(inner_val))); }), next);
		}
		else {
			return generator(f_last(item));
		}
	}
	else {
		return generator(f_empty());
	}
}
