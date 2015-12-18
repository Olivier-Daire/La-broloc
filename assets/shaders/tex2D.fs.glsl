#version 330

in vec2 TexCoords;

out vec3 fFragColor;

uniform sampler2D uTexture;

void main() {
	vec4 texture = texture(uTexture, TexCoords);
	// FIXME won't take the right texture (use nanosuit's one)
	fFragColor =  vec3(texture);	
   
}
