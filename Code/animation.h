#ifndef ANIMATION_H
#define ANIMATION_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <memory>
#include <QVector3D>
#include "transform.h"
#include "material.h"
#include "batch.h"

/**
 * @brief The TransformAnimator class is the super class for any animation
 *   on a Transform3f.
 */
class TransformAnimator {
public:
    virtual Transform3f transformAt( float time ) = 0;
};

/**
 * @brief The AnimatedBatch class is a batch that has a base transform with zero or more
 *   animated transforms. These transforms are evaluated at render time, and then concatenated
 *   for the final transform at that particular time.
 */
class AnimatedBatch {
public:
    std::shared_ptr< GeneralBatch > pBatch;
    std::shared_ptr< Material > pMaterial;
    std::vector< std::shared_ptr< TransformAnimator > > animators;

    AnimatedBatch( std::shared_ptr< GeneralBatch > pBatch,
                   const std::shared_ptr< Material >& pMaterial,
                   std::vector< std::shared_ptr< TransformAnimator > >& animators );
    /**
     * @brief render Renders the batch with the transformation at the specified time
     * @param pProgram The program for which the material should be set
     * @param time The time at which the transforms should be evaluated
     */
    void render( QOpenGLShaderProgram *pProgram, float time );
private:
    QOpenGLFunctions_3_3_Core *pGl;
};

/**
 * @brief The RotationAnimator class is an animation that rotates by the given euclidean
 *   rotation vector every time unit.
 */
class RotationAnimator : public TransformAnimator {
public:
    RotationAnimator( QVector3D rotVec );
    Transform3f transformAt( float time );
private:
    QVector3D rotVec;
};

/**
 * @brief The BounceAnimator class is an animation that bounces linearly between the given
 *   two locations on the y-axis.
 */
class BounceAnimator : public TransformAnimator {
public:
    BounceAnimator( float lowY, float highY, float speed );
    Transform3f transformAt( float time );
private:
    float lowY, highY;
    float speed;
    float frac;
};

/**
 * @brief The ConstantAnimator class is an a constant "animation". At any time interval the
 *   same transform will be obtained.
 *
 * This class is necessary to allow a constant transform to be part of the animation sequence.
 *   That is, for example, a constant translation occurring between two rotations. (Resulting
 *   in one translation around the origin in local space, and another around the origin in
 *   "translated space")
 */
class ConstantAnimator : public TransformAnimator {
public:
    ConstantAnimator( Transform3f t ) : transform( t ) { }
    Transform3f transformAt( float time ) {
        Q_UNUSED( time )
        return transform;
    }
private:
    Transform3f transform;
};

#endif // ANIMATION_H
