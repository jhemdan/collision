#version 120

uniform sampler2D tex;

varying vec2 texCoords;

void main()
{
    gl_FragColor = texture2D(tex, texCoords);
    //gl_FragColor = vec4(1, 0, 1, 0);
}