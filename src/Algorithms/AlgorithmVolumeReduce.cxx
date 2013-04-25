/*LICENSE_START*/
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

#include "AlgorithmVolumeReduce.h"
#include "AlgorithmException.h"
#include "CaretLogger.h"
#include "GiftiLabelTable.h"
#include "ReductionOperation.h"
#include "VolumeFile.h"

#include <vector>

using namespace caret;
using namespace std;

AString AlgorithmVolumeReduce::getCommandSwitch()
{
    return "-volume-reduce";
}

AString AlgorithmVolumeReduce::getShortDescription()
{
    return "PERFORM REDUCTION OPERATION ACROSS SUBVOLUMES";
}

OperationParameters* AlgorithmVolumeReduce::getParameters()
{
    OperationParameters* ret = new OperationParameters();
    ret->addVolumeParameter(1, "volume-in", "the volume file to reduce");
    
    ret->addStringParameter(2, "operation", "the reduction operator to use");
    
    ret->addVolumeOutputParameter(3, "volume-out", "the output volume");
    
    OptionalParameter* excludeOpt = ret->createOptionalParameter(4, "-exclude-outliers", "exclude outliers from each timeseries by standard deviation");
    excludeOpt->addDoubleParameter(1, "sigma-below", "number of standard deviations below the mean to include");
    excludeOpt->addDoubleParameter(2, "sigma-above", "number of standard deviations above the mean to include");
    
    ret->setHelpText(
        AString("For each voxel, takes the data across subvolumes as a vector, and performs the specified reduction on it, putting the result ") +
        "into the single output volume at that voxel.  The reduction operators are as follows:\n\n" + ReductionOperation::getHelpInfo()
    );
    return ret;
}

void AlgorithmVolumeReduce::useParameters(OperationParameters* myParams, ProgressObject* myProgObj)
{
    VolumeFile* volumeIn = myParams->getVolume(1);
    AString opString = myParams->getString(2);
    VolumeFile* volumeOut = myParams->getOutputVolume(3);
    OptionalParameter* excludeOpt = myParams->getOptionalParameter(4);
    bool ok = false;
    ReductionEnum::Enum myReduce = ReductionEnum::fromName(opString, &ok);
    if (!ok) throw AlgorithmException("unrecognized operation string '" + opString + "'");
    if (excludeOpt->m_present)
    {
        AlgorithmVolumeReduce(myProgObj, volumeIn, myReduce, volumeOut, excludeOpt->getDouble(1), excludeOpt->getDouble(2));
    } else {
        AlgorithmVolumeReduce(myProgObj, volumeIn, myReduce, volumeOut);
    }
}

AlgorithmVolumeReduce::AlgorithmVolumeReduce(ProgressObject* myProgObj, const VolumeFile* volumeIn, const ReductionEnum::Enum& myReduce, VolumeFile* volumeOut) : AbstractAlgorithm(myProgObj)
{
    LevelProgress myProgress(myProgObj);
    vector<int64_t> myDims, newDims = volumeIn->getOriginalDimensions();
    newDims.resize(3, 1);//have only one subvolume
    volumeIn->getDimensions(myDims);
    volumeOut->reinitialize(newDims, volumeIn->getSform(), myDims[4], volumeIn->getType());
    if (volumeIn->getType() == SubvolumeAttributes::LABEL)
    {
        CaretLogWarning("reduction operation performed on label volume");
        *(volumeOut->getMapLabelTable(0)) = *(volumeIn->getMapLabelTable(0));
    }
    int64_t frameSize = myDims[0] * myDims[1] * myDims[2];
    vector<float> scratchArray(myDims[3]), outFrame(frameSize);
    for (int c = 0; c < myDims[4]; ++c)
    {
        for (int64_t i = 0; i < frameSize; ++i)
        {
            for (int b = 0; b < myDims[3]; ++b)
            {
                const float* tempFrame = volumeIn->getFrame(b, c);
                scratchArray[b] = tempFrame[i];
            }
            outFrame[i] = ReductionOperation::reduce(scratchArray.data(), myDims[3], myReduce);
        }
        volumeOut->setFrame(outFrame.data(), 0, c);
    }
}

AlgorithmVolumeReduce::AlgorithmVolumeReduce(ProgressObject* myProgObj, const VolumeFile* volumeIn, const ReductionEnum::Enum& myReduce, VolumeFile* volumeOut, const float& sigmaBelow, const float& sigmaAbove) : AbstractAlgorithm(myProgObj)
{
    LevelProgress myProgress(myProgObj);
    vector<int64_t> myDims, newDims = volumeIn->getOriginalDimensions();
    newDims.resize(3, 1);//have only one subvolume
    volumeIn->getDimensions(myDims);
    volumeOut->reinitialize(newDims, volumeIn->getSform(), myDims[4], volumeIn->getType());
    if (volumeIn->getType() == SubvolumeAttributes::LABEL)
    {
        CaretLogWarning("reduction operation performed on label volume");
        *(volumeOut->getMapLabelTable(0)) = *(volumeIn->getMapLabelTable(0));
    }
    int64_t frameSize = myDims[0] * myDims[1] * myDims[2];
    vector<float> scratchArray(myDims[3]), outFrame(frameSize);
    for (int c = 0; c < myDims[4]; ++c)
    {
        for (int64_t i = 0; i < frameSize; ++i)
        {
            for (int b = 0; b < myDims[3]; ++b)
            {
                const float* tempFrame = volumeIn->getFrame(b, c);
                scratchArray[b] = tempFrame[i];
            }
            outFrame[i] = ReductionOperation::reduceExcludeDev(scratchArray.data(), myDims[3], myReduce, sigmaBelow, sigmaAbove);
        }
        volumeOut->setFrame(outFrame.data(), 0, c);
    }
}

float AlgorithmVolumeReduce::getAlgorithmInternalWeight()
{
    return 1.0f;//override this if needed, if the progress bar isn't smooth
}

float AlgorithmVolumeReduce::getSubAlgorithmWeight()
{
    //return AlgorithmInsertNameHere::getAlgorithmWeight();//if you use a subalgorithm
    return 0.0f;
}
