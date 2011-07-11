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
#include "itksys/SystemTools.hxx"
#include <stdlib.h>

#ifdef __APPLE__
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

namespace itk
{
extern "C"
{
typedef void *( *c_void_cast )(void *);
}

int MultiThreader::SpawnThread(ThreadFunctionType f, void *UserData)
{
  int id = 0;

  while ( id < ITK_MAX_THREADS )
    {
    if ( !m_SpawnedThreadActiveFlagLock[id]  )
      {
      m_SpawnedThreadActiveFlagLock[id] = MutexLock::New();
      }
    m_SpawnedThreadActiveFlagLock[id]->Lock();
    if ( m_SpawnedThreadActiveFlag[id] == 0 )
      {
      // We've got a useable thread id, so grab it
      m_SpawnedThreadActiveFlag[id] = 1;
      m_SpawnedThreadActiveFlagLock[id]->Unlock();
      break;
      }
    m_SpawnedThreadActiveFlagLock[id]->Unlock();

    id++;
    }

  if ( id >= ITK_MAX_THREADS )
    {
    itkExceptionMacro(<< "You have too many active threads!");
    }

  m_SpawnedThreadInfoArray[id].UserData        = UserData;
  m_SpawnedThreadInfoArray[id].NumberOfThreads = 1;
  m_SpawnedThreadInfoArray[id].ActiveFlag = &m_SpawnedThreadActiveFlag[id];
  m_SpawnedThreadInfoArray[id].ActiveFlagLock = m_SpawnedThreadActiveFlagLock[id];

  pthread_attr_t attr;

  pthread_attr_init(&attr);
#ifndef __CYGWIN__
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
#endif

  pthread_create( &( m_SpawnedThreadProcessID[id] ),
                  &attr, reinterpret_cast< c_void_cast >( f ),
                  ( (void *)( &m_SpawnedThreadInfoArray[id] ) ) );

  return id;
}

void MultiThreader::TerminateThread(ThreadIdType ThreadID)
{
  if ( !m_SpawnedThreadActiveFlag[ThreadID] )
    {
    return;
    }

  m_SpawnedThreadActiveFlagLock[ThreadID]->Lock();
  m_SpawnedThreadActiveFlag[ThreadID] = 0;
  m_SpawnedThreadActiveFlagLock[ThreadID]->Unlock();

  pthread_join(m_SpawnedThreadProcessID[ThreadID], 0);

  m_SpawnedThreadActiveFlagLock[ThreadID] = 0;
  m_SpawnedThreadActiveFlagLock[ThreadID] = 0;
}

void
MultiThreader
::WaitForSingleMethodThread(ThreadProcessIDType threadHandle)
{
  // Using POSIX threads
  if ( pthread_join(threadHandle, 0) )
    {
    itkExceptionMacro(<< "Unable to join thread.");
    }
}

ThreadProcessIDType
MultiThreader
::DispatchSingleMethodThread(MultiThreader::ThreadInfoStruct *threadInfo)
{
  // Using POSIX threads
  pthread_attr_t attr;
  pthread_t      threadHandle;

  pthread_attr_init(&attr);
#if !defined( __CYGWIN__ )
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
#endif

  int threadError;
  threadError =
    pthread_create( &threadHandle, &attr, reinterpret_cast< c_void_cast >( this->SingleMethodProxy ),
                    reinterpret_cast< void * >( threadInfo ) );
  if ( threadError != 0 )
    {
    itkExceptionMacro(<< "Unable to create a thread.  pthread_create() returned "
                      << threadError);
    }
  return threadHandle;
}
} // end namespace itk
