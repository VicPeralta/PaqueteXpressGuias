#include "pch.h"
#include "Logger.h"
#include <fstream>
#include "..\include\Logger.h"

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

void Logger::logToAir(std::string filename, std::string rastreo, std::string directory)
{
	std::filesystem::path file{ filename };
	auto justFilename = file.stem();
	justFilename += ".air";
	std::filesystem::path newFilename{ directory };
	newFilename += "\\";
	newFilename += justFilename;
	std::ofstream os(newFilename, std::ios::binary);
	os.write(rastreo.c_str(), rastreo.size());
	os.close();
}

void Logger::logError(std::string filename, std::string directory, const std::string& error) {
	std::filesystem::path file{ filename };
	auto justFilename = file.stem();
	justFilename += ".err";
	std::filesystem::path newFilename{ directory };
	newFilename += "\\";
	newFilename += justFilename;
	std::ofstream os(newFilename, std::ios::binary);
	os.write(error.c_str(), error.size());
	os.close();
}

void Logger::logHistory(std::string filename, std::string rastreo, std::string directory)
{
	std::filesystem::path newFilename{ directory };
	std::filesystem::path file{ filename };
	newFilename += "\\history.log";
	std::string data{ file.stem().string() };
	data += "-" + rastreo;
	std::ofstream os;
	os.open(newFilename, std::ios::app);
	os << data << "\n";
	os.close();
}
