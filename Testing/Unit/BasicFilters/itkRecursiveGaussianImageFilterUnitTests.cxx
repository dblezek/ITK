/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkNrrdImageReadWriteTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined( _MSC_VER )
#pragma warning ( disable : 4786 )
#endif

#include "itkTestHarness.h"
#include <itkRecursiveGaussianImageFilter.h>

typedef itk::Image< float, 3 > FloatImage;
typedef itk::Image< short, 3 > ShortImage;
typedef itk::RecursiveGaussianImageFilter< FloatImage, FloatImage > GaussianFilterType;
typedef itk::RecursiveGaussianImageFilter< ShortImage, ShortImage > GaussianFilterTypeShort;

class RecursiveGaussianImageFilter : public ::testing::Test
{
public:
  virtual void SetUp()
  {
    filter = GaussianFilterType::New();
    filterShort = GaussianFilterTypeShort::New();
    EXPECT_NO_THROW ( floatImage = LoadImage<FloatImage> ( dataFinder.GetFile ( "Input/HeadMRVolumeWithDirection.nhdr" ) ) ) << "Failed to load float volume";
    EXPECT_NO_THROW ( shortImage = LoadImage<ShortImage> ( dataFinder.GetFile ( "Input/HeadMRVolumeWithDirection.nhdr" ) ) ) << "Failed to load short volume";
  }
  GaussianFilterType::Pointer filter;
  GaussianFilterTypeShort::Pointer filterShort;
  FloatImage::Pointer floatImage;
  ShortImage::Pointer shortImage;
};

TEST_F(RecursiveGaussianImageFilter, FloatBasics)
{
  filter->SetInput (floatImage);
  EXPECT_IMAGE_NEAR(FloatImage, filter->GetOutput(), "float", 1.0);
  filter->SetSigma(2.0);
  filter->SetZeroOrder();
  EXPECT_IMAGE_NEAR(FloatImage, filter->GetOutput(), "sigma_2.0", 1.0);
}

TEST_F(RecursiveGaussianImageFilter, ShortBasics)
{
  filterShort->SetInput (shortImage);
  EXPECT_IMAGE_NEAR(ShortImage, filterShort->GetOutput(), "short", 1.0);
  filterShort->SetSigma(2.0);
  filterShort->SetZeroOrder();
  EXPECT_IMAGE_NEAR(ShortImage, filterShort->GetOutput(), "sigma_2.0", 1.0);
}

TEST_F(RecursiveGaussianImageFilter,ZSmoothing)
{
  filter->SetDirection( 2 );  // apply along Z
  filter->SetOrder( GaussianFilterType::ZeroOrder );
  filter->SetInput ( floatImage );
  EXPECT_IMAGE_NEAR(FloatImage, filter->GetOutput(), "ZeroOrder", 1.0);
  filter->SetOrder ( GaussianFilterType::FirstOrder );
  EXPECT_IMAGE_NEAR(FloatImage, filter->GetOutput(), "FirstOrder", 1.0);
  filter->SetOrder ( GaussianFilterType::SecondOrder );
  EXPECT_IMAGE_NEAR(FloatImage, filter->GetOutput(), "SecondOrder", 1.0);
}
