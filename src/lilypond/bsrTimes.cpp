/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <sstream>

#include "bsrTimes.h"

#include "bsrNumbers.h"

#include "messagesHandling.h"

#include "generalOptions.h"
#include "bsrOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrTimeItem bsrTimeItem::create (
  int inputLineNumber)
{
  bsrTimeItem* o =
    new bsrTimeItem (
      inputLineNumber);
  assert (o!=0);

  return o;
}

bsrTimeItem::bsrTimeItem (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{
  fTimeBeatValue = -1;
  
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceTimes) {
    gLogIOstream <<
      "Creating time item" <<
      ", line = " << inputLineNumber <<
      endl;
  }
#endif
}

bsrTimeItem::~bsrTimeItem ()
{}

bool bsrTimeItem::isEqualTo (S_bsrTimeItem otherTimeItem) const
{  
  if (! otherTimeItem) {
    return false;
  }
    
  if (
    ! (
        fTimeBeatValue == otherTimeItem->fTimeBeatValue
            &&
        fTimeBeatsNumbersVector.size ()
          ==
        otherTimeItem->fTimeBeatsNumbersVector.size ()
      )
    ) {
    return false;
  }
    
  for (unsigned int i = 0; i < fTimeBeatsNumbersVector.size (); i++) {
    if (
      ! (
        fTimeBeatsNumbersVector [i]
          ==
        otherTimeItem->fTimeBeatsNumbersVector [i]
        )
      ) {
      return false;
    }
  } // for
 
  return true;
}

void bsrTimeItem::appendBeatsNumber (int beatsNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceTimes) {
    gLogIOstream <<
      "Appending beat number '" <<
      beatsNumber <<
      "' to time item '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  fTimeBeatsNumbersVector.insert (
    fTimeBeatsNumbersVector.end (),
    beatsNumber);
}

void bsrTimeItem::setTimeBeatValue (int timeBeatValue)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceTimes) {
    gLogIOstream <<
      "Setting beat value to '" <<
      timeBeatValue <<
      "' in time item '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  fTimeBeatValue = timeBeatValue;
}

int bsrTimeItem::getTimeBeatsNumber () const
{
  int result = 0;
  
  for (unsigned int i = 0; i < fTimeBeatsNumbersVector.size (); i++) {
    result +=
      fTimeBeatsNumbersVector [i];
    } // for

  return result;
}

void bsrTimeItem::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrTimeItem::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrTimeItem>*
    p =
      dynamic_cast<visitor<S_bsrTimeItem>*> (v)) {
        S_bsrTimeItem elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrTimeItem::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrTimeItem::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrTimeItem::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrTimeItem>*
    p =
      dynamic_cast<visitor<S_bsrTimeItem>*> (v)) {
        S_bsrTimeItem elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrTimeItem::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrTimeItem::browseData (basevisitor* v)
{}

string bsrTimeItem::asString () const
{
  stringstream s;

  int timeBeatsNumbersVectorSize =
    fTimeBeatsNumbersVector.size ();

  s <<
    "TimeItem " <<
    ", " <<
    singularOrPlural (
      timeBeatsNumbersVectorSize, "timeBeatNumber", "timeBeatNumbers");

  switch (timeBeatsNumbersVectorSize) {
    case 0:
    /* JMI
      msrInternalError (
        gGeneralOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "time item beats numbers vector is empty");
        */
      break;
      
    case 1:
      s <<
        ", " <<
        fTimeBeatsNumbersVector [0] << "/" << fTimeBeatValue;
      break;
      
    default:
      s <<
        ", beats numbers: ";

      for (int i = 0; i < timeBeatsNumbersVectorSize; i++) {
        s <<
          fTimeBeatsNumbersVector [i];
  
        if (i != timeBeatsNumbersVectorSize - 1) {
          s <<
            " ";
        }
      } // for

      s <<
        ", beat value: " << fTimeBeatValue;
  } // switch
  
  s <<
    ", line " << fInputLineNumber;
     
  return s.str ();
}

void bsrTimeItem::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_bsrTimeItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrTime bsrTime::create (
  int         inputLineNumber,
  bsrTimeKind timeKind)
{
  bsrTime* o =
    new bsrTime (
      inputLineNumber,
      timeKind);
  assert (o!=0);
  return o;
}

bsrTime::bsrTime (
  int         inputLineNumber,
  bsrTimeKind timeKind)
    : bsrLineElement (inputLineNumber)
{
  fTimeKind = timeKind;

  /*
  fTimeCellsList =
    bsrCellsList::create (fInputLineNumber);
    */

#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceTimes) {
    gLogIOstream <<
      "Creating bsrTimes '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif
}

bsrTime::~bsrTime ()
{}

void bsrTime::appendTimeItem (S_bsrTimeItem timeItem)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceTimes) {
    gLogIOstream <<
      "Appending time item '" <<
      timeItem->asString () <<
      "' to time '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif

  fTimeItemsVector.push_back (timeItem);
}

S_bsrCellsList bsrTime::asCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  switch (fTimeKind) {
    case bsrTime::kTimeNone:
      break;

    case bsrTime::kTimeCommon:
      result->appendCellKindToCellsList (kDots46);
      result->appendCellKindToCellsList (kDots14);
      break;
      
    case bsrTime::kTimeCut:
      result->appendCellKindToCellsList (kDots456);
      result->appendCellKindToCellsList (kDots14);
      break;
      
    case bsrTime::kTimeNumerical:
      {
        if (fTimeItemsVector.size ()) {
          vector<S_bsrTimeItem>::const_iterator
            iBegin = fTimeItemsVector.begin (),
            iEnd   = fTimeItemsVector.end (),
            i      = iBegin;

          S_bsrTimeItem bTimeItem = (*i);

          const vector<int>&
            timeBeatsNumbersVector =
              bTimeItem->getTimeBeatsNumbersVector ();
          
          int bTimeBeatValue =
              bTimeItem->getTimeBeatValue ();

          int vectorSize =
            timeBeatsNumbersVector.size ();
        
          switch (vectorSize) {
            case 0:
            /* JMI
              msrInternalError (
                gGeneralOptions->fInputSourceName,
                fInputLineNumber,
                __FILE__, __LINE__,
                "time item beats numbers vector is empty");
                */
              break;
              
            case 1:
              {
                // create the beats number
                S_bsrNumber
                  beatsNumber =
                    bsrNumber::create (
                      fInputLineNumber,
                      timeBeatsNumbersVector [0],
                      bsrNumber::kNumberSignIsNeededYes);
        
                // append it to result
                result->appendCellsListToCellsList (
                  beatsNumber->asCellsList ());
          
                // append the beat value sign to result
                switch (bTimeBeatValue) {
                  case 1:
                    result->appendCellKindToCellsList (
                      kCellLower1);
                    break;
                  case 2:
                    result->appendCellKindToCellsList (
                      kCellLower2);
                    break;
                  case 4:
                    result->appendCellKindToCellsList (
                      kCellLower4);
                    break;
                  case 8:
                    result->appendCellKindToCellsList (
                      kCellLower8);
                    break;
                  case 16:
                    result->appendCellKindToCellsList (
                      kCellLower1);
                    result->appendCellKindToCellsList (
                      kCellLower6);
                    break;
                  case 32:
                    result->appendCellKindToCellsList (
                      kCellLower3);
                    result->appendCellKindToCellsList (
                      kCellLower2);
                    break;
                  default:
                    {
                      stringstream s;
                  
                      s <<
                        "MSR time beat value '" <<
                        bTimeBeatValue <<
                        "' is not supported in Braille music";
                        
                      bsrMusicXMLWarning (
                        gGeneralOptions->fInputSourceName,
                        fInputLineNumber,
                        s.str ());
                    }
                } // switch
              }
              break;
              
            default:
              ;
            /*
              s <<
                "beats numbers: ";
        
              for (int i = 0; i < vectorSize; i++) {
                s <<
                  timeBeatsNumbersVector [i];
          
                if (i != vectorSize - 1) {
                  s <<
                    " ";
                }
              } // for
        
              s <<
                ", beat value: " << fTimeBeatValue;
                */
          } // switch
        }
      }
      break;
  } // switch

  return result;
}

int bsrTime::fetchCellsNumber() const
{
  // time items may have been appended after construction
  return asCellsList ()->fetchCellsNumber ();
}

void bsrTime::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrTime::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrTime>*
    p =
      dynamic_cast<visitor<S_bsrTime>*> (v)) {
        S_bsrTime elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrTime::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrTime::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrTime::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrTime>*
    p =
      dynamic_cast<visitor<S_bsrTime>*> (v)) {
        S_bsrTime elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrTime::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrTime::browseData (basevisitor* v)
{}

string bsrTime::timeKindAsString (
  bsrTimeKind timeKind)
{
  string result;
 
  switch (timeKind) {
    case bsrTime::kTimeNone:
      result = "timeNone";
      break;

    case bsrTime::kTimeCommon:
      result = "timeCommon";
      break;
    case bsrTime::kTimeCut:
      result = "timeCut";
      break;
    case bsrTime::kTimeNumerical:
      result = "timeNumerical";
      break;
  /* 
    case bsrTime::kTimeSymbolDottedNote:
      break;
    case bsrTime::kTimeSymbolSingleNumber:
      break;
    case bsrTime::kTimeSymbolSenzaMisura:
      break;
      */
  } // switch

  return result;
}

string bsrTime::asString () const
{
  stringstream s;

  s <<
    "Time" <<
    ", timeKind " << " : " <<
    timeKindAsString (fTimeKind) <<
// JMI    ", timeCellsList: " << fTimeCellsList->asString () <<
    ", timeCellsList: " << asCellsList ()->asString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;
 
  return s.str ();
}

void bsrTime::print (ostream& os)
{
  int timeItemsVectorSize = fTimeItemsVector.size ();
  
  os <<
    "Time" <<
    ", " <<
    singularOrPlural (
      timeItemsVectorSize, "item", "items") <<
    ", line " << fInputLineNumber <<
    ":" <<
    endl;

  gIndenter++;

  const int fieldWidth = 16;

  os << left <<
    setw (fieldWidth) <<
    "timeKind " << " : " <<
    timeKindAsString (fTimeKind) <<
    endl <<
    setw (fieldWidth) <<
    "timeItemsVector" << " : ";

  if (timeItemsVectorSize) {
    os <<
      endl;
      
    gIndenter++;
    
    vector<S_bsrTimeItem>::const_iterator
      iBegin = fTimeItemsVector.begin (),
      iEnd   = fTimeItemsVector.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
    
    gIndenter--;
  }
  
  else {
    os <<
      " none" <<
      endl;
  }
    
  os <<
    setw (fieldWidth) <<
 // JMI   "timeCellsList" << " : " << fTimeCellsList->asString () <<
    "timeCellsList" << " : " << asCellsList ()->asString () <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
   endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrTime& elt)
{
  elt->print (os);
  return os;
}


}