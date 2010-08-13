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

#ifndef __itkTestHarnessHelper_h
#define __itkTestHarnessHelper_h

#include <gtest/gtest.h>
#include <itkImage.h>
#include "itkImageHash.h"

namespace itk {
  namespace testing {
    template<class ImageType>
    class ImageCompare {
    public:
      bool Compare ( const ImageType* image, std::string name );
      std::string GetMessage();
    protected:
      std::string m_Message;
    };
  }
}

#include "itkImageCompare.txx"


#endif
