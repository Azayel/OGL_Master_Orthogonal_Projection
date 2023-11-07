#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 atexture;
layout (location = 2) in float textureID;


uniform mat4 projection;	
uniform mat4 translation;


out vec2 TexCoord;
out float v_TextureID;

void main()
{
	mat4 model = translation;
	mat4 MVP = projection * model;
   gl_Position = MVP * vec4(aPos, 1.0);
   TexCoord = atexture;
   v_TextureID = textureID;
}