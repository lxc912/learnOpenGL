#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "drawTriangle.h"
#include "vertexShader.h"
#include "fragmentShader.h"
#include "callBackGroup.h"
#include "stb_image.h"
#include "shaders.h"
#define STB_IMAGE_IMPLEMENTATION
namespace drawTriangle_np {
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
		GLFWwindow* window = glfwCreateWindow(1600, 1200, "LearnOpenGL", NULL, NULL);
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
		glViewport(0, 0, 1600, 1200);//the difference between openGLviewport and GLFW window
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//��Ӧ�Ӵ��仯
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//G:_LINE�߿� GL_FILL:���
		//��д������ɫ����GLSL��
		unsigned int vertexShader;
		vertexShader1(vertexShader);
		//fragment shader
		unsigned int fragmentShader;
		fragmentShader2(fragmentShader);

		//������ɫ������--�Ѷ������ɫ����������
			//�����������-����id��
		unsigned int shaderProgram;
		shaderProgram = glCreateProgram();
		//����ɫ���ӵ�������--���ӹ���
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		//�ڶ�����Ⱦ����
		unsigned int vertexShader2_id, fragmentShader2_id, shaderProgram2;
		vertexShader2(vertexShader2_id);
		fragmentShader1(fragmentShader2_id);
		shaderProgram2 = glCreateProgram();
		glAttachShader(shaderProgram2, vertexShader2_id);
		glAttachShader(shaderProgram2, fragmentShader2_id);
		glLinkProgram(shaderProgram2);


		//������ɣ�ɾ����ɫ������Ȼ����Ҫ��
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader2_id);

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
			//// ����uniform��ɫ
			//float timeValue = glfwGetTime();
			//float greenValue = sin(timeValue) / 2.0f + 0.5f;
			//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
			//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
			//glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			//glDrawArrays(GL_TRIANGLES, 0, 3);
			//pipeline1 end

			//-----pipeline2--vertexShader: vertexShader2 || fragmentShader: fragmentShader1 
			glUseProgram(shaderProgram2);
			glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			glDrawArrays(GL_TRIANGLES, 0, 3);
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
		glDeleteProgram(shaderProgram);

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
		GLFWwindow* window = glfwCreateWindow(1600, 1200, "LearnOpenGL", NULL, NULL);
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
		glViewport(0, 0, 1600, 1200);//the difference between openGLviewport and GLFW window
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//��Ӧ�Ӵ��仯
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//G:_LINE�߿� GL_FILL:���
		//��д������ɫ����GLSL��
		unsigned int vertexShader;
		vertexShader2(vertexShader);
		//fragment shader
		unsigned int fragmentShader;
		fragmentShader2(fragmentShader);

		//������ɫ������--�Ѷ������ɫ����������
			//�����������-����id��
		unsigned int shaderProgram;
		shaderProgram = glCreateProgram();
		//����ɫ���ӵ�������--���ӹ���
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		//�������
		glUseProgram(shaderProgram);
		//������ɣ�ɾ����ɫ������Ȼ����Ҫ��
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices[] = {
			//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		//glBindBuffer(GL_ARRAY_BUFFER, 0);//glBindBuffer 0 ��ʾ������
		//glBindVertexArray(0);
		//�ڵ�ǰVAOû���ǰ���ܽ��EBO����
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		//load texture
		//create texture object
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// ���ز���������
		int width, height, nrChannels;
		unsigned char* data = stbi_load("resource/container.jfif", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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

		// uncomment this call to draw in wireframe polygons.
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



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
			//bind texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			// draw our first triangle
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//ͼԪ���ͣ����������������������ͣ������������е�һ��������ƫ����
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
		glDeleteProgram(shaderProgram);

		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
		return 0;
	}

	int draw() {
		return drawWithoutTexture();
	}

}
