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

#ifndef __itkImageHash_h
#define __itkImageHash_h

#include <gtest/gtest.h>
#include <itkImage.h>

namespace itk {
  namespace testing {
    template<class T, unsigned int dimension>
    std::string ImageSHA1Hash (itk::Image<T,dimension>* image );
  }
}

#include "itkImageHash.txx"

#endif
