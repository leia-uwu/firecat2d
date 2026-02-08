/*
    This file is part of the firecat2d project.
    SPDX-License-Identifier: LGPL-3.0-only
    SPDX-FileCopyrightText: 2026 firecat2d developers
*/

#include "fc/client/render/gl/shader.h"

#include <glad/gl.h>
#include <iostream>

Shader& Shader::use()
{
    glUseProgram(this->id);
    return *this;
}

static void checkCompileErrors(GLuint object, bool isProgram, const std::string& type)
{
    GLint success;

    char infoLog[1024];
    if (!isProgram) {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);

        if (success != GL_TRUE) {
            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
            std::cerr << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- \n";
        }
    } else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);

        if (success != GL_TRUE) {
            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            std::cerr << "| ERROR::SHADER: Link-time error: Type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- \n";
        }
    }
}

void Shader::compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    GLuint sVertex, sFragment, gShader;

    // vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, nullptr);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, false, "VERTEX");

    // fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, nullptr);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, false, "FRAGMENT");

    // if geometry shader source code is given, also compile geometry shader
    if (geometrySource != nullptr) {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, nullptr);
        glCompileShader(gShader);
        checkCompileErrors(gShader, false, "GEOMETRY");
    }

    // shader program
    this->id = glCreateProgram();
    glAttachShader(this->id, sVertex);
    glAttachShader(this->id, sFragment);
    if (geometrySource != nullptr) {
        glAttachShader(this->id, gShader);
    }
    glLinkProgram(this->id);
    checkCompileErrors(this->id, true, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != nullptr) {
        glDeleteShader(gShader);
    }
}

void Shader::setFloat(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(this->id, name), value);
}

void Shader::setInt(const char* name, GLint value) const
{
    glUniform1i(glGetUniformLocation(this->id, name), value);
}

void Shader::setVec2(const char* name, const Vec2F& vec) const
{
    glUniform2f(glGetUniformLocation(this->id, name), vec.x, vec.y);
}

void Shader::setVec2(const char* name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(this->id, name), x, y);
}

void Shader::setVec3(const char* name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(this->id, name), x, y, z);
}

void Shader::setVec4(const char* name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w);
}

void Shader::setMatrix3(const char* name, const Matrix3x3& matrix) const
{
    glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, GL_FALSE, matrix.arr.data());
}
