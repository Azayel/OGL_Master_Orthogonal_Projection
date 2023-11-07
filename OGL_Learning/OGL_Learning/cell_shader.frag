
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in float v_TextureID;

uniform sampler2D ourTexture;

void main()
{
    //FragColor = vec4(ourCol+acol,1.0f);
    FragColor = texture(ourTexture, TexCoord);
}