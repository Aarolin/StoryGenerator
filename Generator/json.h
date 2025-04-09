#pragma once

#include <cwctype>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <locale>
#include <clocale>
#include <unordered_map>
#include <vector>
#include <variant>

namespace json {
	class Node;
	using Dict = std::unordered_map<std::wstring, Node>;
	using Array = std::vector<Node>;

	class ParsingError : public std::runtime_error {
	public:
		using runtime_error::runtime_error;
	};

	class Node final : public std::variant<Array, Dict, int, std::wstring> {
	public:
		using variant::variant;
		using Value = variant;

		bool isString() const {
			return std::holds_alternative<std::wstring>(*this);
		}

		const std::wstring& AsString() const {

			if (!isString()) {
				throw std::logic_error("Node is not a string");
			}

			return std::get<std::wstring>(*this);
		}

		bool isInt() const {
			return std::holds_alternative<int>(*this);
		}

		int asInt() const {
			return std::get<int>(*this);
		}

	};

	Node loadNode(std::wifstream& input);
	Node loadArray(std::wifstream& input);
	Node loadDict(std::wifstream& input);
	Node loadString(std::wifstream& input);
	Node loadInt(std::wifstream& input);
}