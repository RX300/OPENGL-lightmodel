//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////
#include "stadx.h"


Model myModel;
GLuint program;

// �������
glm::vec3 cameraPosition(0, 0, 0);      // ���λ��
glm::vec3 cameraDirection(0, 0, -1);    // ������߷���
glm::vec3 cameraUp(0, 1, 0);            // ����ռ�����ֱ��������
float pitch = 0.0f;
float roll = 0.0f;
float yaw = 0.0f;

// �ӽ������
float left = -1, right = 1, bottom = -1, top = 1, zNear = 0.01, zFar = 100.0;

int windowWidth = 512;  // ���ڿ�
int windowHeight = 512; // ���ڸ�

bool keyboardState[1024];   // ����״̬���� keyboardState[x]==true ��ʾ����x��




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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // ��մ�����ɫ����

	// ��ģ�ͱ任����
	// ����ͼ����
	// ��ͶӰ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // ��մ�����ɫ����

	glm::mat4 unit(   // ��λ����
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(0, 0, 0, 1)
	);
	glm::mat4 scale = glm::scale(unit, glm::vec3(0.05, 0.05, 0.05));
	glm::mat4 translate = glm::translate(unit, glm::vec3(0.0, -0.5, -0.5));

	// ģ�ͱ任����
	glm::mat4 model = translate * scale;
	GLuint mlocation = glGetUniformLocation(program, "model");    // ��Ϊmodel��uniform������λ������
	glUniformMatrix4fv(mlocation, 1, GL_FALSE, glm::value_ptr(model));   // �����Ⱦ���



	// ����ŷ������ȷ���������
	cameraDirection.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraDirection.y = sin(glm::radians(pitch));
	cameraDirection.z = -cos(glm::radians(pitch)) * cos(glm::radians(yaw)); // �������z�Ḻ����

	// ����ͼ����
	glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
	GLuint vlocation = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(vlocation, 1, GL_FALSE, glm::value_ptr(view));

	// ��ͶӰ����
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
