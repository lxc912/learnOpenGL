#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void vertexShader1(unsigned int& vertexShader) {
	const char* vertexShaderSource = "#version 330 core\n"//�����汾��ģʽ
		"layout (location = 0) in vec3 aPos;\n"//�����һ��vec3 �ı������ֽ�apos��Ȼ����ڶ������Եĵ�0��λ��
		"out vec4 vertexColor;\n"
		//layout(location = 0) : ����һ������˵����ָ��������� aPos ��λ������Ϊ 0����OpenGL�У�λ����������ָ�����������ڶ��������е�������
		// layout (location = 0) in vec3 aPos;   ����λ��
		//layout(location = 1) in vec3 aNormal; ����
		//layout(location = 2) in vec2 aTexCoord; ��������
		//in : �ؼ��ֱ�ʾ����һ�������������ζ�Ÿñ�����ֵ�����ڶ������ݵ����롣
		//vec3 : ��ʾ����������Ϊ��ά����������������������ͨ����ʾX��Y��Z���꣩��
		//aPos : �����������ڱ�ʶ����Ķ���λ�����ݡ�
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"//���¶������꣬���������������ά���������ϵ
		"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
		//gl_Position: ����һ�����õı������������ն���Ĳü��ռ����ꡣOpenGLʹ�����������ȷ����������Ļ�ϵ�λ�á�
		//vec4 : ��ʾһ����ά�����������ĸ�����������ͨ�����ڱ�ʾ�ռ��е�λ�û���ɫ������������ڽ���ά����ת��Ϊ������ꡣ
		//aPos.x, aPos.y, aPos.z : ��������� aPos ����ȡX��Y��Z���꣬�ֱ��ʾ��������ά�ռ��е�λ�á�
		//1.0 : ������������W��������Ϊ1.0��ʾ����һ��λ�ã�������һ��������������������ͨ����W = 0��ʾ����
		"}\0";
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//����һ����vertex shader����
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//�����涨�����ɫ��Դ���븽�ӵ���������ɫ��������
	glCompileShader(vertexShader);//������ɫ��
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
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//����һ����vertex shader����
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//�����涨�����ɫ��Դ���븽�ӵ���������ɫ��������
	glCompileShader(vertexShader);//������ɫ��
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
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//����һ����vertex shader����
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//�����涨�����ɫ��Դ���븽�ӵ���������ɫ��������
	glCompileShader(vertexShader);//������ɫ��
	return;
}
