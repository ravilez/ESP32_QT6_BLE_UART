#ifndef CSPXOPENGLWIDGET_H
#define CSPXOPENGLWIDGET_H

#include <QScreen>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include <QWidget>

#include <QMouseEvent>
#include <QWheelEvent>

class CSPXOpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    CSPXOpenGLWidget(QWidget *parent = nullptr);
    ~CSPXOpenGLWidget();

    QVector<QVector3D>* getVertices();
    void setVertices(QVector<QVector3D>& vec3D);

    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

    void wheelEvent(QWheelEvent *event) override;
    void SetAlpha(int a);
    void SetBeta(int b);
    void SetGamma(int c);
    void SetZoom(int d);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void createUnitCircleVertices(uint sectorCount);
    void buildVerticesSmooth(float height, uint sectorCount, float radius);

signals:
    void Rotation(double,double,double);
    void Zoom(double);

public slots:
    void cleanup();

private:
    QMatrix4x4 pMatrix;
    QOpenGLShaderProgram shaderProgram;
    QVector<QVector3D> vertices;
    QVector<QVector3D> cube_vertices;
    QVector<QVector3D> plane_vertices;
    QVector<QVector3D> x_axis_vertices;
    QVector<QVector3D> y_axis_vertices;
    QVector<QVector3D> z_axis_vertices;

    QVector<QVector3D> unitCircleVertices;
    QVector<QVector3D> cyl_vertices;
    QVector<QVector3D> cyl_normals;
    QVector<QVector2D> cyl_texCoords;

    double alpha;
    double beta;
    double gamma;
    double distance;
    double dx,dy,dz;
    double cdx,cdy,cdz;
    QPoint lastMousePosition;
};

#endif // CSPXOPENGLWIDGET_H
