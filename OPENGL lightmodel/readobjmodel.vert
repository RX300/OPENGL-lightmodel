#version 330 core

// ������ɫ������
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexcoord;
layout (location = 2) in vec3 vNormal;

out vec2 texcoord;

uniform mat4 model;         // ģ�ͱ任����
uniform mat4 view;          // ģ�ͱ任����
uniform mat4 projection;    // ģ�ͱ任����

void main()
{
    gl_Position = projection * view * model * vec4(vPosition, 1.0); // ָ��ndc����
    texcoord = vTexcoord;   // �����������굽Ƭ����ɫ��
}
