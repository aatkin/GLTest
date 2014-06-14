#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#define PI 3.141592653589793234

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void errorCallback(int error, const char *description);
GLFWwindow *create_glfw(char* title);
GLuint loadShaderFromFile( std::string path, GLenum shaderType );
#endif // MAIN_HPP_INCLUDED
