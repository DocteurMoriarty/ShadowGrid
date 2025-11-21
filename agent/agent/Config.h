#pragma once

#include <string>
#include <vector>

class Config
{
public:
	// Constructor and Deconstructor
	Config();
	~Config();

	bool loadFromFile(const std::string& path);
	bool loadFromJSON(const std::string& json);
	int getInt(const std::string& key, int defaultValue) const;
	std::string getString(const std::string& key, const std::string& defVal) const;
	bool getBool(const std::string& key, bool defaultValue) const;
	std::vector<std::string> getList(const std::string& key) const;

private:
	bool parseYAML(const std::string& content);
	bool parseJSON(const std::string& content);
	bool validateConfig();

};
