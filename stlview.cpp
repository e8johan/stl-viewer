/*
 * STL Viewer
 * Copyright (C) 2014 Johan Thelin <e8johan@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "stlview.h"

#include <math.h>

#include <GL/glu.h>

#include <QFile>
#include <QTimer>
#include <QWheelEvent>

StlView::StlView(QWidget *parent)
    : QGLWidget(parent)
    , m_rotx(0), m_roty(0), m_rotz(0)
    , m_offsetX(0), m_offsetY(0), m_offsetZ(0)
    , m_scale(1)
    , m_deltaRotX(0), m_deltaRotY(0)
    , m_deltaPanX(0), m_deltaPanY(0)
    , m_isRotating(false), m_isPanning(false)
{
    StlSolid s;
    StlFacet f;
    StlVertex v;

    qreal minx, maxx;
    qreal miny, maxy;
    qreal minz, maxz;

    minx = miny = minz = 0;
    maxx = maxy = maxz = 0;

//    QFile file("C:\\Users\\Thelins\\Downloads\\gearPlate.stl");
//    QFile file("C:\\Users\\Thelins\\Downloads\\TheFreakyTiki.stl");
    QFile file("owl.stl");
    if (file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd())
        {
            QString line = file.readLine().trimmed();

            if (line.toLower().startsWith("solid"))
            {
                s.name = line.mid(5).trimmed();
                s.facets.clear();
            }
            else if (line.toLower().startsWith("facet normal"))
            {
                bool ok;
                v = parseVertex(line.mid(12).trimmed(), &ok);

                if (ok)
                    f.normal = v;
                else
                    qDebug("Could not parse vertex '%s'", qPrintable(line));
            }
            else if (line.toLower().startsWith("outer loop"))
            {
                f.vertexes.clear();
            }
            else if (line.toLower().startsWith("vertex"))
            {
                bool ok;
                v = parseVertex(line.mid(6).trimmed(), &ok);

                if (ok)
                {
                    minx = qMin(v.x, minx);
                    maxx = qMax(v.x, maxx);
                    miny = qMin(v.y, miny);
                    maxy = qMax(v.y, maxy);
                    minz = qMin(v.z, minz);
                    maxz = qMax(v.z, maxz);
                    f.vertexes << v;
                }
                else
                    qDebug("Could not parse vertex '%s'", qPrintable(line));
            }
            else if (line.toLower().startsWith("endloop"))
            {
                // Do nothing
            }
            else if (line.toLower().startsWith("endfacet"))
            {
                s.facets << f;
            }
            else if (line.toLower().startsWith("endsolid"))
            {
                m_solids << s;
            }
            else
                qDebug("Unrecognized line '%s'", qPrintable(line));
        }

        file.close();
    }
    else
    {
        // File did not open, use a standard object

        // normal
        v.x = 0; v.y = 0; v.z = -1;
        f.normal = v;
        // v1
        v.x = 0; v.y = 1; v.z = 0;
        f.vertexes << v;
        // v2
        v.x = -1; v.y = -1; v.z = 0;
        f.vertexes << v;
        // v3
        v.x = 1; v.y = -1; v.z = 0;
        f.vertexes << v;
        s.facets << f;

        m_solids << s;
    }

    qreal max = maxx;
    max = qMax(max, maxy);
    max = qMax(max, maxz);
    m_offsetZ = -max*1.5;

    QTimer *timer = new QTimer(this);
    timer->setInterval(50);
    timer->start();
}

StlView::StlVertex StlView::parseVertex(const QString &line, bool *ok)
{
    bool isOk = false;
    StlVertex res;
    QStringList parts = line.split(" ", QString::SkipEmptyParts);

    if (parts.count() == 3)
    {
        bool okX, okY, okZ;
        res.x = parts[0].trimmed().toDouble(&okX);
        res.y = parts[1].trimmed().toDouble(&okY);
        res.z = parts[2].trimmed().toDouble(&okZ);

        isOk = okX && okY && okZ;
    }

    if (ok)
        *ok = isOk;

    return res;
}

void StlView::initializeGL()
{
    glShadeModel(GL_SMOOTH);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_LIGHTING);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat LightAmbient[4];
    GLfloat LightDiffuse[4];
    GLfloat LightPosition[4];

    LightAmbient[0]=  0.5f;
    LightAmbient[1]= 0.5f;
    LightAmbient[2]=  0.5f;
    LightAmbient[3]=	 1.0f;

    LightDiffuse[0]= 0.1; // 1.0f;
    LightDiffuse[1]= 0.1; // 1.0f;
    LightDiffuse[2]= 0.1; // 1.0f;
    LightDiffuse[3]= 0.1; // 1.0f;

    LightPosition[0]= 0.0f;
    LightPosition[1]=0.0f;
    LightPosition[2]=0.0f;
    LightPosition[3]=-50.0f;

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
    glEnable(GL_LIGHT1);
}

void StlView::resizeGL(int width, int height)
{
    height = height?height:1;

    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void StlView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(m_offsetX + m_deltaPanX, m_offsetY + m_deltaPanY, m_offsetZ);
    glScalef(m_scale, m_scale, m_scale);
    glRotatef(m_rotx + m_deltaRotX, 1.0f, 0.0f, 0.0f);
    glRotatef(m_roty + m_deltaRotY, 0.0f, 1.0f, 0.0f);
    glRotatef(m_rotz, 0.0f, 0.0f, 1.0f);

    glBegin(GL_TRIANGLES);
    foreach(const StlSolid &s, m_solids)
    {
        glColor3f(0.7, 1, 0.3);
        foreach(const StlFacet &f, s.facets)
        {
            glNormal3f(f.normal.x, f.normal.y, f.normal.z);
            foreach(const StlVertex &v, f.vertexes)
            {
                glVertex3f(v.x, v.y, v.z);
            }
        }
    }
    glEnd();
}

void StlView::wheelEvent(QWheelEvent *e)
{
    m_scale = m_scale*pow(0.999, e->delta());
    updateGL();
}

void StlView::mousePressEvent(QMouseEvent *e)
{
    if (m_isRotating || m_isPanning)
        return;

    m_startPoint = e->pos();

    if (e->button() == Qt::LeftButton && e->modifiers() == Qt::NoModifier)
    {
        m_isRotating = true;
    }
    else if (e->button() == Qt::RightButton || (e->button() == Qt::LeftButton && e->modifiers() == Qt::ControlModifier))
    {
        m_isPanning = true;
    }
}

void StlView::mouseReleaseEvent(QMouseEvent *e)
{
    // update deltaRot/deltaPan
    mouseMoveEvent(e);

    if (m_isRotating && e->button() == Qt::LeftButton)
    {
        m_isRotating = false;

        m_rotx += m_deltaRotX;
        m_roty += m_deltaRotY;

        m_deltaRotX = m_deltaRotY = 0;

        updateGL();
    }
    else if (m_isPanning)
    {
        m_isPanning = false;

        m_offsetX += m_deltaPanX;
        m_offsetY += m_deltaPanY;

        m_deltaPanX = m_deltaPanY = 0;

        updateGL();
    }
}

void StlView::mouseMoveEvent(QMouseEvent *e)
{
    QPoint delta = e->pos() - m_startPoint;

    if (m_isRotating)
    {
        m_deltaRotX = delta.y();
        m_deltaRotY = delta.x();

        updateGL();
    }
    else if (m_isPanning)
    {
        m_deltaPanX = delta.x();
        m_deltaPanY = -delta.y();

        updateGL();
    }
}
