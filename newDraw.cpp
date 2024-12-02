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
				// 计算当前像素的起始位置
				int index = (y * width + x) * nrChannels;

				// 读取 R, G, B 值（假设图片有 3 个通道或 4 个通道）
				unsigned char r = data[index];     // Red channel
				unsigned char g = data[index + 1]; // Green channel
				unsigned char b = data[index + 2]; // Blue channel

				// 打印像素的 RGB 值
				std::cout << "Pixel (" << x << ", " << y << "): R = "
					<< static_cast<int>(r) << ", G = "
					<< static_cast<int>(g) << ", B = "
					<< static_cast<int>(b) << std::endl;

				// 如果图像有 4 个通道，获取 Alpha 值
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
		glfwMakeContextCurrent(window);//确认上下文是当前窗口
		//initialize GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
		//glViewport(0, 0, 1600, 1200);//the difference between openGLviewport and GLFW window
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//适应视窗变化
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//G:_LINE线框 GL_FILL:填充
		//渲染管线
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
			// 位置              // 颜色
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
			 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
		};


		unsigned int VBO, VAO, EBO;//!!!绑定顺序很重要：VAO->VBO->EBO
		//申请缓冲空间
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		//先绑定VAO，再绑VBO再绑EBO 注意顺序！！！！
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
		//如何解析顶点数据
		// 	glVertexAttribPointer 函数的每个参数作用总结如下：
		//	第一个参数：指定顶点属性的索引（位置）与vertexShader中的layout对应。
		//	第二个参数：每个顶点属性的组件数量(每个属性的长度)。
		//	第三个参数：顶点属性的数据类型。
		//	第四个参数：是否对数据进行标准化。
		//	第五个参数：相邻顶点属性之间的字节步长（stride）每一条数据的长度（3+3+2）。
		//	第六个参数：数据在缓冲区中的偏移量（起始地址）。
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //VAO会 保存 glVertexAttribPointer 的设置
		glEnableVertexAttribArray(0);
		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		//在当前VAO没解绑前不能解绑EBO！！
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//解绑EBO
		//glBindBuffer(GL_ARRAY_BUFFER, 0);//解绑VBO
		glBindVertexArray(0);//解绑VAO

		unsigned int VBO2, VAO2;
		glGenVertexArrays(1, &VAO2);
		glGenBuffers(1, &VBO2);
		glBindVertexArray(VAO2);
		//每个VAO都要独立绑定顶点属性（glVertexAttribPointer）和启用属性（glEnableVertexAttribArray）即使使用一样的VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //VAO会 保存 glVertexAttribPointer 的设置
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //VAO会 保存 glVertexAttribPointer 的设置
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
			//// 更新uniform颜色
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
			//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);//图元类型，绘制索引个数，索引类型，索引缓冲区中第一个索引的偏移量
			//直接VBO+VAO画 glDrawArrays(GL_TRIANGLES, 0, 3);
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
		glfwMakeContextCurrent(window);//确认上下文是当前窗口
		//initialize GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
		//glViewport(0, 0, 1600, 1200);//the difference between openGLviewport and GLFW window
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//适应视窗变化
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//G:_LINE线框 GL_FILL:填充
		//编写顶点着色器（GLSL）
		unsigned int vertexShader;
		Shader shader1("./resource/vs3.vs", "./resource/fs3.fs");

		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		//float vertices[] = {
		//	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		//		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		//		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		//		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		//		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
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
		//申请缓冲空间
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		//先绑定VAO，再绑VBO再绑EBO 注意顺序！！！！
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



		//如何解析顶点数据
		// 	glVertexAttribPointer 函数的每个参数作用总结如下：
		//	第一个参数：指定顶点属性的索引（位置）。
		//	第二个参数：每个顶点属性的组件数量。
		//	第三个参数：顶点属性的数据类型。
		//	第四个参数：是否对数据进行标准化。
		//	第五个参数：相邻顶点属性之间的字节步长（stride）每一条数据的长度（3+3+2）。
		//	第六个参数：数据在缓冲区中的偏移量（起始地址）。
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
		//glBindBuffer(GL_ARRAY_BUFFER, 0);//glBindBuffer 0 表示解绑操作
		//glBindVertexArray(0);
		//在当前VAO没解绑前不能解绑EBO！！
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		//！！！load texture 纹理单元和纹理着色器对应使用 纹理通过uniform的方式直接读取
		// 纹理单元加载纹理GL_TEXTURE0 ,若只有一个纹理默认绑定到0
		// 纹理着色器在着色器中声明，并通过
		// glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), idx);把特定纹理单元绑定到着色器上
		// 
		// 注意到纹理上下颠倒了！这是因为OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。
		// ----数据-------------绘制---------------着色器--------
		// texture数据	GL_TEXTURE连接两部分	    uniform纹理类型变量
		// 着色器中纹理类型的变量与对应纹理单元绑定--------绘制时激活纹理单元并将纹理数据绑定到纹理单元上
		// 
		//create texture object
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// 为当前绑定的纹理对象设置环绕、过滤方式
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// 加载并生成纹理
		int width, height, nrChannels;
		unsigned char* data = stbi_load("./resource/container.jfif", &width, &height, &nrChannels, 0);
		if (data)
		{
			//GL_TEXTURE_2D:目标纹理类型，表示你正在创建一个二维纹理。
			//0 (level) :Mipmap 的级别，0 表示基础级别（不使用 mipmap 或这是基础级的纹理）。
			//GL_RGB(internalFormat) :纹理的内部格式，表示 GPU 如何存储纹理数据。GL_RGB 表示每个纹素有 3 个分量：红色、绿色和蓝色。
			//width 和 height :纹理的宽度和高度（像素为单位）。这些值必须为正数。
			//0 (border) : 纹理边框的宽度，必须为 0。OpenGL 现在不再支持非零边框。
			//GL_RGB(format) : 输入图像数据的格式，表示数据中每个像素包含的颜色通道。GL_RGB 表示每个像素由红、绿、蓝 3 个分量组成。
			//GL_UNSIGNED_BYTE(type) : 输入数据的类型，表示数据的每个颜色分量是一个无符号字节（0 到 255）。
			//data : 指向实际图像数据的指针。通常这是从图像加载库（如 stb_image）获取的像素数据。
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//分配纹理数据
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

		// 为当前绑定的纹理对象设置环绕、过滤方式
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
		glUniform1i(glGetUniformLocation(shader1.ID, "texture1"), 0);//着色器中texture1的变量对应的纹理单元是GL_TEXTURE0
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
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清除颜色，也就是窗口的背景颜色
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);;//清除buffer
			//将texture绑定到对应纹理单元上
			glActiveTexture(GL_TEXTURE0);//激活纹理单元0
			glBindTexture(GL_TEXTURE_2D, texture);//把texture对应的数据绑定到纹理单元0上
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);

			// draw our first triangle
			shader1.use();
			glEnable(GL_DEPTH_TEST); 

			//MVP transformation
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 projection;
			// 注意，我们将矩阵向我们要进行移动场景的反方向移动。
			//camera coordinate:
			glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, -3.0f);
			view = glm::translate(view, camera_pos);
			//projection matrix: fov, 宽高比, 近平面距离，远裁剪平面距离
			projection = glm::perspective(glm::radians(fov), (float)window_size_x / (float)window_size_y, 0.1f, 100.0f);//透视算法
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
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//图元类型，绘制索引个数，索引类型，索引缓冲区中第一个索引的偏移量
			//直接VBO+VAO画 glDrawArrays(GL_TRIANGLES, 0, 3);
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

		//glm::mat4 trans;//0.99版本前初始化后默认值就是单位矩阵， 0.99后是全0矩阵
		glm::mat4 trans = glm::translate(input, glm::vec3(1.0f, 1.0f, 0.0f));//位移变化：给一个位移坐标,在输入矩阵上叠加一个位移操作生成一个变换矩阵
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