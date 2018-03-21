#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"
#include "animation.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QVector3D>
#include <memory>

class MainView : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core {
    Q_OBJECT

    QOpenGLDebugLogger *debugLogger;
    QTimer timer; // timer used for animation

    QOpenGLShaderProgram buzzShaderProgram;

public:
    MainView(QWidget *parent = 0);
    ~MainView();

protected:
    void initializeGL();
    void resizeGL(int newWidth, int newHeight);
    void paintGL();

    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    // Function for mouse input events
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);

private slots:
    void onMessageLogged( QOpenGLDebugMessage Message );

private:
    void createShaderPrograms();
    void createShaderProgram( QOpenGLShaderProgram& program, const QString& name );

    void setupAnimationBatches( );

    void bindLight( QOpenGLShaderProgram& program );
    void loadTexture( QString file, GLuint texPtr );

    bool isLightLocked;

    QMatrix4x4 projectionMat;
    Transform3f viewTransform;

    float time;

    std::unique_ptr< AnimatedBatch > mainBatch;
    std::vector< std::unique_ptr< AnimatedBatch > > batches;
};

#endif // MAINVIEW_H
