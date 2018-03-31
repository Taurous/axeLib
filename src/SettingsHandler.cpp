#include "SettingsHandler.h"

using namespace axe;

SettingsHandler::SettingsHandler()
{

}

SettingsHandler::~SettingsHandler()
{
	m_ints.clear();
	m_bools.clear();
	m_doubles.clear();
	m_strings.clear();
}

void SettingsHandler::printSettings()
{
	std::map<std::string, bool>::iterator it_b;
	std::map<std::string, double>::iterator it_d;
	std::map<std::string, int>::iterator it_i;
	std::map<std::string, std::string>::iterator it_s;

	printf("Settings:\n");
	for (it_b = m_bools.begin(); it_b != m_bools.end(); ++it_b)
	{
		printf("   %s:%s\n", (*it_b).first.c_str(), ((*it_b).second ? "true" : "false"));
	}
	for (it_d = m_doubles.begin(); it_d != m_doubles.end(); ++it_d)
	{
		printf("   %s:%f\n", (*it_d).first.c_str(), (*it_d).second);
	}
	for (it_i = m_ints.begin(); it_i != m_ints.end(); ++it_i)
	{
		printf("   %s:%i\n", (*it_i).first.c_str(), (*it_i).second);
	}
	for (it_s = m_strings.begin(); it_s != m_strings.end(); ++it_s)
	{
		printf("   %s:%s\n", (*it_s).first.c_str(), (*it_s).second.c_str());
	}
}

bool SettingsHandler::loadSettings(const char *path)
{
	std::ifstream file(path, std::ios_base::in);

	if (file.is_open())
	{
		while (file.good())
		{
			std::string name;
			std::string value = " ";

			getline(file, name);

			for (unsigned int i = 0; i < name.size(); i++)
			{
				if (name[i] == ':')
				{
					value = name.substr(i+1, name.size()-i-1);
					name.erase(i, name.size()-i);
					break;
				}
			}

			if (m_bools.count(name))
			{
				bool flag;

				if (value == "true") flag = true;
				else if (value == "false") flag = false;
				else continue;

				set(name, flag);
			}
			else if (m_strings.count(name))
			{
				set(name, value);
			}
			else if (m_doubles.count(name))
			{
				if (value == "0")
				{
					set(name, (double)0);
					continue;
				}

				double v = 0;

				try
				{
					v = std::stod(value, nullptr);
				}
				catch (const std::invalid_argument &ia)
				{
					printf("Invalid Argument: %s. %s:%s", ia.what(), name.c_str(), value.c_str());
					continue;
				}
				catch (const std::out_of_range &oor)
				{
					printf("Out of range: %s. %s:%s", oor.what(), name.c_str(), value.c_str());
					continue;
				}
				set(name, v);
			}
			else if (m_ints.count(name))
			{
				if (value == "0")
				{
					set(name, 0);
					continue;
				}

				int v = 0;

				try
				{
					v = std::stoi(value, nullptr);
				}
				catch (const std::invalid_argument &ia)
				{
					printf("Invalid Argument: %s. %s:%s", ia.what(), name.c_str(), value.c_str());
					continue;
				}
				catch (const std::out_of_range &oor)
				{
					printf("Out of range: %s. %s:%s", oor.what(), name.c_str(), value.c_str());
					continue;
				}
				set(name, v);
			}
		}
		file.close();
	}
	else
	{
		printf("Could not open settings file: %s\n", path);
		return false;
	}

	return true;
}

void SettingsHandler::saveSettings(const char *path)
{
	if (m_ints.empty() && m_bools.empty() && m_doubles.empty() && m_strings.empty()) return;

	std::ofstream file(path, std::ios_base::out);

	if (file.is_open())
	{
		std::map<std::string, bool>::iterator			it_b;
		std::map<std::string, double>::iterator			it_d;
		std::map<std::string, int>::iterator			it_i;
		std::map<std::string, std::string>::iterator	it_s;

		for (it_b = m_bools.begin(); it_b != m_bools.end(); ++it_b)
		{
			std::string output;
			output = (*it_b).first + ':' + ((*it_b).second ? "true" : "false") + "\n";

			file << output;
		}
		for (it_d = m_doubles.begin(); it_d != m_doubles.end(); ++it_d)
		{
			char buffer[64];
			sprintf_s(buffer, 64, "%s:%f\n", (*it_d).first.c_str(), (*it_d).second);

			file << buffer;

		}
		for (it_i = m_ints.begin(); it_i != m_ints.end(); ++it_i)
		{
			char buffer[64];
			sprintf_s(buffer, 64, "%s:%i\n", (*it_i).first.c_str(), (*it_i).second);

			file << buffer;
		}
		for (it_s = m_strings.begin(); it_s != m_strings.end(); ++it_s)
		{
			file << (*it_s).first << ":" << (*it_s).second << "\n";
		}

		file.close();
	}
	else
	{
		printf("Could not create settings file.");
	}

}

void axe::SettingsHandler::set(std::string name, double value)
{
	m_doubles[name] = value;
}

void axe::SettingsHandler::set(std::string name, int value)
{
	m_ints[name] = value;
}

void SettingsHandler::set(std::string name, bool value)
{
	m_bools[name] = value;
}

void SettingsHandler::set(std::string name, std::string value)
{
	m_strings[name] = value;
}

void SettingsHandler::get(std::string name, bool &ref)
{
	if (m_bools.count(name))
	{
		ref = m_bools[name];
	}
	else
	{
		printf("Setting %s does not exist.\n", name.c_str());
	}
}
void SettingsHandler::get(std::string name, std::string &ref)
{
	if (m_strings.count(name))
	{
		ref = m_strings[name];
	}
	else
	{
		printf("Setting %s does not exist.\n", name.c_str());
	}
}