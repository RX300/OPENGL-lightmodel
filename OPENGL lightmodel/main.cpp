//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////
#include "stadx.h"


Model myModel;
GLuint program;

// 相机参数
glm::vec3 cameraPosition(0, 0, 0);      // 相机位置
glm::vec3 cameraDirection(0, 0, -1);    // 相机视线方向
glm::vec3 cameraUp(0, 1, 0);            // 世界空间下竖直向上向量
float pitch = 0.0f;
float roll = 0.0f;
float yaw = 0.0f;

// 视界体参数
float left = -1, right = 1, bottom = -1, top = 1, zNear = 0.01, zFar = 100.0;

int windowWidth = 512;  // 窗口宽
int windowHeight = 512; // 窗口高

bool keyboardState[1024];   // 键盘状态数组 keyboardState[x]==true 表示按下x键




void
init(void)
{
	myModel.load("model/nanosuit/nanosuit.obj");


	glEnable(GL_DEPTH_TEST);  // 开启深度测试

	glClearColor(0.0, 1.0, 1.0, 1.0);   // 背景颜色

	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "readobjmodel.vert" },
		{ GL_FRAGMENT_SHADER, "readobjmodel.frag" },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shaders);//读取文件和创建着色器程序
	glUseProgram(program);//使用链接过的着色器程序



}


//----------------------------------------------------------------------------
//
// display
//

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // 清空窗口颜色缓存

	// 传模型变换矩阵
	// 传视图矩阵
	// 传投影矩阵
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // 清空窗口颜色缓存

	glm::mat4 unit(   // 单位矩阵
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(0, 0, 0, 1)
	);
	glm::mat4 scale = glm::scale(unit, glm::vec3(0.05, 0.05, 0.05));
	glm::mat4 translate = glm::translate(unit, glm::vec3(0.0, -0.5, -0.5));

	// 模型变换矩阵
	glm::mat4 model = translate * scale;
	GLuint mlocation = glGetUniformLocation(program, "model");    // 名为model的uniform变量的位置索引
	glUniformMatrix4fv(mlocation, 1, GL_FALSE, glm::value_ptr(model));   // 列优先矩阵



	// 计算欧拉角以确定相机朝向
	cameraDirection.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraDirection.y = sin(glm::radians(pitch));
	cameraDirection.z = -cos(glm::radians(pitch)) * cos(glm::radians(yaw)); // 相机看向z轴负方向

	// 传视图矩阵
	glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
	GLuint vlocation = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(vlocation, 1, GL_FALSE, glm::value_ptr(view));

	// 传投影矩阵
	glm::mat4 projection = glm::perspective(glm::radians(70.0f), (GLfloat)windowWidth / (GLfloat)windowHeight, zNear, zFar);
	GLuint plocation = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(plocation, 1, GL_FALSE, glm::value_ptr(projection));

	myModel.draw(program);



}

//----------------------------------------------------------------------------
//
// main
//


int
main(int argc, char** argv)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 800, "Te", NULL, NULL);

	glfwMakeContextCurrent(window);
	gl3wInit();

	init();

	while (!glfwWindowShouldClose(window))
	{

		display();
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
