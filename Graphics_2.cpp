
#include "Source.cpp"

Pipeline p;

GLuint IBO; // index buffer object

GLuint VBO; // 32-битовое беззнаковое целое
// VBO - Vertex_Buffer_Object

GLuint gWorldLocation;

// исходный код вершинного шейдера
const char* VScode =
"#version 330 core\n"
"layout (location = 0) in vec3 Position;\n"
"out vec4 Color;\n" // выходное значение
"uniform mat4 gWorld;\n"
"void main()\n"
"{\n"
    "gl_Position = gWorld * vec4(Position, 1.0);\n"
    "Color = vec4(clamp(Position, 0.0, 1.0), 1.0);\n"
"};\n";

// исходный код пиксельного (фрагментного) шейдера
const char* PScode =
"#version 330 core\n"
"in vec4 Color;\n" // входное значение
"out vec4 FragColor;\n" // выходное значение
"void main()\n"
"{\n"
    "FragColor = Color;\n"
"}\n";


void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    static float Scale = 0.0f;
    Scale += 0.001f;
    
    // матрицы
    glm::mat4 MatrixYZ(
        cosf(Scale), 0.0f, -sinf(Scale), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sinf(Scale), 0.0f, cosf(Scale), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    glm::mat4 MatrixXZ(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cosf(Scale), -sinf(Scale), 0.0f,
        0.0f, sinf(Scale), cosf(Scale), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    glm::mat4 MatrixXY(
        cosf(Scale), -sinf(Scale), 0.0f, 0.0f,
        sinf(Scale), cosf(Scale), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    glm::mat4 MatrixScale(
        sinf(Scale), 0.0f, 0.0f, 0.0f,
        0.0f, cosf(Scale), 0.0f, 0.0f,
        0.0f, 0.0f, sinf(Scale), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    glm::mat4 ResMatrix = MatrixYZ;

    p.Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
    p.WorldPos(sinf(Scale), 0.0f, 0.0f);
    p.Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());

    glEnableVertexAttribArray(0);
    glColor3f(0.3f, 0.8f, 0.6f);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);

    glutSwapBuffers();

    glutPostRedisplay();
}

void CreateShader(GLuint ShaderProgram, const char* pShaderText, GLint shaderType) {


    GLuint ShaderObj = glCreateShader(shaderType);
    // создание шейдера

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0] = strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, sizeof(InfoLog), NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, InfoLog);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

void LinkShader(GLuint ShaderProgram)
{
    GLint success;

    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (success == 0) {
        GLchar ErrorLog[1024];
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
    }

    glValidateProgram(ShaderProgram);

    glUseProgram(ShaderProgram);
}

int main(int argc, char** argv)
{

    glutInit(&argc, argv); // инициализация GLUT

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // двойная буферизация, rgba
    glutInitWindowPosition(100, 100); // начальное расположение окна
    glutInitWindowSize(1024, 768); // начальный размер окна
    glutCreateWindow("Tutorial 01");
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f); // указывает четкие значения для буферов цветов (очистка буферов цветов)

    GLenum res = glewInit(); //GLenum -- 32-битовое беззнаковое целое
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    glm::vec3 Vertices[4]{
        {-1.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 1.0f},
        {1.0f, -1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
    };

    unsigned int Indices[] = { 0, 3, 1,
                           1, 3, 2,
                           2, 3, 0,
                           0, 2, 1 };

    GLuint ShaderProgram = glCreateProgram();
    CreateShader(ShaderProgram, VScode, GL_VERTEX_SHADER);
    CreateShader(ShaderProgram, PScode, GL_FRAGMENT_SHADER);
    LinkShader(ShaderProgram);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);

    glutMainLoop();

    return 0;
}
