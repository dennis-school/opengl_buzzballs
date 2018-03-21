/* Authors: Dennis G. Sprokholt (s2983842), Luigi Gao (s2915375) */

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "batch.h"
#include <memory>
#include <QOpenGLFunctions_3_3_Core>

std::unique_ptr< Batch > createCube( QOpenGLFunctions_3_3_Core *pGl );

std::unique_ptr< Batch > createPyramid( QOpenGLFunctions_3_3_Core *pGl );

#endif // PRIMITIVES_H
