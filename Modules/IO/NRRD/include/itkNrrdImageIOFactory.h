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
#ifndef __itkNrrdImageIOFactory_h
#define __itkNrrdImageIOFactory_h

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace itk
{
/** \class NrrdImageIOFactory
 * \brief Create instances of NrrdImageIO objects using an object factory.
 * \ingroup ITK-IO-NRRD
 */
class ITK_EXPORT NrrdImageIOFactory:public ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef NrrdImageIOFactory         Self;
  typedef ObjectFactoryBase          Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char * GetITKSourceVersion(void) const;

  virtual const char * GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(NrrdImageIOFactory, ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    NrrdImageIOFactory::Pointer nrrdFactory = NrrdImageIOFactory::New();

    ObjectFactoryBase::RegisterFactoryInternal(nrrdFactory);
  }

protected:
  NrrdImageIOFactory();
  ~NrrdImageIOFactory();
private:
  NrrdImageIOFactory(const Self &); //purposely not implemented
  void operator=(const Self &);     //purposely not implemented
};
} // end namespace itk

#endif
