#version 120

uniform sampler2D tex;

varying vec2 frag_tex_coords;
varying vec4 frag_color;

varying vec4 color_mix_color;
varying float color_mix_value;

void main()
{
    //gl_FragColor = mix(texture2D(tex, frag_tex_coords) * frag_color, color_mix_color, color_mix_value);
    
    vec4 final_color = texture2D(tex, frag_tex_coords) * frag_color;
    vec3 mixed_color_rgb = mix(final_color.rgb, color_mix_color.rgb, color_mix_value);
    gl_FragColor = vec4(mixed_color_rgb.rgb, final_color.a);
}