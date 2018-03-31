#pragma once

#include "Util\Timer.h"

namespace axe
{
	class FPSObject
	{
	public:
		FPSObject();
		~FPSObject();

		void calculateAverageFps(void);
		double getAverageFps(void);

	private:
		axe::Timer t;

		m_secs elapsed;
		m_secs *arr_of_frame_times;

		double fps;

		int index;
	};
}