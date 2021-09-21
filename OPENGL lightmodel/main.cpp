//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////
#include "stadx.h"


Model myModel;
GLuint program;

// 相机参数
vmath::vec3 cameraPosition(0, 0, 0);      // 相机位置
vmath::vec3 cameraDirection(0, 0, -1);    // 相机视线方向
vmath::vec3 cameraUp(0, 1, 0);            // 世界空间下竖直向上向量
float pitch = 0.0f;
float roll = 0.0f;
float yaw = 0.0f;

// 视界体参数
float left = -1, right = 1, bottom = -1, top = 1, zNear = 0.01, zFar = 100.0;

int windowWidth = 512;  // 窗口宽
int windowHeight = 512; // 窗口高

bool keyboardState[1024];   // 键盘状态数组 keyboardState[x]==true 表示按下x键

float starttime,stoptime;

unsigned int apptime()
{
	return (unsigned int)(GetTickCount64() - starttime);
}

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
	float time = float(apptime() & 0xFFFF) / float(0xFFFF);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // 清空窗口颜色缓存

	// 传模型变换矩阵
	// 传视图矩阵
	// 传投影矩阵
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // 清空窗口颜色缓存

	vmath::mat4 vmodel=vmath::translate(0.0f,-0.5f,-0.5f)*vmath::scale(0.05f)* vmath::rotate(987.0f * time * 3.14159f, vmath::vec3(0.0f, 1.0f, 0.0f));
	GLuint mlocation = glGetUniformLocation(program, "model");    // 名为model的uniform变量的位置索引
	glUniformMatrix4fv(mlocation, 1, GL_FALSE, vmodel);   // 列优先矩阵
	



	// 计算欧拉角以确定相机朝向
	cameraDirection[0] = cos(vmath::radians(pitch)) * sin(vmath::radians(yaw));
	cameraDirection[1] = sin(vmath::radians(pitch));
	cameraDirection[2] = -cos(vmath::radians(pitch)) * cos(vmath::radians(yaw)); // 相机看向z轴负方向

	// 传视图矩阵
	vmath::mat4 view = vmath::lookat(cameraPosition, cameraPosition + cameraDirection, cameraUp);
	GLuint vlocation = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(vlocation, 1, GL_FALSE, view);

	// 传投影矩阵
	vmath::mat4 projection = vmath::perspective(70.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, zNear, zFar);
	GLuint plocation = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(plocation, 1, GL_FALSE, projection);

	myModel.draw(program);



}

//----------------------------------------------------------------------------
//
// main
//


int
main(int argc, char** argv)
{
	starttime = GetTickCount64();
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1080, 1080, "Te", NULL, NULL);

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
