#pragma once

#include "axeEngine\ResourceManager.h"
#include "axeEngine\GUI\Frame.h"

namespace axe
{
	class ImageFrame : public Frame
	{
	protected:
		ResourceHandle<Bitmap> bitmap;

	public:
		ImageFrame(DrawEngine *draw, GUIObject *parent, int x, int y, int width, int height, const std::string &filename, int anchor = ANCHOR_TOPLEFT);
		virtual ~ImageFrame();

		virtual void draw();
	};
}