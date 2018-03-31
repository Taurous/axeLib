#pragma once

#include <string>
#include <vector>
#include <stack>

#include <type_traits>
#include <memory>
#include <functional>

#include "ResourceTypes.h"

namespace axe
{
	const int EMPTY_RESOURCE_ = 0;

	template <typename T>
	struct ResourceHandle
	{
		static_assert(std::is_base_of<ResourceBase, T>::value, "Type must be derived from ResourceBase!");
		ResourceHandle() : resource(nullptr) { }

		ResourceHandle(T *r) : resource(r)
		{
			resource->incRef();
		}
		ResourceHandle(const ResourceHandle &r)
		{
			resource = r.resource;
			resource->incRef();
		}
		ResourceHandle &operator=(const ResourceHandle &r)
		{
			resource = r.resource;
			resource->incRef();
			return *this;
		}

		~ResourceHandle()
		{
			resource->decRef();
		}

		bool isNull() { return (resource == nullptr); }
		bool isEmpty() { return (resource->getID() == EMPTY_RESOURCE_); }
		bool isLoaded() { return resource->isLoaded(); }

		bool operator==(const ResourceHandle& other) const { return (other.resource->getID() == this->resource->getID()); }
		bool operator!=(const ResourceHandle& other) const { return !(*this == other); }

		operator int() const { return this->resource->getID(); }
		operator bool() const
		{
			if (resource == nullptr) return false;

			return this->resource->getID() != EMPTY_RESOURCE_;
		}

		bool operator==(const int other) const { return (other == this->resource->getID()); }
		bool operator!=(const int other) const { return (other != this->resource->getID()); }
		bool operator>=(const int other) const { return (*this > other || *this == other); }
		bool operator<=(const int other) const { return (*this < other || *this == other); }
		bool operator<(const int other) const { return (this->resource->getID() < other); }
		bool operator>(const int other) const { return (this->resource->getID() > other); }

	private:
		T *resource;
	};

	template <typename T>
	class ResourceManager
	{
		static_assert(std::is_base_of<ResourceBase, T>::value, "Type must be derived from ResourceBase!");
	public:
		ResourceManager() : m_path_to_resources("/") { }
		ResourceManager(std::string path_to_resources) : m_path_to_resources(path_to_resources) { }

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) & = delete;
		ResourceManager& operator=(ResourceManager&&) & = delete;

		~ResourceManager()
		{
			for (unsigned int i = 0; i < m_resources.size(); ++i)
			{
				if (m_resources[i] != nullptr)
				{
					if (m_resources[i]->isLoaded())
					{
						m_resources[i]->destroy();
					}
					delete m_resources[i];
					m_resources[i] = nullptr;
				}
			}
		}

		void setPathToResources(std::string path) { m_path_to_resources = path; }

		ResourceHandle<T> getResource(const std::string &name);
		T* operator[](const ResourceHandle<T> &handle);
		void removeUnreferencedResources();

	protected:

		std::stack<int> m_unused_handles;
		std::vector<T*> m_resources;

		std::string m_path_to_resources;
	};

	// Resource Manager member function definitions

	template <typename T> ResourceHandle<T> ResourceManager<T>::getResource(const std::string &name)
	{
		if (m_resources.size() == 0) // Create default resource
		{
			T *res = new T(EMPTY_RESOURCE_, m_path_to_resources, "default");
			res->createDefault();
			m_resources.push_back(res);
		}

		for (unsigned int i = 1; i < m_resources.size(); i++)
		{
			if (m_resources[i]->getName() == name) // If resource with the supplied path exists
			{
				if (m_resources[i]->isLoaded()) // If it's loaded, return a handle
				{
					return ResourceHandle<T>(m_resources[i]);
				}
				else // If it's not loaded, try to load it.
				{
					if (m_resources[i]->load())
					{
						return ResourceHandle<T>(m_resources[i]); // Success
					}
					else
					{
						axe::log(LOGGER_ERROR, "Failed to load existing resource: %s\n", name);
						return ResourceHandle<T>(m_resources[i]);
					}
				}
			}
		}
		// Resource with path did not exist, create new resource
		T *res = new T(short(m_resources.size()), m_path_to_resources, name);

		res->load();

		if (!res->isLoaded())
		{
			res->destroy();
			delete res;
			return ResourceHandle<T>(m_resources[EMPTY_RESOURCE_]);
		}

		ResourceHandle<T> handle(res);

		if (!m_unused_handles.empty()) // Reuse previously created handle, or add new handle to vector
		{
			res->setID(m_unused_handles.top());
			m_unused_handles.pop();
			m_resources[int(handle)] = res;
		}
		else
		{
			res->setID(short(m_resources.size()));
			m_resources.push_back(res);
		}

		return handle;
	}

	template <typename T>
	T* ResourceManager<T>::operator[](const ResourceHandle<T> &handle)
	{
		if (handle > EMPTY_RESOURCE_ && handle < int(m_resources.size()))
		{
			if (m_resources[int(handle)]->isLoaded()) return m_resources[int(handle)]; // if loaded, return pointer to object, else return nothing, should try to reload.
		}

		return m_resources[EMPTY_RESOURCE_];
	}

	template <typename T>
	void ResourceManager<T>::removeUnreferencedResources()
	{
		for (unsigned int i = 1; i < m_resources.size(); ++i)
		{
			if (m_resources[i]->isUnreferenced() && m_resources[i]->isLoaded())
			{
				m_unused_handles.push(i);
				m_resources[i]->destroy();
				m_resources[i]->setID(EMPTY_RESOURCE_);
			}
		}
	}
};
