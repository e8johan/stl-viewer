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

#ifndef STLVIEW_H
#define STLVIEW_H

#include <QGLWidget>

class StlView : public QGLWidget
{
    Q_OBJECT
public:
    explicit StlView(QWidget *parent = 0);
    
protected:
    void initializeGL();
    void resizeGL(int, int);
    void paintGL();

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

    void wheelEvent(QWheelEvent *);

private:
    struct StlVertex {
        qreal x, y, z;
    };

    typedef QList<StlVertex> StlVertexList;

    struct StlFacet {
        StlVertex normal;
        StlVertexList vertexes;

    };

    typedef QList<StlFacet> StlFacetList;

    struct StlSolid {
        QString name;
        StlFacetList facets;
    };

    typedef QList<StlSolid> StlSolidList;

    StlVertex parseVertex(const QString &line, bool *ok);

    StlSolidList m_solids;
    qreal m_rotx, m_roty, m_rotz;
    qreal m_offsetX, m_offsetY, m_offsetZ;
    qreal m_scale;

    qreal m_deltaRotX, m_deltaRotY;
    qreal m_deltaPanX, m_deltaPanY;

    bool m_isRotating;
    bool m_isPanning;

    QPoint m_startPoint;
};

#endif // STLVIEW_H
