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
#ifndef __itkIterativeSupervisedTrainingFunction_h
#define __itkIterativeSupervisedTrainingFunction_h

#include "itkTrainingFunctionBase.h"


namespace itk
{
namespace Statistics
{

template<class TSample, class TTargetVector, class ScalarType>
class IterativeSupervisedTrainingFunction : public TrainingFunctionBase<TSample, TTargetVector, ScalarType>
{
public:

  typedef IterativeSupervisedTrainingFunction                      Self;
  typedef TrainingFunctionBase<TSample, TTargetVector, ScalarType> Superclass;
  typedef SmartPointer<Self>                                       Pointer;
  typedef SmartPointer<const Self>                                 ConstPointer;

  /** Method for creation through the object factory. */
  itkTypeMacro(IterativeSupervisedTrainingFunction, TrainingFunctionBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef typename Superclass::NetworkType        NetworkType;
  typedef typename Superclass::InternalVectorType InternalVectorType;

  void SetNumOfIterations(SizeValueType i);

  virtual void Train(NetworkType* net, TSample* samples, TTargetVector* targets);

  itkSetMacro(Threshold, ScalarType);

protected:

  IterativeSupervisedTrainingFunction();
  virtual ~IterativeSupervisedTrainingFunction(){};

  /** Method to print the object. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;

  ScalarType m_Threshold;
  bool       m_Stop; //stop condition
};

} // end namespace Statistics
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkIterativeSupervisedTrainingFunction.txx"
#endif

#endif
