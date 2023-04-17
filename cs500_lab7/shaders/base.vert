#version 400

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTextCoords;

out vec2 UV;

void main()
{
   gl_Position = vPosition;
   
   UV = vTextCoords;
}