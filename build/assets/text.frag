#version 120

uniform sampler2D tex;

varying vec2 frag_tex_coords;
varying vec4 frag_color;

void main()
{
    gl_FragColor = vec4(1, 1, 1, texture2D(tex, frag_tex_coords).r) * frag_color;
}