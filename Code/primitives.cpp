/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#include "primitives.h"

// Documentation can be found in the primitives.h file

QVector< Vertex3 > createCubeVertices( ) {
    // Note that there are created 4 vertices for each of the 6 faces
    // Instead of 8 corners for the cube. This is done because this
    // cube is intended to have distinctly colored faces, instead of
    // discinctly colored corners.

    QVector< Vertex3 > vertices( 6 * 4 );

    // Front face
    vertices[ 0] = Vertex3( QVector3D(  1,  1,  1 ), Color3D( 1, 0, 1 ) );
    vertices[ 1] = Vertex3( QVector3D( -1,  1,  1 ), Color3D( 1, 0, 1 ) );
    vertices[ 2] = Vertex3( QVector3D( -1, -1,  1 ), Color3D( 1, 0, 1 ) );
    vertices[ 3] = Vertex3( QVector3D(  1, -1,  1 ), Color3D( 1, 0, 1 ) );

    // Rear face
    vertices[ 4] = Vertex3( QVector3D( -1,  1, -1 ), Color3D( 0, 0, 1 ) );
    vertices[ 5] = Vertex3( QVector3D(  1,  1, -1 ), Color3D( 0, 0, 1 ) );
    vertices[ 6] = Vertex3( QVector3D(  1, -1, -1 ), Color3D( 0, 0, 1 ) );
    vertices[ 7] = Vertex3( QVector3D( -1, -1, -1 ), Color3D( 0, 0, 1 ) );

    // Top face
    vertices[ 8] = Vertex3( QVector3D(  1,  1, -1 ), Color3D( 1, 0, 0 ) );
    vertices[ 9] = Vertex3( QVector3D( -1,  1, -1 ), Color3D( 1, 0, 0 ) );
    vertices[10] = Vertex3( QVector3D( -1,  1,  1 ), Color3D( 1, 0, 0 ) );
    vertices[11] = Vertex3( QVector3D(  1,  1,  1 ), Color3D( 1, 0, 0 ) );

    // Bottom face
    vertices[12] = Vertex3( QVector3D(  1, -1,  1 ), Color3D( 0, 1, 0 ) );
    vertices[13] = Vertex3( QVector3D( -1, -1,  1 ), Color3D( 0, 1, 0 ) );
    vertices[14] = Vertex3( QVector3D( -1, -1, -1 ), Color3D( 0, 1, 0 ) );
    vertices[15] = Vertex3( QVector3D(  1, -1, -1 ), Color3D( 0, 1, 0 ) );

    // Right face
    vertices[16] = Vertex3( QVector3D(  1,  1, -1 ), Color3D( 1, 1, 0 ) );
    vertices[17] = Vertex3( QVector3D(  1,  1,  1 ), Color3D( 1, 1, 0 ) );
    vertices[18] = Vertex3( QVector3D(  1, -1,  1 ), Color3D( 1, 1, 0 ) );
    vertices[19] = Vertex3( QVector3D(  1, -1, -1 ), Color3D( 1, 1, 0 ) );

    // Left face
    vertices[20] = Vertex3( QVector3D( -1,  1,  1 ), Color3D( 0, 1, 1 ) );
    vertices[21] = Vertex3( QVector3D( -1,  1, -1 ), Color3D( 0, 1, 1 ) );
    vertices[22] = Vertex3( QVector3D( -1, -1, -1 ), Color3D( 0, 1, 1 ) );
    vertices[23] = Vertex3( QVector3D( -1, -1,  1 ), Color3D( 0, 1, 1 ) );

    return vertices;
}

QVector< Triangle > createCubeTriangles( ) {
    QVector< Triangle > triangles( 6 * 2 );

    for ( int i = 0; i < 6; i++ ) {
        triangles[i*2  ] = Triangle( i*4+0, i*4+1, i*4+2 );
        triangles[i*2+1] = Triangle( i*4+0, i*4+2, i*4+3 );
    }

    return triangles;
}

std::unique_ptr< Batch > createCube( QOpenGLFunctions_3_3_Core *pGl ) {
    return std::make_unique< Batch >( pGl, createCubeVertices( ), createCubeTriangles( ) );
}

QVector< Vertex3 > createPyramidVertices( ) {
    // Also the pyramid has distinctly colored faces, instead of distinctly
    // colored corners

    // 3 vertices for each triangular side
    // 4 vertices for the bottom
    QVector< Vertex3 > vertices( 4 * 3 + 4 );

    // Left
    vertices[ 0] = Vertex3( QVector3D(  0,  1,  0 ), Color3D( 1, 0, 0 ) );
    vertices[ 1] = Vertex3( QVector3D( -1, -1, -1 ), Color3D( 1, 0, 0 ) );
    vertices[ 2] = Vertex3( QVector3D( -1, -1,  1 ), Color3D( 1, 0, 0 ) );

    // Right
    vertices[ 3] = Vertex3( QVector3D(  0,  1,  0 ), Color3D( 0, 1, 0 ) );
    vertices[ 4] = Vertex3( QVector3D(  1, -1,  1 ), Color3D( 0, 1, 0 ) );
    vertices[ 5] = Vertex3( QVector3D(  1, -1, -1 ), Color3D( 0, 1, 0 ) );

    // Front
    vertices[ 6] = Vertex3( QVector3D(  0,  1,  0 ), Color3D( 0, 0, 1 ) );
    vertices[ 7] = Vertex3( QVector3D( -1, -1,  1 ), Color3D( 0, 0, 1 ) );
    vertices[ 8] = Vertex3( QVector3D(  1, -1,  1 ), Color3D( 0, 0, 1 ) );

    // Back
    vertices[ 9] = Vertex3( QVector3D(  0,  1,  0 ), Color3D( 1, 0, 1 ) );
    vertices[10] = Vertex3( QVector3D(  1, -1, -1 ), Color3D( 1, 0, 1 ) );
    vertices[11] = Vertex3( QVector3D( -1, -1, -1 ), Color3D( 1, 0, 1 ) );

    // Bottom
    vertices[12] = Vertex3( QVector3D( -1, -1, -1 ), Color3D( 1, 1, 0 ) );
    vertices[13] = Vertex3( QVector3D(  1, -1, -1 ), Color3D( 1, 1, 0 ) );
    vertices[14] = Vertex3( QVector3D(  1, -1,  1 ), Color3D( 1, 1, 0 ) );
    vertices[15] = Vertex3( QVector3D( -1, -1,  1 ), Color3D( 1, 1, 0 ) );

    return vertices;
}

QVector< Triangle > createPyramidTriangles( ) {
    // 1 triangle for each side, 2 for the bottom
    QVector< Triangle > triangles( 4 + 2 );

    // 4 sides
    for ( int i = 0; i < 4; i++ ) {
        triangles[ i ] = Triangle( i * 3 + 0, i * 3 + 1, i * 3 + 2 );
    }

    // 2 triangles for bottom
    triangles[ 4] = Triangle( 12, 13, 14 );
    triangles[ 5] = Triangle( 12, 14, 15 );

    return triangles;
}

std::unique_ptr< Batch > createPyramid( QOpenGLFunctions_3_3_Core *pGl ) {
    return std::make_unique< Batch >( pGl, createPyramidVertices( ), createPyramidTriangles( ) );
}
