#version 400
in vec4 vPosition;
in vec4 vColor;
out vec4 color;

uniform mat4 Model_View;

void main()
{
    gl_Position = Model_View * vPosition;
	color=vColor;
}//