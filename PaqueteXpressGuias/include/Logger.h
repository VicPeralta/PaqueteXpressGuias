#pragma once
#include <string>
#include <filesystem>

class Logger
{
public:
	static void logToFile(std::string filename, std::string rastreo,
		std::string extension, std::string directory, const std::string& data);
	static void logToAir(std::string filename, std::string rastreo, std::string directory);
	static void logError(std::string filename, std::string directory, const std::string& error);
	static void logHistory(std::string filename, std::string rastreo, std::string directory);
};

