#pragma once

#include <allegro5\allegro.h>

#include <type_traits>

namespace axe
{
	typedef uint8_t Bitfield_8;
	typedef uint16_t Bitfield_16;
	typedef uint32_t Bitfield_32;

	template <typename T>
	inline bool getBit(const T &bitfield, const int &bit)
	{
		static_assert(std::is_integral<T>::value, "Cannot do bitwise operations on non-integral type!\n");
		return (bitfield & bit);
	}

	template <typename T>
	inline void setBit(T &bitfield, const int &bit, bool flag)
	{
		static_assert(std::is_integral<T>::value, "Cannot do bitwise operations on non-integral type!\n");
		bitfield = flag ? bitfield | bit : ~(~bitfield | bit);
	}

	const inline int allegro_init()
	{
		if (!al_is_system_installed())
		{
			return al_init();
		}
		else return 0;
	}
}