// boundaries.cpp
//
// Copyright (C) 2002-2009, the Celestia Development Team
// Original version by Chris Laurel <claurel@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include <cassert>
#include <numeric>
#include "boundaries.h"
#include "astro.h"
#include <GL/glew.h>
#include "staticshaders.h"

using namespace Eigen;
using namespace std;


static const float BoundariesDrawDistance = 10000.0f;


ConstellationBoundaries::ConstellationBoundaries() :
    currentChain(nullptr)
{
    currentChain = new Chain();
    currentChain->push_back(Vector3f::Zero());
}

ConstellationBoundaries::~ConstellationBoundaries()
{
    cleanup();
}

void ConstellationBoundaries::cleanup()
{
    for (auto const chain : chains)
        delete chain;
    chains.clear();

    delete currentChain;
    currentChain = nullptr;

    delete[] vtx_buf;
    vtx_buf = nullptr;
}


void ConstellationBoundaries::moveto(float ra, float dec)
{
    assert(currentChain != nullptr);

    Vector3f v = astro::equatorialToEclipticCartesian(ra, dec, BoundariesDrawDistance);
    if (currentChain->size() > 1)
    {
        chains.insert(chains.end(), currentChain);
        currentChain = new Chain();
        currentChain->push_back(v);
    }
    else
    {
        (*currentChain)[0] = v;
    }
}


void ConstellationBoundaries::lineto(float ra, float dec)
{
    currentChain->push_back(astro::equatorialToEclipticCartesian(ra, dec, BoundariesDrawDistance));
}


void ConstellationBoundaries::render(Color color, float opacity)
{
    if (vboId == 0)
    {
        if (!prepared)
            prepare();
        glGenBuffers(1, &vboId);
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, vtx_num*3*sizeof(GLfloat), vtx_buf, GL_STATIC_DRAW);
        cleanup();
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
    }

    if (glsl::simple)
    {
        glsl::simple->use();
        Vec3ShaderParameter(glsl::simple->getID(), "color") = color.toVector3();
        FloatShaderParameter(glsl::simple->getID(), "opacity") = opacity;
    }

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, vtx_num);
    glDisableVertexAttribArray(0);

    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ConstellationBoundaries::prepare()
{
    vtx_num = accumulate(chains.begin(), chains.end(), 0,
                         [](int a, Chain* b) { return a + b->size(); });

    // as we use GL_LINES we should double the number of vertices
    vtx_num *= 2;

    vtx_buf = new GLfloat[vtx_num * 3];
    GLfloat* ptr = vtx_buf;
    for (const auto chain : chains)
    {
        memcpy(ptr, (*chain)[0].data(), sizeof(GLfloat) * 3);
        ptr += 3;
        for(unsigned i = 1; i < chain->size(); i++)
        {
            memcpy(ptr, (*chain)[i].data(), sizeof(GLfloat) * 3);
            ptr += 3;
	    memcpy(ptr, (*chain)[i].data(), sizeof(GLfloat) * 3);
            ptr += 3;
        }
        memcpy(ptr, (*chain)[0].data(), sizeof(GLfloat) * 3);
        ptr += 3;
    }
    prepared = true;
}

ConstellationBoundaries* ReadBoundaries(istream& in)
{
    ConstellationBoundaries* boundaries = new ConstellationBoundaries();
    string lastCon;
    int conCount = 0;
    int ptCount = 0;

    for (;;)
    {
        float ra = 0.0f;
        float dec = 0.0f;
        in >> ra;
        if (!in.good())
            break;
        in >> dec;

        string pt;
        string con;

        in >> con;
        in >> pt;
        if (!in.good())
            break;

        if (con != lastCon)
        {
            boundaries->moveto(ra, dec);
            lastCon = con;
            conCount++;
        }
        else
        {
            boundaries->lineto(ra, dec);
        }
        ptCount++;
    }

    return boundaries;
}
