/*
Set default settings before loading from file, else the loader won't find any settings.
*/

#include <map>
#include <string>
#include <fstream>
#include <exception>

namespace axe
{

	class SettingsHandler
	{
	public:
		SettingsHandler();
		~SettingsHandler();

		bool loadSettings(const char *path);
		void saveSettings(const char *path);

		template <typename t>
		void set(std::string name, t value)
		{
			m_doubles[name] = static_cast<double>(value);
		}
		void set(std::string name, bool value);
		void set(std::string name, std::string value);

		template <typename t>
		void get(std::string name, t &ref)
		{
			if (m_doubles.count(name))
			{
				ref = static_cast<t>(m_doubles[name]);
			}
			else
			{
				printf("Setting %s does not exist.", name.c_str());
			}
		}
		void get(std::string name, bool &ref);
		void get(std::string name, std::string &ref);

		void printSettings();

	private:
		std::map<std::string, bool> m_bools;
		std::map<std::string, double> m_doubles;
		std::map<std::string, std::string> m_strings;
	};
};