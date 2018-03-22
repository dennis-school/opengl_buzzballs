/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#include "batch.h"

#include <QDebug>

// Documentation can be found in the batch.h file

template class Batch< Vertex3 >;
template class Batch< BuzzVertex3 >;

template< typename T >
Batch< T >::Batch( QOpenGLFunctions_3_3_Core *pGl, QVector< T > vertices, QVector< Triangle > triangles )
        : pGl( pGl ), numTriangles( triangles.length( ) ) {

    GLuint vbos[2];
    pGl->glGenBuffers( 2, vbos );
    verticesVbo = vbos[0];
    indicesVbo = vbos[1];
    pGl->glGenBuffers( 1, &verticesVbo );
    pGl->glGenVertexArrays( 1, &vao );

    pGl->glBindVertexArray( vao );

    // Upload data to GPU
    pGl->glBindBuffer( GL_ARRAY_BUFFER, verticesVbo );
    pGl->glBufferData( GL_ARRAY_BUFFER, sizeof( T ) * vertices.length( ), vertices.constData( ), GL_STATIC_READ );
    pGl->glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indicesVbo );
    pGl->glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( Triangle ) * triangles.length( ), triangles.constData( ), GL_STATIC_READ );

    // Note that the memory layout has not yet been setup. This should be done in sub-classes
}

template< typename T >
Batch< T >::~Batch( ) {
    qDebug( ) << "Batch destructed";
    GLuint vbos[2];
    vbos[ 0 ] = verticesVbo;
    vbos[ 1 ] = indicesVbo;
    pGl->glDeleteBuffers( 2, vbos );
    pGl->glDeleteVertexArrays( 1, &vao );
}

template< typename T >
void Batch< T >::draw( ) {
    pGl->glBindVertexArray( vao );
    pGl->glDrawElements( GL_TRIANGLES, 3 * numTriangles, GL_UNSIGNED_SHORT, (void *) 0 );
}

DefaultBatch::DefaultBatch( QOpenGLFunctions_3_3_Core *pGl, QVector< Vertex3 > vertices, QVector< Triangle > triangles  )
        : Batch< Vertex3 >( pGl, vertices, triangles ) {
    setupMemoryLayout( );
}

void DefaultBatch::setupMemoryLayout( ) {
    qDebug( ) << "DefaultBatch::setupMemoryLayout()";

    // Describe memory layout to OpenGL
    pGl->glEnableVertexAttribArray( II_POSITION );
    pGl->glEnableVertexAttribArray( II_NORMAL );
    pGl->glEnableVertexAttribArray( II_TEXCOORD );
    pGl->glEnableVertexAttribArray( II_TANGENT );
    pGl->glEnableVertexAttribArray( II_BITANGENT );

    pGl->glVertexAttribPointer( II_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex3 ), (void *) 0 );
    pGl->glVertexAttribPointer( II_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex3 ), (void *) sizeof( QVector3D ) );
    pGl->glVertexAttribPointer( II_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex3 ), (void *) ( 2 * sizeof( QVector3D ) ) );
    pGl->glVertexAttribPointer( II_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex3 ), (void *) ( 2 * sizeof( QVector3D ) + sizeof( QVector2D ) ) );
    pGl->glVertexAttribPointer( II_BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex3 ), (void *) ( 3 * sizeof( QVector3D ) + sizeof( QVector2D ) ) );
}

BuzzBatch::BuzzBatch( QOpenGLFunctions_3_3_Core *pGl, QVector< BuzzVertex3 > vertices, QVector< Triangle > triangles  )
        : Batch< BuzzVertex3 >( pGl, vertices, triangles ) {
    setupMemoryLayout( );
}

void BuzzBatch::setupMemoryLayout( ) {
    // Describe memory layout to OpenGL
    pGl->glEnableVertexAttribArray( II_POSITION );
    pGl->glEnableVertexAttribArray( II_POSITION2 );
    pGl->glEnableVertexAttribArray( II_POSITION3 );

    pGl->glVertexAttribPointer( II_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( BuzzVertex3 ), (void *) 0 );
    pGl->glVertexAttribPointer( II_POSITION2, 3, GL_FLOAT, GL_FALSE, sizeof( BuzzVertex3 ), (void *) sizeof( QVector3D ) );
    pGl->glVertexAttribPointer( II_POSITION3, 3, GL_FLOAT, GL_FALSE, sizeof( BuzzVertex3 ), (void *) ( 2 * sizeof( QVector3D ) ) );
}

// Writes the tangent and bitangent vectors to the vertices
// These are computed from the positions and UVs.
// Used for computing the per-fragment normals.
void computeTangents( QVector< Vertex3 >& vertices, const QVector<unsigned>& indices ) {
    for ( int i = 0; i < indices.size( ); i+=3 ) {
        Vertex3& v0 = vertices[ indices[ i + 0 ] ];
        Vertex3& v1 = vertices[ indices[ i + 1 ] ];
        Vertex3& v2 = vertices[ indices[ i + 2 ] ];

        QVector3D dPos1 = v1.position - v0.position;
        QVector3D dPos2 = v2.position - v0.position;
        QVector3D dTex1 = v1.texCoord - v0.texCoord;
        QVector3D dTex2 = v2.texCoord - v0.texCoord;

        float r = 1.0f / ( dTex1.x( ) * dTex2.y( ) - dTex1.y( ) * dTex2.x( ) );
        QVector3D tangent = ( dPos1 * dTex2.y( ) - dPos2 * dTex1.y( ) ) * r;
        QVector3D bitangent = ( dPos2 * dTex1.x( ) - dPos1 * dTex2.x( ) ) * r;

        // Keep adding. At the end it will be normalized (averaged)
        v0.tangent += tangent;
        v0.bitangent += bitangent;
        v1.tangent += tangent;
        v1.bitangent += bitangent;
        v2.tangent += tangent;
        v2.bitangent += bitangent;
    }

    // Make all tangents and bitangents unit vectors orthogonal to the normal
    for ( int i = 0; i < vertices.size( ); i++ ) {
        QVector3D& n = vertices[ i ].normal;
        QVector3D& t = vertices[ i ].tangent;
        QVector3D& b = vertices[ i ].bitangent;

        // Reorthogonalize
        t = ( t - n * QVector3D::dotProduct( n, t ) ).normalized( );
        b = ( b - n * QVector3D::dotProduct( n, b ) ).normalized( );
    }
}

std::unique_ptr< DefaultBatch > defaultBatchFromModel( QOpenGLFunctions_3_3_Core *pGl, Model model ) {
    QVector< QVector3D > positions = model.getVertices_indexed( );
    QVector< QVector3D > normals = model.getNormals_indexed( );
    QVector< QVector2D > texCoords = model.getTextureCoords_indexed( );
    QVector<unsigned> indices = model.getIndices( );
    unsigned int numVertices = positions.length( );

    QVector< Vertex3 > vertices( numVertices );
    for ( unsigned int i = 0; i < numVertices; i++ ) {
        vertices[ i ] = Vertex3( positions[ i ], normals[ i ], texCoords[ i ] );
    }

    computeTangents( vertices, indices );

    QVector< Triangle > triangles( indices.length( ) / 3 );
    for ( int i = 0; i < triangles.length( ); i++ ) {
        triangles[ i ] = Triangle( indices[ i * 3 + 0 ]
                                 , indices[ i * 3 + 1 ]
                                 , indices[ i * 3 + 2 ] );
    }

    return std::make_unique< DefaultBatch >( pGl, vertices, triangles );
}

std::unique_ptr< BuzzBatch > buzzBatchFromModel( QOpenGLFunctions_3_3_Core *pGl, Model model ) {
    QVector< QVector3D > positions = model.getVertices( );

    QVector< QVector3D > normals = model.getNormals( );

    QVector< BuzzVertex3 > vertices( positions.size( ) );
    for ( int i = 0; i < positions.size( ); i+=3 ) {
        vertices[ i + 0 ] = BuzzVertex3( positions[ i + 0 ], positions[ i + 1 ], positions[ i + 2 ] );
        vertices[ i + 1 ] = BuzzVertex3( positions[ i + 1 ], positions[ i + 2 ], positions[ i + 0 ] );
        vertices[ i + 2 ] = BuzzVertex3( positions[ i + 2 ], positions[ i + 0 ], positions[ i + 1 ] );
    }

    QVector< Triangle > triangles( positions.size( ) / 3 );
    for ( int i = 0; i < triangles.size( ); i++ ) {
        triangles[ i ] = Triangle( i * 3 + 0, i * 3 + 1, i * 3 + 2 );
    }

    return std::make_unique< BuzzBatch >( pGl, vertices, triangles );
}
