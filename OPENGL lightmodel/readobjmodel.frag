#version 330 core

in vec2 texcoord;    // ��������

out vec4 fColor;    // ƬԪ������ص���ɫ

uniform sampler2D texture;  // ����ͼƬ

void main()
{
    fColor.rgb =  texture2D(texture, texcoord).rgb;
}
