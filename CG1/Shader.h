#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {
public:
    // id шейдерной программы
    unsigned int ID;



    //читает и собирает шейдеры из файлов
    Shader(const char* vertexPath, const char* fragmentPath);

    ~Shader();

    void use();

    //uniform-функции
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, float x, float y);
    void setVec3(const std::string& name, float x, float y, float z);
    void setVec4(const std::string& name, float x, float y, float z, float w);

    void setMat4(const std::string& name, const glm::mat4& mat);

    //установка цвета одной строкой
    void setColor(const std::string& name, float r, float g, float b, float a = 1.0f);

private:
    void checkCompileErrors(unsigned int shader, std::string type);
    std::string readFile(const char* path);
};

#endif
