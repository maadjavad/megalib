/*
 * MPETEvent.cxx
 *
 *
 * Copyright (C) by Andreas Zoglauer.
 * All rights reserved.
 *
 *
 * This code implementation is the intellectual property of
 * Andreas Zoglauer.
 *
 * By copying, distributing or modifying the Program (or any work
 * based on the Program) you indicate your acceptance of this statement,
 * and all its terms.
 *
 */


////////////////////////////////////////////////////////////////////////////////
//
// MPETEvent
//
////////////////////////////////////////////////////////////////////////////////


// Include the header:
#include "MPETEvent.h"

// Standard libs:
#include <cstdlib>
#include <iostream>
using namespace std;

// ROOT libs:

// MEGAlib libs:
#include "MStreams.h"
#include "MAssert.h"


////////////////////////////////////////////////////////////////////////////////


#ifdef ___CINT___
ClassImp(MPETEvent)
#endif


////////////////////////////////////////////////////////////////////////////////


//! Default constructor
MPETEvent::MPETEvent()
{
  m_EventType = c_PET;

  Reset();
}


////////////////////////////////////////////////////////////////////////////////


//! Default destructor
MPETEvent::~MPETEvent()
{
  // Intentionally empty
}


////////////////////////////////////////////////////////////////////////////////


//! Convert to a human readable string
MString MPETEvent::ToString() const
{
  char Text[1000];
  MString String("The data of the PET-event:\n"); 
  
  sprintf(Text, "Energy1: %.3f\n", m_Energy1);
  String += MString(Text);
  
  sprintf(Text, "Position1: %.3f, %.3f, %.3f\n", 
          m_Position1.X(), m_Position1.Y(), 
          m_Position1.Z());
  String += MString(Text);
  
  sprintf(Text, "Energy2: %.3f\n", m_Energy2);
  String += MString(Text);
  
  sprintf(Text, "Position2: %.3f, %.3f, %.3f\n", 
          m_Position2.X(), m_Position2.Y(), 
          m_Position2.Z());
  String += MString(Text);


  return String;
}


////////////////////////////////////////////////////////////////////////////////


bool MPETEvent::Assimilate(MPETEvent* PET)
{
  // Take over all the necessary event data and perform some elementary computations:
  // the compton angle, the cone axis, the most probable origin of the gamma ray...
  //
  // If an error occures, normally because the event data is so bad that the event
  // can hardly be caused by compton scattering, we return false.
  
  Reset();
  
  // Get only the basic data and calculate the rest:
  MPhysicalEvent::Assimilate(dynamic_cast<MPhysicalEvent*>(PET));

  m_Energy1 = PET->GetEnergy1();       
  m_Position1 = PET->GetPosition1();
  m_Energy2 = PET->GetEnergy2();       
  m_Position2 = PET->GetPosition2();

  return Validate();
}


////////////////////////////////////////////////////////////////////////////////


bool MPETEvent::Assimilate(MPhysicalEvent* Event)
{
  // Simply Call: MPETEvent::Assimilate(const MPETEventData *PETEventData)

  if (Event->GetType() == MPhysicalEvent::c_PET) {
    return Assimilate(dynamic_cast<MPETEvent*>(Event));
  } else {
    merr<<"Trying to assimilate a non-PET event!"<<endl; 
    return false; 
  }
}


////////////////////////////////////////////////////////////////////////////////


bool MPETEvent::Assimilate(MVector Position1, double Energy1, MVector Position2, double Energy2)
{
  Reset();
  
  // Get only the basic data and calculate the rest:
  m_Energy1 = Energy1;       
  m_Position1 = Position1;     
  m_Energy2 = Energy2;       
  m_Position2 = Position2;     
  
  return Validate();
}


////////////////////////////////////////////////////////////////////////////////


MPhysicalEvent* MPETEvent::Duplicate()
{
  MPhysicalEvent* Event = new MPETEvent();
  Event->Assimilate(this);

  return dynamic_cast<MPhysicalEvent*>(Event);
}


////////////////////////////////////////////////////////////////////////////////


bool MPETEvent::Validate()
{
  // Do some sanity checks on the event and calculate all high level data:

  if (m_Energy1 <= 0) return false;
  if (m_Energy2 <= 0) return false;
  
  m_IsGoodEvent = true;

  return true;
}


////////////////////////////////////////////////////////////////////////////////


bool MPETEvent::Stream(MFile& File, int Version, bool Read, bool Fast, bool ReadDelayed)
{
  // Sstream data from and to a file than ROOT...

  // If we are reading, then this handles everything....
  bool Return = MPhysicalEvent::Stream(File, Version, Read, Fast, ReadDelayed);

  if (Read == false) {
    // Write PET specific infos:
    ostringstream S;
    S<<"EF "<<m_Energy1<<endl;
    S<<"PF "<<m_Position1[0]<<" "<<m_Position1[1]<<" "<<m_Position1[2]<<endl;
    S<<"ES "<<m_Energy2<<endl;
    S<<"PS "<<m_Position2[0]<<" "<<m_Position2[1]<<" "<<m_Position2[2]<<endl;
    File.Write(S);
    File.Flush();
  }

  return Return;
}


////////////////////////////////////////////////////////////////////////////////


int MPETEvent::ParseLine(const char* Line, bool Fast)
{
  // Return  0, if the line got correctly parsed
  // Return  1, if the line got not correctly parsed
  // Return  2, if the line got not parsed
  // Return -1, if the end of event has been reached
  
  int Ret = MPhysicalEvent::ParseLine(Line, Fast);
  if (Ret != 2) {
    return Ret;
  }
  
  Ret = 0;
  
  if (Line[0] == 'E' && Line[1] == 'F') {
    if (Fast == true) {
      m_Energy1 = strtod(Line+3, NULL);
    } else {
      if (sscanf(Line, "EF %lf", &m_Energy1) != 1) {
        cout<<"Unable to parse EF of PET event "<<m_Id<<"!"<<endl;
        Ret = 1;
      }
    }
  } else if (Line[0] == 'P' && Line[1] == 'F') {
    if (Fast == true) {
      char* p;
      m_Position1[0] = strtod(Line+3, &p);
      m_Position1[1] = strtod(p, &p);
      m_Position1[2] = strtod(p, NULL);
    } else {
      if (sscanf(Line, "PF %lf %lf %lf", &m_Position1[0], &m_Position1[1], &m_Position1[2]) != 3) {
        cout<<"Unable to parse PF of PET event "<<m_Id<<"!"<<endl;
        Ret = 1;
      }
    }
  } else if (Line[0] == 'E' && Line[1] == 'S') {
    if (Fast == true) {
      m_Energy2 = strtod(Line+3, NULL);
    } else {
      if (sscanf(Line, "ES %lf", &m_Energy2) != 1) {
        cout<<"Unable to parse ES of PET event "<<m_Id<<"!"<<endl;
        Ret = 1;
      }
    }
  } else if (Line[0] == 'P' && Line[1] == 'S') {
    if (Fast == true) {
      char* p;
      m_Position2[0] = strtod(Line+3, &p);
      m_Position2[1] = strtod(p, &p);
      m_Position2[2] = strtod(p, NULL);
    } else {
      if (sscanf(Line, "PS %lf %lf %lf", &m_Position2[0], &m_Position2[1], &m_Position2[2]) != 3) {
        cout<<"Unable to parse PF of PET event "<<m_Id<<"!"<<endl;
        Ret = 1;
      }
    }
  } else {
    Ret = 2;
  }
  
  return Ret;
}


////////////////////////////////////////////////////////////////////////////////


void MPETEvent::Reset() 
{
  // Reset all values to zero:
  
  MPhysicalEvent::Reset();
  
  m_Energy1 = 0.0;       
  m_Position1 = MVector(0.0, 0.0, 0.0);     
  m_Energy2 = 0.0;       
  m_Position2 = MVector(0.0, 0.0, 0.0);     
  
  m_IsGoodEvent = false;

  return;
}


////////////////////////////////////////////////////////////////////////////////


MPhysicalEvent* MPETEvent::Data() 
{
  // Return a pointer to this event
 
  return dynamic_cast<MPhysicalEvent*>(this); 
} 


// MPETEvent: the end...
////////////////////////////////////////////////////////////////////////////////
