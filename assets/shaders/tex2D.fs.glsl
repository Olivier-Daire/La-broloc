#version 330

in vec2 vTexture;

out vec3 fFragColor;

uniform vec3 uColor;
uniform sampler2D uTexture;

void main() {
	vec4 texture = texture(uTexture, vTexture);
	if (uColor == vec3(0.98,0.58,0.57))
	{
		fFragColor = uColor;//vec3(texture)
	} else {
		fFragColor = vec3(texture);	
	}
    
}