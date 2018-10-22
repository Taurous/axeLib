#pragma once

namespace axe
{
	static const int EMPTY_RESOURCE_ = 0;

	template <typename T>
	class ResourceManager;

	template <typename T>
	struct ResourceHandle
	{
		static_assert(std::is_base_of<ResourceBase, T>::value, "Type must be derived from ResourceBase!");
		
		ResourceHandle(ResourceManager<T> *manager, unsigned int id) : m_id(id), m_manager(manager)
		{
			m_manager->incRef(id);
		}
		ResourceHandle(const ResourceHandle &r)
		{
			m_id = r.m_id;
			m_manager = r.m_manager;
			m_manager->incRef(id);
		}
		ResourceHandle &operator=(const ResourceHandle &r)
		{
			m_id = r.m_id;
			m_manager = r.m_manager;
			m_manager->incRef(id);
			return *this;
		}

		~ResourceHandle()
		{
			m_manager->decRef(id);
			m_manager = nullptr;
			m_id = EMPTY_RESOURCE_;
		}

		bool isLoaded() { return bool(m_id); }

		bool operator==(const ResourceHandle& other) const { return (other.m_id == m_id); }
		bool operator!=(const ResourceHandle& other) const { return !(*this == other); }

		operator int() const { return m_id; }
		operator bool() const { return m_id != EMPTY_RESOURCE_; }

		bool operator==(const int other) const { return (other == m_id); }
		bool operator!=(const int other) const { return (other != m_id); }
		bool operator< (const int other) const { return (m_id  < other); }
		bool operator> (const int other) const { return (m_id  > other); }
		bool operator>=(const int other) const { return (*this > other || *this == other); }
		bool operator<=(const int other) const { return (*this < other || *this == other); }

	private:

		unsigned int m_id;
		ResourceManager<T> *m_manager;
	};
}