#version 430

uniform sampler2D textureData;
uniform usampler2D textureData2;

in vec2 UV;

out vec4 color;

void main() 
{
    vec3 c = texture(textureData, UV).xyz;
    uvec3 c2 = texture(textureData2, UV).xyz;
    color = vec4(c, 1.0);
}