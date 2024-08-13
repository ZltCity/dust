#version 320 es

precision highp float;

// in vec2 texCoord;

// uniform sampler2D diffuse;

in vec3 vs_normal;

layout(location = 0) out vec4 out_color;

void main()
{
    vec3 ldir = vec3(0.0, 1.0, 0.0);
    vec3 baseColor = vec3(1.0, 1.0, 1.0);
    vec3 ambient = vec3(0.2, 0.2, 0.2);
    float gamma = 2.2;
    vec3 color = clamp(baseColor * dot(ldir, vs_normal) + ambient, 0.0, 1.0);
	out_color = vec4(pow(color, vec3(1.0 / gamma)), 1.0);

//         FragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
// 	out_color = texture(diffuse, texCoord);
//     out_color = vec4(1.0, 1.0, 1.0, 1.0);
}