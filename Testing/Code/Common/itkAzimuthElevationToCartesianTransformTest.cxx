/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkAzimuthElevationToCartesianTransformTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>

#include "itkAzimuthElevationToCartesianTransform.h"
#include "itkTestHarness.h"

typedef  double  CoordinateRepresentationType;

typedef  itk::Point<CoordinateRepresentationType,3>   PointType;

TEST(CommonTest, AzimuthElevationToCartesianTransformTest)
{

    const CoordinateRepresentationType ACCEPTABLE_ERROR = 1E-10;

    typedef itk::AzimuthElevationToCartesianTransform<
                             CoordinateRepresentationType 
                                  > AzimuthElevationToCartesianTransformType;

    AzimuthElevationToCartesianTransformType::Pointer transform = 
                              AzimuthElevationToCartesianTransformType::New();

    transform->SetAzimuthElevationToCartesianParameters(1.0,5.0,45,45);
    PointType p;
    p[0] = 3;
    p[1] = 3;
    p[2] = 25;

    transform->SetForwardAzimuthElevationToCartesian();

    PointType answer = transform->TransformPoint(p);
    PointType answerBackwards = transform->BackTransformPoint(answer);
    transform->SetForwardCartesianToAzimuthElevation();
    PointType reverseDirectionAnswer = transform->BackTransformPoint(answerBackwards);
    PointType reverseDirectionAnswerBackwards = transform->TransformPoint(reverseDirectionAnswer);

    for (unsigned int i=0; i < p.PointDimension; i++)
      {
        ASSERT_NEAR ( p[i], answerBackwards[i], ACCEPTABLE_ERROR);
        ASSERT_NEAR ( p[i], reverseDirectionAnswerBackwards[i], ACCEPTABLE_ERROR);
        ASSERT_NEAR ( answer[i], reverseDirectionAnswer[i], ACCEPTABLE_ERROR);
      }
}
