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
#include "itkSemaphore.h"

extern "C" {
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
}

namespace itk
{
static int SemaphoreCount = 0;

static std::string GetUniqueName()
{
  char   s[255];
  time_t t = time(0);

  snprintf(s, 254, "MACSEM%d%d", static_cast< int >( t ), SemaphoreCount);
  SemaphoreCount++;
  std::cout << "GetUniqueName: " << std::string(s) << std::endl;
  return std::string(s);
}

Semaphore::Semaphore ()
{
  m_Sema = 0;
  m_PThreadsSemaphoreRemoved = false;
  std::cout << "Constructor" << std::endl;
}

void Semaphore::Initialize(unsigned int value)
{
  std::string name = GetUniqueName();
  m_PThreadsSemaphoreRemoved = false;

  m_Sema  = sem_open(name.c_str(), O_CREAT, 0x0644, value);
  if ( m_Sema == (sem_t *)SEM_FAILED )
    {
    //  perror("FAILED WITH ERROR:" );
    itkExceptionMacro( << "sem_open call failed on " << name.c_str() );
    }
  std::cout << "Initialized" << std::endl;
}

void Semaphore::Up()
{
  std::cout << "Semaphore::Up" << std::endl;
  if ( sem_post(m_Sema) != 0 )
    {
    itkExceptionMacro(<< "sem_post call failed.");
    }
}

void Semaphore::Down()
{
  std::cout << "Semaphore::Down" << std::endl;

  if ( sem_wait(m_Sema) != 0 )
    {
    itkExceptionMacro(<< "sem_wait call failed.");
    }
}

Semaphore::~Semaphore()
{
  std::cout << "Semaphore::~Semaphore" << std::endl;
  if ( !m_PThreadsSemaphoreRemoved )
    {
    this->Remove();
    }
}

void Semaphore::Remove()
{
  std::cout << "Semaphore::Remove" << std::endl;
  m_PThreadsSemaphoreRemoved = true;
  if ( sem_destroy(m_Sema) != 0 )
    {
    itkExceptionMacro(<< "sem_destroy call failed. ");
    }
}

} //end if namespace itk
