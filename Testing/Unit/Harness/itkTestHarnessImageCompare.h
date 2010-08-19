/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkImage.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkTestHarnessImageCompare_h
#define __itkTestHarnessImageCompare_h

#include <gtest/gtest.h>
#include <itkImage.h>

namespace itk
{
namespace testing
{
template< class ImageType >
class ImageCompare
  {
public:
  bool Compare(const typename ImageType::Pointer image, std::string name, double tolerance = 0.0);

  std::string GetMessage();

protected:
  std::string m_Message;
  };
}
}

#define EXPECT_IMAGE_EQ(ImageType, image, name) \
    {                                             \
    itk::testing::ImageCompare< ImageType > imageCompare;                 \
    EXPECT_TRUE ( imageCompare.Compare (image, name) ) << imageCompare.GetMessage(); \
    }
#define EXPECT_IMAGE_NEAR(ImageType, image, name, tolerance)      \
    {                                             \
    itk::testing::ImageCompare< ImageType > imageCompare;                 \
    EXPECT_TRUE ( imageCompare.Compare (image, name, tolerance) ) << imageCompare.GetMessage(); \
    }

#define ASSERT_IMAGE_EQ(ImageType, image, name) \
    {                                             \
    itk::testing::ImageCompare< ImageType > imageCompare;                 \
    ASSERT_TRUE ( imageCompare.Compare (image, name) ) << imageCompare.GetMessage(); \
    }
#define ASSERT_IMAGE_NEAR(ImageType, image, name, tolerance)      \
    {                                             \
    itk::testing::ImageCompare< ImageType > imageCompare;                 \
    ASSERT_TRUE ( imageCompare.Compare (image, name, tolerance) ) << imageCompare.GetMessage(); \
    }

#include "itkTestHarnessImageCompare.txx"

#endif
