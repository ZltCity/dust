#version 320 es

// Turn triangles into three triangles by vertices.
layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

precision highp float;

//in lowp vec3 vAmbDif[];
//out lowp vec3 fragColor;

void pushVertex(int i)
{
    gl_Position = gl_in[i].gl_Position;
    EmitVertex();
}

void main() {
    pushVertex(0);
    pushVertex(1);
    pushVertex(2);
    pushVertex(0);

    EndPrimitive();
}