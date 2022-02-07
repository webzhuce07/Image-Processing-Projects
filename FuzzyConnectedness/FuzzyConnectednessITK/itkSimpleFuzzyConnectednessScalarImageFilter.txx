/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSimpleFuzzyConnectednessScalarImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2004/12/28 15:04:19 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkSimpleFuzzyConnectednessScalarImageFilter_txx
#define _itkSimpleFuzzyConnectednessScalarImageFilter_txx
#include "itkSimpleFuzzyConnectednessScalarImageFilter.h"

#include "vnl/vnl_math.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkNumericTraits.h"

namespace itk{

template <class TInputImage, class TOutputImage>
SimpleFuzzyConnectednessScalarImageFilter<TInputImage,TOutputImage>
::SimpleFuzzyConnectednessScalarImageFilter()
{
  m_Mean = 0.0;
  m_Diff_Mean = 0.0;
  m_Variance = 0.0;
  m_Diff_Variance = 0.0;
  ;
}

template <class TInputImage, class TOutputImage>
SimpleFuzzyConnectednessScalarImageFilter<TInputImage,TOutputImage>
::~SimpleFuzzyConnectednessScalarImageFilter()
{
}


template <class TInputImage, class TOutputImage>
void 
SimpleFuzzyConnectednessScalarImageFilter<TInputImage,TOutputImage>
::SetParameters
(const double inmean,const double invar,const double indifmean,
 const double indifvar, const double inweight)
{
  m_Mean = inmean;
  m_Variance = invar;
  m_Diff_Mean = indifmean;
  m_Diff_Variance = indifvar;

  if(inweight < 0)
    {
    this->SetWeight(0);
    }
  else if(inweight > 1)
    {
    this->SetWeight(1);
    }
  else 
    {
    this->SetWeight(inweight);
    }
}

template <class TInputImage, class TOutputImage>
double 
SimpleFuzzyConnectednessScalarImageFilter<TInputImage,TOutputImage>
::FuzzyAffinity(const PixelType f1,const PixelType f2)
{
  double tmp1 = 0.5 * (f1 + f2) - m_Mean;
  if(this->GetWeight() == 1)
    {
    return( (NumericTraits<unsigned short>::max())* 
            (exp(-0.5 * tmp1 * tmp1 / m_Variance)));
    }
  else{
  double tmp2 = fabs(static_cast<double>(f1) - static_cast<double>(f2)) - m_Diff_Mean;
  return( (NumericTraits<unsigned short>::max()) *
          (this->GetWeight() * exp(-0.5 * tmp1 * tmp1 / m_Variance) + 
           (1 - this->GetWeight()) * exp(-0.5 * tmp2 * tmp2 / m_Diff_Variance)));
  }
}

template <class TInputImage, class TOutputImage>
void
SimpleFuzzyConnectednessScalarImageFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Mean = " << m_Mean << std::endl;
  os << indent << "Diff_Mean = " << m_Diff_Mean << std::endl;
  os << indent << "Variance = " << m_Variance << std::endl;
  os << indent << "Diff_Variance = " << m_Diff_Variance << std::endl;

}
} /* end namespace itk. */


#endif
