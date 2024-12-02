#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//FragColor = texture(ourTexture, TexCoord)* vec4(ourColor, 1.0);
	//0.2�᷵��80%�ĵ�һ��������ɫ��20%�ĵڶ���������ɫ�����������������Ļ��ɫ��
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.8);
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), 0.8);y�ᷭת
}