#version 120

uniform sampler2D tex;

varying vec2 frag_tex_coords;
varying vec4 frag_color;

void main()
{
    gl_FragColor = texture2D(tex, frag_tex_coords) * frag_color;
}