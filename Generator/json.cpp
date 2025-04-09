#include "json.h"

json::Node json::loadNode(std::wifstream& input) {
	wchar_t c;

	if (!(input >> c)) {
		throw ParsingError("Unexpected EOF");
	}

	switch (c) {
	case '[':
		return loadArray(input);
	case '{':
		return loadDict(input);
	case '"':
		return loadString(input);
	default:
		input.putback(c);
		return loadInt(input);
	}
}

json::Node json::loadArray(std::wifstream& input) {
	std::vector<Node> result;

	for (wchar_t c; input >> c && c != ']';) {
		if (c != ',') {
			input.putback(c);
		}
		result.push_back(loadNode(input));
	}

	if (!input) {
		throw ParsingError("Array parsing error");
	}

	return result;
}

json::Node json::loadDict(std::wifstream& input) {

	Dict result;

	for (wchar_t c; input >> c && c != '}';) {

		if (c == '"') {
			std::wstring key = loadString(input).AsString();

			if (input >> c and c == ':') {

				if (result.find(key) != result.end()) {
					throw std::logic_error("Duplicate key has been found");
				}

				result.emplace(std::move(key), loadNode(input));
			}
			else {
				throw ParsingError(": was expected but some other symbol has been found");
			}
		}

		if (!input) {
			throw ParsingError("Dictionary parsing error");
		}
	}

	return result;

}

json::Node json::loadString(std::wifstream& input) {

	std::wstring result;
	wchar_t c;

	while (input >> c and c != '"') {
		result += c;
	}

	return result;
}

json::Node json::loadInt(std::wifstream& input) {

	std::wstring parsedNum;
	std::getline(input, parsedNum);
	/*while (input.peek() != '\n') {
		if (std::iswdigit(input.peek())) {
			parsedNum += static_cast<wchar_t>(input.get());
		}
		else {
			input.get();
		}
	}*/

	try {
		int num = std::stoi(parsedNum);
		return num;
	}
	catch (...) {
		throw ParsingError("Failed convertion to number");
	}

	return -1;
}
