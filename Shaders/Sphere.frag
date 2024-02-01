#version 450
 
layout (location = 0) in VertexInput {
  vec4 color;
} vertexInput;

layout(location = 0) out vec4 out_Color;
layout(location = 1) out vec4 out_Normal;
layout(location = 2) out vec4 out_Vert;
 

void main()
{
	out_Color = vertexInput.color;
    out_Normal = vec4(1.0,0.0,0.0,1.0);
    out_Vert = vec4(0.0,0.0,0.0,1.0);
}
