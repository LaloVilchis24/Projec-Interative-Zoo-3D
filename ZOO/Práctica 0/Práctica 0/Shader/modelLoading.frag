#version 330 core
out vec4 FragColor;

uniform sampler2D texture_diffuse;
in vec2 TexCoords;

void main()
{
    vec4 texColor = texture(texture_diffuse, TexCoords);
    if (texColor.a < 0.1)
        discard;
    FragColor = texColor;
}
