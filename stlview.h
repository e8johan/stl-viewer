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
