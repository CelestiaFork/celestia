// fragmentprog.cpp
//
// Copyright (C) 2003 Chris Laurel <claurel@shatters.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include <iostream>
#include <fstream>
#include <string>
#include <celutil/util.h>
#include <GL/glew.h>
#include "fragmentprog.h"

using namespace std;


unsigned int fp::sphereShadowOnRings = 0;
unsigned int fp::eclipseShadow1      = 0;
unsigned int fp::eclipseShadow2      = 0;
unsigned int fp::texDiffuse          = 0;
unsigned int fp::texDiffuseBump      = 0;
unsigned int fp::texSpecular         = 0;
unsigned int fp::texSpecularAlpha    = 0;


class FragmentProcessorARB : public FragmentProcessor
{
 public:
    FragmentProcessorARB();
    virtual ~FragmentProcessorARB();

    virtual void enable();
    virtual void disable();
    virtual void use(unsigned int);
    virtual void parameter(fp::Parameter, float, float, float, float);
    virtual void parameter(fp::Parameter, const float*);
};


#if 0
static string* ReadTextFromFile(const string& filename)
{
    ifstream textFile(filename.c_str(), ios::in);
    if (!textFile.good())
        return NULL;

    string* s = new string();

    char c;
    while (textFile.get(c))
        *s += c;

    return s;
}
#endif

#if 0
static int findLineNumber(const string& s, unsigned int index)
{
    if (index >= s.length())
        return -1;

    int lineno = 1;
    for (unsigned int i = 0; i < index; i++)
    {
        if (s[i] == '\n')
            lineno++;
    }

    return lineno;
}
#endif


FragmentProcessor* fp::initARB()
{
    cout << _("Initializing ARB fragment programs . . .\n");

    return new FragmentProcessorARB();
}


FragmentProcessor::FragmentProcessor()
{
}

FragmentProcessor::~FragmentProcessor()
{
}

void FragmentProcessor::parameter(fp::Parameter param, const Eigen::Vector3f& v)
{
    parameter(param, v.x(), v.y(), v.z(), 0.0f);
}

void FragmentProcessor::parameter(fp::Parameter param, const Eigen::Vector4f& v)
{
    parameter(param, v.x(), v.y(), v.z(), v.w());
}

void FragmentProcessor::parameter(fp::Parameter param, const Color& c)
{
    parameter(param, c.red(), c.green(), c.blue(), c.alpha());
}



// FragmentProcessorARB implementation

FragmentProcessorARB::FragmentProcessorARB()
{
}

FragmentProcessorARB::~FragmentProcessorARB()
{
}

void FragmentProcessorARB::enable()
{
    //glEnable(GL_FRAGMENT_PROGRAM_ARB);
}

void FragmentProcessorARB::disable()
{
    //glDisable(GL_FRAGMENT_PROGRAM_ARB);
}

void FragmentProcessorARB::use(unsigned int /*prog*/)
{
    //glBindProgramARB(GL_VERTEX_PROGRAM_ARB, prog);
}

void FragmentProcessorARB::parameter(fp::Parameter /*param*/,
                                     float /*x*/, float /*y*/,
                                     float /*z*/, float /*w*/)
{
    //glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, param, x, y, z, w);
}

void FragmentProcessorARB::parameter(fp::Parameter /*param*/, const float* /*fv*/)
{
    //glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB, param, fv);
}
