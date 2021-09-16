#version 330 core

// 顶点着色器输入
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexcoord;
layout (location = 2) in vec3 vNormal;

out vec2 texcoord;

uniform mat4 model;         // 模型变换矩阵
uniform mat4 view;          // 模型变换矩阵
uniform mat4 projection;    // 模型变换矩阵

void main()
{
    gl_Position = projection * view * model * vec4(vPosition, 1.0); // 指定ndc坐标
    texcoord = vTexcoord;   // 传递纹理坐标到片段着色器
}
