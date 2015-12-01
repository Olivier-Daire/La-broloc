#version 330

layout(location = 3) in vec2 aVertexPosition;
layout(location = 8) in vec2 aVertexTexture;

out vec2 vTexture;

uniform mat3 uModelMatrix;

void main() {
   	vTexture = aVertexTexture;
	gl_Position = vec4( (uModelMatrix*vec3(aVertexPosition,1)).xy, 0, 1);
}