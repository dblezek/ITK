/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRecursiveGaussianImageFiltersTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkTestHarnessDataFinder.h"
#include "hl_sha1.h"

::itk::testing::DataFinder dataFinder;

namespace itk
{
namespace testing
{
DataFinder::DataFinder ()
  {
  m_Directory = "NotFoundAnywhere";
  }

void DataFinder::SetDirectory(std::string dir)
{
  m_Directory = dir;
}

void DataFinder::SetTempDirectory(std::string dir)
{
  m_TempDirectory = dir;
}

std::string DataFinder::GetDirectory() { return m_Directory; }
std::string DataFinder::GetTempDirectory() { return m_TempDirectory; }
std::string DataFinder::GetTempFile(std::string filename) { return m_TempDirectory + "/" + filename; }
std::string DataFinder::GetFile(std::string filename) { return m_Directory + "/" + filename; }

static itk::ImageRegion< 3 > GetRegion(long w, long h, long d)
{
  itk::ImageRegion< 3 > region;
  itk::Size< 3 >        size;
  size[0] = w; size[1] = h; size[2] = d;
  itk::Index< 3 > index;
  index[0] = 0; index[1] = 0; index[2] = 0;
  region.SetSize(size);
  region.SetIndex(index);
  return region;
}

static itk::ImageRegion< 2 > GetRegion(long w, long h)
{
  itk::ImageRegion< 2 > region;
  itk::Size< 2 >        size;
  size[0] = w; size[1] = h;
  itk::Index< 2 > index;
  index[0] = 0; index[1] = 0;
  region.SetSize(size);
  region.SetIndex(index);
  return region;
}

itk::Image< float, 3 >::Pointer DataFinder::CreateFloatVolume(long width, long height, long depth, float value)
{
  itk::ImageRegion< 3 >           region = GetRegion (width, height, depth);
  itk::Image< float, 3 >::Pointer image = itk::Image< float, 3 >::New();
  image->SetLargestPossibleRegion (region);
  image->SetBufferedRegion (region);
  image->SetRequestedRegion (region);
  image->Allocate();
  image->FillBuffer (value);
  return image;
}

itk::Image< float, 2 >::Pointer DataFinder::CreateFloatImage(long width, long height, float value)
{
  itk::ImageRegion< 2 >           region = GetRegion (width, height);
  itk::Image< float, 2 >::Pointer image = itk::Image< float, 2 >::New();
  image->SetLargestPossibleRegion (region);
  image->SetBufferedRegion (region);
  image->SetRequestedRegion (region);
  image->Allocate();
  image->FillBuffer (value);
  return image;
}

itk::Image< short, 3 >::Pointer DataFinder::CreateShortVolume(long width, long height, long depth, short value)
{
  itk::ImageRegion< 3 >           region = GetRegion (width, height, depth);
  itk::Image< short, 3 >::Pointer image = itk::Image< short, 3 >::New();
  image->SetLargestPossibleRegion (region);
  image->SetBufferedRegion (region);
  image->SetRequestedRegion (region);
  image->Allocate();
  image->FillBuffer (value);
  return image;
}

itk::Image< short, 2 >::Pointer DataFinder::CreateShortImage(long width, long height, short value)
{
  itk::ImageRegion< 2 >           region = GetRegion (width, height);
  itk::Image< short, 2 >::Pointer image = itk::Image< short, 2 >::New();
  image->SetLargestPossibleRegion (region);
  image->SetBufferedRegion (region);
  image->SetRequestedRegion (region);
  image->Allocate();
  image->FillBuffer (value);
  return image;
}

itk::Image< char, 3 >::Pointer DataFinder::CreateCharVolume(long width, long height, long depth, char value)
{
  itk::ImageRegion< 3 >          region = GetRegion (width, height, depth);
  itk::Image< char, 3 >::Pointer image = itk::Image< char, 3 >::New();
  image->SetLargestPossibleRegion (region);
  image->SetBufferedRegion (region);
  image->SetRequestedRegion (region);
  image->Allocate();
  image->FillBuffer (value);
  return image;
}

itk::Image< char, 2 >::Pointer DataFinder::CreateCharImage(long width, long height, char value)
{
  itk::ImageRegion< 2 >          region = GetRegion (width, height);
  itk::Image< char, 2 >::Pointer image = itk::Image< char, 2 >::New();
  image->SetLargestPossibleRegion (region);
  image->SetBufferedRegion (region);
  image->SetRequestedRegion (region);
  image->Allocate();
  image->FillBuffer (value);
  return image;
}
}
}
