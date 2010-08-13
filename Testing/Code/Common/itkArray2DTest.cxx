/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkArray2DTest.cxx
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

#include "itkArray2D.h"
#include "itkTestHarness.h"

typedef itk::Array2D< double > ArrayType;
typedef vnl_matrix<double> VnlMatrixType;
const unsigned int rows = 3;
const unsigned int cols = 4;
const double tolerance = 1e-6;

class Array2D : public testing::Test {
public:
  virtual void SetUp()
  {
    a = ArrayType ( rows, cols );
    vm = VnlMatrixType ( rows, cols );
    for( unsigned int r=0; r<rows; r++)
      {
        for( unsigned int c=0; c<cols; c++)
          {
            const double value = static_cast<double>( r + c );
            a(r,c)  = value;
            vm(r,c) = value;
          }
      }
  }
  ArrayType a;
  VnlMatrixType vm;
};

TEST_F(Array2D,Copy)
{
  // test copy constructor
  ArrayType b( a );

  for( unsigned int r=0; r<rows; r++)
    {
    for( unsigned int c=0; c<cols; c++)
      {
        EXPECT_NEAR ( a(r,c), b(r,c), tolerance );
      }
    }
}

TEST_F(Array2D,ConstructionFromVNL)
{
  // test construction from vnl_matrix
  ArrayType d( vm );

  for( unsigned int r=0; r<rows; r++)
    {
    for( unsigned int c=0; c<cols; c++)
      {
        EXPECT_NEAR ( d(r,c), vm(r,c), tolerance ) << "Error in construction from vn_matrix" << std::endl;
      }
    }
}

TEST_F(Array2D,AssignmentFromArray2D)
{
  // test for assignment from Array2D
  ArrayType e;
  
  e = a;

  for( unsigned int r=0; r<rows; r++)
    {
    for( unsigned int c=0; c<cols; c++)
      {
        EXPECT_NEAR ( a(r,c), e(r,c), tolerance ) << "Error in assignment from Array2D constructor " << std::endl;
      }
    }
}

TEST_F(Array2D,AssignmentFromvnl_matrix)
{
  // test for assignment from vnl_matrix
  ArrayType f;
  
  f = vm;

  for( unsigned int r=0; r<rows; r++)
    {
    for( unsigned int c=0; c<cols; c++)
      {
        EXPECT_NEAR ( f(r,c), vm(r,c), tolerance ) << "Error in assignment from  vn_matrix" << std::endl;
      }
    }
}
