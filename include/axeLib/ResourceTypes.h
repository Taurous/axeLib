#pragma once

//#include <stdexcept>

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h>

#include <string>

#include "util\Logger.h"

namespace axe
{
	class ResourceBase
	{
	public:
		ResourceBase(short id, const std::string& path, const std::string& name) : m_id(id), m_path(path), m_name(name), m_loaded(false), m_refs(0) { }

		ResourceBase(const ResourceBase&) = delete;
		ResourceBase(ResourceBase&&) = delete;
		ResourceBase& operator=(const ResourceBase&) & = delete;
		ResourceBase& operator=(ResourceBase&&) & = delete;

		virtual ~ResourceBase() {}

		virtual bool load() = 0;
		virtual bool reload() = 0;
		virtual void destroy() = 0;
		virtual void createDefault() = 0;

		bool isLoaded() { return m_loaded; }

		std::string getPath() { return m_path; }
		void setPath(const std::string path) { m_path = path; }

		std::string getName() { return m_name; }
		void setName(const std::string name) { m_name = name; }

		void incRef() { m_refs++; }
		unsigned short getNumRefs() const { return m_refs; }
		void decRef() {
			m_refs--;
			if (m_refs < 0) axe::log(LOGGER_WARNING, "Resource reference dropping below 0! Ref Count: %i, Name: %s\n", m_refs, m_name);
		}

		bool isUnreferenced() { return m_refs <= 0 ? true : false; }

		short getID() { return m_id; }
		void setID(short id) { m_id = id; }

	protected:
		bool m_loaded;
		unsigned short m_refs;
		short m_id;
		std::string m_name;
		std::string m_path;

	};

	class Bitmap : public ResourceBase
	{
	public:
		Bitmap(short id, const std::string& path, const std::string& name) : m_data(nullptr), ResourceBase(id, path, name), m_width(1), m_height(1) { }
		~Bitmap();

		virtual bool load();
		virtual bool reload();
		virtual void destroy();
		virtual void createDefault();

		int getWidth() { return m_width; }
		int getHeight() { return m_height; }

		ALLEGRO_BITMAP *getData() { return m_data; }

	private:
		ALLEGRO_BITMAP *m_data;
		int m_width, m_height;
	};

	class Sound : public ResourceBase
	{
	public:
		Sound(short id, const std::string& path, const std::string& name) : m_data(nullptr), ResourceBase(id, path, name) { }
		~Sound();

		virtual bool load();
		virtual bool reload();
		virtual void destroy();
		virtual void createDefault();

	private:
		ALLEGRO_SAMPLE *m_data;
		ALLEGRO_SAMPLE_ID m_sample_id;
	};

	class Font : public ResourceBase
	{
	public:
		Font(short id, const std::string& path, const std::string& name) : m_data(nullptr), m_font_height(0), ResourceBase(id, path, name) { }
		~Font();

		virtual bool load();
		virtual bool reload();
		virtual void destroy();
		virtual void createDefault();

		unsigned int getStringWidth(std::string str);
		int getFontHeight() { return m_font_height; }

		ALLEGRO_FONT *getData() { return m_data; }

	private:
		ALLEGRO_FONT *m_data;
		short m_font_height;
	};
}