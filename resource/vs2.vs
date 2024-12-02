#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 vertexColor;
//out vec4 ourPosition;

uniform float drift;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	//ourPosition = gl_Position;
	vertexColor = vec4(aColor, 1.0);
}