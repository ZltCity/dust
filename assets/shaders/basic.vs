#version 320 es

precision highp float;

// layout(location = 0) in vec3 in_pos;
// layout(location = 1) in vec2 in_texCoord;

layout(binding = 0, std430) readonly buffer PositionBuffer {
    float in_position[];
};

layout(std140) uniform Transform {
    mat4 projection, view, model;
};

// out vec2 vs_texCoord;

vec3 getPosition(int index) {
    return vec3(
        in_position[index * 3],
        in_position[index * 3 + 1],
        in_position[index * 3 + 2]
    );
}

void main()
{
	// vec4 position = projection * view * model * vec4(in_pos, 1.0);

    gl_Position = projection * view * model * vec4(getPosition(gl_VertexID), 1.0);
	// gl_Position = position;
	// vs_texCoord = in_texCoord;
}