#pragma once
#include "glm/glm.hpp"
#include "ft2build.h"
#include <glad/glad.h>
#include <string>
#include <map>
#include <iostream>
#include FT_FREETYPE_H

class Shader;

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class Text {
private:
    Shader* textShader;
    std::string font_name;// = "res/font/arial.ttf";
    std::map<GLchar, Character> Characters;
    unsigned int VAO, VBO;
public:
    Text(Shader* shader, std::string font);
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
};