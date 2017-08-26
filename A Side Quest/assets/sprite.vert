#version 120

attribute vec2 position;
attribute vec2 tex_coords;
attribute vec4 color;

uniform mat4 mvp;
uniform vec2 quad_size;
uniform vec4 my_color;

uniform vec4 my_color_mix_color;
uniform float my_color_mix_value;

varying vec2 frag_tex_coords;
varying vec4 frag_color;

varying vec4 color_mix_color;
varying float color_mix_value;

void main()
{
    gl_Position = mvp * vec4(position * quad_size, 0, 1);
    
    frag_tex_coords = tex_coords;
    frag_color = color * my_color;
    
    color_mix_color = my_color_mix_color;
    color_mix_value = my_color_mix_value;
}