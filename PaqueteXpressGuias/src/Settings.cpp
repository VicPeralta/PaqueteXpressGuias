#include "pch.h"
#include "Settings.h"
#include <fstream>
#include <vector>
#include "Poco\JSON\Parser.h"
#include "Poco\SharedPtr.h"
#include "Poco\JSON\Object.h"
#include "Poco/Dynamic/Var.h"


void Settings::loadSettingsFromJson(std::string filename)
{
	auto data = getData(filename);
	if (data.empty()) {
		std::string messageError{"Error cargando opciones desde archivo JSON " };
		messageError += filename;
		throw std::exception{ messageError.c_str() };
	}
	Poco::JSON::Parser parser;
	Poco::Dynamic::Var parseResult = parser.parse(data);
	Poco::JSON::Object::Ptr content = parseResult.extract<Poco::JSON::Object::Ptr>();
	std::vector<std::string> names;
	content->getNames(names);
	for (auto& name : names) {
		auto value = content->getValue<std::string>(name);
		addSetting(name, value);
	}
}

void Settings::addSetting(std::string key, std::string value)
{
	m_settings.insert({ key,value });
}

std::string Settings::getData(std::string fileName)
{
	std::ifstream is(fileName, std::ios::binary);
	std::string result;
	char buffer[1024];
	while (is) {
		is.read(buffer, 1024);
		result.append(buffer, is.gcount());
	}
	is.close();
	return result;
}
