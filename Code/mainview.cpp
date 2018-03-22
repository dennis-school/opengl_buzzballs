/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#include "mainview.h"
#include "math.h"
#include "batch.h"
#include "model.h"
#include "material.h"

#include <QDateTime>
#include <cstdlib>

struct Light {
    QVector3D position;
    Color3D color;

    Light( const QVector3D& position, const Color3D& color )
        : position( position ), color( color ) { }
};

/**
 * @brief MainView::MainView
 *
 * Constructor of MainView
 *
 * @param parent
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
    debugLogger->stopLogging();

    qDebug() << "MainView destructor";
}

// --- OpenGL initialization

static QVector<quint8> imageToBytes(QImage image) {
    // needed since (0,0) is bottom left in OpenGL
    QImage im = image.mirrored();
    QVector<quint8> pixelData;
    pixelData.reserve(im.width()*im.height()*4);

    for (int i = 0; i != im.height(); ++i) {
        for (int j = 0; j != im.width(); ++j) {
            QRgb pixel = im.pixel(j,i);

            // pixel is of format #AARRGGBB (in hexadecimal notation)
            // so with bitshifting and binary AND you can get
            // the values of the different components
            quint8 r = (quint8)((pixel >> 16) & 0xFF); // Red component
            quint8 g = (quint8)((pixel >> 8) & 0xFF); // Green component
            quint8 b = (quint8)(pixel & 0xFF); // Blue component
            quint8 a = (quint8)((pixel >> 24) & 0xFF); // Alpha component

            // Add them to the Vector
            pixelData.append(r);
            pixelData.append(g);
            pixelData.append(b);
            pixelData.append(a);
        }
    }
    return pixelData;
}

void MainView::loadTexture( QString file, GLuint texPtr ) {
    QImage img( file );
    QVector<quint8> data = imageToBytes( img );
    glBindTexture( GL_TEXTURE_2D, texPtr );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, img.width( ), img.height( ), 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data( ) );
}

/**
 * @brief MainView::initializeGL
 *
 * Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions
 */
void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    debugLogger = new QOpenGLDebugLogger();
    connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ),
             this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

    if ( debugLogger->initialize() ) {
        qDebug() << ":: Logging initialized";
        debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
        debugLogger->enableMessages();
    }

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    createShaderPrograms();

    time = 0;

    viewTransform.setTranslationZ( -10 );

    setupAnimationBatches( );

    timer.start( 1000.0 / 60.0 );
}

/**
 * @brief MainView::setupAnimationBatches set up several buzz balls
 */
void MainView::setupAnimationBatches( ) {
    Model modelBall( ":/models/buzzball.obj" );

    qDebug( ) << modelBall.getNumTriangles( );

    Material materialBase( this );
    materialBase.ka = 0.5f;
    materialBase.ks = 0.1f;
    materialBase.kd = 0.9f;
    materialBase.p = 16.0f;

    // This should be a shared pointer, because in case it has
    // textures they should only be deallocated upon true destruction
    // (not applicable for this application - no textures)
    std::shared_ptr< Material > pMaterialRed = std::make_shared< Material >( materialBase );
    pMaterialRed->color = QVector3D( 1, 0, 0 );

    std::shared_ptr< Material > pMaterialGreen = std::make_shared< Material >( materialBase );
    pMaterialGreen->color = QVector3D( 0, 1, 0 );

    std::shared_ptr< Material > pMaterialPurple = std::make_shared< Material >( materialBase );
    pMaterialPurple->color = QVector3D( 0.5, 0, 0.5 );

    std::shared_ptr< Material > pMaterialYellow = std::make_shared< Material >( materialBase );
    pMaterialYellow->color = QVector3D( 1, 1, 0 );

    std::shared_ptr< Material > pMaterialBlue = std::make_shared< Material >( materialBase );
    pMaterialBlue->color = QVector3D( 0, 0, 1 );

    std::shared_ptr< BuzzBatch > pBatchBall = std::move( buzzBatchFromModel( this, modelBall ) );

    // Setup main batch
    std::vector< std::shared_ptr< TransformAnimator > > noAnimation; // empty list
    noAnimation.push_back( std::make_shared< ConstantAnimator >( Transform3f::Scale( 0.5 ) ) );

    mainBatch = std::make_unique< AnimatedBatch >( pBatchBall, pMaterialRed, noAnimation );

    // Setup several arbitrary orbiting bouncing balls
    {
        std::vector< std::shared_ptr< TransformAnimator > > animation;
        animation.push_back( std::make_shared< ConstantAnimator >( Transform3f::Rotation( QVector3D( 0, 0, 45 ) ) ) );
        animation.push_back( std::make_shared< RotationAnimator >( RotationAnimator( QVector3D( 0, 0.1, 0 ) ) ) );
        animation.push_back( std::make_shared< ConstantAnimator >( Transform3f( 0.5, QVector3D( ), QVector3D( 3, 0, 0 ) ) ) );
        batches.push_back( std::make_unique< AnimatedBatch >( pBatchBall, pMaterialGreen, animation ) );
    }

    {
        std::vector< std::shared_ptr< TransformAnimator > > animation;
        animation.push_back( std::make_shared< ConstantAnimator >( Transform3f::Rotation( QVector3D( 0, 0, 135 ) ) ) );
        animation.push_back( std::make_shared< RotationAnimator >( RotationAnimator( QVector3D( 0, 0.07, 0.02 ) ) ) );
        animation.push_back( std::make_shared< ConstantAnimator >( Transform3f( 0.5, QVector3D( ), QVector3D( 6, 0, 0 ) ) ) );
        batches.push_back( std::make_unique< AnimatedBatch >( pBatchBall, pMaterialPurple, animation ) );
    }

    {
        std::vector< std::shared_ptr< TransformAnimator > > animation;
        animation.push_back( std::make_shared< BounceAnimator >( BounceAnimator( 0, 1, 0.001 ) ) );
        animation.push_back( std::make_shared< ConstantAnimator >( Transform3f( 0.5, QVector3D( ), QVector3D( -5, 2, 1 ) ) ) );
        batches.push_back( std::make_unique< AnimatedBatch >( pBatchBall, pMaterialYellow, animation ) );
    }

    {
        std::vector< std::shared_ptr< TransformAnimator > > animation;
        animation.push_back( std::make_shared< BounceAnimator >( BounceAnimator( 0, 1, 0.001 ) ) );
        animation.push_back( std::make_shared< ConstantAnimator >( Transform3f( 0.5, QVector3D( ), QVector3D( 5, -3, 1 ) ) ) );
        batches.push_back( std::make_unique< AnimatedBatch >( pBatchBall, pMaterialBlue, animation ) );
    }
}

void MainView::createShaderPrograms( ) {
    createShaderProgram( buzzShaderProgram, "buzz" );
}

void MainView::createShaderProgram( QOpenGLShaderProgram& shaderProgram, const QString& name ) {
    // Create shader program
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/" + name + "_vertshader.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/" + name + "_fragshader.glsl");
    shaderProgram.link( );
}

// --- OpenGL drawing

/**
 * @brief MainView::bindLight Sets up the light in the scene
 *
 * @param program The program to bind the light to
 */
void MainView::bindLight( QOpenGLShaderProgram& program ) {
    Light light0( QVector3D( -5, 0, 3 ), Color3D( 0.3, 0.2, 0.5 ) );
    Light light1( QVector3D( 10, 0, 10 ), Color3D( 0.1, 0.35, 0.1 ) );
    Light light2( QVector3D( -10, 0, -10 ), Color3D( 0.3, 0.25, 0.1 ) );

    program.setUniformValue( "u_lights[0].position", light0.position );
    program.setUniformValue( "u_lights[0].color", light0.color );

    program.setUniformValue( "u_lights[1].position", light1.position );
    program.setUniformValue( "u_lights[1].color", light1.color );

    program.setUniformValue( "u_lights[2].position", light2.position );
    program.setUniformValue( "u_lights[2].color", light2.color );
}

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {
    time += 1000.0f / 60.0f;

    // Set the color of the screen to be blue on clear (new frame)
    glClearColor( abs( sin( 2.0f * M_PI * time * 5 / 100000.0f ) )
                , abs( sin( 2.0f * M_PI * time * 7 / 100000.0f ) )
                , abs( sin( 2.0f * M_PI * time * 19 / 100000.0f ) )
                , 1.0f);

    // Clear the screen before rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QOpenGLShaderProgram *pProgram = &buzzShaderProgram;

    pProgram->bind( );

    // Setup lighting
    bindLight( *pProgram );

    pProgram->setUniformValue( "u_projectionMat", projectionMat );
    pProgram->setUniformValue( "u_viewMat", viewTransform.matrix( ) );

    // spike exageration
    float ex1 = sin( 2.0f * M_PI * time / 700.0f );
    float ex2 = sin( 2.0f * M_PI * time / 1100.0f + 100 );
    float spike = 3 + 2 * (float) ( abs( ex1 * ex2 ) + ex1 );
    pProgram->setUniformValue( "u_spike", spike );
    mainBatch->render( pProgram, time );

    pProgram->setUniformValue( "u_spike", spike / 2 );
    for ( std::unique_ptr< AnimatedBatch >& batch : batches ) {
        batch->render( pProgram, time );
    }
}

/**
 * @brief MainView::resizeGL
 *
 * Called upon resizing of the screen
 *
 * @param newWidth
 * @param newHeight
 */
void MainView::resizeGL(int newWidth, int newHeight) 
{
    projectionMat.setToIdentity( );
    projectionMat.perspective( 60, (float) newWidth / (float) newHeight, 0.001f, 100 );
}

// --- Private helpers

/**
 * @brief MainView::onMessageLogged
 *
 * OpenGL logging function, do not change
 *
 * @param Message
 */
void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
    qDebug() << " → Log:" << Message;
}
