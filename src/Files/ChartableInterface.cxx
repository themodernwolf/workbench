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

#define __CHARTABLE_INTERFACE_DECLARE__
#include "ChartableInterface.h"
#undef __CHARTABLE_INTERFACE_DECLARE__

using namespace caret;

/**
 * \class caret::ChartableInterface
 * \brief Interface for files that are able to produce charts.
 * \ingroup Files
 */


/**
 * Is the given chart data type supported by this file.
 *
 * @param chartDataType
 *    Chart data type for testing support.
 * @return
 *    True if chart data type is supported by the file, else false.
 */
bool
ChartableInterface::isChartDataTypeSupported(const ChartDataTypeEnum::Enum chartDataType) const
{
    std::vector<ChartDataTypeEnum::Enum> validTypes;
    getSupportedChartDataTypes(validTypes);
    
    if (std::find(validTypes.begin(),
                  validTypes.end(),
                  chartDataType) != validTypes.end()) {
        return true;
    }
    
    return false;
}