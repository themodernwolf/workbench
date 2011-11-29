

/*
 *  Copyright 1995-2002 Washington University School of Medicine
 *
 *  http://brainmap.wustl.edu
 *
 *  This file is part of CARET.
 *
 *  CARET is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  CARET is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CARET; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
/*LICENSE_END*/

#include <cstdlib>

#include <QGLWidget>

#define __BRAIN_OPENGL_DEFINE_H
#include "BrainOpenGL.h"
#undef __BRAIN_OPENGL_DEFINE_H

#include "IdentificationManager.h"

using namespace caret;

/**
 * Constructor.
 *
 * @param parentGLWidget
 *   The Qt QGLWidget that uses this instance for OpenGL
 *   rendering.  The QGLWidget is used for text rendering.
 *   This parameter may be NULL in which case no text
 *   rendering is performed.
 */
BrainOpenGL::BrainOpenGL(QGLWidget* parentGLWidget)
{
    this->parentGLWidget = parentGLWidget;
    this->identificationManager = new IdentificationManager();
}

/**
 * Destructor.
 */
BrainOpenGL::~BrainOpenGL()
{
    delete this->identificationManager;
    this->identificationManager = NULL;
}

/**
 * @return The identification manager.
 */
IdentificationManager* 
BrainOpenGL::getIdentificationManager()
{
    return this->identificationManager;
}

