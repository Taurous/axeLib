#include "axeEngine\GUI\GUIInteractable.h"

namespace axe
{
	GUIInteractable::GUIInteractable(DrawEngine *draw, GUIObject *parent, int x, int y, int width, int height, int anchor)
		: GUIObject(draw, parent, x, y, width, height, anchor), state(NORMAL), m_hovered(false), keybind(0)
	{

	}
}