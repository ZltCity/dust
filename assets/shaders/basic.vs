#version 320 es

precision highp float;

layout(std430, binding = 0) readonly buffer PositionBuffer {
    float in_position[];
};

layout(std430, binding = 1) readonly buffer TexCoordBuffer {
    float in_texCoord[];
};

layout(std430, binding = 2) readonly buffer NormalBuffer {
    float in_normal[];
};

struct InstanceInfo {
    mat4 transform;
    int positionOffset;
    int texCoordOffset;
    int normalOffset;
};

layout(std430, binding = 6) readonly buffer InstanceInfoBuffer {
    InstanceInfo instance[];
};

layout(std140) uniform View {
    mat4 projection, view;
};

uniform int drawID;

mat4 getTransform() {
    return instance[drawID].transform;
};

vec3 getPosition(int index) {
    int offset = instance[drawID].positionOffset;

    return vec3(
        in_position[(offset + index) * 3],
        in_position[(offset + index) * 3 + 1],
        in_position[(offset + index) * 3 + 2]
    );
}

vec2 getTexCoord(int index) {
    int offset = instance[drawID].texCoordOffset;

    return vec2(
        in_texCoord[(offset + index) * 3],
        in_texCoord[(offset + index) * 3 + 1]
    );
}

vec3 getNormal(int index) {
    int offset = instance[drawID].normalOffset;

    return vec3(
        in_normal[(offset + index) * 3],
        in_normal[(offset + index) * 3 + 1],
        in_normal[(offset + index) * 3 + 2]
    );
}

out vec3 vs_normal;

void main()
{
	gl_Position = projection * view * getTransform() * vec4(getPosition(gl_VertexID), 1.0);
	vs_normal = getNormal(gl_VertexID);
}