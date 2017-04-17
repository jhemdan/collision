#version 120

attribute vec2 position;
attribute vec2 tex_coords;
attribute vec4 color;

uniform mat4 mvp;
uniform vec2 quad_size;

varying vec2 frag_tex_coords;
varying vec4 frag_color;

void main()
{
    gl_Position = mvp * vec4(position * quad_size, 0, 1);
    
    frag_tex_coords = tex_coords;
    frag_color = color;
}