#ifndef __itkImageHash_h
#define __itkImageHash_h
#include <gtest/gtest.h>
#include <itkImage.h>
#include "itkByteSwapper.h"
#include "itkImageRegionConstIterator.h"
#include "hl_sha1.h"

template< class ImageType >
std::string ImageSHA1Hash(typename ImageType::Pointer image)
{
  typedef typename ImageType::ValueType T;
  typedef itk::ByteSwapper< T >         Swapper;
  unsigned int dimension = image->GetImageDimension();
  ::SHA1 sha1;
  ::HL_SHA1_CTX sha1Context;
  sha1.SHA1Reset (&sha1Context);
  typedef itk::ImageRegionConstIterator< ImageType > IteratorType;
  IteratorType iterator = IteratorType ( image, image->GetLargestPossibleRegion() );
  iterator.GoToBegin();
  // Start by encoding the origin, spacing and cosines
  typename ImageType::PointValueType origin[dimension];
  typename ImageType::SpacingValueType spacing[dimension];
  double directions[dimension * dimension];
  for ( unsigned int i = 0; i < dimension; i++ )
    {
    origin[i] = image->GetOrigin()[i];
    spacing[i] = image->GetSpacing()[i];
    for ( unsigned int j = 0; j < dimension; j++ )
      {
      directions[i + j * dimension] = image->GetDirection()[i][j];
      }
    }
  itk::ByteSwapper< typename ImageType::PointValueType >::SwapRangeFromSystemToLittleEndian ( origin, dimension
                                                                                             * sizeof( typename ImageType::PointValueType ) );
  sha1.SHA1Input ( &sha1Context, (unsigned char *)origin, dimension * sizeof( typename ImageType::PointValueType ) );
  itk::ByteSwapper< typename ImageType::SpacingValueType >::SwapRangeFromSystemToLittleEndian ( spacing, dimension
                                                                                               * sizeof( typename ImageType::
                                                                                                         SpacingValueType ) );
  sha1.SHA1Input ( &sha1Context, (unsigned char *)spacing, dimension * sizeof( typename ImageType::SpacingValueType ) );
  itk::ByteSwapper< double >::SwapRangeFromSystemToLittleEndian ( directions, dimension * dimension * sizeof( double ) );
  sha1.SHA1Input ( &sha1Context, (unsigned char *)directions, dimension * dimension * sizeof( double ) );
  // Create a buffer
  int BufferSize = 256;
  T * buffer = new T[BufferSize];
  int index = 0;
  while ( !iterator.IsAtEnd() )
    {
    buffer[index] = iterator.Value();
    ++iterator;
    index++;
    if ( index == BufferSize )
      {
      // Possibly byte swap
      Swapper::SwapRangeFromSystemToLittleEndian ( buffer, index * sizeof( T ) );
      sha1.SHA1Input ( &sha1Context, (unsigned char *)buffer, index * sizeof( T ) );
      index = 0;
      }
    }
  // Add any remaining data
  Swapper::SwapRangeFromSystemToLittleEndian ( buffer, index * sizeof( T ) );
  sha1.SHA1Input ( &sha1Context, (unsigned char *)buffer, index * sizeof( T ) );
  delete[] buffer;
  std::string   hash;
  int           HashSize;
  unsigned char Digest[1024];
  HashSize = SHA1HashSize;
  sha1.SHA1Result (&sha1Context, Digest);
  // Print to a string
  std::ostringstream os;
  for ( int i = 0; i < HashSize; ++i )
    {
    // set the width to 2, fill with 0, and convert to hex
    os.width(2);
    os.fill('0');
    os << std::hex << static_cast< unsigned int >( Digest[i] );
    }
  return os.str();
}

#endif
