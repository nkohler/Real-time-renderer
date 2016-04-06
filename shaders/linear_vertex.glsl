#version 450 core
layout (location = 0) in vec3 position;

layout (location = 0) uniform vec3 colors[4];

out vec4 vsColor;

void main()
{
vsColor = vec4(colors[gl_VertexID], 1.0f);
gl_Position = vec4(position, 1.0f);
}