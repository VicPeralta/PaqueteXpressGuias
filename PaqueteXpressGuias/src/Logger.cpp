#include "pch.h"
#include "Logger.h"
#include <fstream>

void Logger::logToFile(std::string filename, std::string rastreo, std::string extension, std::string directory, const std::string& data)
{
	std::filesystem::path file{ filename };
	auto justFilename = file.stem();
	justFilename += "-" + rastreo + "." + extension;
	std::filesystem::path newFilename{ directory };
	newFilename += "\\";
	newFilename += justFilename;
	std::ofstream os(newFilename, std::ios::binary);
	os.write(data.c_str(), data.size());
	os.close();
}
