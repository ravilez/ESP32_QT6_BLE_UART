#include "cspxopenglwidget.h"

CSPXOpenGLWidget::CSPXOpenGLWidget(QWidget *parent)
    :QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus) ;

    alpha = 0;
    beta = 0;
    gamma = 0;
    distance = 2.5;
    dx = dy = dz = 0.0;
    cdx = cdy = cdz = 0.0;
}

CSPXOpenGLWidget::~CSPXOpenGLWidget()
{
    cleanup();
}

void CSPXOpenGLWidget::cleanup()
{
}

void CSPXOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.5,0.5,0.5,1.0);

    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "vertexShader.vsh");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "fragmentShader.fsh");
    shaderProgram.link();

    //triangle
    //vertices << QVector3D(1, 0, -2) << QVector3D(0, 1, -2) << QVector3D(-1, 0, -2);

    /*cube_vertices << QVector3D(-0.1, -0.1,  0.1) << QVector3D( 0.1, -0.1,  0.1) << QVector3D( 0.1,  0.1,  0.1) // Front
             << QVector3D( 0.1,  0.1,  0.1) << QVector3D(-0.1,  0.1,  0.1) << QVector3D(-0.1, -0.1,  0.1)
             << QVector3D( 0.1, -0.1, -0) << QVector3D(-0.1, -0.1, -0) << QVector3D(-0.1,  0.1, -0) // Back
             << QVector3D(-0.1,  0.1, -0) << QVector3D( 0.1,  0.1, -0) << QVector3D( 0.1, -0.1, -0)
             << QVector3D(-0.1, -0.1, -0) << QVector3D(-0.1, -0.1,  0.1) << QVector3D(-0.1,  0.1,  0.1) // Left
             << QVector3D(-0.1,  0.1,  0.1) << QVector3D(-0.1,  0.1, -0) << QVector3D(-0.1, -0.1, -0)
             << QVector3D( 0.1, -0.1,  0.1) << QVector3D( 0.1, -0.1, -0) << QVector3D( 0.1,  0.1, -0) // Right
             << QVector3D( 0.1,  0.1, -0) << QVector3D( 0.1,  0.1,  0.1) << QVector3D( 0.1, -0.1,  0.1)
             << QVector3D(-0.1,  0.1,  0.1) << QVector3D( 0.1,  0.1,  0.1) << QVector3D( 0.1,  0.1, -0) // Top
             << QVector3D( 0.1,  0.1, -0) << QVector3D(-0.1,  0.1, -0) << QVector3D(-0.1,  0.1,  0.1)
             << QVector3D(-0.1, -0.1, -0) << QVector3D( 0.1, -0.1, -0) << QVector3D( 0.1, -0.1,  0.1) // Bottom
             << QVector3D( 0.1, -0.1,  0.1) << QVector3D(-0.1, -0.1,  0.1) << QVector3D(-0.1, -0.1, -0);
    */

    buildVerticesSmooth(0.1f,16,0.01f);

    plane_vertices  << QVector3D( 0.f, 0.f, 0.0f) // Bottom left vertex
                    << QVector3D( 0.6f, 0.f, 0.0f) // Bottom right vertex
                    << QVector3D( 0.6f, 0.8f, 0.0f) // Top right vertex
                    << QVector3D( 0.0f,  0.8f, 0.0f); // Top left vertex

    x_axis_vertices << QVector3D( 0.0f, 0.0f, 0.0f) // x
                    << QVector3D( 0.6f, 0.0f, 0.0f);

    y_axis_vertices << QVector3D( 0.0f, 0.0f, 0.0f) // y
                    << QVector3D( 0.0f, 0.8f, 0.0f);

    z_axis_vertices << QVector3D( 0.0f, 0.0f, 0.0f) // z
                    << QVector3D( 0.0f, 0.0f, 1.0f);
}

void CSPXOpenGLWidget::resizeGL(int w, int h)
{
    if (h == 0)
    {
        h = 1;
    }

    pMatrix.setToIdentity();
    pMatrix.perspective(60.0, (float)w / (float)h, 0.001, 1000);

    glViewport(0, 0, w, h);
}

void CSPXOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 tMatrix;
    tMatrix.translate(dx,dy,0);

    QMatrix4x4 cameraTransformation;
    cameraTransformation.rotate(alpha, 0, 1, 0);
    cameraTransformation.rotate(beta, 1, 0, 0);
    cameraTransformation.rotate(gamma, 0, 0, 1);

    QVector3D cameraPosition = cameraTransformation.map(QVector3D(0, 0, distance));
    QVector3D cameraUpDirection = cameraTransformation.map(QVector3D(0, 1, 0));

    QMatrix4x4 vMatrix;
    vMatrix.lookAt(cameraPosition, QVector3D(cdx, cdy, cdz), cameraUpDirection);

    shaderProgram.bind();

    shaderProgram.setUniformValue("mvpMatrix", pMatrix * vMatrix * tMatrix);


    //test cube
    //shaderProgram.setUniformValue("color", QColor(Qt::white));
    //shaderProgram.setAttributeArray("vertex", cube_vertices.constData());
    //shaderProgram.enableAttributeArray("vertex");
    //glDrawArrays(GL_TRIANGLES, 0, cube_vertices.size());

    //plane
    shaderProgram.setUniformValue("color", QColor(Qt::white));
    shaderProgram.setAttributeArray("vertex", plane_vertices.constData());
    shaderProgram.enableAttributeArray("vertex");
    glDrawArrays(GL_QUADS, 0, plane_vertices.size());

    //cylinder
    shaderProgram.setUniformValue("color", QColor(Qt::lightGray));
    shaderProgram.setAttributeArray("vertex", cyl_vertices.constData());
    shaderProgram.enableAttributeArray("vertex");
    glDrawArrays(GL_TRIANGLES, 0, cyl_vertices.size());

    //hpgl lines
    if (!vertices.empty())
    {
        shaderProgram.setUniformValue("color", QColor(Qt::black));
        shaderProgram.setAttributeArray("vertex", vertices.constData());
        shaderProgram.enableAttributeArray("vertex");
        glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
    }

    //axes lines
    shaderProgram.setUniformValue("color", QColor(Qt::red));
    shaderProgram.setAttributeArray("vertex", x_axis_vertices.constData());
    shaderProgram.enableAttributeArray("vertex");
    glDrawArrays(GL_LINE_LOOP, 0, x_axis_vertices.size());

    shaderProgram.setUniformValue("color", QColor(Qt::green));
    shaderProgram.setAttributeArray("vertex", y_axis_vertices.constData());
    shaderProgram.enableAttributeArray("vertex");
    glDrawArrays(GL_LINE_LOOP, 0, y_axis_vertices.size());

    shaderProgram.setUniformValue("color", QColor(Qt::blue));
    shaderProgram.setAttributeArray("vertex", z_axis_vertices.constData());
    shaderProgram.enableAttributeArray("vertex");
    glDrawArrays(GL_LINE_LOOP, 0, z_axis_vertices.size());

    shaderProgram.disableAttributeArray("vertex");

    shaderProgram.release();
}

void CSPXOpenGLWidget::SetAlpha(int a)
{
    alpha = a;

    update();
}

void CSPXOpenGLWidget::SetBeta(int b)
{
    beta = b;

    update();
}

void CSPXOpenGLWidget::SetGamma(int c)
{
    gamma = c;

    update();
}

void CSPXOpenGLWidget::SetZoom(int d)
{
    distance = d;

    update();
}

void CSPXOpenGLWidget::keyPressEvent( QKeyEvent *e )
{
    if (e->type() == QEvent::KeyPress)
    {
        if (e->key() == Qt::Key_Plus || e->key() == Qt::Key_Minus)
        {
            if (e->key() == Qt::Key_Plus) {
                distance *= 0.9;
            } else {
                distance *= 1.1;
            }
            emit Zoom(distance);
        }
        if (e->key() == Qt::Key_Left || e->key() == Qt::Key_Right)
        {
            if (e->key() == Qt::Key_Right) {
                dx += 0.1;
            } else {
                dx -= 0.1;
            }
        }
        if (e->key() == Qt::Key_Up || e->key() == Qt::Key_Down)
        {
            if (e->key() == Qt::Key_Up) {
                dy += 0.1;
            } else {
                dy -= 0.1;
            }
        }
        if (e->key() == Qt::Key_Y) {
            if (e->modifiers()  == Qt::ShiftModifier)
                alpha -= 1;
            else
                alpha += 1;
            if (alpha <= -360 || alpha >= 360) {
                alpha = 0;
            }
            emit Rotation(this->alpha,this->beta,this->gamma);
        }
        else if (e->key() == Qt::Key_P) {
            if (e->modifiers()  == Qt::ShiftModifier)
                beta -= 1;
            else
                beta += 1;
            if (beta <= -360 || beta >= 360) {
                beta = 0;
            }
            emit Rotation(this->alpha,this->beta,this->gamma);
        }
        else if (e->key() == Qt::Key_R) {
            if (e->modifiers()  == Qt::ShiftModifier)
                gamma -= 1;
            else
                gamma += 1;
            if (gamma <= -360 || gamma >= 360) {
                gamma = 0;
            }
            emit Rotation(this->alpha,this->beta,this->gamma);
        }
    }

    update();
}

void CSPXOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();

    event->accept();
}

void CSPXOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int deltaX = event->pos().x() - lastMousePosition.x();
    int deltaY = event->pos().y() - lastMousePosition.y();

    if (event->buttons() & Qt::LeftButton) {
        alpha -= deltaX;
        while (alpha < 0) {
            alpha += 360;
        }
        while (alpha >= 360) {
            alpha -= 360;
        }

        beta -= deltaY;
        if (beta < -90) {
            beta = -90;
        }
        if (beta > 90) {
            beta = 90;
        }

        emit Rotation(this->alpha,this->beta,this->gamma);

        update();
    }

    lastMousePosition = event->pos();

    event->accept();
}

void CSPXOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() < 0) {
        distance *= 1.1;
    } else {
        distance *= 0.9;
    }
    update();

    event->accept();
}

QVector<QVector3D>* CSPXOpenGLWidget::getVertices()
{
    return &vertices;
}

void CSPXOpenGLWidget::setVertices(QVector<QVector3D>& vec3D)
{
    vertices = vec3D;
}


// generate a unit circle on XY-plane
void CSPXOpenGLWidget::createUnitCircleVertices(uint sectorCount)
{
    const float PI = 3.1415926f;
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    unitCircleVertices.clear();
    for(uint i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices << QVector3D( cos(sectorAngle), sin(sectorAngle), 0);

    }
}

// generate vertices for a cylinder
void CSPXOpenGLWidget::buildVerticesSmooth(float height, uint sectorCount, float radius)
{
    // clear memory of prev arrays
    cyl_vertices.clear();
    cyl_normals.clear();
    cyl_texCoords.clear();

    // create unit circle vectors on XY-plane
    createUnitCircleVertices(sectorCount);

    // top and bottom triangles
    for(int i = 0; i < 2; ++i)
    {
        float h = (1-i) * height;
        float r = (1-i) * radius + i*(1/10000.)*8.f; //tip = 0.2mm

        for(int j = 0; j <= sectorCount; ++j)
        {
            float ux1 = unitCircleVertices[j][0];
            float uy1 = unitCircleVertices[j][1];

            float ux2 = unitCircleVertices[(j+1)%sectorCount][0];
            float uy2 = unitCircleVertices[(j+1)%sectorCount][1];

            // create triangle position vectors
            cyl_vertices << QVector3D(ux1 * r,uy1 * r, -h);
            cyl_vertices << QVector3D(ux2 * r,uy2 * r, -h);
            cyl_vertices << QVector3D(0,0,-h);

        }
    }

    float h1 = -height;
    float r1 = radius;
    float h2 = 0;
    float r2 = (1/10000.)*8.f;

    for(int j = 0; j < sectorCount; ++j)
    {
        float ux1 = unitCircleVertices[j][0];
        float uy1 = unitCircleVertices[j][1];
        float ux2 = unitCircleVertices[(j+1)%sectorCount][0];
        float uy2 = unitCircleVertices[(j+1)%sectorCount][1];


        // first triangle
        cyl_vertices << QVector3D(ux1 * r1,uy1 * r1,h1);
        cyl_vertices << QVector3D(ux2 * r1,uy2 * r1,h1);
        cyl_vertices << QVector3D(ux1 * r2,uy1 * r2,h2);

        // second triangle
        cyl_vertices << QVector3D(ux1 * r2,uy1 * r2,h2);
        cyl_vertices << QVector3D(ux2 * r2,uy2 * r2, h2);
        cyl_vertices << QVector3D(ux2 * r1,uy2 * r1,h1);
    }
}
