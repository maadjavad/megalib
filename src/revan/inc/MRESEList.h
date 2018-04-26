/*
 * MRESEList.h
 *
 * Copyright (C) by Andreas Zoglauer.
 * All rights reserved.
 *
 * Please see the source-file for the copyright-notice.
 *
 */


#ifndef __MRESEList__
#define __MRESEList__


////////////////////////////////////////////////////////////////////////////////


// Standrad libs:
#include <vector>
using namespace std;

// ROOT libs:

// MEGAlib libs:
#include "MGlobal.h"

// Forward declarations:
class MRESE;


////////////////////////////////////////////////////////////////////////////////


class MRESEList
{
  // Public Interface:
 public:
  MRESEList();
  MRESEList(MRESEList* RESEList);
  virtual ~MRESEList();

  int GetNRESEs();
  MRESE* GetRESEAt(int i);
  MRESE* GetRESE(int ID);
  MRESE* GetRESEContainingRESE(int ID);
  bool ContainsRESE(MRESE *RESE);
  bool ContainsRESE(int ID);
  void SetRESEAt(int i, MRESE* R);
  void AddRESE(MRESE* RESE);
  void AddRESEFirst(MRESE* RESE);
  MRESE* RemoveRESE(MRESE* RESE);
  MRESE* RemoveRESEAt(int i);
  //! Removes all RESE - The list is NOT compressed!
  void RemoveAllRESEs();
  void DeleteAll();
  void CompressRESEs() { Compress(); } // depreciated!
  void Compress();

  virtual MRESEList* Duplicate();
  MString ToString(bool WithLink = true, int Level = 0);

  // private members:
 private:
  // The vector with the RESEs:
  vector<MRESE*> m_RESEList;



#ifdef ___CINT___
 public:
  ClassDef(MRESEList, 0) // a list of raw event sub elements 
#endif

};

#endif


////////////////////////////////////////////////////////////////////////////////
