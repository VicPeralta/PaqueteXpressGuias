#pragma once
#include <string>
#include <JSONData.h>
class JSONParser
{
public:
	LoginData ParseLogin(std::string data);
	CartaData ParseCarta(std::string data);
	ZPLData PaserZPL(std::string data);
	std::string UpdateToken(const std::string& data, std::string newToken);
};

