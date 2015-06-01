#include "Gem.h"

BlueGem::BlueGem()
{
	_state = 0;
	_type = blue;
	_particleColor1 = kColor2;
}

bool BlueGem::createGemSprite(Node *layer, Point point,bool visible)
{
	Gem::createGemSpriteByName(layer, point, visible, "blue.png");
	return true;
}
