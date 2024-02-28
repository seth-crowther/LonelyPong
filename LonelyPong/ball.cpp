#include "shapes.h"
#include <glm/glm.hpp>

using namespace Shapes;

class Ball
{
	Circle sprite = Circle(0.0f, 0.0f, 0.03f);
	glm::vec2 direction;
	float speed = 1.0f;
	bool gameOver = false;

	float* right = &(sprite.vertexPositions[0]);
	float* top = &(sprite.vertexPositions[9]);
	float* left = &(sprite.vertexPositions[16]);
	float* bottom = &(sprite.vertexPositions[25]);
	float* bottomX = &(sprite.vertexPositions[24]);

public:
	Ball()
	{
		direction = glm::vec2(0.0f, 1.0f);
	}

	Circle* GetCircle()
	{
		return &sprite;
	}

	bool IsGameOver()
	{
		return gameOver;
	}

	void Update(double deltaTime)
	{
		for (int i = 0; i < Circle::vCount; i++)
		{
			sprite.vertexPositions[i * 2] += direction.x * speed * (float)deltaTime;
			sprite.vertexPositions[i * 2 + 1] += direction.y * speed * (float)deltaTime;
		}
		speed += 0.0005f;
	}

	void CheckBounce(float* paddleLeft, float* paddleRight)
	{
		if (direction.x > 0 && *right > 1.0f || direction.x < 0 && *left < -1.0f)
		{
			direction = glm::vec2(-direction.x, direction.y);
		}

		if (direction.y > 0 && *top > 1.0f)
		{
			direction = glm::vec2(direction.x, -direction.y);
		}

		if (direction.y < 0 && *bottom < -0.85f && *bottomX > *paddleLeft && *bottomX < *paddleRight)
		{
			float paddleCentreX = (*paddleLeft + *paddleRight) / 2;
			glm::vec2 dirToBall = glm::vec2(*left + 0.03f - paddleCentreX, *bottom + 0.03f - -0.875f);
			dirToBall = glm::normalize(dirToBall);

			direction = dirToBall;
		}

		if (direction.y < 0 && *bottom < -1.0f)
		{
			gameOver = true;
		}
	}
};