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

#include <sstream>
#include <stdlib.h>     /* abort, NULL */
#include <climits>      /* INT_MIN */
#include <algorithm>    /* for_each */
#include <iomanip>      // setw, set::precision, ...

#include "conversions.h"

#include "lpsr2LilyPondVisitor.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
lpsr2LilyPondVisitor::lpsr2LilyPondVisitor (
  S_msrOptions&  msrOpts,
  S_lpsrOptions& lpsrOpts,
  ostream&       os,
  S_lpsrScore    lpScore)
    : fOstream (os)
{
  fMsrOptions       = msrOpts;
  fLpsrOptions      = lpsrOpts;
  fVisitedLpsrScore = lpScore;

  fOnGoingHeader = false;
  
  fOnGoingStaff  = false;

  fOngoingNonEmptyLyrics = false;

  fOnGoingScoreBlock = false;
};
  
lpsr2LilyPondVisitor::~lpsr2LilyPondVisitor () {}

//________________________________________________________________________
void lpsr2LilyPondVisitor::generateLilyPondCodeFromLpsrScore ()
{
  if (fVisitedLpsrScore) {    
    // browse a msrScore browser
    msrBrowser<lpsrScore> browser (this);
    browser.browse (*fVisitedLpsrScore);
  }
}

//________________________________________________________________________
string lpsr2LilyPondVisitor::noteMsrPitchAsLilyPondString (
  S_msrNote& note)
{
  stringstream s;
  
  //JMI assertMsr(fMsrPitch != k_NoMsrPitch, "fMsrPitch != k_NoMsrPitch");

  bool noteIsGraceNote =
    note->getNoteIsGraceNote ();
    
  if (noteIsGraceNote)
    s <<
      "\\grace { ";

  if (note->getNoteIsUnpitched ())
    s << "unpitched ";
  else
    s << note->notePitchAsString ();
  
  // in MusicXML, octave number is 4 for the octave starting with middle C

  msrMusicXMLNoteData::msrDiatonicPitch
    noteDiatonicPitch =
      note->getDiatonicPitch ();
      
  string
    noteDiatonicPitchAsString =
      note->noteDiatonicPitchAsString ();
        
  int noteAbsoluteOctave =
    note->getNoteMusicXMLOctave ();

  bool noteIsChordFirstNote =
    note->getNoteIsChordFirstNote ();
    
  // should an absolute octave be generated?
  bool genAbsoluteOctave =
    fLpsrOptions->fGenerateAbsoluteOctaves
      ||
    ! fRelativeOctaveReference;
/* JMI
      ||
    noteIsChordFirstNote;
*/

  if (fMsrOptions->fForceDebug || fMsrOptions->fDebugDebug) {
    cerr << left <<
      endl <<
      idtr <<
          setw(33) << "% line" <<
          " = " <<
          note->getInputLineNumber () <<
          endl <<
      idtr <<
        setw(33) << "% noteDiatonicPitchAsString" <<
        " = - " <<
        noteDiatonicPitchAsString <<
        " -" <<
        endl <<
      idtr <<
        setw(33) << "% noteAbsoluteOctave" <<
        " = - " <<
        noteAbsoluteOctave <<
        " -" <<
        endl <<
      idtr <<
        setw(33) << "% noteIsChordFirstNote" <<
        " = " <<
        string (noteIsChordFirstNote
          ? "true"
          : "false") <<
        endl <<
/*
      idtr <<
        setw(33) << "% noteDiatonicPitch" <<
        " = " <<
        noteDiatonicPitch <<
        endl <<
*/
    endl;
  }

  if (genAbsoluteOctave) {
    
    // generate LilyPond absolute octave
    switch (noteAbsoluteOctave) {
      case 0:
        s << ",,,";
        break;
      case 1:
        s << ",,";
        break;
      case 2:
        s << ",";
        break;
      case 3:
        s << "";
        break;
      case 4:
        s << "'";
        break;
      case 5:
        s << "''";
        break;
      case 6:
        s << "'''";
        break;
      case 7:
        s << "''''";
        break;
      case 8:
        s << "'''''";
        break;
      default:
        s << "###";
    } // switch
  }

  else {
    
    // generate LilyPond octave relative to fRelativeOctaveReference

    msrMusicXMLNoteData::msrDiatonicPitch
      referenceDiatonicPitch =
        fRelativeOctaveReference->getDiatonicPitch ();

    string
      referenceDiatonicPitchAsString =
        fRelativeOctaveReference->noteDiatonicPitchAsString ();
        
    int
      referenceAbsoluteOctave =
        fRelativeOctaveReference->getNoteMusicXMLOctave ();

    /*
      If no octave changing mark is used on a pitch, its octave is calculated
      so that the interval with the previous note is less than a fifth.
      This interval is determined without considering accidentals.
    */
      
    int
      noteAboluteDiatonicOrdinal =
        noteAbsoluteOctave * 7
          +
        noteDiatonicPitch - msrMusicXMLNoteData::kC,
        
      referenceAboluteDiatonicOrdinal =
        referenceAbsoluteOctave * 7
          +
        referenceDiatonicPitch - msrMusicXMLNoteData::kC;

    if (fMsrOptions->fForceDebug || fMsrOptions->fDebugDebug) {
      cerr << left <<
/*
        idtr <<
          setw(33) <<
          "% referenceDiatonicPitch" <<
          " = " <<
          referenceDiatonicPitch <<
          endl <<
*/
        idtr <<
          setw(33) << "% referenceDiatonicPitchAsString" <<
          " = " <<
          referenceDiatonicPitchAsString <<
          endl <<
        idtr <<
          setw(33) << "% referenceAbsoluteOctave" <<
           " = " <<
           referenceAbsoluteOctave <<
           endl <<
        endl <<
        idtr <<
          setw(33) << "% referenceAboluteDiatonicOrdinal" <<
          " = " <<
          referenceAboluteDiatonicOrdinal <<
          endl <<
        idtr <<
          setw(33) << "% noteAboluteDiatonicOrdinal" <<
          " = " <<
          noteAboluteDiatonicOrdinal <<
          endl <<
        endl;
    }

    // generate the octaves as needed
    if (noteAboluteDiatonicOrdinal >= referenceAboluteDiatonicOrdinal) {
      noteAboluteDiatonicOrdinal -= 4;
      while (noteAboluteDiatonicOrdinal >= referenceAboluteDiatonicOrdinal) {
        s << "'";
        noteAboluteDiatonicOrdinal -= 7;
      } // while
    }
    
    else {
      noteAboluteDiatonicOrdinal += 4;
      while (noteAboluteDiatonicOrdinal <= referenceAboluteDiatonicOrdinal) {
        s << ",";
        noteAboluteDiatonicOrdinal += 7;
      } // while
    }
  }
  
  if (noteIsGraceNote)
    s <<
      " } ";

  if (noteIsChordFirstNote)
    fRelativeOctaveReference = note;

  return s.str();
}
        

/*
    if (abs (absoluteDiatonicDistance) > 5) {
      // a relative octave is to be generated
      switch (octavesToBeDisplayed) {
        case -8:
          s << ",,,,,,,,";
          break;
        case -7:
          s << ",,,,,,,";
          break;
        case -6:
          s << ",,,,,,";
          break;
        case -5:
          s << ",,,,,";
          break;
        case -4:
          s << ",,,,";
          break;
        case -3:
          s << ",,,";
          break;
        case -2:
          s << ",,";
          break;
        case -1:
          s << ",";
          break;
        case 0:
          s << "";
          break;
        case 1:
          s << "'";
          break;
        case 2:
          s << "''";
          break;
        case 3:
          s << "'''";
          break;
        case 4:
          s << "''''";
          break;
        case 5:
          s << "'''''";
          break;
        case 6:
          s << "''''''";
          break;
        case 7:
          s << "'''''''";
          break;
        case 8:
          s << "''''''''";
          break;
        default:
          s << "###";
      } // switch
    }
    */

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrScore& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrScore" << endl;

  // initial empty line in LilyPond code
  // to help copy/paste it
  fOstream << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrScore& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrScore" << endl;

  // final empty line in LilyPond code
  // to help copy/paste it
  fOstream << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrLilypondVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrLilypondVarValAssoc" << endl;

  fOstream << idtr;

  if (elt->getComment ().size())
    fOstream <<
      "% " << elt->getComment () << endl <<
      idtr;

  if (elt->getCommentedKind () == lpsrLilypondVarValAssoc::kCommented)
    fOstream << "\%";
  
  switch (elt->getBackslashKind ()) {
    case lpsrLilypondVarValAssoc::kWithBackslash:
      fOstream << "\\";
      break;
    case lpsrLilypondVarValAssoc::kWithoutBackslash:
      break;
  } // switch

  // the largest variable name length in a header is 18
  int fieldWidth;
  
  if (fOnGoingHeader)
    fieldWidth = 18;
  else
    fieldWidth = elt->getVariableName ().size();
    
  fOstream <<
    left<< setw(fieldWidth) << elt->getVariableName ();
  
  if (elt->getVarValSeparator () == lpsrLilypondVarValAssoc::kEqualSign)
    fOstream << " = ";
  else
    fOstream << " ";
  
  if (elt->getQuotesKind () == lpsrLilypondVarValAssoc::kQuotesAroundValue)
    fOstream << "\"";
    
  fOstream <<
    elt->getVariableValue ();

  if (elt->getUnit ().size())
    fOstream <<
      "\\" <<
      elt->getUnit ();
  
  if (elt->getQuotesKind () == lpsrLilypondVarValAssoc::kQuotesAroundValue)
    fOstream << "\"";
  
  fOstream << endl;

  switch (elt->getEndlKind ()) {
    case lpsrLilypondVarValAssoc::kWithEndl:
      fOstream << endl;
      break;
    case lpsrLilypondVarValAssoc::kWithEndlTwice:
      fOstream << endl << endl;
      break;
    case lpsrLilypondVarValAssoc::kWithoutEndl:
      break;
  } // switch
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrLilypondVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrLilypondVarValAssoc" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrSchemeVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrSchemeVarValAssoc" << endl;

  fOstream << idtr;

  if (elt->getComment ().size())
    fOstream <<
      "% " << elt->getComment () << endl <<
      idtr;

  if (elt->getCommentedKind () == lpsrSchemeVarValAssoc::kCommented)
    fOstream << "\% ";
  
  fOstream <<
    "#(" <<
    elt->getVariableName () <<
    " " <<
    elt->getVariableValue () <<
    ")";
    
  switch (elt->getEndlKind ()) {
    case lpsrSchemeVarValAssoc::kWithEndl:
      fOstream << endl;
      break;
    case lpsrSchemeVarValAssoc::kWithEndlTwice:
      fOstream << endl << endl;
      break;
    case lpsrSchemeVarValAssoc::kWithoutEndl:
      break;
  } // switch
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrSchemeVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrSchemeVarValAssoc" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrHeader& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrHeader" << endl;

  fOstream << idtr <<
    "\\header" << " {" <<
    endl;

  idtr++;

  fOnGoingHeader = true;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrHeader& elt)
{
  fOnGoingHeader = false;

  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrHeader" << endl;

  fOstream << idtr <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrPaper& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrPaper" << endl;

  fOstream << idtr <<
    "\\paper" << " {" <<
    endl;

  idtr++;
  
  if (elt->getPaperWidth () > 0) {
    fOstream << idtr <<
      setw(13) << left << "paper-width" << " = " <<
      setprecision(4) << elt->getPaperWidth () << "\\cm" << endl;
  }
  if (elt->getPaperHeight () > 0) {
    fOstream << idtr << 
      setw(13) << left << "paper-height" << " = " <<
      setprecision(4) << elt->getPaperHeight () << "\\cm" << endl;
  }
  
  if (elt->getTopMargin () > 0) {
    fOstream << idtr << 
      setw(13) << left << "top-margin" << " = " <<
      setprecision(4) << elt->getTopMargin () << "\\cm" << endl;
  }
  if (elt->getBottomMargin () > 0) {
    fOstream << idtr << 
      setw(13) << left << "bottom-margin" << " = " <<
      setprecision(4) << elt->getBottomMargin () << "\\cm" << endl;
  }
  if (elt->getLeftMargin () > 0) {
    fOstream << idtr << 
      setw(13) << left << "left-margin" << " = " <<
      setprecision(4) << elt->getLeftMargin () << "\\cm" << endl;
  }
  if (elt->getRightMargin () > 0) {
    fOstream << idtr << 
      setw(13) << left << "right-margin" << " = " <<
    setprecision(4) << elt->getRightMargin () << "\\cm" << endl;
  }

/*
 * 
  if (fBetweenSystemSpace > 0) {
    os << idtr << "between-system-space = " << setprecision(4) << fBetweenSystemSpace << "\\cm" << endl;
  }

  if (elt->getPageTopSpace > 0) {
    os << idtr << "page-top-space = " << setprecision(4) << elt->getPageTopSpace << "\\cm" << endl;
  }
*/
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrPaper& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrPaper" << endl;
      
  fOstream << idtr <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrLayout& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrLayout" << endl;

  fOstream <<
    idtr << "\\layout" << " {" <<
    endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrLayout& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrLayout" << endl;

  fOstream << idtr <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrScoreBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrScoreBlock" << endl;

  fOstream << idtr <<
    "\\score {" <<
    endl;

  idtr++;
/*
  if (elt->getScoreBlockElements ().size()) {
    fOstream <<
      idtr << "<<" <<
      endl;
  
    idtr++;
  }
*/
  fOnGoingScoreBlock = true;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrScoreBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrScoreBlock" << endl;
/*
  if (elt->getScoreBlockElements ().size()) {
    idtr--;
    
    fOstream <<
      idtr << ">>" <<
      endl << endl;
  }
*/
  idtr--;
  
  fOstream << idtr <<
    "}" <<
    endl;

  fOnGoingScoreBlock = false;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrParallelMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrParallelMusic" << endl;

  if (elt->getParallelMusicElements ().size()) { // JMI
    fOstream <<
      idtr << "<<" <<
      endl;
  
    idtr++;
  }
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrParallelMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrParallelMusic" << endl;

  if (elt->getParallelMusicElements ().size()) { // JMI
    idtr--;
    
    fOstream <<
      idtr << ">>" <<
      endl << endl;
  }
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrPartgroupBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrPartgroupBlock" << endl;

  // the top level part group is the score block's S_lpsrParallelMusic
  
  if (elt->getPartgroup ()->getPartgroupPartgroupUplink ()) {
    // the part group is not the top level one

    S_msrPartgroup
      partgroup =
        elt->getPartgroup ();
        
    msrPartgroup::msrPartgroupSymbolKind
      partgroupSymbolKind =
        partgroup->getPartgroupSymbolKind ();
  
    string
      partgroupInstrumentName =
        partgroup->getPartgroupInstrumentName ();
        
    string partGroupContextName;
  
    // LPNR, page 567
    
    switch (partgroupSymbolKind) {
      case msrPartgroup::k_NoPartgroupSymbol:
        partGroupContextName = "";
        break;
        
      case msrPartgroup::kBracePartgroupSymbol: // JMI
        if (partgroupInstrumentName.size ())
          partGroupContextName = "PianoStaff";
        else
          partGroupContextName = "GrandStaff";
        break;
        
      case msrPartgroup::kBracketPartgroupSymbol:
        partGroupContextName = "GrandStaff";
        break;
        
      case msrPartgroup::kLinePartgroupSymbol:
        partGroupContextName = "StaffGroup";
        break;
        
      case msrPartgroup::kSquarePartgroupSymbol:
        partGroupContextName = "StaffGroup";
        break;
    } // switch
  
  
    fOstream << idtr <<
      "\\new " << partGroupContextName << " " "{";
      
    if (fLpsrOptions->fGenerateComments)
      fOstream <<
        setw(30) << "% part group " <<
        elt->getPartgroup ()->getPartgroupCombinedName ();
        
    fOstream <<
      endl;
  
    if (partgroupInstrumentName.size ())
      fOstream << idtr <<
        "\\set PianoStaff.instrumentName = #\"" <<
        partgroupInstrumentName <<
        "\"" <<
        endl;
  
    fOstream <<
      endl;
  
    idtr++;
  }
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrPartgroupBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrPartgroupBlock" << endl;

  // the top level part group is the score block's S_lpsrParallelMusic
  
  if (elt->getPartgroup ()->getPartgroupPartgroupUplink ()) {
    // the part group is not the top level one
    idtr--;

    fOstream <<
      idtr <<
      "}";
    if (fLpsrOptions->fGenerateComments)
      fOstream <<
        setw(30) << "% part group " <<
        elt->getPartgroup ()->getPartgroupCombinedName ();
    fOstream << endl;
  }
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrPartBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrPartBlock" << endl;

  S_msrPart
    part =
      elt->getPart ();
      
  string
    partInstrumentName =
      part->getPartInstrumentName ();

  fOstream << idtr <<
    "\\new StaffGroup" <<  " " "{";
    
  if (fLpsrOptions->fGenerateComments)
    fOstream <<
      setw(30) << "% part " <<
      elt->getPart ()->getPartCombinedName ();
      
  if (partInstrumentName.size ())
    fOstream << idtr <<
      "\\set Staff.instrumentName = #\"" <<
      partInstrumentName <<
      "\"" <<
      endl;

  fOstream << endl;
  
  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrPartBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrPartBlock" << endl;

  idtr--;

  fOstream <<
    idtr <<
    "}";
    
  if (fLpsrOptions->fGenerateComments)
    fOstream <<
      setw(30) << "% part " <<
      elt->getPart ()->getPartCombinedName ();
      
  fOstream << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrStaffBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrStaffBlock" << endl;

  S_msrStaff
    staff =
      elt->getStaff ();
      
  string
    staffInstrumentName =
      staff->getStaffInstrumentName ();

  fOstream << idtr <<
    "\\new Staff" " " "<<";
    
  if (fLpsrOptions->fGenerateComments)
    fOstream <<
      setw(30) << "% staff " <<
      elt->getStaff ()->getStaffName ();
      
  fOstream << endl;

  if (staffInstrumentName.size ())
    fOstream << idtr <<
      "\\set Staff.instrumentName = #\"" <<
      staffInstrumentName <<
      "\"" <<
      endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrStaffBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrStaffBlock" << endl;

  idtr--;

  fOstream <<
    idtr <<
    ">>";
  if (fLpsrOptions->fGenerateComments)
    fOstream <<
      setw(30) << "% staff " <<
      elt->getStaff ()->getStaffName ();
  fOstream << endl;
}

/*
//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrNewStaffgroupBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrNewStaffgroupBlock" << endl;

   fOstream << idtr <<
     "\\new StaffGroup" << " " << "{" <<
      endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrNewStaffgroupBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrNewStaffgroupBlock" << endl;

  idtr--;

  fOstream <<
    " }" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrNewStaffBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrNewStaffBlock" << endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrNewStaffBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrNewStaffBlock" << endl;

  idtr--;
}
*/

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrUseVoiceCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrUseVoiceCommand" << endl;

  S_msrVoice voice = elt->getVoice ();

  S_msrStaff staff = voice-> getVoiceStaffUplink ();

  fOstream << idtr <<
    "\\context Voice" << " = " <<
    "\"" << voice->getVoiceName () << "\""<<
    " " <<
    "{" <<
     endl;

  idtr++;

  if (fLpsrOptions->fNoAutoBeaming)
    fOstream << idtr <<
      "\\set Voice.autoBeaming = ##f" <<
      endl;

  if (staff->getStaffVoicesMap ().size () > 1) {
    fOstream << idtr;
    switch (voice->getVoiceNumber ()) {
      case 1:
        fOstream << "\\voiceOne ";
        break;
      case 2:
        fOstream << "\\voiceTwo ";
        break;
      case 3:
        fOstream << "\\voiceThree ";
        break;
      case 4:
        fOstream << "\\voiceFour ";
        break;
      default:
        {}
    } // switch
    fOstream <<
      endl;
  }

  fOstream <<
    idtr << "\\" << voice->getVoiceName () << endl;

  idtr--;
  
  fOstream <<
    idtr << "}" <<
    endl;

 // JMI \context Voice = "PartPOneVoiceOne" { \PartPOneVoiceOne }
 
}
void lpsr2LilyPondVisitor::visitEnd (S_lpsrUseVoiceCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrUseVoiceCommand" << endl;
}
  
//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrNewLyricsBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrNewLyricsBlock" << endl;

//  if (fOngoingNonEmptyLyrics) {
  if (true || fOngoingNonEmptyLyrics) { // JMI
    fOstream <<
      idtr <<
        "\\new Lyrics" <<
      endl;

    idtr++;
    
    fOstream <<
      idtr << "\\lyricsto" << " " <<
        "\""  << elt->getVoice ()->getVoiceName () << "\""  <<
    endl <<
    idtr << "\\" << elt->getLyrics ()->getLyricsName () <<
    endl;
  }
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrNewLyricsBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrNewLyricsBlock" << endl;

//  if (fOngoingNonEmptyLyrics) {
  if (true || fOngoingNonEmptyLyrics) { // JMI
    idtr--;
  }
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrVariableUseCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrVariableUseCommand" << endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrVariableUseCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrVariableUseCommand" << endl;

  idtr--;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrContext& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrContext" << endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrContext& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrContext" << endl;

  idtr--;
}
  
//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrBarCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrBarCommand" << endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrBarCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrBarCommand" << endl;

  idtr--;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrScore& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrScore" << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrScore& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrScore" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrPartgroup& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrPartgroup" << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrPartgroup& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrPartgroup" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrPart& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrPart" << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrPart& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrPart" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrStaff& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrStaff" << endl;
  
  fOnGoingStaff = true;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrStaff& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrStaff" << endl;

  fOnGoingStaff = false;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrVoice& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrVoice" << endl;

  fOstream << idtr <<
    elt->getVoiceName () << " = ";

  if (! fLpsrOptions->fGenerateAbsoluteOctaves)
    fOstream << "\\relative ";
    
  fOstream <<
    "{" <<
    endl;

  idtr++;

  fRelativeOctaveReference = 0;

//  JMI fCurrentMsrVoiceNotesCounter = 0;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrVoice& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrVoice" << endl;
  
  fOstream << idtr <<
    "}" <<
    endl <<
    endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrUpbeat& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrUpbeat" << endl;

  int upbeatDivisions =
    elt->getUpbeatDivisions ();
    
  string upbeatDivisionsAsString =
    elt->getUpbeatDivisionsAsString ();

  fOstream << idtr <<
    "\\partial" " " <<
    upbeatDivisionsAsString <<
    " % (" << upbeatDivisions << ")" <<
  endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrUpbeat& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrUpbeat" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrVoicechunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrVoicechunk" << endl;

  if (fLpsrOptions->fGenerateComments) {
    fOstream << idtr <<
      "{" <<
      setw(30) << "% start of msrVoicechunk" <<
      endl;

    idtr++;
  }
  else {
    fOstream <<
      idtr << "{" << // JMI
      endl;
  }

  fVoicechunkNotesAndChordsCountersStack.push (0);
}

void lpsr2LilyPondVisitor::visitEnd (S_msrVoicechunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrVoicechunk" << endl;

  if (fLpsrOptions->fGenerateComments) {
    idtr--;
    
    fOstream <<
      endl <<
      idtr <<
      "}" <<
      setw(30) << "% end of msrVoicechunk" <<
      endl;
  }
  else {
    fOstream <<
      endl <<
      idtr <<
      "}" << // JMI
      endl;
  }

  fVoicechunkNotesAndChordsCountersStack.pop ();
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrLyrics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrLyrics" << endl;

  fOngoingNonEmptyLyrics =
    elt->getLyricsTextPresent ();

//  if (fOngoingNonEmptyLyrics) {
  if (true || fOngoingNonEmptyLyrics) { // JMI
    fOstream << idtr <<
      elt->getLyricsName () << " = " << "\\lyricmode" << " {" <<
      endl;
      
    idtr++;
    
    fOstream << idtr;
    
    fLyricschunksCounter = 0;
  }
}

void lpsr2LilyPondVisitor::visitEnd (S_msrLyrics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrLyrics" << endl;

//  if (fOngoingNonEmptyLyrics) {
  if (true || fOngoingNonEmptyLyrics) { // JMI
    idtr--;
  
    fOstream <<
      idtr << "}" <<
      endl <<
      endl;
  }

  fOngoingNonEmptyLyrics = false;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrLyricschunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrLyricschunk" << endl;

//  if (fOngoingNonEmptyLyrics) {
  if (true || fOngoingNonEmptyLyrics) { // JMI
    if (++fLyricschunksCounter > 10) {
      fOstream <<
        endl <<
        idtr;
      fLyricschunksCounter = 1;
    }
    
    switch (elt->getLyricschunkKind ()) {
      case msrLyricschunk::kSingleChunk:
        fOstream <<
          quoteStringIfNonAlpha (elt->getChunkText ()) << " ";
        break;
        
      case msrLyricschunk::kBeginChunk:
        fOstream <<
          quoteStringIfNonAlpha (elt->getChunkText ()) << " ";
        break;
        
      case msrLyricschunk::kMiddleChunk:
        fOstream <<
          "-- " <<
          quoteStringIfNonAlpha (elt->getChunkText ()) << " ";
        break;
        
      case msrLyricschunk::kEndChunk:
        fOstream <<
          "-- " <<
          quoteStringIfNonAlpha (elt->getChunkText ()) << " ";
        break;
        
      case msrLyricschunk::kSkipChunk:
        fOstream <<
          "\\skip" <<
            elt->getLyricschunkNote ()->noteDivisionsAsMSRString () <<
            " ";
        break;
        
      case msrLyricschunk::kSlurChunk:
        fOstream <<
          "%{ slur " << "\"" << elt->getChunkText () << "\"" << " %}" <<
          endl <<
          idtr;
        break;

      case msrLyricschunk::kTiedChunk:
        fOstream <<
          "%{ ~ " << "\"" << elt->getChunkText () << "\"" << " %}" <<
          endl <<
          idtr;
        break;
        
      case msrLyricschunk::kSlurBeyondEndChunk:
// JMI       fOstream <<
 //         "__ " << " ";
        break;

      case msrLyricschunk::kBarcheckChunk:
        fOstream <<
   // JMI       "%{ | % " << elt->getChunkText () << " %}" <<
          "| %{ " << elt->getChunkText () << " %}" <<
          endl <<
          idtr;
        break;
  
      case msrLyricschunk::kBreakChunk:
        fOstream <<
          "%{ break " << "\"" << elt->getChunkText () << "\"" << " %}" <<
          endl <<
          idtr;
        break;
  
      case msrLyricschunk::k_NoChunk: // JMI
        break;
    } // switch
  }
}

void lpsr2LilyPondVisitor::visitEnd (S_msrLyricschunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrLyricschunk" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrClef& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrClef" << endl;

  fOstream << idtr <<
    "\\clef" << " \"";

  if ( elt->getSign () == "G") {
    
    if ( elt->getLine () == 2 )
      fOstream << "treble"; 
    else { // unknown G clef line !!
      cerr << 
        "warning: unknown G clef line \"" << elt->getLine () << "\"" <<
        endl;
      return; 
    }
  }
    
  else
  if ( elt->getSign () == "F") {
    
    if ( elt->getLine () == 4 )
      fOstream << "bass"; 
    else { // unknown F clef line !!
      cerr << 
        "warning: unknown F clef line \"" << elt->getLine () << "\"" <<
        endl;
      return; 
    }
  }
    
  else
  if ( elt->getSign () == "C") {
    
    if ( elt->getLine () == 4 )
      fOstream << "tenor"; 
    else if ( elt->getLine () == 3 )
      fOstream << "alto"; 
    else { // unknown C clef line !!
      cerr << 
        "warning: unknown C clef line \"" <<
        elt->getLine () << 
        "\"" <<
        endl;
      return; 
    }
  }

  else
  if ( elt->getSign () == "percussion") {
    fOstream << "perc";
  }

  else
  if ( elt->getSign () == "TAB") {
    fOstream << "TAB";
  }

  else
  if ( elt->getSign () == "none") {
    fOstream << "none";
  }
    
  else { // unknown clef sign !!
    cerr << 
      "warning: unknown clef sign \"" <<
       elt->getSign () << 
       "\"" <<
      endl;
    return; 
  }
        
  if (elt->getOctaveChange () == 1)
    fOstream << "^8";
  else
  if (elt->getOctaveChange () == -1)
    fOstream << "_8";

  fOstream <<
    "\"" <<
    endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrClef& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrClef" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrKey& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrKey" << endl;

  fOstream << idtr <<
    "\\key " << elt->getTonic () << " ";

  switch (elt->getKeyMode ()) {
    case msrKey::kMajor:
      fOstream << "\\major";
      break;
      
    case msrKey::kMinor:
      fOstream << "\\minor";
      break;
  } // switch
  
  fOstream << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrKey& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrKey" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrTime& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrTime" << endl;

  if (fLpsrOptions->fGenerateNumericalTime)
    fOstream << "\\numericTimeSignature ";
    
  fOstream << idtr <<
    "\\time" << " " <<
    elt->getBeatsNumber () <<
    "/" <<
    elt->getBeatsValue () <<
    endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrTime& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrTime" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrWords& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrWords" << endl;

  msrWords::msrWordsPlacementKind
    wordsPlacementKind =
      elt->getWordsPlacementKind ();

  string wordsContents =
    elt->getWordsContents ();

  fOstream << idtr;
  switch (wordsPlacementKind) {
    case msrWords::kAbove:
      fOstream << "<>^"; // JMI
      break;
    case msrWords::kBelow:
      fOstream << "<>_";
      break;
  } // switch

  fOstream <<
    "\\markup" << " { " << wordsContents << " } " <<
    endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrWords& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrWords" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrTempo& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrTempo" << endl;

  string tempoIndication = elt->getTempoIndication ();
  
  int tempoUnit = elt->getTempoUnit ();
  int perMinute = elt->getPerMinute ();

  fOstream << idtr <<
    "\\tempo";

  if (tempoIndication.size ())
    fOstream <<
      " \"" << tempoIndication << "\"";

  if (tempoUnit)
    fOstream <<
      " " << tempoUnit << " = " << perMinute;

  fOstream << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrTempo& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrTempo" << endl;
}

//________________________________________________________________________
/*
void lpsr2LilyPondVisitor::visitStart (S_msrDuration& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrDuration" << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrDuration& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrDuration" << endl;
}
*/

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrArticulation& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrArticulation" << endl;

  switch (elt->getArticulationKind ()) {
    case msrArticulation::kStaccato:
      fOstream << "-.";
      break;
    case msrArticulation::kStaccatissimo:
      fOstream << "-^";
      break;
    case msrArticulation::kFermata:
      fOstream << "\\fermata";
      break;
    case msrArticulation::kTrill:
      fOstream << "\\trill";
      break;
  } // switch
  fOstream << " ";
}

void lpsr2LilyPondVisitor::visitEnd (S_msrArticulation& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrArticulation" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrDynamics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrDynamics" << endl;
      
  fOstream << elt->dynamicsKindAsLilypondString () << " ";
}

void lpsr2LilyPondVisitor::visitEnd (S_msrDynamics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrDynamics" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrWedge& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrWedge" << endl;

  switch (elt->getWedgeKind ()) {
    case msrWedge::kCrescendoWedge:
      fOstream << "\\< ";
      break;
    case msrWedge::kDecrescendoWedge:
      fOstream << "\\> ";
      break;
    case msrWedge::kStopWedge:
      fOstream << "\\! ";
      break;
  } // switch
}

void lpsr2LilyPondVisitor::visitEnd (S_msrWedge& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrWedge" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrNote& elt)
{
//  if (true || fMsrOptions->fDebug) { // JMI
  if (fMsrOptions->fDebug) {
    fOstream << idtr <<
      "% --> Start visiting ";
      
    switch (elt->getNoteKind ()) {
      
      case msrNote::kStandaloneNote:
        if (++fSequentialMusicElementsCounter > 10) {
          fOstream <<
            endl <<
            idtr;
          fSequentialMusicElementsCounter = 1;
        }

        fOstream << "standalone";
        break;
        
      case msrNote::kRestNote:
        if (++fSequentialMusicElementsCounter > 10) {
          fOstream <<
            endl <<
            idtr;
          fSequentialMusicElementsCounter = 1;
        }
        
        fOstream << "rest";
        break;
        
      case msrNote::kChordMemberNote:
        fOstream << "chord member";
        break;
        
      case msrNote::kTupletMemberNote:
        fOstream << "tuplet member";
        break;
    } // switch
    fOstream << " msrNote" << endl;
  }

  // indent before the fist note of the msrVoicechunk if needed
  if (++ fVoicechunkNotesAndChordsCountersStack.top () == 1)
    fOstream << idtr;
  
  switch (elt->getNoteKind ()) {
    
    case msrNote::kStandaloneNote:
      if (++fSequentialMusicElementsCounter > 10) {
        fOstream <<
          endl <<
          idtr;
        fSequentialMusicElementsCounter = 1;
      }

      // print the note name
      fOstream <<
        noteMsrPitchAsLilyPondString (elt);
      
      // print the note duration
      fOstream <<
        elt->noteDivisionsAsMSRString ();
      
      // print the tie if any
      if (
        elt->getNoteTieKind ()
          ==
        msrMusicXMLNoteData::kStartTie) {
          fOstream << " ~";
      }

      // this note is the new relative octave reference
      fRelativeOctaveReference = elt;
      break;
      
    case msrNote::kRestNote:
      if (++fSequentialMusicElementsCounter > 10) {
        fOstream <<
          endl <<
          idtr;
        fSequentialMusicElementsCounter = 1;
      }
      
      // print the rest name
      fOstream <<
        "r";
      
      // print the note duration
      fOstream <<
        elt->noteDivisionsAsMSRString ();

      // a rest is no relative octave reference,
      // the preceding one is kept
      break;
      
    case msrNote::kChordMemberNote:
      // print the note name
      fOstream <<
        noteMsrPitchAsLilyPondString (elt);
      
      // don't print the note duration,
      // it will be printed for the chord itself

      // inside chords, a note is relative to the preceding one
      fRelativeOctaveReference = elt;
      break;
      
    case msrNote::kTupletMemberNote:
      if (++fSequentialMusicElementsCounter > 10) {
        fOstream <<
          endl <<
          idtr;
        fSequentialMusicElementsCounter = 1;
      }
      
      // print the note name
      if (elt->getNoteIsARest ())
        fOstream <<
          "r";
      else
        fOstream <<
          noteMsrPitchAsLilyPondString (elt);
      
      // print the note duration
      fOstream <<
        elt->noteDivisionsAsMSRString ();

      // print the tie if any
      if (
        elt->getNoteTieKind ()
          ==
        msrMusicXMLNoteData::kStartTie) {
          fOstream << " ~";
      }

      // a rest is no relative octave reference,
      if (! elt->getNoteIsARest ())
        // this note is the new relative octave reference
        fRelativeOctaveReference = elt;
      break;
  } // switch

  fOstream << " ";
}

void lpsr2LilyPondVisitor::visitEnd (S_msrNote& elt)
{
  if (fMsrOptions->fDebug) {
    fOstream << idtr <<
      "% --> Start visiting ";
    switch (elt->getNoteKind ()) {
      case msrNote::kStandaloneNote:
        fOstream << "standalone";
        break;
      case msrNote::kRestNote:
        fOstream << "rest";
        break;
      case msrNote::kChordMemberNote:
        fOstream << "chord member";
        break;
      case msrNote::kTupletMemberNote:
        fOstream << "tuplet member";
        break;
    } // switch
    fOstream << " msrNote" << endl;
  }

  switch (elt->getNoteKind ()) {
    case msrNote::kStandaloneNote:
      break;
    case msrNote::kRestNote:
      break;
    case msrNote::kChordMemberNote:
      break;
    case msrNote::kTupletMemberNote:
      break;
  } // switch

  switch (elt->getNoteSlurKind ()) {
    case msrSlur::kStartSlur:
      fOstream << "( ";
      break;
    case msrSlur::kContinueSlur:
      break;
    case msrSlur::kStopSlur:
      fOstream << ") ";
      break;
    case msrSlur::k_NoSlur:
      break;
  } // switch  
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrBeam& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrBeam" << endl;

  // LilyPond will take care of multiple beams automatically,
  // so we need only generate code for the first number (level)
  switch (elt->getBeamKind ()) {
    
    case msrBeam::kBeginBeam:
      if (elt->getBeamNumber () == 1)
        fOstream << "[ ";
      break;
      
    case msrBeam::kContinueBeam:
      break;
      
    case msrBeam::kEndBeam:
      if (elt->getBeamNumber () == 1)
      fOstream << "] ";
      break;
      
    case msrBeam::kForwardHookBeam:
      break;
      
    case msrBeam::kBackwardHookBeam:
      break;
      
    case msrBeam::k_NoBeam:
      break;
  } // switch      
}

void lpsr2LilyPondVisitor::visitEnd (S_msrBeam& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrBeam" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrChord& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrChord" << endl;

  // indent before the fist chord of the msrVoicechunk if needed
  if (++ fVoicechunkNotesAndChordsCountersStack.top () == 1)
    fOstream << idtr;

  // don't take the chord into account for line breaking
  /* JMI
  if (++fSequentialMusicElementsCounter > 10) {
    fOstream <<
      endl <<
      idtr;
    fSequentialMusicElementsCounter = 1;
  }
  */
  
  fOstream << "<";
}

void lpsr2LilyPondVisitor::visitEnd (S_msrChord& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrChord" << endl;

  fOstream <<
    ">" <<
    elt->chordDivisionsAsMSRString () << " ";

  // print the tie if any
  if (
    elt->getChordTieKind ()
      ==
    msrMusicXMLNoteData::kStartTie) {
      fOstream << "~ ";
  }

  // if the preceding item is a chord, the first note of the chord
  // is used as the reference point for the octave placement
  // of a following note or chord
  fRelativeOctaveReference = elt->getChordNotes () [0];
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrTuplet& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrTuplet" << endl;

  fOstream <<
    "\\tuplet " <<
    elt->getActualNotes () <<
    "/" <<
    elt->getNormalNotes() << " { ";
}

void lpsr2LilyPondVisitor::visitEnd (S_msrTuplet& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrTuplet" << endl;

  fOstream << " } ";
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrSlur& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrSlur" << endl;
  
  switch (elt->getSlurKind ()) {
    case msrSlur::kStartSlur:
      fOstream << "( ";
      break;
    case msrSlur::kContinueSlur:
      break;
    case msrSlur::kStopSlur:
      fOstream << ") ";
      break;
    default:
      fOstream << "Slur" << elt->getSlurKind () << "###";
  } // switch
}

void lpsr2LilyPondVisitor::visitEnd (S_msrSlur& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrSlur" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrBarline& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrBarline" << endl;

  switch (elt->getBarlineCategory ()) {
    
    case msrBarline::kStandaloneBar:
      fOstream <<
        endl <<
        idtr;
      
      switch (elt->getStyle ()) {
        case msrBarline::k_NoStyle:
          break;
        case msrBarline::kRegular:
          fOstream << "\\bar \"|\"";
          break;
        case msrBarline::kDotted:
          fOstream << "\\bar \";\"";
          break;
        case msrBarline::kDashed:
          fOstream << "\\bar \"!\"";
          break;
        case msrBarline::kHeavy:
          fOstream << "\\bar \".\"";
          break;
        case msrBarline::kLightLight:
          fOstream << "\\bar \"||\"";
          break;
        case msrBarline::kLightHeavy:
          fOstream << "\\bar \"|.\"";
          break;
        case msrBarline::kHeavyLight:
          fOstream << "\\bar \".|\"";
          break;
        case msrBarline::kHeavyHeavy:
          fOstream << "\\bar \"..\"";
          break;
        case msrBarline::kTick:
          fOstream << "\\bar \"'\"";
          break;
        case msrBarline::kShort:
          fOstream << "\\bar \"|short???\"";
          break;
      } // switch

      fOstream <<
        endl <<
        endl << idtr;
      break;

    case msrBarline::kRepeatStart:
    case msrBarline::kRepeatEnd:
    case msrBarline::kEndingStart:
    case msrBarline::kHookedEndingEnd:
    case msrBarline::kHooklessEndingEnd:
      // should not occur, since
      // LilyPond will take care of the repeat display
      break;

  } // switch
}

void lpsr2LilyPondVisitor::visitEnd (S_msrBarline& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrBarline" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrBarCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrBarCheck" << endl;
      
  int nextBarNumber =
    elt->getNextBarNumber ();

  // don't generate a bar check before the end of measure 1
  if (nextBarNumber > 1)
    fOstream <<
      " | % " << nextBarNumber <<
      endl <<
      idtr;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrBarCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrBarCheck" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrBarnumberCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrBarnumberCheck" << endl;

  fOstream <<
    "\\barNumberCheck #" << elt->getNextBarNumber () <<
    endl << idtr;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrBarnumberCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrBarnumberCheck" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrBreak& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrBreak" << endl;

  fOstream <<
    "\\myBreak | % " << elt->getNextBarNumber () <<
    endl <<
    endl <<
    idtr;

  fSequentialMusicElementsCounter = 1;
  fLyricschunksCounter = 1;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrBreak& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrBreak" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrRepeat& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrRepeat" << endl;

  int voltaNumber =
    elt->getRepeatEndings ().size();
  
  if (voltaNumber == 0)
    voltaNumber = 2; // implicitly

  stringstream s;
  s <<
    "\\repeat volta " << voltaNumber << " {";
  
  fOstream << idtr <<
    setw(30) << s.str() << "% start of repeat" <<
    endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrRepeat& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrRepeat" << endl;

  if (elt->getRepeatEndings ().size() == 0) {
    // the end of the repeat has not been generated yet

    idtr --;
    
    fOstream << idtr <<
      setw(30) << "}" << "% end of repeat" <<
      endl << endl;
  }
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrRepeatending& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrRepeatending" << endl;

  if (elt->getRepeatendingNumber () == 1) {
    
    idtr--;
    
    // first repeat ending is in charge of
    // outputting the end of the repeat
    fOstream << idtr <<
      setw(30) << "}" << "% end of repeat" <<
      endl << endl;

    // first repeat ending is in charge of
    // outputting the start of the alternative
    fOstream << idtr <<
      setw(30) << "\\alternative" " " "{" <<
      "% start of alternative" <<
      endl;
    
    idtr++;
  }
}

void lpsr2LilyPondVisitor::visitEnd (S_msrRepeatending& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrRepeatending" << endl;

  if (
  // JMI warning: comparison between signed and unsigned integer expressions [-Wsign-compare]
    elt->getRepeatendingNumber ()
      ==
    (int) elt->getRepeatendingRepeatUplink ()->getRepeatEndings ().size()) {
      
    idtr--;
    
    // last repeat ending is in charge of
    // outputting the end of the alternative
    fOstream <<
      endl <<
      idtr <<
      setw(30) << "}" << "% end of alternative" <<
      endl << endl;
  }
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrComment& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrComment" << endl;

  fOstream << idtr <<
    "% " << elt->getContents () <<
    endl;
    
  if (elt->getGapKind () == lpsrComment::kGapAfterwards)
    fOstream << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrComment& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrComment" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrMidi& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrMidi" << endl;

  fOstream << idtr <<
    "\\midi" << " {" <<
    endl;
  
  idtr++;
  
  fOstream << idtr <<
    "% to be completed" <<
    endl;
  
  idtr--;
  
  fOstream << idtr <<
    "}" <<
    endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrMidi& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrMidi" << endl;
}


} // namespace
