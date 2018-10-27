#include "axeLib/util/FPS.h"

const int NUM_FRAMES_AVERAGE = 50;

namespace axe
{
	FPSObject::FPSObject() : elapsed(0), fps(0), index(0)
	{
		arr_of_frame_times = new long long[NUM_FRAMES_AVERAGE];
		for (int i = 0; i < NUM_FRAMES_AVERAGE; ++i)
		{
			arr_of_frame_times[i] = 0;
		}

		t.start();
	}

	FPSObject::~FPSObject()
	{
		delete[] arr_of_frame_times;
		arr_of_frame_times = nullptr;
	}

	void FPSObject::dumpArray()
	{
		printf("FPS Array Dump: ");
		for (int i = 0; i < NUM_FRAMES_AVERAGE-1; ++i)
		{
			printf("%lli, ", arr_of_frame_times[i]);
		}
		printf("%lli\n", arr_of_frame_times[NUM_FRAMES_AVERAGE-1]);
	}

	void FPSObject::calculateAverageFps()
	{
		elapsed = t.restart();

		// Add elapsed to array  (hold a history of last NUM_FRAMES_AVERAGE to have smooth average)
		arr_of_frame_times[index] = elapsed.count();
		index++;
		if (index >= NUM_FRAMES_AVERAGE) index = 0;

		// Calculate the average
		long long total = 0;
		for (int i = 0; i < NUM_FRAMES_AVERAGE; i++)
		{
			total += arr_of_frame_times[i];
		}
		total /= NUM_FRAMES_AVERAGE;

		// Convert milliseconds to seconds
		double seconds = total / 1000.f;

		// Convert seconds per frame into frames per second
		fps = 1 / seconds;
		//if (fps > 1000) fps = 1000;
	}

	double FPSObject::getAverageFps()
	{
		return fps;
	}
}