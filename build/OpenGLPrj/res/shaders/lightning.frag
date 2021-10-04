#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture6;

void main()
{
        vec4 texColor= texture(texture6, TexCoord);
        if(texColor.a < 0.1)
                discard;
        FragColor=texColor;
}