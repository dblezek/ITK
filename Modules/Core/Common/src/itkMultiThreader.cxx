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
#include "itkMultiThreader.h"
#include "itkObjectFactory.h"
#include "itkNumericTraits.h"
#include "itksys/SystemTools.hxx"
#include <stdlib.h>
#include <algorithm>

#include "itkOpenMP.h"

#if defined(ITK_USE_PTHREADS)
#include "itkMultiThreaderPThreads.cxx"
#elif defined(ITK_USE_WIN32_THREADS)
#include "itkMultiThreaderWinThreads.cxx"
#else
#include "itkMultiThreaderNoThreads.cxx"
#endif


namespace itk
{
// Initialize static member that controls global maximum number of threads.
ThreadIdType MultiThreader:: m_GlobalMaximumNumberOfThreads = ITK_MAX_THREADS;

// Initialize static member that controls global default number of threads : 0
// => Not initialized.
ThreadIdType MultiThreader:: m_GlobalDefaultNumberOfThreads = 0;

void MultiThreader::SetGlobalMaximumNumberOfThreads(ThreadIdType val)
{
  m_GlobalMaximumNumberOfThreads = val;

  // clamp between 1 and ITK_MAX_THREADS
  m_GlobalMaximumNumberOfThreads = std::min( m_GlobalMaximumNumberOfThreads,
                                             (ThreadIdType) ITK_MAX_THREADS );
  m_GlobalMaximumNumberOfThreads = std::max( m_GlobalMaximumNumberOfThreads,
                                             NumericTraits<ThreadIdType>::One );

  // If necessary reset the default to be used from now on.
  m_GlobalDefaultNumberOfThreads = std::min ( m_GlobalDefaultNumberOfThreads,
                                              m_GlobalMaximumNumberOfThreads);
}

ThreadIdType MultiThreader::GetGlobalMaximumNumberOfThreads()
{
  std::cout << "GetGlobalMaximumNumberOfThreads: " << m_GlobalMaximumNumberOfThreads << std::endl;
#pragma omp critical (GetGlobalMaximumNumberOfThreads)
  if ( m_GlobalMaximumNumberOfThreads == 0 )
    {
    m_GlobalMaximumNumberOfThreads = ITK_MAX_THREADS;
    }
  return m_GlobalMaximumNumberOfThreads;
}

void MultiThreader::SetGlobalDefaultNumberOfThreads(ThreadIdType val)
{
  m_GlobalDefaultNumberOfThreads = val;

  // clamp between 1 and m_GlobalMaximumNumberOfThreads
  m_GlobalDefaultNumberOfThreads  = std::min( m_GlobalDefaultNumberOfThreads,
                                              m_GlobalMaximumNumberOfThreads );
  m_GlobalDefaultNumberOfThreads  = std::max( m_GlobalDefaultNumberOfThreads,
                                              NumericTraits<ThreadIdType>::One );

}

void MultiThreader::SetNumberOfThreads(ThreadIdType numberOfThreads)
{
  if ( m_NumberOfThreads == numberOfThreads &&
       numberOfThreads <= m_GlobalMaximumNumberOfThreads )
    {
    return;
    }

  m_NumberOfThreads = numberOfThreads;

  // clamp between 1 and m_GlobalMaximumNumberOfThreads
  m_NumberOfThreads  = std::min( m_NumberOfThreads,
                                 m_GlobalMaximumNumberOfThreads );
  m_NumberOfThreads  = std::max( m_NumberOfThreads, NumericTraits<ThreadIdType>::One );

}


ThreadIdType MultiThreader::GetGlobalDefaultNumberOfThreads()
{
  // if default number has been set then don't try to update it; just
  // return the value
  if ( m_GlobalDefaultNumberOfThreads != 0 )
    {
    return m_GlobalDefaultNumberOfThreads;
    }

  // first, check for enviornment variable
  itksys_stl::string itkGlobalDefaultNumberOfThreadsEnv = "0";
  if ( itksys::SystemTools::GetEnv("ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS",
                                   itkGlobalDefaultNumberOfThreadsEnv) )
    {
    m_GlobalDefaultNumberOfThreads =
      atoi( itkGlobalDefaultNumberOfThreadsEnv.c_str() );
    }

  // otherwise, set number of threads based on system information
  if ( m_GlobalDefaultNumberOfThreads <= 0 )
    {
    ThreadIdType num;
    num = omp_get_max_threads();
    m_GlobalDefaultNumberOfThreads = num;
    }

  // limit the number of threads to m_GlobalMaximumNumberOfThreads
  m_GlobalDefaultNumberOfThreads  = std::min( m_GlobalDefaultNumberOfThreads,
                                              GetGlobalMaximumNumberOfThreads() );

  // verify that the default number of threads is larger than zero
  m_GlobalDefaultNumberOfThreads  = std::max( m_GlobalDefaultNumberOfThreads,
                                              NumericTraits<ThreadIdType>::One );

  return m_GlobalDefaultNumberOfThreads;
}


// Constructor. Default all the methods to NULL. Since the
// ThreadInfoArray is static, the ThreadIDs can be initialized here
// and will not change.
MultiThreader::MultiThreader()
{
  for ( ThreadIdType i = 0; i < ITK_MAX_THREADS; i++ )
    {
    m_ThreadInfoArray[i].ThreadID           = i;
    m_ThreadInfoArray[i].ActiveFlag         = 0;
    m_ThreadInfoArray[i].ActiveFlagLock     = 0;

    m_SpawnedThreadActiveFlag[i]            = 0;
    m_SpawnedThreadActiveFlagLock[i]        = 0;
    m_SpawnedThreadInfoArray[i].ThreadID    = i;
    }

  m_SingleMethod = 0;
  m_SingleData = 0;
  m_NumberOfThreads = this->GetGlobalDefaultNumberOfThreads();
}

MultiThreader::~MultiThreader()
{}

// Set the user defined method that will be run on NumberOfThreads threads
// when SingleMethodExecute is called.
void MultiThreader::SetSingleMethod(ThreadFunctionType f, void *data)
{
  m_SingleMethod = f;
  m_SingleData   = data;
}

// Execute the method set as the SingleMethod on NumberOfThreads threads.
void MultiThreader::SingleMethodExecute()
{
  if ( !m_SingleMethod )
    {
    itkExceptionMacro(<< "No single method set!");
    return;
    }

  // obey the global maximum number of threads limit
  m_NumberOfThreads = std::min( m_GlobalMaximumNumberOfThreads, m_NumberOfThreads );
  std::cout << "SingleMethodExecute MultiThreader Global default: " << this->GetGlobalDefaultNumberOfThreads() << " GlobalMax: " << m_GlobalMaximumNumberOfThreads << " # threads: " << m_NumberOfThreads << std::endl;

 //
  // Enable nested parallel regions
  //
  if ( !omp_get_nested() )
    {
// The OpenMP 2.5 spec is unclear of the omp_get_nested binding
// thread set. In one location it's the current thread, in another
// it says it is implementation defined. It'll have to do.

#if !(_OPENMP >= 200805 )
    // OpenMP 3.0 explicitly specifies that omp_set_nested works in
    // nexted parallel regions or tasks, where as for 2.5 its
    // undefined.
    if ( omp_in_parallel() )
      {
      itkWarningMacro( << "omp_set_nested() was not set in outter thread team. "
                       << "Effect of setting omp_set_nested() from within parallel "
                       << "region is implementation defined." );
      }
#endif
      omp_set_nested(1);


      // For implementations which don't support nested parallelism
      // the following value will remain false.
      if ( !omp_get_nested() )
        {
        itkExceptionMacro( << "Failure to set omp_get_nested when nexted "
                           << "parallel regions are required." );
        }
    }

#if (_OPENMP >= 200805 )
#warning need to add support for checking the nesting level
#endif

  // Initialize the m_ThreadInforArray
  //
  // While this method could be part of the multi-threaded region, it
  // is not because of the potential for false-sharing between
  // threads, or process cache thrashing to to the close proximity of
  // the elements in the array.
  //
  // Instead we hope for SIMD optimization by the compiler ( so keep
  // this a simple loop ).
  int NThreads = this->m_NumberOfThreads;
  for ( int thread_loop = 0; thread_loop < NThreads; ++thread_loop )
    {
    m_ThreadInfoArray[thread_loop].UserData    = m_SingleData;
    m_ThreadInfoArray[thread_loop].NumberOfThreads = m_NumberOfThreads;
    m_ThreadInfoArray[thread_loop].ThreadFunction = m_SingleMethod;
    m_ThreadInfoArray[thread_loop].ThreadExitCode = MultiThreader::ThreadInfoStruct::SUCCESS;
    }


  // Spawn a set of threads through the SingleMethodProxy. Exceptions
  // thrown from a thread will be caught by the SingleMethodProxy. A
  // naive mechanism is in place for determining whether a thread
  // threw an exception.
  //
  // Thanks to Hannu Helminen for suggestions on how to catch
  // exceptions thrown by threads.
  int         exceptionOccurred = false;
  std::string exceptionDetails;


#pragma omp parallel                            \
  num_threads(NThreads)          \
  reduction(|:exceptionOccurred)               \
  default( shared )
  {
  bool localExceptionOccurred = false;
  try
    {
    if ( omp_get_num_threads() != NThreads )
      {
      itkWarningMacro( "Failed to spawn the required number of threads\n"
                         << " Requested " << this->m_NumberOfThreads
                         << " but only got " << omp_get_num_threads() );
      }

    int threadNum = omp_get_thread_num();
    this->SingleMethodProxy ( reinterpret_cast< void * >( &m_ThreadInfoArray[threadNum] ) );
    }
  catch ( std::exception & e )
    {
    // get the details of the exception to rethrow them
#pragma omp critical
    { exceptionDetails = e.what();}
    // If creation of any thread failed, we must make sure that all
    // threads are correctly cleaned
    localExceptionOccurred = true;
    }
  catch ( ... )
    {
    // If creation of any thread failed, we must make sure that all
    // threads are correctly cleaned
    localExceptionOccurred = true;
    }
  exceptionOccurred |= localExceptionOccurred;
  }

  if ( exceptionOccurred )
    {
    if ( exceptionDetails.empty() )
      {
      itkExceptionMacro("Exception occurred during SingleMethodExecute");
      }
    else
      {
      itkExceptionMacro(<< "Exception occurred during SingleMethodExecute" << std::endl << exceptionDetails);
      }
    }

  for ( int thread_loop = 0; thread_loop < NThreads; ++thread_loop )
    {
    if (m_ThreadInfoArray[thread_loop].ThreadExitCode != MultiThreader::ThreadInfoStruct::SUCCESS )
      {
      itkExceptionMacro("Exception occurred during SingleMethodExecute");
      }
    }
}
ITK_THREAD_RETURN_TYPE
MultiThreader
::SingleMethodProxy(void *arg)
{
  // grab the ThreadInfoStruct originally prescribed
  MultiThreader::ThreadInfoStruct
  * threadInfoStruct =
    reinterpret_cast< MultiThreader::ThreadInfoStruct * >( arg );

  // execute the user specified threader callback, catching any exceptions
  try
    {
    ( *threadInfoStruct->ThreadFunction )(threadInfoStruct);
    threadInfoStruct->ThreadExitCode = MultiThreader::ThreadInfoStruct::SUCCESS;
    }
  catch ( ProcessAborted & )
    {
    threadInfoStruct->ThreadExitCode =
      MultiThreader::ThreadInfoStruct::ITK_PROCESS_ABORTED_EXCEPTION;
    throw;
    }
  catch ( ExceptionObject & )
    {
    threadInfoStruct->ThreadExitCode =
      MultiThreader::ThreadInfoStruct::ITK_EXCEPTION;
    throw;
    }
  catch ( std::exception & )
    {
    threadInfoStruct->ThreadExitCode =
      MultiThreader::ThreadInfoStruct::STD_EXCEPTION;
    throw;
    }
  catch ( ... )
    {
    threadInfoStruct->ThreadExitCode = MultiThreader::ThreadInfoStruct::UNKNOWN;
    throw;
    }

  return ITK_THREAD_RETURN_VALUE;
}
// Print method for the multithreader
void MultiThreader::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Thread Count: " << m_NumberOfThreads << "\n";
  os << indent << "Global Maximum Number Of Threads: "
     << m_GlobalMaximumNumberOfThreads << std::endl;
  os << indent << "Global Default Number Of Threads: "
     << m_GlobalDefaultNumberOfThreads << std::endl;
}


}
