// marker.h
//
// Copyright (C) 2003, Chris Laurel <claurel@shatters.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include "marker.h"
#include <GL/glew.h>


using namespace std;


Marker::Marker(const Selection& s) :
    m_object(s),
    m_priority(0),
    m_representation(MarkerRepresentation::Diamond),
    m_occludable(true),
    m_sizing(ConstantSize)
{
}

Marker::~Marker()
{
}


UniversalCoord Marker::position(double jd) const
{
    return m_object.getPosition(jd);
}


Selection Marker::object() const
{
    return m_object;
}


int Marker::priority() const
{
    return m_priority;
}


void Marker::setPriority(int priority)
{
    m_priority = priority;
}


void Marker::setRepresentation(const MarkerRepresentation& rep)
{
    m_representation = rep;
}


bool Marker::occludable() const
{
    return m_occludable;
}


void Marker::setOccludable(bool occludable)
{
    m_occludable = occludable;
}


MarkerSizing Marker::sizing() const
{
    return m_sizing;
}


void Marker::setSizing(MarkerSizing sizing)
{
    m_sizing = sizing;
}


void Marker::render(float size) const
{
    m_representation.render(m_sizing == DistanceBasedSize ? size : m_representation.size());
}


#ifdef UseOpenGL
static void DrawCircle(float s)
#else
static void DrawCircle(float s, GLenum style)
#endif
{
    if (s < 1.0f)
        s = 1.0f; //  0 and negative values are not allowed in the case of circle markers.
    else if (s > 1024.0f)
        s = 1024.0f; //  Bigger values would give a too high number of segments in the circle markers.

    int step = (int) (60 / sqrt(s));
#ifdef UseOpenGL
    for (int i = 0; i < 360; i += step)
#else
    auto vtx1 = new GLfloat[2 * 360 / step];
    for (int i = 0, j = 0; i < 360; i += step)
#endif
    {
        float degInRad = (float) (i * PI / 180);
#ifdef UseOpenGL
        glVertex3f((float) cos(degInRad) * s, (float) sin(degInRad) * s, 0.0f);
#else
        vtx1[j] = (float) cos(degInRad) * s; j++;
        vtx1[j] = (float) sin(degInRad) * s; j++;
#endif
    }

#ifndef UseOpenGL
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vtx1);
    glDrawArrays(style, 0, 360 / step);
    glDisableClientState(GL_VERTEX_ARRAY);
    delete[] vtx1;
#endif
}


MarkerRepresentation::MarkerRepresentation(const MarkerRepresentation& rep) :
    m_symbol(rep.m_symbol),
    m_size(rep.m_size),
    m_color(rep.m_color),
    m_label(rep.m_label)
{
}


MarkerRepresentation&
MarkerRepresentation::operator=(const MarkerRepresentation& rep)
{
    m_symbol = rep.m_symbol;
    m_size = rep.m_size;
    m_color = rep.m_color;
    m_label = rep.m_label;

    return *this;
}


void MarkerRepresentation::setColor(Color color)
{
    m_color = color;
}


void MarkerRepresentation::setSize(float size)
{
    m_size = size;
}


void MarkerRepresentation::setLabel(const std::string& label)
{
    m_label = label;
}


/*! Render the marker symbol at the specified size. The size is
 *  the diameter of the marker in pixels.
 */
void MarkerRepresentation::render(float size) const
{
    float s = size / 2.0f;

    switch (m_symbol)
    {
    case Diamond:
#ifdef UseOpenGL
        glBegin(GL_LINE_LOOP);
        glVertex3f(0.0f,    s, 0.0f);
        glVertex3f(   s, 0.0f, 0.0f);
        glVertex3f(0.0f,   -s, 0.0f);
        glVertex3f(  -s, 0.0f, 0.0f);
        glEnd();
#else
        {
            GLfloat vtx1[] = {
                0.0f,    s,
                   s, 0.0f,
                0.0f,   -s,
                  -s, 0.0f
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vtx1);
            glDrawArrays(GL_LINE_LOOP, 0, 4);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
#endif
        break;

    case Plus:
#ifdef UseOpenGL
        glBegin(GL_LINES);
        glVertex3f(0.0f,  s, 0.0f);
        glVertex3f(0.0f, -s, 0.0f);
        glVertex3f( s, 0.0f, 0.0f);
        glVertex3f(-s, 0.0f, 0.0f);
        glEnd();
#else
        {
            GLfloat vtx1[] = {
                0.0f,  s,
                0.0f, -s,
                 s, 0.0f,
                -s, 0.0f
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vtx1);
            glDrawArrays(GL_LINES, 0, 4);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
#endif
        break;

    case X:
#ifdef UseOpenGL
        glBegin(GL_LINES);
        glVertex3f(-s, -s, 0.0f);
        glVertex3f( s,  s, 0.0f);
        glVertex3f( s, -s, 0.0f);
        glVertex3f(-s,  s, 0.0f);
        glEnd();
#else
        {
            GLfloat vtx1[] = {
                -s, -s,
                 s,  s,
                 s, -s,
                -s,  s
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vtx1);
            glDrawArrays(GL_LINES, 0, 4);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
#endif
        break;

    case Square:
#ifdef UseOpenGL
        glBegin(GL_LINE_LOOP);
#endif

    case FilledSquare:
#ifdef UseOpenGL
        glBegin(GL_POLYGON);

        glVertex3f(-s, -s, 0.0f);
        glVertex3f( s, -s, 0.0f);
        glVertex3f( s,  s, 0.0f);
        glVertex3f(-s,  s, 0.0f);
        glEnd();
#else
        {
            GLfloat vtx1[] = {
                -s, -s,
                 s, -s,
                 s,  s,
                -s,  s
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vtx1);
            glDrawArrays(m_symbol == FilledSquare ? GL_POLYGON : GL_LINE_LOOP, 0, 4);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
#endif
        break;

    case Triangle:
#ifdef UseOpenGL
        glBegin(GL_LINE_LOOP);
        glVertex3f(0.0f,  s, 0.0f);
        glVertex3f(   s, -s, 0.0f);
        glVertex3f(  -s, -s, 0.0f);
        glEnd();
#else
        {
            GLfloat vtx1[] = {
                0.0f, s,
                 s,  -s,
                -s,  -s,
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vtx1);
            glDrawArrays(GL_LINE_LOOP, 0, 3);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
#endif
        break;

    case RightArrow:
#ifdef UseOpenGL
        glBegin(GL_POLYGON);
        glVertex3f(-3*s, float(s/3), 0.0f);
        glVertex3f(-3*s, float(-s/3), 0.0f);
        glVertex3f(-2*s, float(-s/4), 0.0f);
        glVertex3f(-2*s, float(s/4), 0.0f);
        glEnd();
        glBegin(GL_POLYGON);
        glVertex3f(-2*s, float(2*s/3), 0.0f);
        glVertex3f(-2*s, float(-2*s/3), 0.0f);
        glVertex3f(-s, 0.0f, 0.0f);
        glEnd();
#else
        {
            GLfloat vtx1[] = {
                -3*s, float(s/3),
                -3*s, float(-s/3),
                -2*s, float(-s/4),
                -2*s, float(s/4),
                -2*s, float(2*s/3),
                -2*s, float(-2*s/3),
                  -s, 0.0f
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vtx1);
            glDrawArrays(GL_POLYGON, 0, 4);
            glDrawArrays(GL_POLYGON, 4, 3);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
#endif
        break;

    case LeftArrow:
#ifdef UseOpenGL
        glBegin(GL_POLYGON);
        glVertex3f(3*s, float(-s/3), 0.0f);
        glVertex3f(3*s, float(s/3), 0.0f);
        glVertex3f(2*s, float(s/4), 0.0f);
        glVertex3f(2*s, float(-s/4), 0.0f);
        glEnd();
        glBegin(GL_POLYGON);
        glVertex3f(2*s, float(-2*s/3), 0.0f);
        glVertex3f(2*s, float(2*s/3), 0.0f);
        glVertex3f(s, 0.0f, 0.0f);
        glEnd();
#else
        {
            GLfloat vtx1[] = {
                3*s, float(-s/3),
                3*s, float(s/3),
                2*s, float(s/4),
                2*s, float(-s/4),
                2*s, float(-2*s/3),
                2*s, float(2*s/3),
                  s, 0.0f
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vtx1);
            glDrawArrays(GL_POLYGON, 0, 4);
            glDrawArrays(GL_POLYGON, 4, 3);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
#endif

        break;

    case UpArrow:
#ifdef UseOpenGL
        glBegin(GL_POLYGON);
        glVertex3f(float(-s/3), -3*s, 0.0f);
        glVertex3f(float(s/3), -3*s, 0.0f);
        glVertex3f(float(s/4), -2*s, 0.0f);
        glVertex3f(float(-s/4), -2*s, 0.0f);
        glEnd();
        glBegin(GL_POLYGON);
        glVertex3f(float(-2*s/3), -2*s, 0.0f);
        glVertex3f(float(2*s/3), -2*s, 0.0f);
        glVertex3f( 0.0f, -s, 0.0f);
        glEnd();
#else
        {
            GLfloat vtx1[] = {
                float(-s/3),   -3*s,
                float(s/3),    -3*s,
                float(s/4),    -2*s,
                float(-s/4),   -2*s,
                float(-2*s/3), -2*s,
                float(2*s/3),  -2*s,
                        0.0f,    -s
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vtx1);
            glDrawArrays(GL_POLYGON, 0, 4);
            glDrawArrays(GL_POLYGON, 4, 3);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
#endif

        break;

    case DownArrow:
#ifdef UseOpenGL
        glBegin(GL_POLYGON);
        glVertex3f(float(s/3), 3*s, 0.0f);
        glVertex3f(float(-s/3), 3*s, 0.0f);
        glVertex3f(float(-s/4), 2*s, 0.0f);
        glVertex3f(float(s/4), 2*s, 0.0f);
        glEnd();
        glBegin(GL_POLYGON);
        glVertex3f(float(2*s/3), 2*s, 0.0f);
        glVertex3f(float(-2*s/3), 2*s, 0.0f);
        glVertex3f( 0.0f, s, 0.0f);
        glEnd();
#else
        {
            GLfloat vtx1[] = {
                float(s/3),    3*s,
                float(-s/3),   3*s,
                float(-s/4),   2*s,
                float(s/4),    2*s,
                float(2*s/3),  2*s,
                float(-2*s/3), 2*s,
                         0.0f,   s
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vtx1);
            glDrawArrays(GL_POLYGON, 0, 4);
            glDrawArrays(GL_POLYGON, 4, 3);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
#endif

        break;

    case Circle:
#ifdef UseOpenGL
        glBegin(GL_LINE_LOOP);
        DrawCircle(s);
        glEnd();
#else
        DrawCircle(s, GL_LINE_LOOP);
#endif
        break;

    case Disk:
#ifdef UseOpenGL
        glBegin(GL_POLYGON);
        DrawCircle(s);
        glEnd();
#else
        DrawCircle(s, GL_POLYGON);
#endif
        break;

    default:
        break;
    }
}
