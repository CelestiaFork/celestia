#include <string>
#include <iostream>
#include <fstream>
#include "staticshaders.h"

using namespace std;

static GLProgram* LoadGLSLProgram(const string& name);

namespace glsl
{
    GLProgram* starDisc = nullptr;
};


bool glsl::initGLSL()
{
    starDisc = LoadGLSLProgram("star");
    return true;
}

static string* ReadTextFromFile(const string& filename)
{
    ifstream textFile(filename, ios::in);
    if (!textFile.good())
        return nullptr;

    string* s = new string;

    char c;
    while (textFile.get(c))
        *s += c;

    return s;
}


static GLProgram*
LoadGLSLProgram(const string& name)
{
    GLProgram* prog = nullptr;
    GLShaderStatus status;

    GLVertexShader* vs = nullptr;
    GLFragmentShader* fs = nullptr;

    string* source;

    source = ReadTextFromFile("shader/" + name + ".vert");
    GLShaderLoader::CreateVertexShader(*source, &vs);
    delete source;

    source = ReadTextFromFile("shader/" + name + ".frag");
    GLShaderLoader::CreateFragmentShader(*source, &fs);
    delete source;

    if (vs != nullptr && fs != nullptr)
    {
        status = GLShaderLoader::CreateProgram(*vs, *fs, &prog);
        if (status == ShaderStatus_OK)
            status = prog->link();
    }
    else
    {
        status = ShaderStatus_CompileError;
    }

    delete vs;
    delete fs;

    if (status != ShaderStatus_OK)
    {
        // If the shader creation failed for some reason, substitute the
        // error shader.
/*
        status = GLShaderLoader::CreateProgram(errorVertexShaderSource,
                                               errorFragmentShaderSource,
                                               &prog);
        if (status != ShaderStatus_OK)
        {
            if (g_shaderLogFile != nullptr)
                *g_shaderLogFile << "Failed to create error shader!\n";
        }
        else
        {
            status = prog->link();
        }
*/
        clog << "Failed to read shader " << name << "!\n";
    }

    return prog;
}
