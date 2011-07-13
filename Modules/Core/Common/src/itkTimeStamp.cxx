/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
/*=========================================================================
 *
 *  Portions of this file are subject to the VTK Toolkit Version 3 copyright.
 *
 *  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
 *
 *  For complete copyright, license and disclaimer of warranty information
 *  please refer to the NOTICE file at the top of the ITK source tree.
 *
 *=========================================================================*/
#include "itkTimeStamp.h"
#include "itkFastMutexLock.h"

#if defined( _WIN32 )
  #include "itkWindows.h"

#elif defined( __APPLE__ )
// OSAtomic.h optimizations only used in 10.5 and later
  #include <AvailabilityMacros.h>
  #if MAC_OS_X_VERSION_MAX_ALLOWED >= 1050
    #include <libkern/OSAtomic.h>
  #endif

#elif defined( __GLIBCPP__ ) || defined( __GLIBCXX__ )
  #if ( __GNUC__ > 4 ) || ( ( __GNUC__ == 4 ) && ( __GNUC_MINOR__ >= 2 ) )
  #include <ext/atomicity.h>
  #else
  #include <bits/atomicity.h>
  #endif

#endif

namespace itk
{
#if defined( __GLIBCXX__ ) // g++ 3.4+

using __gnu_cxx::__exchange_and_add;

#endif

/**
 * Instance creation.
 */
TimeStamp *
TimeStamp
::New()
{
  return new Self;
}

/**
 * Make this timestamp to be the same as another one.
 */
const TimeStamp &
TimeStamp::operator=( const Self & other )
{
  this->m_ModifiedTime = other.m_ModifiedTime;
  return *this;
}

/**
 * Make sure the new time stamp is greater than all others so far.
 */
void
TimeStamp
::Modified()
{
  /**
   * Initialize static member
   */
  static unsigned long itkTimeStampTime = 0;

#pragma omp critical (itkTimeStampTime)
  {
  m_ModifiedTime = ++itkTimeStampTime;
  }
}
} // end namespace itk
