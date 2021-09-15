//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////
#ifndef _STADX_H
#define _STADX_H
#include "stadx.h"
#endif // !stadx.h


enum VAO_IDs { Triangles, NumVAOs = 4 };
enum Buffer_IDs { ArrayBuffer, NumBuffers = 5 };
enum Attrib_IDs { vPosition = 0 };

GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];
GLuint ebo[1];
GLint  uniformindex;
GLfloat timeLoc = 0;


const GLuint  NumVertices = 6;

//----------------------------------------------------------------------------
//
// init
//
GLfloat  vertices[NumVertices][2] = {
		{ -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
		{  0.90f,  0.90f },{  0.90f, -0.85f },  { -0.85f,  0.90f }   // Triangle 2
};
GLfloat  vertices2[NumVertices][2] = {
{ -0.50f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
		{  0.90f,  0.90f },{  0.90f, -0.85f },  { -0.85f,  0.90f }   // Triangle 2
};
GLfloat  vertices_LINE[NumVertices][2] = {
	{ -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
	{  0.90f,  0.90f },{  0.90f, -0.85f },  { -0.85f,  0.90f }   // Triangle 2
};
GLfloat color[NumVertices][4] = {
{ 0.0f, 1.0f,0,1 },
{  0.0f, 1.0f,0,1  },
{ 0.0f, 1.0f,0,1  },
	{ 0.0f, 1.0f,0,1 },
{  0.0f, 1.0f,0,1  },
{ 0.0f, 1.0f,0,1  }
};
// A single triangle
static const GLfloat vertex_positions[] =
{
	-0.5f, 0.5f,  0.0f, 1.0f,//����������
	-0.5f, -0.5f,  0.0f, 1.0f,//����������
	0.5f,  -0.5f,  0.0f, 1.0f,//����������
	0.5f, 0.5f, 0.0f, 1.0f,//����������
};
static const GLfloat vertex_colors[] =
{
	0.5f, 0.8f, 1.0f, 1.0f,
};
void
init(void)
{   //��԰
	const GLint n = 180;
	GLfloat round[n + 2][2] = {0.0f,0.0f};
	
	for (int i = 1; i < n+2; i++)
	{
		round[i][0] = 0.5 * cos(2*(i - 1)* M_PI/180);
		round[i][1] = 0.5 * sin(2 * (i - 1) * M_PI / 180);
	}
	GLuint roundvao;
	GLuint roundbuffer;
	glCreateVertexArrays(1,&roundvao);
	glBindVertexArray(roundvao);
	glCreateBuffers(1,&roundbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,roundbuffer);
	glNamedBufferStorage(roundbuffer, sizeof(round), round, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT,
		GL_FALSE, 2 * sizeof(float), BUFFER_OFFSET(0));//glVertexAttribPointer ָ����
//��Ⱦʱ����ֵΪ index��vPosition�� �Ķ���������������ݸ�ʽ��λ�ã�Ҳ����ָ��ɫ����location=0������
	glEnableVertexAttribArray(vPosition);//���ö�����������

	//����3dģ��
	Assimp::Importer importer;
	const aiScene* sceneObjPtr = importer.ReadFile("A2",
		aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!sceneObjPtr
		|| sceneObjPtr->mFlags == AI_SCENE_FLAGS_INCOMPLETE
		|| !sceneObjPtr->mRootNode)
	
		std::cerr << "Error:Model::loadModel, description: "
			<< importer.GetErrorString() << std::endl;
		
	

	/*
	//����û����VAO���ǳ���Ҳ�����е�ԭ��ò����opengl��Ĭ��VAO,����������VAO���øı�Ļ�display()����
	//�е�glBindVertexArray(VAOs[Triangles])����ȡ����,��Ϊinit()���Ѿ��󶨹�һ����.
	glCreateVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[0]);
	glCreateBuffers(NumBuffers, Buffers);//����NumBuffers�����㻺�����,����NumBuffers��δʹ�ö�����������Buffers��
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glNamedBufferStorage(Buffers[0], sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT);//Ҳ������������
	glVertexAttribPointer(vPosition, 2, GL_FLOAT,
		GL_FALSE, 2 * sizeof(float), BUFFER_OFFSET(0));//glVertexAttribPointer ָ����
//��Ⱦʱ����ֵΪ index��vPosition�� �Ķ���������������ݸ�ʽ��λ�ã�Ҳ����ָ��ɫ����location=0������
	glEnableVertexAttribArray(vPosition);//���ö�����������


	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glNamedBufferStorage(Buffers[1], sizeof(vertices2), vertices2, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT,
		GL_FALSE, 2 * sizeof(float), BUFFER_OFFSET(0));//glVertexAttribPointer ָ����
//��Ⱦʱ����ֵΪ index��vPosition�� �Ķ���������������ݸ�ʽ��λ�ã�Ҳ����ָ��ɫ����location=0������
	glEnableVertexAttribArray(vPosition);//���ö�����������


//ebo����
	static const GLushort vertex_indices[] =
	{
		0, 1, 2, 0,2,3
	};
	glBindVertexArray(VAOs[2]);
	glCreateBuffers(1, ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glNamedBufferStorage(ebo[0], sizeof(vertex_indices), vertex_indices, GL_DYNAMIC_STORAGE_BIT);


	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions) + sizeof(vertex_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_positions), vertex_positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_positions), sizeof(vertex_colors), vertex_colors);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(vertex_positions));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//��Ե����
	glBindVertexArray(VAOs[3]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[4]);
	glNamedBufferStorage(Buffers[4], sizeof(vertices_LINE), vertices_LINE, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT,
		GL_FALSE, 2 * sizeof(float), BUFFER_OFFSET(0));//
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[3]);
	glNamedBufferStorage(Buffers[3], sizeof(color), color, GL_DYNAMIC_STORAGE_BIT);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);//���ö�����������
	glEnableVertexAttribArray(1);//���ö�����������
*/

	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);//��ȡ�ļ��ʹ�����ɫ������
	glUseProgram(program);//ʹ�����ӹ�����ɫ������

	uniformindex = glGetUniformLocation(program, "time");
	int c;

}

//----------------------------------------------------------------------------
//
// display
//

void
display(void)
{
	timeLoc += 0.00;
	if (timeLoc >= 2)
		timeLoc = 0;
	glUniform1f(uniformindex, timeLoc);
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glClearBufferfv(GL_COLOR, 0, black);//���֡����
	/*static bool cur = 0;
	if (cur == 0)
	{
		glBindVertexArray(VAOs[0]);//ѡ����Ϊ��������ʹ�õĶ�������
		cur = !cur;
	}
	else if (cur == 1)
	{
		glBindVertexArray(VAOs[1]);//ѡ����Ϊ��������ʹ�õĶ�������
		cur = !cur;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);//ʹ�õ�ǰ�󶨵Ķ�������Ԫ�ؽ���һϵ�еļ���ͼԪ
	glBindVertexArray(VAOs[2]);
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(VAOs[3]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);//ʹ�õ�ǰ�󶨵Ķ�������Ԫ�ؽ���һϵ�еļ���ͼԪ
*/
	glDrawArrays(GL_TRIANGLE_FAN, 0, 182);
	Sleep(15);


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
