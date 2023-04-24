#include "helpers.h"
#include "item.h"
#include "to_string.h"

#include <iostream>
#include <memory>
#include <regex>
#include <string>

const auto valid_regex = std::regex("^(0|-?[1-9][0-9]*|[A-Za-z][0-9A-Z_a-z]*)$");

bool is_valid_string(std::string const& str) {
	return std::regex_match(str, valid_regex);
}

int compare_digits(std::string const& str, std::string const& oth);

bool insert_before(std::string const& val, Item<std::string> const& item) {
	const int digit_cmp = compare_digits(val, item.value);
	return digit_cmp < 0 ? true : (digit_cmp > 0 ? false : val <= item.value);
}

bool value_equal(Item<std::string> const& item, std::string const& val) {
	return item.value == val;
}

int main() {
	std::unique_ptr<Item<std::string>> start;

	bool begin = true;
	std::string input;

	while (true) {
		if (!begin) {
			std::cout << '\n';
		}
		else {
			begin = false;
		}
		
		std::cout << "Awaiting input...\n";
		std::getline(std::cin, input);

		if (input.length() == 0) {
			std::cout << "\nProgram terminated!\n";
			remove_all(start);
			return 0;
		}
		else if (input.at(0) == '~') {
			if (input.length() == 1) {
				std::cout << "\nDeleting list...\n";
				remove_all(start);
			}
			else {
				input = input.substr(1);
				if (is_valid_string(input)) {
					std::cout << "\nRemoving item...\n";
					remove_item<std::string>(&start, input, &value_equal);
				}
				else {
					std::cout << "\nCould not parse input!\n";
				}
			}
		}
		else if (input == "l") {
			std::cout << "\nLoop print...\n";
			print_loop(start.get());
		}
		else if (input == "i") {
			std::cout << "\nIterator print...\n";
			print_iterator(start.get());
		}
		else if (input == "a") {
			std::cout << "\nArray print...\n";
			print_array(start.get());
		}
		else if (input == "r") {
			std::cout << "\nRecursive print...\n";
			print_recursive(start.get());
		}
		else if (input == "f") {
			std::cout << "\nFold print...\n";
			print_fold(start.get());
		}
		else if (input == "b") {
			std::cout << "\nFoldback print...\n";
			print_foldback(start.get());
		}
		else if (is_valid_string(input)) {
			std::cout << "\nInserting item...\n";
			insert_item<std::string>(&start, input, &insert_before);
		}
		else {
			std::cout << "\nCould not parse input!\n";
		}
	}
}

int compare_digits(std::string const& str, std::string const& oth) {
	const size_t str_len = str.length(), oth_len = oth.length();
	if (str_len == 0 || oth_len == 0) {
		return -1;
	}

	char str_char = str.at(0), oth_char = oth.at(0);
	const bool str_minus = str_char == '-', oth_minus = oth_char == '-';
	if (str_minus ^ oth_minus) {
		return str_minus ? -1 : 1;
	}
	else if (!str_minus && (!isdigit(str_char) || !isdigit(oth_char))) {
		return 0;
	}
	else if (str_len > oth_len) {
		return str_minus ? -1 : 1;
	}
	else if (str_len < oth_len) {
		return str_minus ? 1 : -1;
	}

	for (size_t i = str_minus ? 1 : 0; i < str_len; ++i) {
		str_char = str.at(i);
		oth_char = oth.at(i);
		if (str_char > oth_char) {
			return str_minus ? -1 : 1;
		}
		else if (str_char < oth_char) {
			return str_minus ? 1 : -1;
		}
	}

	return -1;
}
