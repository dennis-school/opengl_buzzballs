#include "material.h"

Material::Material( QOpenGLFunctions_3_3_Core *pGl )
    // By OpenGL spec no valid texture can have ID 0 (as it equals GL_FALSE)
    : diffuseTexture( 0 )
    , normalTexture( 0 )
    , specularTexture( 0 )
    , ka( 0 )
    , ks( 0 )
    , kd( 0 )
    , p( 0 )
    , pGl( pGl ) {

}

Material::~Material( ) {
    GLuint textures[3];
    textures[0] = diffuseTexture;
    textures[1] = normalTexture;
    textures[2] = specularTexture;

    int numToDelete = 0;
    for ( int i = 0; i < 3; i++ ) {
        if ( textures[i] != 0 ) {
            textures[numToDelete] = textures[i];
            numToDelete++;
        }
    }

    pGl->glDeleteTextures( numToDelete, textures );
}

void Material::applyTo( QOpenGLShaderProgram& shaderProgram ) {
    // Note that not all uniforms actually exist in all shaders
    // however, for now pretend they do. Let OpenGL figure out
    // which ones exist, as this performance hit is neglectable
    // for this application.

    shaderProgram.setUniformValue( "u_color", color );
    shaderProgram.setUniformValue( "u_ka", ka );
    shaderProgram.setUniformValue( "u_ks", ks );
    shaderProgram.setUniformValue( "u_kd", kd );
    shaderProgram.setUniformValue( "u_p", p );

    if ( diffuseTexture != 0 ) {
        pGl->glActiveTexture( GL_TEXTURE0 );
        pGl->glBindTexture( GL_TEXTURE_2D, diffuseTexture );
        shaderProgram.setUniformValue( "u_diffuseTex", 0 );
    }

    if ( normalTexture != 0 ) {
        pGl->glActiveTexture( GL_TEXTURE1 );
        pGl->glBindTexture( GL_TEXTURE_2D, normalTexture );
        shaderProgram.setUniformValue( "u_normalTex", 1 );
    }

    if ( specularTexture != 0 ) {
        pGl->glActiveTexture( GL_TEXTURE2 );
        pGl->glBindTexture( GL_TEXTURE_2D, specularTexture );
        shaderProgram.setUniformValue( "u_specularTex", 2 );
    }
}
