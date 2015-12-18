#pragma once 

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "shader.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <string>
#include <ft2build.h>
#include "scene.hpp"
#include FT_FREETYPE_H

class Text {

	public: 

		Text();
		void LoadText(Shader &shader,GLuint width,GLuint height);
		void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
		void nextText(bool &isAnswer, bool &answer,int &cptDialogue, Scene scene1, std::string &dialogue, std::string answers[]);
 
	private:

		/// Holds all state information relevant to a character as loaded using FreeType
		struct Character {
		    GLuint TextureID;   // ID handle of the glyph texture
		    glm::ivec2 Size;    // Size of glyph
		    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
		    GLuint Advance;    // Horizontal offset to advance to next glyph
		};

		std::string dialogue;
		std::map<GLchar, Character> Characters;
		GLuint VAO, VBO;


};