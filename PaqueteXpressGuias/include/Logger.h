#pragma once
#include <string>
#include <filesystem>

class Logger
{
public:
	static void logToFile(std::string filename, std::string rastreo,
		std::string extension, std::string directory, const std::string& data);
};

