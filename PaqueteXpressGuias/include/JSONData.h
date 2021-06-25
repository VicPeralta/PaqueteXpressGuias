#pragma once
#include <string>
struct LoginData {
	bool success{false};
	std::string token{};
	std::string error{};
};

struct CartaData {
	bool success{ false };
	std::string rastreo{};
	std::string folio{};
	std::string error{};
};
struct ZPLData {
	bool success{ false };
	std::string zpl{};
	std::string error{};
};