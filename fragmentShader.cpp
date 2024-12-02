#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void fragmentShader1(unsigned int& fragmentShader) {
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"//������һ�������������ʾ��Ƭ�ε���ɫ
		"in vec4 vertexColor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    FragColor = vertexColor;\n"
		"}\0";
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
}


void fragmentShader2(unsigned int& fragmentShader) {
	//use uniform variable 'ourcolor' as final output
	const char* fragmentShaderSource = R"(
	#version 330 core
	out vec4 FragColor;

	uniform vec4 ourColor; // ��OpenGL����������趨�������

	void main()
	{
		FragColor = ourColor;
	}
	)";
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
}

void fragmentShader3(unsigned int& fragmentShader) {
	const char* fragmentShaderSource = R"(
	#version 330 core
	out vec4 FragColor;

	in vec3 ourColor;
	in vec2 TexCoord;

	uniform sampler2D ourTexture;

	void main()
	{
		FragColor = texture(ourTexture, TexCoord);
	}
	)";
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
}