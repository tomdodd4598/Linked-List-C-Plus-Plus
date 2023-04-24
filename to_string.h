#pragma once

#include <string>

template <typename T>
std::string to_string(T&& val) {
	return val;
}
