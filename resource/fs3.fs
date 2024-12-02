#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//FragColor = texture(ourTexture, TexCoord)* vec4(ourColor, 1.0);
	//0.2会返回80%的第一个输入颜色和20%的第二个输入颜色，即返回两个纹理的混合色。
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.8);
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), 0.8);y轴翻转
}