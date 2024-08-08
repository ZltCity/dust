#version 320 es

precision highp float;

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_texCoord;

layout(std140) uniform Transform {
    mat4 projection, modelview;
};

out vec2 vs_texCoord;

void main()
{
	vec4 position = projection * modelview * vec4(in_pos, 1.0);

	gl_Position = position;
	vs_texCoord = in_texCoord;
}