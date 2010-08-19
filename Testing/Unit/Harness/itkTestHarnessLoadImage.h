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

#ifndef __itkTestHarnessLoadImage_h
#define __itkTestHarnessLoadImage_h

#include <itkImage.h>
#include <itkImageFileReader.h>

template< class ImageType >
typename ImageType::Pointer LoadImage(const std::string filename)
throw ( itk::ExceptionObject )
{
  typename itk::ImageFileReader< ImageType >::Pointer reader = itk::ImageFileReader< ImageType >::New();
  typename ImageType::Pointer image;
  reader->SetFileName ( filename.c_str() );
  reader->Update();
  image = reader->GetOutput();
  return image;
}

#endif
