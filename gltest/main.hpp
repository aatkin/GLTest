#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <ctime>
#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GL/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

void errorCallback(int error, const char *description);
GLFWwindow* create_glfw();
GLuint loadShaderFromFile( std::string path, GLenum shaderType );
#endif // MAIN_HPP_INCLUDED
