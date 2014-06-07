#include "../include/main.hpp"

/**
 *  Error-Callback function for GLFW.
 */
void errorCallback(int error, const char *description)
{
    std::cerr << description << " : " << error << std::endl;
}

/**
 * Create a GLFW-controlled window that OpenGL can use, and return a pointer to it.
 */
GLFWwindow *create_glfw()
{
    glfwInit();

    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello wurld", nullptr, nullptr); // Windowed

    return window;
}

GLuint loadShaderFromFile( std::string path, GLenum shaderType )
{
    //Open file
    GLuint shaderID = 0;
    std::string shaderString;
    std::ifstream sourceFile( path.c_str() );

    //Source file loaded
    if( sourceFile )
    {
        //Get shader source
        shaderString.assign( ( std::istreambuf_iterator< char >( sourceFile ) ), std::istreambuf_iterator< char >() );
        //Create shader ID
        shaderID = glCreateShader( shaderType );

        //Set shader source
        const GLchar* shaderSource = shaderString.c_str();
        glShaderSource( shaderID, 1, (const GLchar**)&shaderSource, NULL );

        //Compile shader source
        glCompileShader( shaderID );

        //Check shader for errors
        GLint shaderCompiled = GL_FALSE;
        glGetShaderiv( shaderID, GL_COMPILE_STATUS, &shaderCompiled );
        if( shaderCompiled != GL_TRUE )
        {
            std::cout << "Unable to compile shader: " << shaderID << ", source: " << shaderSource << std::endl;
            glDeleteShader( shaderID );
            shaderID = 0;
        }
    }
    else
    {
        printf( "Unable to open file %s\n", path.c_str() );
    }

    return shaderID;
}

int main()
{
    /** Create window and context for OpenGL to draw in. */
    GLFWwindow* window = create_glfw();
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    /** Vertex Array Object */
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);

    float vertices[] =
    {
        0.05f,  0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;

    /** Load shaders for OpenGL to use. */
    GLuint vertexShader = loadShaderFromFile("shaders/triangle.vert", GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShaderFromFile("shaders/triangle.frag", GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    /** Main loop of the program. */
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);
    return 0;
}
