#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;


out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int wall;

void main()
{
	vec3 normal;
	// front wall
	if(wall == 0) normal = vec3(0,0,1);
	//left wall
	else if (wall == 1) normal = vec3(1,0,0);
	//right wall
	else if (wall == 2) normal = vec3(1,0,0);
	//back wall
	else if (wall == 3) normal = vec3(0,0,1);
	//floor
	else if (wall == 4) normal = vec3(0,1,0);
	//roof
	else if (wall == 5) normal = vec3(0,1,0);
	else normal = vec3(0,0,0);

    gl_Position = projection * view * model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;
    TexCoords = texCoords;
}
