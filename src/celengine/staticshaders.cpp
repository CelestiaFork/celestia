#include <string>
#include <iostream>
#include <fstream>
#include "staticshaders.h"

using namespace std;

static GLProgram* LoadGLSLProgram(const string& name);

namespace glsl
{
    GLProgram* starDisc = nullptr;
    GLProgram* starNew  = nullptr;
};


bool glsl::initGLSL()
{
#if defined(_DEBUG) || defined(DEBUG) || 1
    // Only write to shader log file if this is a debug build
    if (g_shaderLogFile == NULL)
#ifdef _WIN32
        g_shaderLogFile = new ofstream("shaders.log");
#else
        g_shaderLogFile = new ofstream("/tmp/celestia-shaders.log");
#endif
#endif
    starDisc = LoadGLSLProgram("star");
    starNew  = LoadGLSLProgram("star_new");
    return true;
}

static string* ReadTextFromFile(const string& filename)
{
    ifstream textFile(filename, ios::in);
    if (!textFile.good())
        return nullptr;

    string *s = new string;

    char c;
    while (textFile.get(c))
        *s += c;

    return s;
}


static GLProgram*
LoadGLSLProgram(const string& name)
{
    cout << "Loading GLSL shader: " << name << '\n';

    string *vs_source = ReadTextFromFile("/home/globus/work/devel/Celestia/shaders/" + name + ".vert");
    if (!vs_source)
    {
        cout << "Error loading vertex program: " << name << ".vert\n";
        return nullptr;
    }

    string *fs_source = ReadTextFromFile("/home/globus/work/devel/Celestia/shaders/" + name + ".frag");
    if (!fs_source)
    {
        delete vs_source;
        cout << "Error loading fragment program: " << name << ".frag\n";
        return nullptr;
    }

    GLProgram* prog = nullptr;
    GLShaderStatus status;
    status = GLShaderLoader::CreateProgram(*vs_source, *fs_source, &prog);

    if (status == ShaderStatus_OK)
        status = prog->link();

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
        cout << "Failed to compile and link shader " << name << "!\n";
    }

    delete vs_source;
    delete fs_source;

    return prog;
}
