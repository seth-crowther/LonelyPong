#include "shapes.h"
#include <iostream>
#include <glm/glm.hpp>

class Paddle
{
	Shapes::Rectangle sprite = Shapes::Rectangle(-0.2f, -0.85f, 0.2f, -0.9f);
	int dir = 0;
	float speed = 2.0f;
	float width;
	
public: 
	Paddle()
	{
		width = sprite.vertexPositions[4] - sprite.vertexPositions[0];
	}

	Shapes::Rectangle* GetRect()
	{
		return &sprite;
	}

	void UpdateSprite(double time)
	{
		for (int i = 0; i < 4; i++)
		{
			sprite.vertexPositions[i * 2] += speed * (float)time * dir;
			if (i <= 1)
			{
				sprite.vertexPositions[i * 2] = glm::clamp(sprite.vertexPositions[i * 2], -1.0f, 1.0f - width);
			}
			else
			{
				sprite.vertexPositions[i * 2] = glm::clamp(sprite.vertexPositions[i * 2], -1.0f + width, 1.0f);
			}
		}
	}

	void ChangeDir(int newDir)
	{
		dir = newDir;
	}
};