#pragma once

namespace Shapes
{
	struct Rectangle
	{
		float vertexPositions[8];
		unsigned int vertexIndices[6] = {
			0, 1, 3,
			1, 2, 3
		};

		Rectangle(float x0, float y0, float x1, float y1);
	};

	struct Circle
	{
		static const int vCount = 16;
		float vertexPositions[2 * vCount];
		unsigned int vertexIndices[3 * (vCount - 2)];

		float angle = 360.0f / vCount;

		Circle(float x, float y, float r);
	};
}