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

		bool isArray() const {
			return std::holds_alternative<Array>(*this);
		}

		Array& asArray() {

			if (!isArray()) {
				throw std::logic_error("Node is not an array!");
			}

			return std::get<Array>(*this);
		}

		const Array& asArray() const {

			if (!isArray()) {
				throw std::logic_error("Node is not an array!");
			}

			return std::get<Array>(*this);
		}

		bool isDict() const {
			return std::holds_alternative<Dict>(*this);
		}

		Dict asDict() {

			if (!isDict) {
				throw std::logic_error("Node is not a dict!");
			}

			return std::get<Dict>(*this);
		}

		const Dict& asDict() const {

			if (!isDict) {
				throw std::logic_error("Node is not a dict!");
			}

			return std::get<Dict>(*this);
		}

		bool isString() const {
			return std::holds_alternative<std::wstring>(*this);
		}

		const std::wstring& asString() const {

			if (!isString()) {
				throw std::logic_error("Node is not a string!");
			}

			return std::get<std::wstring>(*this);
		}

		bool isInt() const {
			return std::holds_alternative<int>(*this);
		}

		int asInt() const {

			if (!isInt()) {
				throw std::logic_error("Node is not an integer!");
			}

			return std::get<int>(*this);
		}



	};

	Node loadNode(std::wifstream& input);
	Node loadArray(std::wifstream& input);
	Node loadDict(std::wifstream& input);
	Node loadString(std::wifstream& input);
	Node loadInt(std::wifstream& input);
}