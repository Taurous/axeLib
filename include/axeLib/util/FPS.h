#pragma once

#include "axeLib\util\Timer.h"

namespace axe
{
	class FPSObject
	{
	public:
		FPSObject();
		~FPSObject();

		void calculateAverageFps();
		double getAverageFps();

		void dumpArray();

	private:
		axe::Timer t;

		m_secs elapsed;
		long long *arr_of_frame_times;

		double fps;

		int index;
	};
}