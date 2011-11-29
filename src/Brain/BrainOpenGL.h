
#ifndef __BRAIN_OPENGL_H__
#define __BRAIN_OPENGL_H__

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

#include <stdint.h>

#include "CaretObject.h"

class QGLWidget;

namespace caret {
    
    class BrainOpenGLViewportContent;
    class IdentificationManager;
    
    /**
     * Performs drawing of graphics using OpenGL.
     */
    class BrainOpenGL : public CaretObject {
        
    protected:
        BrainOpenGL(QGLWidget* parentGLWidget);

    public:
        virtual ~BrainOpenGL();
        
        /**
         * Initialize the OpenGL system.
         */
        virtual void initializeOpenGL() = 0;
        
        /**
         * Draw models in their respective viewports.
         *
         * @param viewportContents
         *    Viewport info for drawing.
         */
        virtual void drawModels(std::vector<BrainOpenGLViewportContent*>& viewportContents) = 0;
        
        /**
         * Selection on a model.
         *
         * @param viewportConent
         *    Viewport content in which mouse was clicked
         * @param mouseX
         *    X position of mouse click
         * @param mouseY
         *    Y position of mouse click
         */
        virtual void selectModel(BrainOpenGLViewportContent* viewportContent,
                         const int32_t mouseX,
                         const int32_t mouseY) = 0;
        
        /**
         * Initializle the orthographic viewport.
         * @param windowIndex
         *    Index of the window.
         * @param width
         *    Width of the window.
         * @param height
         *    Height of the window.
         */
        virtual void updateOrthoSize(const int32_t windowIndex, 
                             const int32_t width, 
                             const int32_t height) = 0;

        /**
         * @return Half-size of the model window height.
         */
        static float getModelViewingHalfWindowHeight() { return 90.0f; }
        
        /**
         * @return The identification manager.
         */
        IdentificationManager* getIdentificationManager();

    private:
        BrainOpenGL(const BrainOpenGL&);
        BrainOpenGL& operator=(const BrainOpenGL&);
        
        
    protected:
        /** Optional Qt QGLWidget that uses this OpenGL rendering. */
        QGLWidget* parentGLWidget;
        
        /** version number of OpenGL */
        static float versionOfOpenGL;

    private:
        /** Identification manager */
        IdentificationManager* identificationManager;
    };

#ifdef __BRAIN_OPENGL_DEFINE_H
    float BrainOpenGL::versionOfOpenGL = 0.0f;
#endif //__BRAIN_OPENGL_DEFINE_H

} // namespace


#endif // __BRAIN_OPENGL_H__
