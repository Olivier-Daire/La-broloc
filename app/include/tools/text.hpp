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
		void nextText(bool &isDialogue, bool &isAnswer, bool &answer, int &cptDialogue, Scene scene, std::string &dialogue, std::string answers[], int group);
 		void Draw(Shader shaderText,bool isDialogue, bool isAnswer,int chooseAnswer,std::string dialogue,std::string answers[]);
 		void DrawHint(Shader shaderText);
 		void drawHome(Shader shaderText,float screenwith, float screenheight);
 		void drawCommand(Shader shaderText);
   
	private:

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
