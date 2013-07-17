
/*LICENSE_START*/
/*
 * Copyright 2013 Washington University,
 * All rights reserved.
 *
 * Connectome DB and Connectome Workbench are part of the integrated Connectome 
 * Informatics Platform.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of Washington University nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR  
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*LICENSE_END*/

#define __IMAGE_DIMENSIONS_MODEL_DECLARE__
#include "ImageDimensionsModel.h"
#undef __IMAGE_DIMENSIONS_MODEL_DECLARE__

#include "CaretAssert.h"
using namespace caret;


    
/**
 * \class caret::ImageDimensionsModel 
 * \brief Model for image sizing in both pixels and spatial units.
 * \ingroup Files
 *
 * Allows one to adjust dimensions for both pixels and spatial units.
 * Internally, spatial processing is in centimeters.
 */

/**
 * Constructor.
 */
ImageDimensionsModel::ImageDimensionsModel()
: CaretObject()
{
    m_pixelsPerCentimeter = 72.0 / CENTIMETERS_PER_INCH;
    setPixelWidthAndHeight(512, 512);
}

/**
 * Destructor.
 */
ImageDimensionsModel::~ImageDimensionsModel()
{
}

/**
 * Copy constructor.
 * @param obj
 *    Object that is copied.
 */
ImageDimensionsModel::ImageDimensionsModel(const ImageDimensionsModel& obj)
: CaretObject(obj)
{
    this->copyHelperImageDimensionsModel(obj);
}

/**
 * Assignment operator.
 * @param obj
 *    Data copied from obj to this.
 * @return 
 *    Reference to this object.
 */
ImageDimensionsModel&
ImageDimensionsModel::operator=(const ImageDimensionsModel& obj)
{
    if (this != &obj) {
        CaretObject::operator=(obj);
        this->copyHelperImageDimensionsModel(obj);
    }
    return *this;    
}

/**
 * Helps with copying an object of this type.
 * @param obj
 *    Object that is copied.
 */
void 
ImageDimensionsModel::copyHelperImageDimensionsModel(const ImageDimensionsModel& obj)
{
    m_pixelWidth          = obj.m_pixelWidth;
    m_pixelHeight         = obj.m_pixelHeight;
    m_centimetersWidth    = obj.m_centimetersWidth;
    m_centimetersHeight   = obj.m_centimetersHeight;
    m_pixelsPerCentimeter = obj.m_pixelsPerCentimeter;
}

/**
 * Get a description of this object's content.
 * @return String describing this object's content.
 */
AString 
ImageDimensionsModel::toString() const
{
    return "ImageDimensionsModel";
}

/**
 * @return The width in pixels.
 */
int32_t
ImageDimensionsModel::getPixelWidth() const
{
    /*
     * Internally, pixels are float, so round (by adding 0.5)
     * and return as integer.
     */
    return static_cast<int32_t>(m_pixelWidth + 0.5);
}

/**
 * @return The height in pixels.
 */
int32_t
ImageDimensionsModel::getPixelHeight() const
{
    /*
     * Internally, pixels are float, so round (by adding 0.5)
     * and return as integer.
     */
    return static_cast<int32_t>(m_pixelHeight + 0.5);
}

/**
 * Get the spatial width.
 *
 * @param spatialUnits
 *   Spatial units for the returned width.
 * @return
 *   Width in spatial units.
 */
float
ImageDimensionsModel::getSpatialWidth(const ImageSpatialUnitsEnum::Enum spatialUnits) const
{
    float width = 1.0;
    
    switch (spatialUnits) {
        case ImageSpatialUnitsEnum::CENTIMETERS:
            width = m_centimetersWidth;
            break;
        case ImageSpatialUnitsEnum::INCHES:
            width = m_centimetersWidth / CENTIMETERS_PER_INCH;
            break;
        case ImageSpatialUnitsEnum::MILLIMETERS:
            width = m_centimetersWidth * 100.0;
            break;
    }
    
    return width;
}

/**
 * Get the spatial height.
 *
 * @param spatialUnits
 *   Spatial units for the returned height.
 * @return
 *   Height in spatial units.
 */
float
ImageDimensionsModel::getSpatialHeight(const ImageSpatialUnitsEnum::Enum spatialUnits) const
{
    float height = 1.0;
    
    switch (spatialUnits) {
        case ImageSpatialUnitsEnum::CENTIMETERS:
            height = m_centimetersHeight;
            break;
        case ImageSpatialUnitsEnum::INCHES:
            height = m_centimetersHeight / CENTIMETERS_PER_INCH;
            break;
        case ImageSpatialUnitsEnum::MILLIMETERS:
            height = m_centimetersHeight * 100.0;
            break;
    }
    
    return height;
}

/**
 * Get the number of pixels per given spatial unit.
 *
 * @param pixelsPerSpatialUnit
 *   Pixels per spatial unit type.
 * @return
 *   Number of pixels per spatial unit.
 */
float
ImageDimensionsModel::getNumberOfPixelsPerSpatialUnit(const ImagePixelsPerSpatialUnitsEnum::Enum pixelsPerSpatialUnit) const
{
    float pixelsPerUnit = 1.0;
    
    switch (pixelsPerSpatialUnit) {
        case ImagePixelsPerSpatialUnitsEnum::PIXEL_PER_CENTIMETER:
            pixelsPerUnit = m_pixelsPerCentimeter;
            break;
        case ImagePixelsPerSpatialUnitsEnum::PIXELS_PER_INCH:
            pixelsPerUnit = m_pixelsPerCentimeter * CENTIMETERS_PER_INCH;
            break;
    }
    
    return pixelsPerUnit;
}

/**
 * Set the width and height of the image in pixels.
 *
 * @param pixelWidth
 *    New pixel width.
 * @param pixelHeight
 *    New pixel height.
 */
void
ImageDimensionsModel::setPixelWidthAndHeight(const int32_t pixelWidth,
                                             const int32_t pixelHeight)
{
    CaretAssert(pixelWidth > 0);
    CaretAssert(pixelHeight > 0);
    
    m_pixelWidth = pixelWidth;
    m_pixelHeight = pixelHeight;
    updateSpatialWidthAndHeightFromPixelWidthAndHeight();
}

/**
 * Set the pixel width to the given width.
 *
 * @param pixelWidth
 *    New pixel width.
 * @param preserveAspectRatio
 *    Maintain image aspect ratio.
 */
void
ImageDimensionsModel::setPixelWidth(const int32_t pixelWidth,
                                         const bool preserveAspectRatio)
{
    CaretAssert(pixelWidth > 0);
    
    const float aspectRatio = getAspectRatio();

    m_pixelWidth = pixelWidth;
    
    if (preserveAspectRatio) {
        m_pixelHeight = m_pixelWidth * aspectRatio;
    }
    
    updateSpatialWidthAndHeightFromPixelWidthAndHeight();
}

/**
 * Set the pixel height to the given height.
 *
 * @param pixelHeight
 *    New pixel height.
 * @param preserveAspectRatio
 *    Maintain image aspect ratio.
 */
void
ImageDimensionsModel::setPixelHeight(const int32_t pixelHeight,
                                          const bool preserveAspectRatio)
{
    CaretAssert(pixelHeight > 0);
    
    const float aspectRatio = getAspectRatio();
    
    m_pixelHeight = pixelHeight;
    
    if (preserveAspectRatio) {
        m_pixelWidth = m_pixelHeight / aspectRatio;
    }
    
    updateSpatialWidthAndHeightFromPixelWidthAndHeight();
}

/**
 * Set the spatial width to the given width.
 *
 * @param spatialWidth
 *    New spatial width.
 * @param spatialUnit
 *    The spatial unit of the given width.
 * @param preserveAspectRatio
 *    Maintain image aspect ratio.
 */
void
ImageDimensionsModel::setSpatialWidth(const float spatialWidth,
                     const ImageSpatialUnitsEnum::Enum spatialUnit,
                                           const bool preserveAspectRatio)
{
    CaretAssert(spatialWidth > 0);
    
    const float aspectRatio = getAspectRatio();
    
    switch (spatialUnit) {
        case ImageSpatialUnitsEnum::MILLIMETERS:
            m_centimetersWidth = spatialWidth / 100.0;
            break;
        case ImageSpatialUnitsEnum::INCHES:
            m_centimetersWidth = spatialWidth * CENTIMETERS_PER_INCH;
            break;
        case ImageSpatialUnitsEnum::CENTIMETERS:
            m_centimetersWidth = spatialWidth;
            break;
    }
    
    if (preserveAspectRatio) {
        m_centimetersHeight = m_centimetersWidth * aspectRatio;
    }
    
    updatePixelWidthAndHeightFromSpatialWidthAndHeight();
}

/**
 * Set the spatial height to the given height.
 *
 * @param spatialHeight
 *    New spatial height.
 * @param spatialUnit
 *    The spatial unit of the given height.
 * @param preserveAspectRatio
 *    Maintain image aspect ratio.
 */
void
ImageDimensionsModel::setSpatialHeight(const float spatialHeight,
                      const ImageSpatialUnitsEnum::Enum spatialUnit,
                                            const bool preserveAspectRatio)
{
    CaretAssert(spatialHeight > 0);
    
    const float aspectRatio = getAspectRatio();
    
    switch (spatialUnit) {
        case ImageSpatialUnitsEnum::MILLIMETERS:
            m_centimetersHeight = spatialHeight / 100.0;
            break;
        case ImageSpatialUnitsEnum::INCHES:
            m_centimetersHeight = spatialHeight / CENTIMETERS_PER_INCH;
            break;
        case ImageSpatialUnitsEnum::CENTIMETERS:
            m_centimetersHeight = spatialHeight;
            break;
    }
    
    if (preserveAspectRatio) {
        m_centimetersHeight = m_centimetersWidth * aspectRatio;
    }
    
    updatePixelWidthAndHeightFromSpatialWidthAndHeight();
}

/**
 * Set the number of pixels per spatial unit to the given value.
 *
 * @param numberOfPixelsPerSpatialUnit
 *    New value for number of pixels per spatial unit.
 * @param pixelsPerSpatialUnit
 *   Pixels per spatial unit type.
 */
void
ImageDimensionsModel::setNumberOfPixelsPerSpatialUnit(const float numberOfPixelsPerSpatialUnit,
                                                           const ImagePixelsPerSpatialUnitsEnum::Enum pixelsPerSpatialUnit)
{
    CaretAssert(numberOfPixelsPerSpatialUnit > 0);
    
    switch (pixelsPerSpatialUnit) {
        case ImagePixelsPerSpatialUnitsEnum::PIXELS_PER_INCH:
            m_pixelsPerCentimeter = numberOfPixelsPerSpatialUnit * CENTIMETERS_PER_INCH;
            break;
        case ImagePixelsPerSpatialUnitsEnum::PIXEL_PER_CENTIMETER:
            m_pixelsPerCentimeter = numberOfPixelsPerSpatialUnit;
            break;
    }
    
    updatePixelWidthAndHeightFromSpatialWidthAndHeight();
}

/**
 * Update the pixel width and height after a change to the spatial width 
 * and/or height.
 */
void
ImageDimensionsModel::updatePixelWidthAndHeightFromSpatialWidthAndHeight()
{
    m_pixelWidth  = m_centimetersWidth  * m_pixelsPerCentimeter;
    m_pixelHeight = m_centimetersHeight * m_pixelsPerCentimeter;
}

/**
 * Update the spatial width and height after a change to the pixel width
 * and/or height.
 */
void
ImageDimensionsModel::updateSpatialWidthAndHeightFromPixelWidthAndHeight()
{
    m_centimetersWidth  = m_pixelWidth  / m_pixelsPerCentimeter;
    m_centimetersHeight = m_pixelHeight / m_pixelsPerCentimeter;
}

/**
 * @return the aspect ration (height / width).
 */
float
ImageDimensionsModel::getAspectRatio() const
{
    if (m_pixelWidth == 0.0) {
        return 1;
    }
    
    return (static_cast<float>(m_pixelHeight)
            / static_cast<float>(m_pixelWidth));
}

