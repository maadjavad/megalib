/*
 * MERCSRTMVA.h
 *
 * Copyright (C) 1998-2007 by Andreas Zoglauer.
 * All rights reserved.
 *
 * Please see the source-file for the copyright-notice.
 *
 */


#ifndef __MERCSRTMVA__
#define __MERCSRTMVA__


////////////////////////////////////////////////////////////////////////////////

// Standard libs:
#include <vector>
#include <map>
using namespace std;

// Root libs:
#include "TMVA/Reader.h"

// MEGAlib libs:
#include "MGlobal.h"
#include "MERConstruction.h"
#include "MERCSR.h"
#include "MRawEventIncarnations.h"
#include "MComptonEvent.h"
#include "MVector.h"
#include "MERCSRDataSet.h"
#include "MERCSRTMVAMethods.h"

// Forward declarations:
class MRESE;
class MRETrack;
class MGeometryRevan;

////////////////////////////////////////////////////////////////////////////////


class MERCSRTMVA : public MERCSR
{
  // public interface:
 public:
  MERCSRTMVA();
  virtual ~MERCSRTMVA();

  virtual bool SetParameters(MString FileName,
                             MERCSRTMVAMethods Methods,
                             MGeometryRevan* Geometry,
                             double ThresholdMin = 0.0,
                             double ThresholdMax = 0.5,
                             int MaxNHits = 5,
                             bool GuaranteeStartD1 = true,
                             bool CreateOnlyPermutations = false);


  virtual MString ToString(bool CoreOnly = false) const;
  
  // protected methods:
 protected:
  //MERCSRTMVA() {};
  //MERCSRTMVA(const MERCSRTMVA& ERCSR) {};
      
   /// Compute all quality factors, return the number of good ones
   virtual int ComputeAllQualityFactors(MRERawEvent* RE);
   
  // private methods:
 private:



  // protected members:
 protected:


  // private members:
 private:
  //! The training data file name
  MString m_FileName;

  //! The used TMVA methods
  MERCSRTMVAMethods m_Methods;
  
  //! A string of the used methods for the EvaluateMVA call
  vector<MString> m_MethodNames;

  //! The data set
  MERCSRDataSet m_DS;
  
  //! The TMVA readers - one per sequence length
  vector<TMVA::Reader*> m_Readers;
  
  //! All possible Permutations for fast access:
  //! Build the following way: [Sequence length] [Permuation ID] [Index]
  vector<vector<vector<unsigned int>>> m_Permutator;  

  //! Use path to first IA
  bool m_UsePathToFirstIA;
  
  
#ifdef ___CINT___
 public:
  ClassDef(MERCSRTMVA, 0) // no description
#endif

};

#endif


////////////////////////////////////////////////////////////////////////////////
