#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_4_5_Core>
#include <includedlib.h>

class Shader : protected QOpenGLFunctions_4_5_Core
{
public:
    Shader(std::string vertexPath, std::string fragmentPath);

    GLuint programHandle;

    void Use();
};


#endif // SHADER_H
