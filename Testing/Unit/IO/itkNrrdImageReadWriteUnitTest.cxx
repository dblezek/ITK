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
#include <fstream>
#include "itkTestHarness.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"

typedef float                      PixelType;
typedef itk::Image< PixelType, 3 > myImage;

class NrrdImageReadWrite:public ::testing::Test
  {
public:
  virtual void SetUp()
  {
    ASSERT_NO_THROW ( image = LoadImage< myImage >( dataFinder.GetFile ("Input/box.nhdr") ) ) << " Failed to load image";
  }

  myImage::Pointer image;
  };

TEST_F(NrrdImageReadWrite, Hash)
  {
  ASSERT_EQ ( "7a6507cf7c8138e365bcc8bc3ada4ffc1bfa8e15", ImageSHA1Hash< myImage >(image) );
  }

TEST_F(NrrdImageReadWrite, WriteRead)
  {
  // Generate test image
  itk::ImageFileWriter< myImage >::Pointer writer;
  writer = itk::ImageFileWriter< myImage >::New();
  writer->SetInput(image);
  writer->SetFileName( dataFinder.GetTempFile ("NrrdImageReadWrite-WriteRead.nrrd") );
  ASSERT_NO_THROW ( writer->Update() ) << "Failed to write an image";

  itk::ImageFileReader< myImage >::Pointer reader;
  reader = itk::ImageFileReader< myImage >::New();
  reader->SetFileName( dataFinder.GetTempFile ("NrrdImageReadWrite-WriteRead.nrrd") );
  myImage::Pointer loadedImage;
  ASSERT_NO_THROW ( reader->Update() );
  loadedImage = reader->GetOutput();
  ASSERT_TRUE ( loadedImage.IsNotNull() );
  ASSERT_EQ ( ImageSHA1Hash< myImage >(image),
             ImageSHA1Hash< myImage >(loadedImage) ) << "Written and read image is not the same as original";
  }
