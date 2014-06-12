#version 150

in vec3 position;
in vec4 color;
out vec4 out_color;
uniform mat4 MVP;

void main()
{
    vec4 v = vec4(position, 1.0);
    out_color = color;
    gl_Position = MVP * v;
}
