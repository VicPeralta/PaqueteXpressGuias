#pragma once

#include <string>
#include <unordered_map>
#include <sstream>

class Settings
{
public:
	void loadSettingsFromJson(std::string fileName);
	template<typename T>
	T getValue(std::string key);
	void addSetting(std::string key, std::string value);
	inline static Settings& getInstance();
	size_t count() const;
private:
	Settings() {};
	Settings(const Settings&) = delete;
	
	std::unordered_map<std::string, std::string> m_settings;
	std::string getData(std::string fileName);
	template<typename T>
	T convertTo(std::string value);
};


template<typename T>
inline T Settings::getValue(std::string key)
{
	auto i = m_settings.find(key);
	if (i != m_settings.end()) {
		return convertTo<T>((*i).second);
	}
	return T();
}

template<typename T>
inline T Settings::convertTo(std::string value)
{
	std::stringstream ss(value);
	T to{};
	ss >> to;
	return to;
}

Settings& Settings::getInstance() {
	static Settings theSetting{};
	return theSetting;
}

inline size_t Settings::count() const
{
	return m_settings.size();
}
