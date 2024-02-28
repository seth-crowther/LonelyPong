#include <string.h>
#include <iostream>

struct VertexBuffer
{
	float paddleVertices[8];
	float ballVertices[2 * 16];

	VertexBuffer(float* paddle, float* ball)
	{
		memcpy(paddleVertices, paddle, sizeof(float) * 8);
		memcpy(ballVertices, ball, sizeof(float) * 2 * 16);

		/*for (int i = 0; i < 32; i++)
		{
			std::cout << "Circle vertex X " << i << ": " << ballVertices[i*2] << std::endl;
			std::cout << "Circle vertex Y " << i << ": " << ballVertices[i*2 + 1] << std::endl;
		}*/
	}
};

struct IndexBuffer
{
	unsigned int paddleIndices[6];
	unsigned int ballIndices[3 * 14];

	IndexBuffer(unsigned int* paddle, unsigned int* ball)
	{
		memcpy(paddleIndices, paddle, sizeof(unsigned int) * 6);

		// Offset indices of ball
		for (int i = 0; i < 3 * 14; i++)
		{
			ballIndices[i] = ball[i] + 4;
			//std::cout << "Circle index " << i << ": " << ballIndices[i] << std::endl;
		}
	}
};