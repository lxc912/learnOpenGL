#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "vertexShader.h"
#include "fragmentShader.h"
#include "callBackGroup.h"
#include "stb_image.h"
#include "shaders.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION

namespace newDraw_np {
	int window_size_x = 1200;
	int window_size_y = 1200;
	void printMatrix4(const glm::mat4& mat);
	void printVec4(const glm::vec4& vec);
	void readPic(unsigned char* data, int height, int width, int nrChannels) {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				// ���㵱ǰ���ص���ʼλ��
				int index = (y * width + x) * nrChannels;

				// ��ȡ R, G, B ֵ������ͼƬ�� 3 ��ͨ���� 4 ��ͨ����
				unsigned char r = data[index];     // Red channel
				unsigned char g = data[index + 1]; // Green channel
				unsigned char b = data[index + 2]; // Blue channel

				// ��ӡ���ص� RGB ֵ
				std::cout << "Pixel (" << x << ", " << y << "): R = "
					<< static_cast<int>(r) << ", G = "
					<< static_cast<int>(g) << ", B = "
					<< static_cast<int>(b) << std::endl;

				// ���ͼ���� 4 ��ͨ������ȡ Alpha ֵ
				if (nrChannels == 4) {
					unsigned char a = data[index + 3]; // Alpha channel
					std::cout << "A = " << static_cast<int>(a) << std::endl;
				}
			}
		}
	}
	int drawWithoutTexture() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//set GLFW window size
		GLFWwindow* window = glfwCreateWindow(window_size_x, window_size_y, "LearnOpenGL", NULL, NULL);
		if (window == NULL) {
			std::cout << "Fail to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);//ȷ���������ǵ�ǰ����
		//initialize GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
		//glViewport(0, 0, 1600, 1200);//the difference between openGLviewport and GLFW window
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//��Ӧ�Ӵ��仯
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//G:_LINE�߿� GL_FILL:���
		//��Ⱦ����
		Shader shader1("./resource/vs2.vs", "./resource/fs2.fs");

		Shader shader2("./resource/vs2.vs", "./resource/fs1.fs");


		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices1[] = {
			 0.5f,  0.5f, 0.0f,  // top right
			 0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			//- 0.5f,  0.5f, 0.0f,  // top left
		};
		unsigned int indices1[] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
		};
		float vertices2[] = {
			// λ��              // ��ɫ
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
			 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
		};


		unsigned int VBO, VAO, EBO;//!!!��˳�����Ҫ��VAO->VBO->EBO
		//���뻺��ռ�
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		//�Ȱ�VAO���ٰ�VBO�ٰ�EBO ע��˳�򣡣�����
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
		//��ν�����������
		// 	glVertexAttribPointer ������ÿ�����������ܽ����£�
		//	��һ��������ָ���������Ե�������λ�ã���vertexShader�е�layout��Ӧ��
		//	�ڶ���������ÿ���������Ե��������(ÿ�����Եĳ���)��
		//	�������������������Ե��������͡�
		//	���ĸ��������Ƿ�����ݽ��б�׼����
		//	��������������ڶ�������֮����ֽڲ�����stride��ÿһ�����ݵĳ��ȣ�3+3+2����
		//	�����������������ڻ������е�ƫ��������ʼ��ַ����
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //VAO�� ���� glVertexAttribPointer ������
		glEnableVertexAttribArray(0);
		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		//�ڵ�ǰVAOû���ǰ���ܽ��EBO����
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//���EBO
		//glBindBuffer(GL_ARRAY_BUFFER, 0);//���VBO
		glBindVertexArray(0);//���VAO

		unsigned int VBO2, VAO2;
		glGenVertexArrays(1, &VAO2);
		glGenBuffers(1, &VBO2);
		glBindVertexArray(VAO2);
		//ÿ��VAO��Ҫ�����󶨶������ԣ�glVertexAttribPointer�����������ԣ�glEnableVertexAttribArray����ʹʹ��һ����VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //VAO�� ���� glVertexAttribPointer ������
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //VAO�� ���� glVertexAttribPointer ������
		glEnableVertexAttribArray(1);


		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.



		// render loop
		// -----------
		while (!glfwWindowShouldClose(window))
		{
			// input
			// -----
			processInput(window);

			// render
			// ------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			//-----pipeline1--vertexShader: vertexShader1 || fragmentShader: fragmentShader2 
			//// draw our first triangle
			//glUseProgram(shaderProgram);
			//shader1.use();
			//// ����uniform��ɫ
			//float timeValue = glfwGetTime();
			//float greenValue = sin(timeValue) / 2.0f + 0.5f;
			//int vertexColorLocation = shader1.getUniformLoc("ourColor");
			//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
			//glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			//glDrawArrays(GL_TRIANGLES, 0, 3);
			//pipeline1 end

			//-----pipeline2--vertexShader: vertexShader2 || fragmentShader: fragmentShader1 
			shader2.use();
			glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawArrays(GL_TRIANGLES, 0, 3);
			int drift_loc = shader2.getUniformLoc("drift");
			glUniform1f(drift_loc, 0.5);
			//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);//ͼԪ���ͣ����������������������ͣ������������е�һ��������ƫ����
			//ֱ��VBO+VAO�� glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0); // no need to unbind it every time 

			// glfw: swap buff  ers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
		return 0;
	}

	int drawWithTexture() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//set GLFW window size
		GLFWwindow* window = glfwCreateWindow(window_size_x, window_size_y, "LearnOpenGL", NULL, NULL);
		if (window == NULL) {
			std::cout << "Fail to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);//ȷ���������ǵ�ǰ����
		//initialize GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
		//glViewport(0, 0, 1600, 1200);//the difference between openGLviewport and GLFW window
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//��Ӧ�Ӵ��仯
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//G:_LINE�߿� GL_FILL:���
		//��д������ɫ����GLSL��
		unsigned int vertexShader;
		Shader shader1("./resource/vs3.vs", "./resource/fs3.fs");

		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		//float vertices[] = {
		//	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
		//		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
		//		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
		//		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
		//		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
		//};
		float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
		};

		glm::vec3 cubePositions[] = {
		  glm::vec3(0.0f,  0.0f,  0.0f),
		  glm::vec3(2.0f,  5.0f, -15.0f),
		  glm::vec3(-1.5f, -2.2f, -2.5f),
		  glm::vec3(-3.8f, -2.0f, -12.3f),
		  glm::vec3(2.4f, -0.4f, -3.5f),
		  glm::vec3(-1.7f,  3.0f, -7.5f),
		  glm::vec3(1.3f, -2.0f, -2.5f),
		  glm::vec3(1.5f,  2.0f, -2.5f),
		  glm::vec3(1.5f,  0.2f, -1.5f),
		  glm::vec3(-1.3f,  1.0f, -1.5f)
		};



		unsigned int VBO, VAO, EBO;
		//���뻺��ռ�
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		//�Ȱ�VAO���ٰ�VBO�ٰ�EBO ע��˳�򣡣�����
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



		//��ν�����������
		// 	glVertexAttribPointer ������ÿ�����������ܽ����£�
		//	��һ��������ָ���������Ե�������λ�ã���
		//	�ڶ���������ÿ���������Ե����������
		//	�������������������Ե��������͡�
		//	���ĸ��������Ƿ�����ݽ��б�׼����
		//	��������������ڶ�������֮����ֽڲ�����stride��ÿһ�����ݵĳ��ȣ�3+3+2����
		//	�����������������ڻ������е�ƫ��������ʼ��ַ����
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		//glBindBuffer(GL_ARRAY_BUFFER, 0);//glBindBuffer 0 ��ʾ������
		//glBindVertexArray(0);
		//�ڵ�ǰVAOû���ǰ���ܽ��EBO����
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		//������load texture ����Ԫ��������ɫ����Ӧʹ�� ����ͨ��uniform�ķ�ʽֱ�Ӷ�ȡ
		// ����Ԫ��������GL_TEXTURE0 ,��ֻ��һ������Ĭ�ϰ󶨵�0
		// ������ɫ������ɫ������������ͨ��
		// glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), idx);���ض�����Ԫ�󶨵���ɫ����
		// 
		// ע�⵽�������µߵ��ˣ�������ΪOpenGLҪ��y��0.0��������ͼƬ�ĵײ��ģ�����ͼƬ��y��0.0����ͨ���ڶ�����
		// ----����-------------����---------------��ɫ��--------
		// texture����	GL_TEXTURE����������	    uniform�������ͱ���
		// ��ɫ�����������͵ı������Ӧ����Ԫ��--------����ʱ��������Ԫ�����������ݰ󶨵�����Ԫ��
		// 
		//create texture object
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// ���ز���������
		int width, height, nrChannels;
		unsigned char* data = stbi_load("./resource/container.jfif", &width, &height, &nrChannels, 0);
		if (data)
		{
			//GL_TEXTURE_2D:Ŀ���������ͣ���ʾ�����ڴ���һ����ά����
			//0 (level) :Mipmap �ļ���0 ��ʾ�������𣨲�ʹ�� mipmap �����ǻ�������������
			//GL_RGB(internalFormat) :������ڲ���ʽ����ʾ GPU ��δ洢�������ݡ�GL_RGB ��ʾÿ�������� 3 ����������ɫ����ɫ����ɫ��
			//width �� height :����Ŀ�Ⱥ͸߶ȣ�����Ϊ��λ������Щֵ����Ϊ������
			//0 (border) : ����߿�Ŀ�ȣ�����Ϊ 0��OpenGL ���ڲ���֧�ַ���߿�
			//GL_RGB(format) : ����ͼ�����ݵĸ�ʽ����ʾ������ÿ�����ذ�������ɫͨ����GL_RGB ��ʾÿ�������ɺ졢�̡��� 3 ��������ɡ�
			//GL_UNSIGNED_BYTE(type) : �������ݵ����ͣ���ʾ���ݵ�ÿ����ɫ������һ���޷����ֽڣ�0 �� 255����
			//data : ָ��ʵ��ͼ�����ݵ�ָ�롣ͨ�����Ǵ�ͼ����ؿ⣨�� stb_image����ȡ���������ݡ�
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//������������
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
			//give fail reason
			std::cout << "Failed to load image: " << stbi_failure_reason() << std::endl;
			return 0;
		}
		//-------------------test-----------------------
		stbi_image_free(data);
		//load the second texture
		unsigned int texture2;
		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		GLint texWidth, texHeight;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
		std::cout << "Texture Width: " << texWidth << ", Height: " << texHeight << std::endl;
		data = stbi_load("./resource/container3.png", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
			//give fail reason
			std::cout << "Failed to load image: " << stbi_failure_reason() << std::endl;
			return 0;
		}
		stbi_image_free(data);

		// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
		// -------------------------------------------------------------------------------------------
		shader1.use(); // don't forget to activate/use the shader before setting uniforms!
		// either set it manually like so:
		glUniform1i(glGetUniformLocation(shader1.ID, "texture1"), 0);//��ɫ����texture1�ı�����Ӧ������Ԫ��GL_TEXTURE0
		// or set it via the texture class
		shader1.setInt("texture2", 1);
		// uncomment this call to draw in wireframe polygons.
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


		// render loop
		// -----------
		float fov = 30.0;
		while (!glfwWindowShouldClose(window))
		{
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
				fov += 5;
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
				fov -= 5;
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				window_size_x -= 100;
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				window_size_x += 100;
			if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
				window_size_y -= 100;
			if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
				window_size_y += 100;
			// input
			// -----
			processInput(window);

			// render
			// ------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//���������ɫ��Ҳ���Ǵ��ڵı�����ɫ
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);;//���buffer
			//��texture�󶨵���Ӧ����Ԫ��
			glActiveTexture(GL_TEXTURE0);//��������Ԫ0
			glBindTexture(GL_TEXTURE_2D, texture);//��texture��Ӧ�����ݰ󶨵�����Ԫ0��
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);

			// draw our first triangle
			shader1.use();
			glEnable(GL_DEPTH_TEST); 

			//MVP transformation
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 projection;
			// ע�⣬���ǽ�����������Ҫ�����ƶ������ķ������ƶ���
			//camera coordinate:
			glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, -3.0f);
			view = glm::translate(view, camera_pos);
			//projection matrix: fov, ��߱�, ��ƽ����룬Զ�ü�ƽ�����
			projection = glm::perspective(glm::radians(fov), (float)window_size_x / (float)window_size_y, 0.1f, 100.0f);//͸���㷨
			glUniformMatrix4fv(glGetUniformLocation(shader1.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(shader1.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			for (unsigned int i = 0; i < 10; i++)
			{
				model = glm::mat4(1.0);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * 2;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				glUniformMatrix4fv(glGetUniformLocation(shader1.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//ͼԪ���ͣ����������������������ͣ������������е�һ��������ƫ����
			//ֱ��VBO+VAO�� glDrawArrays(GL_TRIANGLES, 0, 3);
			// glBindVertexArray(0); // no need to unbind it every time 
			
			// glfw: swap buff  ers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		
		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
		return 0;
	}

	glm::mat4 transform(glm::mat4 input) {

		//glm::mat4 trans;//0.99�汾ǰ��ʼ����Ĭ��ֵ���ǵ�λ���� 0.99����ȫ0����
		glm::mat4 trans = glm::translate(input, glm::vec3(1.0f, 1.0f, 0.0f));//λ�Ʊ仯����һ��λ������,����������ϵ���һ��λ�Ʋ�������һ���任����
		//printVec4(vec4);
		return trans;
	}

	void printMatrix4(const glm::mat4& mat) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				std::cout << mat[i][j] << ' ';
			}
			std::cout << std::endl;
		}
	}
	void printVec4(const glm::vec4& vec) {
		for (int i = 0; i < 4; ++i) {
			std::cout << vec[i] << ' ';
		}
		return;
	}

	int newdraw() {
		return drawWithTexture();
	}
}