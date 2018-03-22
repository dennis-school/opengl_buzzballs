/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QMatrix4x4>
#include <QVector3D>

/**
 * @brief The Transform3f class is a set of 3 different types of transformations
 *   that are applied consecutively. First scaling is applied, then rotation,
 *   then translation. From these transformations a matrix can be extracted.
 *
 * The need for this class is introduced to compensate for otherwise need for
 *   introducing 3 separate transformation matrices (or otherwise variables).
 *   As simply only keeping a matrix would not allow for keeping the order of
 *   transformations (e.g. rotation after translation is different than
 *   translation after rotation). This class ensures that any of the composing
 *   transformations can be modified, while maintaining their order as defined
 *   above.
 */
class Transform3f {
public:
    /**
     * @brief Transform3f constructs an identity transformation
     */
    Transform3f( );

    Transform3f( float scale, QVector3D rotation, QVector3D translation );

    /**
     * @brief Translation creates a new Transform with only the specified
     *   scale set
     * @param s The scale
     * @return The resulting transform
     */
    static Transform3f Scale( float s );

    /**
     * @brief Translation creates a new Transform with only the specified
     *   rotation set
     * @param rotVec The euclidean rotation vector (in degrees)
     * @return The resulting transform
     */
    static Transform3f Rotation( QVector3D rotVec );

    /**
     * @brief Translation creates a new Transform with only the specified
     *   translation set
     * @param v The translation
     * @return The resulting transform
     */
    static Transform3f Translation( QVector3D v );

    /**
     * @brief matrix Returns the matrix associated with this transform
     * @return The QMatrix4x4 representation of this transform
     */
    QMatrix4x4 matrix( ) const;

    /**
     * @brief setRotation Sets the rotation of the transform.
     *   All values must be specified in degrees. Note that this resets
     *   any previously applied rotation;
     *
     * @param xAngle Rotation in degrees around the x axis
     * @param yAngle Rotation in degrees around the y axis
     * @param zAngle Rotation in degrees around the z axis
     */
    void setRotation( float xAngle, float yAngle, float zAngle );

    /**
     * @brief setTranslation Sets the translation of the transform.
     *   Note that this resets any previously applied translation.
     *
     * @param x The x-component of the new location in vector space
     * @param y The y-component of the new location in vector space
     * @param z The z-component of the new location in vector space
     */
    void setTranslation( float x, float y, float z );

    void setTranslationX( float x );

    void setTranslationY( float y );

    void setTranslationZ( float z );

    /**
     * @brief setTranslation Sets the translation of the transform.
     *   Note that this resets any previously applied translation.
     *
     * @param v The new location of the object in vector space
     */
    void setTranslation( QVector3D v );

    /**
     * @brief setScale Sets the scale of the transform independently
     *   for all axes. Note that this resets any previously applied scale.
     *
     * @param xScale The amount to scale across the x-axis
     * @param yScale The amount to scale across the y-axis
     * @param zScale The amount to scale across the z-axis
     */
    void setScale( float xScale, float yScale, float zScale );

    /**
     * @brief setScale Sets the scale of the transform independently
     *   for all axes. Note that this resets any previously applied scale.
     *
     * @param scale A vector describing the amount to scale across
     *   its components respective axes.
     */
    void setScale( QVector3D scale );

    /**
     * @brief setScale Sets the scale of the transform the same for
     *   all axes. Note that this resets any previously applied scale.
     *
     * @param scale The amount to scale across all axes
     */
    void setScale( float scale );
private:
    QVector3D translation;
    QVector3D rotation;
    QVector3D scale;

    QMatrix4x4 _matrix;

    void rebuildMatrix( );
};

#endif // TRANSFORM_H
