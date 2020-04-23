#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>

#include "Shader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace glm;
using namespace std;

float vertices[] = {
	 

	0.5f,0.5f, 0.5f,
	-0.5f,0.5f, 0.5f,
	-0.5f,0.5f, -0.5f,
	0.5f,0.5f, -0.5f,


	0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f, -0.5f,
	0.5f,-0.5f, -0.5f,

};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 
unsigned loadSkyBox(vector<string> faces) {

	unsigned id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	int width;
	int height;
	int nChannels;

	for (size_t i = 0; i < faces.size(); i++) {

		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			throw exception("ERROR: no images for cubemap");
		}

	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return id;
}

GLFWwindow* createWindow() {
	GLFWwindow* window;


	if (!glfwInit()) {
		throw exception("glfw init failed");
	}

	window = glfwCreateWindow(1200, 800, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw exception("glfw window init failed");
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw exception("glad init failed");
	}
	glEnable(GL_DEPTH_TEST);
	return window;
}

vector<string> faces{
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/front.jpg",
		"skybox/back.jpg",
};

int main(void)
{
	GLFWwindow* window = createWindow();

	

	unsigned tId = loadSkyBox(faces);


	unsigned vaoSkyBox;
	glGenVertexArrays(1, &vaoSkyBox);
	glBindVertexArray(vaoSkyBox);

	unsigned vboSkyBox;
	glGenBuffers(1, &vboSkyBox);
	glBindBuffer(GL_ARRAY_BUFFER, vboSkyBox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);




	vector<float> verticesPiramid;

	float radius = 1;

	for (unsigned int i = 0; i < 6; i++) {
		float angle = 60 * i;

		float x = cos(radians(angle)) * radius;
		float y = sin(radians(angle)) * radius;
		cout << "x cord: " << x << "y coord: " << y << endl;
		verticesPiramid.push_back(x);
		verticesPiramid.push_back(-0.5f);
		verticesPiramid.push_back(y);

		
	}

	verticesPiramid.push_back(0.0f);
	verticesPiramid.push_back(1.0f);
	verticesPiramid.push_back(0.0f);


	unsigned indiciesPiramid[] = {
		0,2,1,
		0,3,2,
		3,5,0,
		4,5,3,

		0,6,1,
		4,6,2,
		2,6,3,
		3,6,4,
		4,6,5,
		5,6,0

	};

	unsigned indiciesCube[] = {
		0,1,
		1,2,
		2,3,
		3,0,
		0,4,
		1,5,
		2,6,
		3,7,
		4,5,
		5,6,
		6,7,
		7,4
	};

	unsigned vaoPiramid;
	glGenVertexArrays(1, &vaoPiramid);
	glBindVertexArray(vaoPiramid);

	unsigned vboPiramid;
	glGenBuffers(1, &vboPiramid);
	glBindBuffer(GL_ARRAY_BUFFER, vboPiramid);
	glBufferData(GL_ARRAY_BUFFER, verticesPiramid.size() * sizeof(float), &verticesPiramid[0], GL_STATIC_DRAW);

	unsigned ibPiramid;
	glGenBuffers(1, &ibPiramid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibPiramid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indiciesPiramid), indiciesPiramid, GL_STATIC_DRAW);
	

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);



	unsigned vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned ibCube;
	glGenBuffers(1, &ibCube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibCube);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indiciesCube), indiciesCube,GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	Shader shad("basis.shader");
	Shader skyBoxShader("skyBox.shader");
	Shader drawCone("drawWithColors.shader");

	vec3 viewPos(1.5f);
	mat4 model(1.0f);
	model = scale(model, vec3(0.3f));
	mat4 view = lookAt(viewPos, vec3(0), vec3(0, 1, 0));
	mat4 proj = perspective(radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	mat4 lineModel(1.0f);



	shad.setUniform4m("u_model", model);
	shad.setUniform4m("u_view", view);
	shad.setUniform4m("u_proj", proj);


	drawCone.setUniform4m("u_model", model);
	drawCone.setUniform4m("u_view", view);
	drawCone.setUniform4m("u_proj", proj);


	skyBoxShader.setUniform4m("u_model", lineModel);
	skyBoxShader.setUniform4m("u_view", view);
	skyBoxShader.setUniform4m("u_proj", proj);
	skyBoxShader.setUniform1i("skybox", 0);

	vec3 lineColor(1.0f);
	vec3 modelColor(0.5f);

	shad.setUniformVec3("renderColor", modelColor);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	bool isRotating = false;
	vec3 prevRotation;
	while (!glfwWindowShouldClose(window))
	{
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ImGui_ImplOpenGL3_NewFrame();

		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SliderFloat3("viewMat", &viewPos[0],-6.0f,6.0f);

		string text;
		if (isRotating) {
			text = "press to\nstop rotation";
			float x = cos(radians(glfwGetTime()) * 10.0f) * 3.0f;
			float y = sin(radians(glfwGetTime()) * 10.0f) * 3.0f;
			view = lookAt(vec3(x,0.7f,y), vec3(0), vec3(0, 1, 0));
		}
		else {
			
			view = lookAt(viewPos, vec3(0), vec3(0, 1, 0));
			text = "press to\nstart rotation";
		}

		if (ImGui::Button(text.c_str())) {
			isRotating = !isRotating;
			if (!isRotating) {
				viewPos = prevRotation;
			}
			else {
				prevRotation = viewPos;
			}
		}


	




		//render innercube
		glBindVertexArray(vaoPiramid);
		drawCone.setUniform4m("u_model", model);
		drawCone.setUniform4m("u_view", view);
		drawCone.bind();
		glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
		
		

		//render outer line cube
		glBindVertexArray(vao);
		shad.setUniform4m("u_view", view);
		shad.setUniform4m("u_model", lineModel);
		shad.setUniformVec3("renderColor", lineColor);
		shad.bind();
		glDrawElements(GL_LINES,24,GL_UNSIGNED_INT,0);



		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		glDepthMask(GL_FALSE);
		mat4 tView = mat4(mat3(view));
		skyBoxShader.setUniform4m("u_view", tView);
		skyBoxShader.bind();

		glBindVertexArray(vaoSkyBox);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tId);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS); // set depth function back to default
	
		//Render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);	
		glfwPollEvents();
	}
	//Shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}