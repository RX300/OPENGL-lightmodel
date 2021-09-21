//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////
#include "stadx.h"


Model myModel;
GLuint program;

// �������
vmath::vec3 cameraPosition(0, 0, 0);      // ���λ��
vmath::vec3 cameraDirection(0, 0, -1);    // ������߷���
vmath::vec3 cameraUp(0, 1, 0);            // ����ռ�����ֱ��������
float pitch = 0.0f;
float roll = 0.0f;
float yaw = 0.0f;

// �ӽ������
float left = -1, right = 1, bottom = -1, top = 1, zNear = 0.01, zFar = 100.0;

int windowWidth = 512;  // ���ڿ�
int windowHeight = 512; // ���ڸ�

bool keyboardState[1024];   // ����״̬���� keyboardState[x]==true ��ʾ����x��

float starttime,stoptime;

unsigned int apptime()
{
	return (unsigned int)(GetTickCount64() - starttime);
}

void
init(void)
{
	myModel.load("model/nanosuit/nanosuit.obj");


	glEnable(GL_DEPTH_TEST);  // ������Ȳ���

	glClearColor(0.0, 1.0, 1.0, 1.0);   // ������ɫ

	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "readobjmodel.vert" },
		{ GL_FRAGMENT_SHADER, "readobjmodel.frag" },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shaders);//��ȡ�ļ��ʹ�����ɫ������
	glUseProgram(program);//ʹ�����ӹ�����ɫ������



}


//----------------------------------------------------------------------------
//
// display
//

void
display(void)
{
	float time = float(apptime() & 0xFFFF) / float(0xFFFF);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // ��մ�����ɫ����

	// ��ģ�ͱ任����
	// ����ͼ����
	// ��ͶӰ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // ��մ�����ɫ����

	vmath::mat4 vmodel=vmath::translate(0.0f,-0.5f,-0.5f)*vmath::scale(0.05f)* vmath::rotate(987.0f * time * 3.14159f, vmath::vec3(0.0f, 1.0f, 0.0f));
	GLuint mlocation = glGetUniformLocation(program, "model");    // ��Ϊmodel��uniform������λ������
	glUniformMatrix4fv(mlocation, 1, GL_FALSE, vmodel);   // �����Ⱦ���
	



	// ����ŷ������ȷ���������
	cameraDirection[0] = cos(vmath::radians(pitch)) * sin(vmath::radians(yaw));
	cameraDirection[1] = sin(vmath::radians(pitch));
	cameraDirection[2] = -cos(vmath::radians(pitch)) * cos(vmath::radians(yaw)); // �������z�Ḻ����

	// ����ͼ����
	vmath::mat4 view = vmath::lookat(cameraPosition, cameraPosition + cameraDirection, cameraUp);
	GLuint vlocation = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(vlocation, 1, GL_FALSE, view);

	// ��ͶӰ����
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
