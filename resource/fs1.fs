#version 330 core

out vec4 FragColor;

in vec4 vertexColor;
in vec4 ourPosition;

void main()
{
       //FragColor = ourPosition;
       FragColor = vertexColor;
}