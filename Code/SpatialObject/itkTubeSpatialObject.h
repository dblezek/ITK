/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkTubeSpatialObject.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkTubeSpatialObject_h
#define __itkTubeSpatialObject_h

#include <list>

#include "itkSpatialObject.h"
#include "itkTubePoint.h"

namespace itk 
{

/** 
* \class itkTubeSpatialObject
* \brief Representation of a tube based on the spatial object classes.
*
* The tube is basically defined by a set of points. Each tube can
* be connected to a tube network, by using the AddSpatialObject() methods
* of a TubeSpatialObjectNet Object. A tube is also identified by an id number when connected
* to a network.
*
* \also TubePoint TubeNetworkSpatialObject 
*/

template < unsigned int TDimension = 3 , unsigned int PipelineDimension = 3 >
class TubeSpatialObject 
:public SpatialObject<  TDimension, 
                        AffineTransform<double, TDimension>, 
                        PipelineDimension
                     >
{

public:

    typedef TubeSpatialObject                           Self;
    typedef SpatialObject< TDimension, 
                           AffineTransform< double, TDimension>,
                           PipelineDimension
                           >                            Superclass;
    typedef SmartPointer < Self >                       Pointer;
    typedef SmartPointer < const Self >                 ConstPointer;
    typedef double                                      ScalarType;
    typedef unsigned int                                IdentifierType;
    typedef TubePoint< TDimension >                     TubePointType;
    typedef typename TubePointType::Pointer             TubePointPointer; 
    typedef std::list < TubePointPointer >              PointListType;
    typedef PointListType *                             PointListPointer;
    typedef Superclass::PointType                       PointType;
    typedef VectorContainer<unsigned long,PointType>    PointContainerType;
    typedef SmartPointer<PointContainerType>            PointContainerPointer;

    /** 
    * Method for creation through the object factory. 
    */
    itkNewMacro( Self );

    /** 
    * Method for creation through the object factory. 
    */
    itkTypeMacro( Self, Superclass );

    /** 
    * Get the identifier of the tube.
    */
    IdentifierType GetId( void ) const;

    /** 
    * Set the tube identifier.
    */
    void SetId( const IdentifierType newId );

    /** 
    * Returns a reference to the list of the tube points.
    */
    PointListPointer GetPoints( void ) const;

    /** 
    * Set the list of tube points.
    */
    void SetPoints( PointListPointer newPoints );

    /**
    * Calculate the normalized tangent, and orthogonal vector of the tube.
    */
    bool CalcTangent( void );

    /**
    * Returns true if the tube is evaluable at the requested point, 
    * false otherwise.
    */
    bool IsEvaluableAt( const PointType & point );

    /**
    * Returns the value of the tube at that point.
    * Currently this function returns a binary value,
    * but it might want to return a degree of membership
    * in case of fuzzy tubes.
    */
    void ValueAt( const PointType & point, double & value );

    /**
    * Returns true if the point is inside the tube, false otherwise.
    */
    bool IsInside( const PointType & point );

    /**
    * Compute the boundaries of the tube.
    */
    void ComputeBounds( void );

    /**
    * Return the last modified time of the object, and all of its components
    */
    unsigned long GetMTime( void ) const;

protected:

    unsigned int      m_Id;
    PointListPointer  m_Points;
    TimeStamp         m_BoundsMTime; 

    TubeSpatialObject();
    virtual ~TubeSpatialObject();

    /**
    * Method to print the object.
    */
    virtual void PrintSelf( std::ostream& os, Indent indent ) const;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION 
  #include "itkTubeSpatialObject.txx" 
#endif 

#endif // __itkTubeSpatialObject_h
