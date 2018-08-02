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
#include "boundaries.h"
#include "astro.h"
#include <GL/glew.h>
#include "vecgl.h"

using namespace Eigen;
using namespace std;


static const float BoundariesDrawDistance = 10000.0f;


ConstellationBoundaries::ConstellationBoundaries() :
    currentChain(NULL)
{
    currentChain = new Chain();
    currentChain->v.push_back(Vector3f::Zero());
}

ConstellationBoundaries::~ConstellationBoundaries()
{
    for (vector<Chain*>::iterator iter = chains.begin();
         iter != chains.end(); iter++)
    {
        delete *iter;
    }

    delete currentChain;
}


void ConstellationBoundaries::moveto(float ra, float dec)
{
    assert(currentChain != NULL);

    Vector3f v = astro::equatorialToEclipticCartesian(ra, dec, BoundariesDrawDistance);
    if (currentChain->size() > 1)
    {
        chains.insert(chains.end(), currentChain);
        currentChain = new Chain();
        currentChain->v.push_back(v);
    }
    else
    {
        currentChain->v[0] = v;
    }
}


void ConstellationBoundaries::lineto(float ra, float dec)
{
    currentChain->v.push_back(astro::equatorialToEclipticCartesian(ra, dec, BoundariesDrawDistance));
}


void ConstellationBoundaries::render()
{
#ifndef UseOpenGL
    glEnableClientState(GL_VERTEX_ARRAY);
#endif

    for (const auto& chain : chains)
    {
#ifdef UseOpenGL
        glBegin(GL_LINE_STRIP);
        for (const auto& vertex: chain->v)
        {
            glVertex3fv(vertex.data());
        }
        glEnd();
#else
        size_t chain_size = chain->size();
        if (!chain->vtx)
        {
            chain->vtx = new GLfloat[chain_size * 3];
            for (size_t i = 0, j = 0; i < chain_size; i++, j+=3)
            {
                memcpy(&(chain->vtx[j]), chain->v[i].data(), 3*sizeof(GLfloat));
            }
        }
        glVertexPointer(3, GL_FLOAT, 0, chain->vtx);
        glDrawArrays(GL_LINE_STRIP, 0, chain_size);
#endif
    }
#ifndef UseOpenGL
    glDisableClientState(GL_VERTEX_ARRAY);
#endif
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


