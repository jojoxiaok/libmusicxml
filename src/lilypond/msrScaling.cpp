/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "msrScaling.h"

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msrOah.h"
#include "lpsrOah.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrScaling msrScaling::create (
  int inputLineNumber)
{
  msrScaling* o =
    new msrScaling (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrScaling::msrScaling (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  fMillimeters = -1;
  fTenths      = -1;

  fPageLayout =
    msrPageLayout::create (
      inputLineNumber);
}

msrScaling::~msrScaling ()
{}

S_msrScaling msrScaling::createMsrScalingNewbornClone ()
{
  S_msrScaling
    newbornClone =
      msrScaling::create (
        fInputLineNumber);

  newbornClone->fMillimeters =
    fMillimeters;
  newbornClone->fTenths =
    fTenths;

  newbornClone->fPageLayout =
    fPageLayout;

  return newbornClone;
}

float msrScaling::fetchGlobalStaffSize () const
{
  const float optionsGlobalStaffSize =
    gLpsrOah->fGlobalStaffSize;

  const float defaultTenthsToMillimetersRatio = 0.175;

  float millimetersOverTenths =
    fMillimeters / fTenths;
  float ratio =
    millimetersOverTenths / defaultTenthsToMillimetersRatio;

  float staffSize =
    optionsGlobalStaffSize * ratio;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceGeometry) {
    gLogOstream <<
      "fetchGlobalStaffSize():" <<
      endl;

    gIndenter++;

    gLogOstream <<
      "optionsGlobalStaffSize" << " : " <<
      optionsGlobalStaffSize <<
      endl <<
      "defaultTenthsToMillimetersRatio" << " : " <<
      defaultTenthsToMillimetersRatio <<
      endl <<
      "millimetersOverTenths" << " : " <<
      millimetersOverTenths <<
      endl <<
      "ratio" << " : " <<
      ratio <<
      endl <<
      "staffSize" << " : " <<
      staffSize <<
      endl;

    gIndenter--;
  }
#endif

  if (staffSize < 1.0 || staffSize > 100.0) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceGeometry) {
      stringstream s;

      s <<
        "staffSize " << staffSize <<
        " is not between 1.0 and 100.0, replaced by 20.0:" <<
        endl;

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        fInputLineNumber,
        s.str ());
    }
#endif

    staffSize = optionsGlobalStaffSize;
  }

  return staffSize;
}

void msrScaling::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrScaling::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrScaling>*
    p =
      dynamic_cast<visitor<S_msrScaling>*> (v)) {
        S_msrScaling elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrScaling::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrScaling::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrScaling::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrScaling>*
    p =
      dynamic_cast<visitor<S_msrScaling>*> (v)) {
        S_msrScaling elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrScaling::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrScaling::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    gLogOstream <<
      "% ==> msrScaling::browseData ()" <<
      endl;
  }
#endif

  // browse the page layout
  if (fPageLayout) {
    msrBrowser<msrPageLayout> browser (v);
    browser.browse (*fPageLayout);
  }

#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    gLogOstream <<
      "% <== msrScaling::browseData ()" <<
      endl;
  }
#endif
}

void msrScaling::print (ostream& os) const
{
  os <<
    "Scaling" <<
    endl;

  gIndenter++;

  const int fieldWidth = 13;

  // relative to absolute lengths conversion
  os << left <<
    setw (fieldWidth) <<
    "millimeters" << " : " <<
    setprecision (2) << fMillimeters <<
    endl <<
    setw (fieldWidth) <<
    "tenths" << " : " <<
    setprecision (2) << fTenths <<
    endl;

  // page layout
  os << left <<
    setw (fieldWidth) <<
    "pageLayout" << " : ";
    if (fPageLayout) {
      os <<
        fPageLayout;
    }
    else {
      os << "none";
    }
  os << endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrScaling& elt) {
  elt->print (os);
  return os;
}


}
