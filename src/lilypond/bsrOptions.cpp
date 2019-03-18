/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "utilities.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "bsrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
string facSimileKindAsString (
  bsrFacSimileKind facSimileKind)
{
  string result;

  switch (facSimileKind) {
    case kFacSimileYes:
      result = "facSimileYes";
      break;
    case kFacSimileNo:
      result = "facSimileNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_optionsFacSimileKindItem optionsFacSimileKindItem::create (
  string           optionsItemShortName,
  string           optionsItemLongName,
  string           optionsItemDescription,
  string           optionsValueSpecification,
  string           optionsFacSimileKindItemVariableDisplayName,
  bsrFacSimileKind optionsFacSimileKindItemVariable)
{
  optionsFacSimileKindItem* o = new
    optionsFacSimileKindItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsFacSimileKindItemVariableDisplayName,
      optionsFacSimileKindItemVariable);
  assert(o!=0);
  return o;
}

optionsFacSimileKindItem::optionsFacSimileKindItem (
  string           optionsItemShortName,
  string           optionsItemLongName,
  string           optionsItemDescription,
  string           optionsValueSpecification,
  string           optionsFacSimileKindItemVariableDisplayName,
  bsrFacSimileKind optionsFacSimileKindItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsFacSimileKindItemVariableDisplayName (
      optionsFacSimileKindItemVariableDisplayName),
    fOptionsFacSimileKindItemVariable (
      optionsFacSimileKindItemVariable)
{}

optionsFacSimileKindItem::~optionsFacSimileKindItem ()
{}

void optionsFacSimileKindItem::print (ostream& os) const
{
  const int fieldWidth = K_FIELD_WIDTH;
  
  os <<
    "OptionsFacSimileKindItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "optionsFacSimileKindItemVariableDisplayName" << " : " <<
    fOptionsFacSimileKindItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "optionsFacSimileKindItemVariable" << " : \"" <<
    facSimileKindAsString (
      fOptionsFacSimileKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsFacSimileKindItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{  
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsFacSimileKindItemVariableDisplayName <<
    " : \"" <<
    facSimileKindAsString (
      fOptionsFacSimileKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsFacSimileKindItem& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_bsrOptions gBsrOptions;
S_bsrOptions gBsrOptionsUserChoices;
S_bsrOptions gBsrOptionsWithDetailedTrace;

S_bsrOptions bsrOptions::create (
  S_optionsHandler optionsHandler)
{
  bsrOptions* o = new bsrOptions (
    optionsHandler);
  assert(o!=0);
  return o;
}

bsrOptions::bsrOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "BSR",
    "hbsr", "help-bsr",
R"(These options control the way BSR data is handled.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroup (this);
  }

  // initialize it
  initializeBsrOptions (false);
}

bsrOptions::~bsrOptions ()
{}

void bsrOptions::initializeBsrOptions (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OPTIONS
  // display
  // --------------------------------------

  {
    // variables
    
    fDisplayBsr           = boolOptionsInitialValue;
    fDisplayBsrDetails    = boolOptionsInitialValue;
      
    // options
  
    S_optionsSubGroup
      traceAndDisplaySubGroup =
        optionsSubGroup::create (
          "Display",
          "hbsrtd", "help-bsr-display",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (traceAndDisplaySubGroup);
        
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "dbsr", "display-bsr",
R"(Write the contents of the BSR data to standard error.)",
          "displayBsr",
          fDisplayBsr,
          gTraceOptions->fTracePasses));
  
    traceAndDisplaySubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "dbsrd", "display-bsr-details",
R"(Write the contents of the BSR data with more details to standard error.)",
          "displayBsrDetails",
          fDisplayBsrDetails,
          gTraceOptions->fTracePasses));
  }
#endif


  // miscellaneous  
  // --------------------------------------

  {
    // variables  
  
    fNoBrailleLyrics      = boolOptionsInitialValue;
    
    fBrailleCompileDate   = boolOptionsInitialValue;
    
    fFacSimileKind        = kFacSimileNo;
    
    fIncludeClefs         = boolOptionsInitialValue;

    // options
  
    S_optionsSubGroup
      miscellaneousGenerationSubGroup =
        optionsSubGroup::create (
          "Miscellaneous",
          "hlpm", "help-miscellaneous",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (miscellaneousGenerationSubGroup);

    miscellaneousGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "nolpl", "no-braille-lyrics",
R"(Don't generate any lyrics in the Braille code.)",
          "noBrailleLyrics",
          fNoBrailleLyrics));

    miscellaneousGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "lpcd", "braille-compile-date",
R"(Generate code to include the compilation date
when Braille creates the score.)",
          "brailleCompileDate",
          fBrailleCompileDate));

    miscellaneousGenerationSubGroup->
      appendOptionsItem (
        optionsFacSimileKindItem::create (
          "fs", "facsimile",
R"(Generate facsimile Braille nusic code.
By default, non-facsimile code is generated.)",
          "YES_OR_NO",
          "facSimileKind",
          fFacSimileKind));

    miscellaneousGenerationSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "clefs", "",
R"(Include clefs in BSR. By default, they are not.)",
          "includeClefs",
          fIncludeClefs));
  }

  
  // exit after some passes
  // --------------------------------------

  {
    // variables  
  
    // options
  
    S_optionsSubGroup
      exitAfterSomePassesSubGroup =
        optionsSubGroup::create (
          "Exit after some passes",
          "hbe", "help-bsr-exit",
R"()",
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (exitAfterSomePassesSubGroup);
        
    // '-exit-3a' is hidden...
    S_optionsBooleanItem
      exit2aOptionsBooleanItem =
        optionsBooleanItem::create (
          "e3a", "exit-3a",
R"(Exit after pass 3a, i.e. after conversion
of the MSR to the first BSR score.)",
          "exit3a",
          fExit3a);
          /* JMI
    exit2aOptionsBooleanItem->
      setOptionsElementIsHidden ();
      */
      
    exitAfterSomePassesSubGroup->
      appendOptionsItem (
        exit2aOptionsBooleanItem);

    // '-exit-3b' is hidden...
    S_optionsBooleanItem
      exit2bOptionsBooleanItem =
        optionsBooleanItem::create (
          "e3b", "exit-3b",
R"(Exit after pass 3b, i.e. after conversion
of the first BSR to the second BSR.)",
          "exit3b",
          fExit3b);
          /* JMI
    exit2bOptionsBooleanItem->
      setOptionsElementIsHidden ();
      */
      
    exitAfterSomePassesSubGroup->
      appendOptionsItem (
        exit2bOptionsBooleanItem);
  }


#ifdef TRACE_OPTIONS
  // trace  
  // --------------------------------------

  {
    // variables  
  
    fTraceBsr             = boolOptionsInitialValue;

    fTracePages           = boolOptionsInitialValue;

    fTraceLines           = boolOptionsInitialValue;
    
    fTraceSpaces          = boolOptionsInitialValue;
    fTraceNumbers         = boolOptionsInitialValue;
    
    fTraceParallels       = boolOptionsInitialValue;

    fTraceBsrVisitors     = boolOptionsInitialValue;
  
    S_optionsSubGroup
      specificTraceSubGroup =
        optionsSubGroup::create (
          "Trace",
          "hbst", "help-bsr-trace",
R"(Note: the options in this group imply '-t, -trace-passes'.)",
// JMI        optionsSubGroup::kHideDescriptionByDefault,
        optionsSubGroup::kAlwaysShowDescription,
        this);
  
    appendOptionsSubGroup (specificTraceSubGroup);
    
    specificTraceSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tbsr", "trace-bsr",
R"(Write a trace of the BSR graphs visiting activity to standard error.)",
          "traceBsr",
          fTraceBsr));
  
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tpages", "trace-pages",
R"()",
          "tracePages",
          fTracePages,
          gTraceOptions->fTracePasses));
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tlines", "trace-lines",
R"()",
          "traceLines",
          fTraceLines));
  
    specificTraceSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tspaces", "trace-spaces",
R"(Write a trace of the BSR spaces activity to standard error.)",
          "traceSpaces",
          fTraceSpaces));
  
    specificTraceSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tnums", "trace-numbers",
R"(Write a trace of the BSR numbers activity to standard error.)",
          "traceNumbers",
          fTraceNumbers));
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tpars", "trace-parallels",
R"()",
          "traceParallels",
          fTraceParallels,
          gTraceOptions->fTracePasses));

    specificTraceSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tbsrv", "trace-bsr-visitors",
R"(Write a trace of the BSR tree visiting activity to standard error.)",
          "traceBsrVisitors",
          fTraceBsrVisitors));
  }
#endif
}

S_bsrOptions bsrOptions::createCloneWithDetailedTrace ()
{
  S_bsrOptions
    clone =
      bsrOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // display
  // --------------------------------------
    
  clone->fDisplayBsr =
    true;
  clone->fDisplayBsrDetails =
    true;


  // miscellaneous
  // --------------------------------------

    
  // exit after some passes
  // --------------------------------------


  return clone;
}

//______________________________________________________________________________
void bsrOptions::enforceQuietness ()
{
  fDisplayBsr = false;
  fDisplayBsrDetails = false;
}

//______________________________________________________________________________
void bsrOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void bsrOptions::printBsrOptionsValues (int fieldWidth)
{
  gLogIOstream <<
    "The BSR options are:" <<
    endl;

  gIndenter++;
  
  // trace and display
  // --------------------------------------
  gLogIOstream <<
    "Trace and display:" <<
    endl;

  gIndenter++;
  
  gLogIOstream << left <<
    setw (fieldWidth) << "displayBsr" << " : " <<
    booleanAsString (fDisplayBsr) <<
    endl <<
    
    setw (fieldWidth) << "displayBsrDetails" << " : " <<
    booleanAsString (fDisplayBsrDetails) <<
    endl;

  gIndenter--;
  
  // exit after some passes
  // --------------------------------------

  gLogIOstream <<
    "Exit after some passes:" <<
    endl;

  gIndenter++;
  
  gLogIOstream << left <<
    setw (fieldWidth) << "exit3a" << " : " <<
    booleanAsString (fExit3a) <<
    endl <<
    setw (fieldWidth) << "exit3b" << " : " <<
    booleanAsString (fExit3b) <<
    endl;

  gIndenter--;
    
  // miscellaneous
  // --------------------------------------
  
  gLogIOstream <<
    "Miscellaneous:" <<
    endl;

  gIndenter++;
  
  gLogIOstream << left <<
    setw (fieldWidth) << "noBrailleLyrics" << " : " <<
      booleanAsString (fNoBrailleLyrics) <<
      endl <<
    setw (fieldWidth) << "brailleCompileDate" << " : " <<
      booleanAsString (fBrailleCompileDate) <<
      endl <<
    setw (fieldWidth) << "facSimileKind" << " : " <<
      booleanAsString (fFacSimileKind) <<
      endl <<
    setw (fieldWidth) << "includeClefs" << " : " <<
      booleanAsString (fIncludeClefs) <<
      endl;

  gIndenter--;
  
  // trace
  // --------------------------------------

#ifdef TRACE_OPTIONS
  gLogIOstream <<
    "Trace:" <<
    endl;

  gIndenter++;
  
  gLogIOstream << left <<
    setw (fieldWidth) << "traceBsr" << " : " <<
    booleanAsString (fTraceBsr) <<
    endl <<
    setw (fieldWidth) << "tracePages" << " : " <<
    booleanAsString (fTracePages) <<
    endl <<
    setw (fieldWidth) << "traceLines" << " : " <<
    booleanAsString (fTraceLines) <<
    endl <<
    setw (fieldWidth) << "traceSpaces" << " : " <<
    booleanAsString (fTraceSpaces) <<
    endl <<
    setw (fieldWidth) << "traceNumbers" << " : " <<
    booleanAsString (fTraceNumbers) <<
    endl <<
    setw (fieldWidth) << "traceParallels" << " : " <<
    booleanAsString (fTraceParallels) <<
    endl <<
    setw (fieldWidth) << "traceBsrVisitors" << " : " <<
    booleanAsString (fTraceBsrVisitors) <<
    endl;

  gIndenter--;
#endif

  gIndenter--;
}

S_optionsItem bsrOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;

  /* JMI
  if (
    // BSR pitches language item?
    S_optionsBsrPitchesLanguageItem
      pitchesLanguageItem =
        dynamic_cast<optionsBsrPitchesLanguageItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsBsrPitchesLanguageItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = pitchesLanguageItem;
  }

  else if (
    // chords language item?
    S_optionsBsrChordsLanguageItem
      bsrChordsLanguageItem =
        dynamic_cast<optionsBsrChordsLanguageItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsBsrChordsLanguageItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = bsrChordsLanguageItem;
  }
  */

  return result;
}

void bsrOptions::handleOptionsItemValue (
  ostream&      os,
  S_optionsItem item,
  string        theString)
{
}

ostream& operator<< (ostream& os, const S_bsrOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeBsrOptionsHandling (
  S_optionsHandler optionsHandler)
{
  // create the options variables
  // ------------------------------------------------------

  gBsrOptionsUserChoices = bsrOptions::create (
    optionsHandler);
  assert(gBsrOptionsUserChoices != 0);
  
  gBsrOptions =
    gBsrOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------
  gBsrOptionsWithDetailedTrace =
    gBsrOptions->
      createCloneWithDetailedTrace ();
}


}