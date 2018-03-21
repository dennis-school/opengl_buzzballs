#include "animation.h"

// Documentation can be found in the animation.h file

AnimatedBatch::AnimatedBatch( std::shared_ptr< GeneralBatch > pBatch,
                              const std::shared_ptr< Material >& pMaterial,
                              std::vector< std::shared_ptr< TransformAnimator > >& animators )
    : pBatch( pBatch ),
      pMaterial( pMaterial ),
      animators( animators ) {

}

void AnimatedBatch::render( QOpenGLShaderProgram *pProgram, float time ) {
    // Bind materials
    pMaterial->applyTo( *pProgram );

    QMatrix4x4 composedMat;
    for ( std::shared_ptr< TransformAnimator >& aTrans : animators ) {
        composedMat = composedMat * aTrans->transformAt( time ).matrix( );
    }

    pProgram->setUniformValue( "u_modelMat", composedMat );
    pProgram->setUniformValue( "u_normalMat", composedMat.normalMatrix() );

    // Draw here
    pBatch->draw( );
}

// -- RotationAnimator --

RotationAnimator::RotationAnimator( QVector3D rotVec )
    : rotVec( rotVec ) {

}

Transform3f RotationAnimator::transformAt( float time ) {
    Transform3f transform;
    transform.setRotation( rotVec.x( ) * time, rotVec.y( ) * time, rotVec.z( ) * time );
    return transform;
}

// -- BounceAnimator --

BounceAnimator::BounceAnimator( float lowY, float highY, float speed )
    : lowY( lowY ),
      highY( highY ),
      speed( speed ),
      frac( 0 ) {

}

Transform3f BounceAnimator::transformAt( float time ) {
    float frac = speed * time;
    // Put in range [0,2]
    frac -= ( (int) frac / 2 ) * 2;
    if ( frac > 1 ) {
        frac = 2 - frac;
    }
    Transform3f transform;
    transform.setTranslationY( highY * frac + lowY * ( 1 - frac ) );
    return transform;
}
