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

#ifndef __itkTestHarnessDataFinder_h
#define __itkTestHarnessDataFinder_h

#include <gtest/gtest.h>
#include <itkImage.h>
#include "itkTestHarnessImageHash.h"

namespace itk
{
namespace testing
{
class DataFinder
  {
public:
  DataFinder ();
  void SetDirectory(std::string dir);

  void SetTempDirectory(std::string dir);

  std::string GetFile(std::string filename);

  std::string GetDirectory();

  std::string GetTempDirectory();

  std::string GetTempFile(std::string filename);

  // Some helper functions
  ::itk::Image< float, 3 >::Pointer CreateFloatVolume(long width, long height, long depth, float value);

  ::itk::Image< float, 2 >::Pointer CreateFloatImage(long width, long height, float value);

  ::itk::Image< short, 3 >::Pointer CreateShortVolume(long width, long height, long depth, short value);

  ::itk::Image< short, 2 >::Pointer CreateShortImage(long width, long height, short value);

  ::itk::Image< char, 3 >::Pointer CreateCharVolume(long width, long height, long depth, char value);

  ::itk::Image< char, 2 >::Pointer CreateCharImage(long width, long height, char value);

  std::string SHA1Hash(void *buffer, long bufferSizeInBytes);

protected:
  std::string m_Directory;
  std::string m_TempDirectory;
  };
}
}
extern ::itk::testing::DataFinder dataFinder;

#endif
