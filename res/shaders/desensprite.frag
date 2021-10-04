// #version 330 core
// out vec4 FragColor;
// void main()
// {
//         FragColor = vec4(1.18f, 0.55f, 1.78f, 0.0f);
// }
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture3;
uniform sampler2D texture2;

void main()
{
        // linearly interpolate between both textures (80% container, 20% awesomeface)
        vec4 texColor = mix(texture(texture3, TexCoord), texture(texture2, TexCoord), 0.9);
        if(texColor.a < 0.1)
                discard;
        FragColor=texColor;
}