/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#ifndef BATCH_H
#define BATCH_H

#include <cstdint>
#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLFunctions_3_3_Core>
#include <memory>
#include "model.h"

typedef QVector3D Color3D;

/**
 * @brief The Vertex3 struct represents a 3-dimensional
 *   where every vertex has a (3D) position and a (RGB) color.
 */
struct Vertex3 {
    QVector3D position;
    QVector3D normal;
    QVector2D texCoord;

    // The tangent vectors are used to accurately obtain per-fragment normals
    QVector3D tangent;
    QVector3D bitangent;

    Vertex3( )
        : Vertex3( QVector3D( 0, 0, 0 ), QVector3D( 0, 0, 0 ), QVector2D( 0, 0 ) ) { }

    Vertex3( QVector3D position, QVector3D normal, QVector2D texCoord )
        : position( position )
        , normal( normal )
        , texCoord( texCoord )
        , tangent( 0, 0, 0 )
        , bitangent( 0, 0, 0 ) { }
};

struct BuzzVertex3 {
    QVector3D position;
    QVector3D position2;
    QVector3D position3;

    BuzzVertex3( )
        : BuzzVertex3( QVector3D( 0, 0, 0 ), QVector3D( 0, 0, 0 ), QVector2D( 0, 0 ) ) { }

    BuzzVertex3( QVector3D position, QVector3D position2, QVector3D position3 )
        : position( position )
        , position2( position2 )
        , position3( position3 ) { }
};

/**
 * @brief The Triangle struct is a triangle, where each component
 *   represents the index of the vertex in the vertex array.
 */
struct Triangle {
    uint16_t v1;
    uint16_t v2;
    uint16_t v3;

    Triangle( )
        : v1( 0 ), v2( 0 ), v3( 0 ) { }

    Triangle( uint16_t v1, uint16_t v2, uint16_t v3 )
        : v1( v1 ), v2( v2 ), v3( v3 ) { }
};

class GeneralBatch {
public:
    virtual void draw( ) = 0;
};

template< typename T >
class Batch : public GeneralBatch {
public:
    Batch( QOpenGLFunctions_3_3_Core *pGl, QVector< T > vertices, QVector< Triangle > triangles );
    virtual ~Batch( );
    void draw( );
protected:
    virtual void setupMemoryLayout( ) = 0;

    QOpenGLFunctions_3_3_Core *pGl;

    GLuint vao;
    GLuint verticesVbo;
    GLuint indicesVbo;

    int numTriangles;
};

/**
 * @brief The Batch class is a OpenGL model that is uploaded to the GPU. It contains the VBO
 *   and VAO. Upon destruction any allocated resources are destroyed (also on the GPU).
 *   It uses the Vertex3 structure as defined above and would therefore only work properly
 *   with shaders having the same attribute layout.
 *
 * Note that this class can only be called after OpenGL is initialised
 */
class DefaultBatch : public Batch< Vertex3 > {
public:
    DefaultBatch( QOpenGLFunctions_3_3_Core *pGl, QVector< Vertex3 > vertices, QVector< Triangle > triangles )
        : Batch< Vertex3 >( pGl, vertices, triangles ) {
      setupMemoryLayout( );
    }

    ~DefaultBatch( ) { }
protected:
    void setupMemoryLayout( );
private:
    const static unsigned int II_POSITION = 0;
    const static unsigned int II_NORMAL = 1;
    const static unsigned int II_TEXCOORD = 2;
    const static unsigned int II_TANGENT = 3;
    const static unsigned int II_BITANGENT = 4;
};

class BuzzBatch : public Batch< BuzzVertex3 > {
public:
    BuzzBatch( QOpenGLFunctions_3_3_Core *pGl, QVector< BuzzVertex3 > vertices, QVector< Triangle > triangles )
        : Batch< BuzzVertex3 >( pGl, vertices, triangles ) {
      setupMemoryLayout( );
    }
    ~BuzzBatch( ) { }
protected:
    void setupMemoryLayout( );
private:
    const static unsigned int II_POSITION = 0;
    const static unsigned int II_POSITION2 = 1;
    const static unsigned int II_POSITION3 = 2;
};

/**
 * @brief batchFromModel Uploads the model loaded from an Obj file to the GPU
 *   a smart pointer to the representing Batch class is returned.
 *
 * @param pGl A pointer to the OpenGL 3.3 core functions
 * @param model The model that should be uploaded
 * @return A smart pointer to the representing Batch class
 */
std::unique_ptr< DefaultBatch > defaultBatchFromModel( QOpenGLFunctions_3_3_Core *pGl, Model model );

std::unique_ptr< BuzzBatch > buzzBatchFromModel( QOpenGLFunctions_3_3_Core *pGl, Model model );

#endif // BATCH_H
