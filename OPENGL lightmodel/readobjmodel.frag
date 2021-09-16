#version 330 core

in vec2 texcoord;    // 纹理坐标

out vec4 fColor;    // 片元输出像素的颜色

uniform sampler2D texture;  // 纹理图片

void main()
{
    fColor.rgb =  texture2D(texture, texcoord).rgb;
}
