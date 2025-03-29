#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "Sprite.hpp"

#include <vector>

class Engine
{
public:
	Engine();

	void add_sprite(const Sprite &sprite)
	{
		sprites.push_back(sprite);
	}

private:
	std::vector<Sprite> sprites;
};

#endif // ENGINE_HPP
