#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 ourColor; // we set this variable in the OpenGL code.

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = ourColor;
}