#ifndef ENGINE_H
#define ENGINE_H
#include "Sprite.h"


class Engine {
public:
	Engine();

	void add_sprite(const Sprite& sprite) {
		sprites.push_back(sprite);
	}

private:
	std::vector<Sprite> sprites;
};



#endif //ENGINE_H
