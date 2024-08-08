#version 320 es

precision highp float;

// Turn triangles into three triangles by vertices.
layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

in vec2 vs_texCoord[];
out vec2 texCoord;

//in lowp vec3 vAmbDif[];
//out lowp vec3 fragColor;

void pushVertex(int i)
{
    gl_Position = gl_in[i].gl_Position;
    texCoord = vs_texCoord[i];
    EmitVertex();
}

void main() {
    pushVertex(0);
    pushVertex(1);
    pushVertex(2);
    pushVertex(0);

    EndPrimitive();
}