#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void errorCallback(int error, const char *description);
GLFWwindow *create_glfw();
GLuint loadShaderFromFile( std::string path, GLenum shaderType );
#endif // MAIN_HPP_INCLUDED
