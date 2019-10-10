/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, setprecision, ...
#include <cmath>
#include <string>

#include "oah2ManPageGenerator.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "musicXMLOah.h"
#include "bsrOah.h"
#include "brailleOah.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
oah2ManPageGenerator::oah2ManPageGenerator (
  const S_oahHandler handler,
  S_manPageOah&      manPageOpts,
  indentedOstream&   logOstream,
  ostream&            manPageOutputStream)
    : fLogOutputStream (
        logOstream)
{
  fManPageOpts = manPageOpts;

  // the BSR score we're visiting
  fVisitedOahHandler = handler;

/* JMI
  switch (gBrailleOah->fBrailleOutputKind) {
    case kBrailleOutputAscii:
      fBrailleGenerator =
        bsrAsciiBrailleGenerator::create (
          manPageOutputStream);
      break;

    case kBrailleOutputUTF8:
      fBrailleGenerator =
        bsrUTF8BrailleGenerator::create (
          gBrailleOah->fByteOrderingKind,
          manPageOutputStream);
      break;

    case kBrailleOutputUTF16:
      switch (gBrailleOah->fByteOrderingKind) {
        case kByteOrderingNone:
          // JMI ???
          break;

        case kByteOrderingBigEndian:
          fBrailleGenerator =
            bsrUTF16BigEndianBrailleGenerator::create (
              gBrailleOah->fByteOrderingKind,
              manPageOutputStream);
          break;

        case kByteOrderingSmallEndian:
          break;
          fBrailleGenerator =
            bsrUTF16SmallEndianBrailleGenerator::create (
              gBrailleOah->fByteOrderingKind,
              manPageOutputStream);
      } // switch
      break;
  } // switch
*/
}

oah2ManPageGenerator::~oah2ManPageGenerator ()
{}

//________________________________________________________________________
void oah2ManPageGenerator::generateManPageFromOahHandler ()
{
  if (fVisitedOahHandler) {
    // browse a oahHandler browser
    bsrBrowser<oahHandler> browser (this);
    browser.browse (*fVisitedOahHandler);
  }
}

//________________________________________________________________________
void oah2ManPageGenerator::visitStart (S_oahHandler& elt)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> Start visiting oahHandler" <<
      " \"" << elt->getHandlerHeader () << "\"" <<
      endl;
  }
#endif
}

void oah2ManPageGenerator::visitEnd (S_oahHandler& elt)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> End visiting oahHandler" <<
      " \"" << elt->getHandlerHeader () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void oah2ManPageGenerator::visitStart (S_oahGroup& elt)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> Start visiting oahGroup" <<
      " \"" << elt->getGroupHeader () << "\"" <<
      endl;
  }
#endif

/* JMI
  fLogOutputStream <<
    "% --> oahGroup" <<
    ", numberOfSpaces: " << elt->getNumberOfSpaces () <<
    ", spacesCellsList: " << elt->fetchCellsList () <<
    " \"" << elt->getGroupHeader () << "\"" <<
    endl;
*/

}

void oah2ManPageGenerator::visitEnd (S_oahGroup& elt)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> End visiting oahGroup" <<
      " \"" << elt->getGroupHeader () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void oah2ManPageGenerator::visitStart (S_oahSubGroup& elt)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> Start visiting oahSubGroup" <<
      " \"" << elt->getSubGroupHeader () << "\"" <<
      endl;
  }
#endif

}

//________________________________________________________________________
void oah2ManPageGenerator::visitStart (S_oahAtom& elt)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> Start visiting S_oahAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

void oah2ManPageGenerator::visitEnd (S_oahAtom& elt)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> End visiting S_oahAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void oah2ManPageGenerator::visitStart (S_oahAtomWithVariableName& elt)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> Start visiting oahAtomWithVariableName" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

}

void oah2ManPageGenerator::visitEnd (S_oahAtomWithVariableName& elt)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> End visiting oahAtomWithVariableName" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

//________________________________________________________________________
void oah2ManPageGenerator::visitStart (S_oahStringWithDefaultValueAtom& elt)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> Start visiting oahStringWithDefaultValueAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif
}

void oah2ManPageGenerator::visitEnd (S_oahStringWithDefaultValueAtom& elt)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOahVisitors) {
    fLogOutputStream <<
      "% --> End visiting oahStringWithDefaultValueAtom '" <<
      elt->asString () <<
      "'" <<
      " \"" << elt->fetchNames () << "\"" <<
      endl;
  }
#endif

}


} // namespace
