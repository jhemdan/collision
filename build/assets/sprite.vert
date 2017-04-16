#version 120

attribute vec2 vertex;
attribute vec2 uv;

uniform mat4 mvp;

varying vec2 texCoords;

void main()
{
    gl_Position = mvp * vec4(vertex, 0, 1);
    
    texCoords = uv;
}