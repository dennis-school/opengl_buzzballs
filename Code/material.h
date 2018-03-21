#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

/**
 * @brief The Material struct is a material that can be applied to a shader
 *   prior to rendering a mesh.
 */
struct Material {
    GLuint diffuseTexture;
    GLuint normalTexture;
    GLuint specularTexture;

    QVector3D color;

    float ka; // Ambient multiplier
    float ks; // Specular multiplier
    float kd; // Diffuse multiplier
    float p; // Specular exponent (shininess)

    QOpenGLFunctions_3_3_Core *pGl;

    Material( QOpenGLFunctions_3_3_Core *pGl );
    ~Material( );
    void applyTo( QOpenGLShaderProgram& shaderProgram );
};

#endif // MATERIAL_H
