#pragma once

#include <allegro5\allegro_native_dialog.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace axe
{
	enum
	{
		LOGGER_ERROR,
		LOGGER_WARNING,
		LOGGER_MESSAGE

	};
	static inline void setOutputFile(const char *file)
	{
		FILE *stream;

		freopen_s(&stream, file, "w", stdout); //redirecting stdout to file
	}

	static void log(int type, const char *fmt, ...)
	{
		char buffer[1024];

		va_list arg;
		va_start(arg, fmt);
		vsnprintf_s(buffer, 1024, fmt, arg);
		va_end(arg);

		std::string st = "Forgot to set the error type, eh? ";

		switch (type)
		{
		case LOGGER_ERROR:
			st = "ERROR: ";
			break;
		case LOGGER_WARNING:
			st = "WARNING: ";
			break;
		case LOGGER_MESSAGE:
			st = "";
		default:
			break;
		}

		printf("%s%s", st.c_str(), buffer);
	}

	static void crash(const char *fmt, ...)
	{
		char buffer[1024];

		va_list arg;
		va_start(arg, fmt);
		vsnprintf_s(buffer, 1024, fmt, arg);
		va_end(arg);

		log(LOGGER_ERROR, buffer);
		al_show_native_message_box(NULL, "CRITICAL ERROR", "CRITICAL ERROR", buffer, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		axe::log(axe::LOGGER_ERROR, buffer);
		axe::log(axe::LOGGER_ERROR, "\nExiting Application...\n");

		exit(EXIT_FAILURE);
	}
};