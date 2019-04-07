#version 420

layout(location = 0) in vec4 i_Position;
layout(location = 1) in vec4 i_Normal;

void main() {
	gl_Position = vec4(i_Position.xyz,1.0);
}
