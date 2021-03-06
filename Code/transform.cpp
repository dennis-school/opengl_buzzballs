/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#include "transform.h"

// See the "transform.h" header file for all documentation

Transform3f::Transform3f( )
    : scale( 1, 1, 1 ) {

}

Transform3f::Transform3f( float scale, QVector3D rotation, QVector3D translation )
        : translation( translation ), rotation( rotation ), scale( scale, scale, scale ) {
    rebuildMatrix( );
}

Transform3f Transform3f::Scale( float s ) {
    Transform3f t;
    t.setScale( s );
    return t;
}

Transform3f Transform3f::Rotation( QVector3D rotVec ) {
    Transform3f t;
    t.setRotation( rotVec.x( ), rotVec.y( ), rotVec.z( ) );
    return t;
}

Transform3f Transform3f::Translation( QVector3D v ) {
    Transform3f t;
    t.setTranslation( v );
    return t;
}

QMatrix4x4 Transform3f::matrix( ) const {
    return _matrix;
}

void Transform3f::setRotation( float xAngle, float yAngle, float zAngle ) {
    rotation = QVector3D( xAngle, yAngle, zAngle );
    rebuildMatrix( );
}

void Transform3f::setTranslation( float x, float y, float z ) {
    setTranslation( QVector3D( x, y, z ) );
}

void Transform3f::setTranslation( QVector3D v ) {
    translation = v;
    rebuildMatrix( );
}

void Transform3f::setTranslationX( float x ) {
    translation.setX( x );
    rebuildMatrix( );
}

void Transform3f::setTranslationY( float y ) {
    translation.setY( y );
    rebuildMatrix( );
}

void Transform3f::setTranslationZ( float z ) {
    translation.setZ( z );
    rebuildMatrix( );
}

void Transform3f::setScale( float xScale, float yScale, float zScale ) {
    setScale( QVector3D( xScale, yScale, zScale ) );
}

void Transform3f::setScale( QVector3D scale ) {
    this->scale = scale;
    rebuildMatrix( );
}

void Transform3f::setScale( float scale ) {
    setScale( QVector3D( scale, scale, scale ) );
}

void Transform3f::rebuildMatrix( ) {
    // Note that for Qt matrices the operations are applied in reverse
    _matrix.setToIdentity( );
    _matrix.translate( translation );
    _matrix.rotate( QQuaternion::fromEulerAngles( rotation ) );
    _matrix.scale( scale );
}
