#pragma once

#include "item.h"

#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

template <typename T, typename U>
void insert_item(std::unique_ptr<Item<T>>* start, U&& val, const std::function<bool(T const&, Item<T> const&)> insert_before) {
	while (*start && !insert_before(val, **start)) {
		start = &(*start)->next;
	}
	*start = std::make_unique<Item<T>>(std::forward<U>(val), std::move(*start));
}

template <typename T, typename U>
void remove_item(std::unique_ptr<Item<T>>* start, U&& val, const std::function<bool(Item<T> const&, T const&)> value_equal) {
	while (*start && !value_equal(**start, val)) {
		start = &(*start)->next;
	}

	if (*start) {
		*start = std::move((*start)->next);
	}
	else {
		std::cout << "Item " << val << " does not exist!\n";
	}
}

template <typename T>
void remove_all(std::unique_ptr<Item<T>>& start) {
	start.reset();
}

template <typename T>
void print_loop(Item<T> const* start) {
	while (start != nullptr) {
		start = start->print_get_next();
	}
}

template <typename T>
void print_iterator(Item<T> const* start) {
	if (start != nullptr) {
		for (auto item : *start) {
			item->print_get_next();
		}
	}
}

template <typename T>
void print_array(Item<T> const* start) {
	auto item = start;
	for (int i = 0; item != nullptr; ++i) {
		item = (*start)[i].print_get_next();
	}
}

template <typename T>
void print_recursive(Item<T> const* start) {
	if (start != nullptr) {
		print_recursive(start->print_get_next());
	}
}

template <typename T>
void print_fold(Item<T> const* start) {
	const auto f_some = [](Item<T> const* current, Item<T> const* next, std::string&& accumulator) {
		std::ostringstream sstream;
		sstream << std::move(accumulator) << current->value_to_string() << ", ";
		return sstream.str();
	};
	const auto f_last = [](Item<T> const* current, std::string&& accumulator) {
		std::ostringstream sstream;
		sstream << std::move(accumulator) << current->value_to_string() << '\n';
		return sstream.str();
	};
	const auto f_empty = [](std::string&& accumulator) { return std::move(accumulator); };
	std::cout << item_fold<T, std::string, std::string>(f_some, f_last, f_empty, "", start);
}

template <typename T>
void print_foldback(Item<T> const* start) {
	const auto f_some = [](Item<T> const* current, Item<T> const* next, std::string&& inner_val) {
		std::ostringstream sstream;
		sstream << current->value_to_string() << ", " << std::move(inner_val);
		return sstream.str();
	};
	const auto f_last = [](Item<T> const* current) {
		std::ostringstream sstream;
		sstream << current->value_to_string() << '\n';
		return sstream.str();
	};
	const auto f_empty = []() { return ""; };
	std::cout << item_foldback<T, std::string, std::string>(f_some, f_last, f_empty, [](std::string&& inner_val) { return std::move(inner_val); }, start);
}
