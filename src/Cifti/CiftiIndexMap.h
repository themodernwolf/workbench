#ifndef __CIFTI_INDEX_MAP_H__
#define __CIFTI_INDEX_MAP_H__

/*LICENSE_START*/
/*
 *  Copyright 1995-2011 Washington University School of Medicine
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

#include "stdint.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace caret
{
    class CiftiIndexMap
    {
    public:
        enum MappingType
        {
            BRAIN_MODELS,
            PARCELS,
            SERIES,
            SCALARS,
            LABELS
        };
        virtual CiftiIndexMap* clone() const = 0;//make a copy, preserving the actual type - NOTE: this returns a dynamic allocation that is not owned by anything
        virtual MappingType getType() const = 0;
        virtual int64_t getLength() const = 0;
        virtual bool operator==(const CiftiIndexMap& rhs) const = 0;//used to check for merging mappings when writing the XML - must compare EVERYTHING that goes into the XML
        bool operator!=(const CiftiIndexMap& rhs) const { return !((*this) == rhs); }
        virtual bool approximateMatch(const CiftiIndexMap& rhs) const = 0;//check if things like doing index-wise math would make sense
        virtual void readXML1(QXmlStreamReader& xml) = 0;//mainly to shorten the type-specific code in CiftiXML
        virtual void readXML2(QXmlStreamReader& xml) = 0;
        virtual void writeXML1(QXmlStreamWriter& xml) const = 0;
        virtual void writeXML2(QXmlStreamWriter& xml) const = 0;
        virtual ~CiftiIndexMap();
    };
}

#endif //__CIFTI_INDEX_MAP_H__
