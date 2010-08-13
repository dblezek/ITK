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

#include "itkImageCompare.h"

namespace itk {
  namespace testing {
    
    template<class ImageType>
    std::string ImageCompare::GetMessage() { return m_Message; }
      
    template<class ImageType>
    bool Compare ( const ImageType* image, std::string name ) {
      if ( image == NULL ) {
        m_Message = "ImageCompare: image is null";
        return false;
      }
      // Does the baseline exist?
      std::string extension = ".nrrd";
      
      std::string rootFilename = std::string ( ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name() )
        .append( "_" )                                                        
        .append(::testing::UnitTest::GetInstance()->current_test_info()->name())
        .append("_")
        .append ( name );

      std::string baselineFilename = mDataDirectory + "/Baseline/" + rootFilename + extension;
  
      if ( !itksys::SystemTools::FileExists ( baselineFilename.c_str() ) ) {
        // Baseline does not exist, write out what we've been given
        std::string newBaselineDir = dataFinder.GetTempDirectory() + "/Newbaseline/";
        itksys::SystemTools::MakeDirectory ( newBaselineDir.c_str() );
        std::string newBaseline = newBaselineDir + rootFilename + extension;

        // Save the image
        itk::ImageFileWriter<ImageType>::Pointer Writer = itk::ImageFileWriter<ImageType>::New();
        Writer->SetFilename ( newBaseline );
        Writer->SetInput ( image );
        try {
          Writer->Write();
        } catch ( itk::ImageFileWriterException e ) {
          m_Message = "Error writing file: " + newBaseline + ": " + e.what();
          return false;
        }
        m_Message = "Baseline does not exist, wrote " + newBaseline + "\ncp " + newBaseline + " " + baselineFilename;
        return false;
      }

      typename ImageType::Pointer baseline;
      
      try {
        itk::ImageFileReader<ImageType>::Pointer Reader = itk::ImageFileReader<ImageType>::New();
        Reader->SetFilename ( baselineFilename );
        Reader->Update();
        baseline = Reader->GetOutput();
      } catch ( itk::ImageFileReaderException e ) {
        m_Message = "ImageCompare: Failed to load image " + baselineFilename + " because: " + e.what();
        return false;
      }

      // Do the diff
      if ( itk::testing::ImageSHA1Hash<ImageType::PixelType,ImageType::ImageDimension> (baseline)
           == itk::testing::ImageSHA1Hash<ImageType::PixelType,ImageType::ImageDimension> (image) ) {
        // Nothing else to do
        return true;
      }
      
      itk::ImageRegion<ImageType::ImageDimension> region, baselineRegion;
      region = image->GetLargestPossibleRegion();
      baselineRegion = baseline->GetLargestPossibleRegion();
      
      if ( region != baselineRegion ) {
        mMessage = "ImageCompare: Image regions are different";
        return false;
      }

      // Images are different, so calculate differences, etc...
      // Get the center slices
      typedef itk::DifferenceImageFilter<ImageType,ImageType> DiffType;
      DiffType::Pointer diff = DiffType::New();
      diff->SetValidInput(baseline);
      diff->SetTestInput(image);
      diff->SetDifferenceThreshold(2.0);
      diff->UpdateLargestPossibleRegion();
      double status = diff->GetTotalDifference();
        
      typedef itk::Image<unsigned char,ImageType::ImageDimension>    OutputType;
      typedef itk::Image<unsigned char,2>                         DiffOutputType;
      
      typedef itk::RescaleIntensityImageFilter<ImageType,OutputType>    RescaleType;
      typedef itk::ExtractImageFilter<OutputType,DiffOutputType>        ExtractType;
      typedef itk::ImageFileWriter<DiffOutputType>                      WriterType;
      typedef itk::ImageRegion<ImageType::ImageDimension>                  RegionType;

      OutputType::IndexType index; index.Fill(0);
      OutputType::SizeType size; size.Fill(0);

      RescaleType::Pointer rescale = RescaleType::New();

      rescale->SetOutputMinimum(itk::NumericTraits<unsigned char>::NonpositiveMin());
      rescale->SetOutputMaximum(itk::NumericTraits<unsigned char>::max());
      rescale->SetInput(diff->GetOutput());
      rescale->UpdateLargestPossibleRegion();

      RegionType region;
      region.SetIndex(index);
    
      size = rescale->GetOutput()->GetLargestPossibleRegion().GetSize();
      for (unsigned int i = 2; i < ImageType::ImageDimension; i++)
        {
          if ( i == 3 ) {
            size[i] = int ( size[i] / 2 );
          } else {
            size[i] = 0;
          }
        }
      region.SetSize(size);
      
      ExtractType::Pointer extract = ExtractType::New();

      extract->SetInput(rescale->GetOutput());
      extract->SetExtractionRegion(region);

      WriterType::Pointer writer = WriterType::New();
      writer->SetInput(extract->GetOutput());


    std::string diffName = mOutputDirectory + "/" + name + ".diff.png";
    mi3c::SaveImage().setFilename ( diffName ).execute ( extract.execute( diff, center ) );
    std::string expectedName = mOutputDirectory + "/" + name + ".expected.png";
    mi3c::SaveImage().setFilename ( expectedName ).execute ( baselineCenter );
    std::string actualName = mOutputDirectory + "/" + name + ".actual.png";
    mi3c::SaveImage().setFilename ( actualName ).execute ( imageCenter );

    std::ostringstream msg;
    msg << "ImageCompare: image difference was " << status << " which exceeds the tolerance of " << mTolerance;
    msg << "\n";
    msg << "ExpectedImage: " << expectedName << "\n";
    msg << "ActualImage: " << actualName << "\n";
    msg << "DiffImage: " << diffName << "\n";
    m_Message = msg.str();


    std::cout << "<DartMeasurementFile name=\"ExpectedImage\" type=\"image/png\">";
    std::cout << expectedName << "</DartMeasurementFile>" << std::endl;
    std::cout << "<DartMeasurementFile name=\"ActualImage\" type=\"image/png\">";
    std::cout << actualName << "</DartMeasurementFile>" << std::endl;
    std::cout << "<DartMeasurementFile name=\"DifferenceImage\" type=\"image/png\">";
    std::cout << diffName << "</DartMeasurementFile>" << std::endl;

    std::cout << "<DartMeasurement name=\"Difference\" type=\"numeric/float\">" << status << "</DartMeasurement>" << std::endl;
    std::cout << "<DartMeasurement name=\"Tolerance\" type=\"numeric/float\">" << mTolerance << "</DartMeasurement>" << std::endl;
    
    // Save everything
    try {
      rescale->SetInput(diff->GetOutput());
      rescale->Update();
      writer->SetFileName ( diffName.c_str() );
      writer->Update();
      
      rescale->SetInput ( baseline );
      rescale->Update();
      writer->SetFileName ( expectedName.c_str() );
      writer->Update();
      
      rescale->SetInput ( image );
      rescale->Update();
      writer->SetFileName ( actualName.c_str() );
      writer->Update();
      catch ( itk::ExceptionObject& e )
      {
        std::cerr << "Error saving files: " + e.what();
        return false;
      }
    return false;
    }
  return true;
}


    
