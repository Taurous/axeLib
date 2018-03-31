#include "util\FPS.h"

const int NUM_FRAMES_AVERAGE = 10;

namespace axe
{
	FPSObject::FPSObject() : elapsed(0), fps(0), index(0)
	{
		arr_of_frame_times = new m_secs[NUM_FRAMES_AVERAGE];
		for (int i = 0; i < NUM_FRAMES_AVERAGE; ++i)
		{
			arr_of_frame_times[i] = m_secs(0);
		}

		t.start();
	}

	FPSObject::~FPSObject()
	{
		delete[] arr_of_frame_times;
		arr_of_frame_times = nullptr;
	}

	void FPSObject::calculateAverageFps()
	{
		elapsed = t.restart();

		// Add elapsed to array  (hold a history of last NUM_FRAMES_AVERAGE to have smooth average)
		arr_of_frame_times[index] = elapsed;
		index++;
		if (index >= NUM_FRAMES_AVERAGE) index = 0;

		// Calculate the average
		std::chrono::seconds total(0);
		for (int i = 0; i < NUM_FRAMES_AVERAGE; i++)
		{
			total += std::chrono::duration_cast<std::chrono::seconds>(arr_of_frame_times[i]);
		}
		total /= NUM_FRAMES_AVERAGE;

		// Convert milliseconds to seconds
		auto seconds = total / 1000.0;

		// Convert seconds per frame into frames per second
		fps = static_cast<double>(std::chrono::seconds(1) / seconds);
		//if (avg_frame_rate > 1000) avg_frame_rate = 1000;
	}

	double FPSObject::getAverageFps()
	{
		return fps;
	}
}