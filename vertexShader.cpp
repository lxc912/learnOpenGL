#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void vertexShader1(unsigned int& vertexShader) {
	const char* vertexShaderSource = "#version 330 core\n"//声明版本和模式
		"layout (location = 0) in vec3 aPos;\n"//输入的一个vec3 的变量名字叫apos，然后存在顶点属性的第0号位置
		"out vec4 vertexColor;\n"
		//layout(location = 0) : 这是一个布局说明，指定输入变量 aPos 的位置属性为 0。在OpenGL中，位置属性用于指明顶点属性在顶点数组中的索引。
		// layout (location = 0) in vec3 aPos;   顶点位置
		//layout(location = 1) in vec3 aNormal; 法线
		//layout(location = 2) in vec2 aTexCoord; 纹理坐标
		//in : 关键字表示这是一个输入变量，意味着该变量的值来自于顶点数据的输入。
		//vec3 : 表示变量的类型为三维向量，包含三个浮点数（通常表示X、Y、Z坐标）。
		//aPos : 变量名，用于标识传入的顶点位置数据。
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"//更新顶点坐标，顶点坐标输出是四维的齐次坐标系
		"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
		//gl_Position: 这是一个内置的变量，代表最终顶点的裁剪空间坐标。OpenGL使用这个变量来确定顶点在屏幕上的位置。
		//vec4 : 表示一个四维向量（包含四个浮点数），通常用于表示空间中的位置或颜色。在这里，它用于将三维坐标转换为齐次坐标。
		//aPos.x, aPos.y, aPos.z : 从输入变量 aPos 中提取X、Y、Z坐标，分别表示顶点在三维空间中的位置。
		//1.0 : 这是齐次坐标的W分量，设为1.0表示这是一个位置，而不是一个方向向量（方向向量通常用W = 0表示）。
		"}\0";
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建一个新vertex shader对象
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//将上面定义的着色器源代码附加到创建的着色器对象上
	glCompileShader(vertexShader);//编译着色器
	return;
}

void vertexShader2(unsigned int& vertexShader) {
	//add uniform attribute based on vertexShader1
	const char* vertexShaderSource = R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aColor;

	out vec4 vertexColor;

	void main()
	{
		gl_Position = vec4(aPos, 1.0);
		vertexColor = vec4(aColor, 1.0);
	}
	)";
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建一个新vertex shader对象
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//将上面定义的着色器源代码附加到创建的着色器对象上
	glCompileShader(vertexShader);//编译着色器
	return;
}

void vertexShader3(unsigned int& vertexShader) {
	//vertex shader for first texture shader
	const char* vertexShaderSource = R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aColor;
	layout (location = 2) in vec2 aTexCoord;

	out vec3 ourColor;
	out vec2 TexCoord;

	void main()
	{
		gl_Position = vec4(aPos, 1.0);
		ourColor = aColor;
		TexCoord = aTexCoord;
	}
	)";
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建一个新vertex shader对象
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//将上面定义的着色器源代码附加到创建的着色器对象上
	glCompileShader(vertexShader);//编译着色器
	return;
}
