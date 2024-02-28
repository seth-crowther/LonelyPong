#include <string.h>
#include <glm/glm.hpp>
#include <iostream>
#include "shapes.h"

using namespace Shapes;

/*float vertexPositions[4 * 2];
unsigned int vertexIndices[6] = {
    0, 1, 3,
    1, 2, 3
};*/

Rectangle::Rectangle(float x0, float y0, float x1, float y1)
{
    float positions[4 * 2] = {
        x0, y0,
        x0, y1,
        x1, y1,
        x1, y0
    };

    memcpy(vertexPositions, positions, sizeof positions);
}


/*static const int vCount = 16;
float vertexPositions[2 * vCount];
unsigned int vertexIndices[3 * (vCount - 2)];

float angle = 360.0f / vCount;*/

Circle::Circle(float x, float y, float r)
{
    for (int i = 0; i < vCount; i++)
    {
        float currentAngle = angle * i;
        vertexPositions[i * 2] = x + r * cos(glm::radians(currentAngle));
        vertexPositions[i * 2 + 1] = y + r * sin(glm::radians(currentAngle));
    }

    int secondIndex = 1;
    int thirdIndex = 2;
    for (int i = 0; i < 3 * (vCount - 2); i += 3)
    {
        vertexIndices[i] = 0;
        vertexIndices[i+1] = secondIndex;
        vertexIndices[i+2] = thirdIndex;
        secondIndex++;
        thirdIndex++;
    }
}
