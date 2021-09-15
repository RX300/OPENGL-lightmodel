#version 430 core
in vec4 fscolor;
out vec4 fColor;
mat3 M=mat3(1,2,3,4,5,6,7,8,9);
void main()
{
    fColor = fscolor;/*片元着色器只需要一个输出变量即可*/
}
