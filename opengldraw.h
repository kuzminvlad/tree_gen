#ifndef OPENGLDRAW_H
#define OPENGLDRAW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTime>
#include <QTimer>
#include <QThread>

#include <camera.h>
#include <shader.h>

#include "plantmain.h"
#include <includedlib.h>

class OpenGlDraw : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    OpenGlDraw(QString fn, QWidget *parent = nullptr, bool toObj = true, bool showScene = true, int quality = 12);

    void initializeGL();
    void paintGL();
    void resizeGL(int w,int h);  

    int quality = 12;

private:
    PlantVertex plantvertex;
    QString fileName;

    void cilinder(Tree*& tr, int j);

    vec3 currentColor = vec3(0.0);
    vector<float> vertexPosition, vertexColor, vertexNormal, vertexLeaf, vertexPositionLeaf, vertexColorLeaf, vertexNormalLeaf;
    ullint indexTreeEnd = 0;
    ullint vertexPositionLeafIndexStart;

    GLuint VAO;
    map<string, GLuint> VBO;

    void initializeBuffers();
    void initializeVertex();

    Shader *ourShader;
    Camera *camera;
    vec2 bufMR;

    void toObj(string file_path);
    bool pObj;
    bool pShowScene;

    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // OPENGLDRAW_H
