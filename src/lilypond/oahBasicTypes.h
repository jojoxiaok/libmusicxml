/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___optionsBasicTypes___
#define ___optionsBasicTypes___

#include "list"
#include "vector"
#include "map"
#include "set"

#include "smartpointer.h"
#include "rational.h"

#include "utilities.h"

#include "msrBasicTypes.h"
#include "lpsrBasicTypes.h"

#include "setTraceOahIfDesired.h"


using namespace std;

namespace MusicXML2
{

// layout settings
//______________________________________________________________________________
const int K_OPTIONS_ELEMENTS_INDENTER_OFFSET = 3;
  // indent a bit more for readability

const int K_OPTIONS_FIELD_WIDTH = 40;

// PRE-declarations for class dependencies
//______________________________________________________________________________
class oahOption;
typedef SMARTP<oahOption> S_oahOption;

class oahSubGroup;
typedef SMARTP<oahSubGroup> S_oahSubGroup;

class oahGroup;
typedef SMARTP<oahGroup> S_oahGroup;

class EXP oahHandler;
typedef SMARTP<oahHandler> S_oahHandler;

// data types
// ------------------------------------------------------

enum oahOptionVisibilityKind {
  kElementVisibilityAlways,
  kElementVisibilityHiddenByDefault };

string optionVisibilityKindAsString (
  oahOptionVisibilityKind optionVisibilityKind);

//______________________________________________________________________________
class oahOption : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOption> create (
      string                  shortName,
      string                  longName,
      string                  description,
      oahOptionVisibilityKind optionVisibilityKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahOption (
      string                  shortName,
      string                  longName,
      string                  description,
      oahOptionVisibilityKind optionVisibilityKind);

    virtual ~oahOption ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink
    void                  setHandlerUpLink (
                            S_oahHandler handler)
                              { fHandlerUpLink = handler; }

    S_oahHandler          getHandlerUpLink () const
                              { return fHandlerUpLink; }

    string                getShortName () const
                              { return fShortName; }

    string                getLongName () const
                              { return fLongName; }

    string                getDescription () const
                              { return fDescription; }

    oahOptionVisibilityKind
                          getOptionVisibilityKind () const
                              { return fOptionVisibilityKind; }

    void                  setIsHidden ()
                              { fIsHidden = true; }

    bool                  getIsHidden () const
                              { return fIsHidden; }

    // services
    // ------------------------------------------------------

    string                fetchNames () const;
    string                fetchNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;

    string                fetchNamesBetweenParentheses () const;
    string                fetchNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    virtual int           fetchVariableNameLength () const
                              { return 0; }

/* JMI ???
    string                operator () () const
                              { return fDescription; }
*/

    S_oahOption           fetchOptionByName (
                            string name);

    virtual void          handleOptionName (
                            string   optionName,
                            ostream& os);

    // print
    // ------------------------------------------------------

    virtual string        asShortNamedOptionString () const;
    virtual string        asLongNamedOptionString () const;

    string                asString () const;

    virtual void          printOptionHeader (ostream& os) const; // virtual ??? JMI

    virtual void          printOptionEssentials (
                            ostream& os,
                            int      fieldWidth) const;

    virtual void          print (ostream& os) const;

    virtual void          printHelp (ostream& os);

  protected:

    // fields
    // ------------------------------------------------------

    // uplink
    S_oahHandler          fHandlerUpLink;

    string                fShortName;
    string                fLongName;
    string                fDescription;

    oahOptionVisibilityKind
                          fOptionVisibilityKind;

    bool                  fIsHidden;
};
typedef SMARTP<oahOption> S_oahOption;
EXP ostream& operator<< (ostream& os, const S_oahOption& elt);

//______________________________________________________________________________
class oahAtom : public oahOption
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~oahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setSubGroupUpLink (
                            S_oahSubGroup subGroup);

    S_oahSubGroup         getSubGroupUpLink () const
                              { return fSubGroupUpLink; }

    // services
    // ------------------------------------------------------

    void                  registerAtomInHandler (
                            S_oahHandler handler);

    void                  handleOptionName (
                            string   optionName,
                            ostream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    virtual void          printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    S_oahSubGroup         fSubGroupUpLink;
};
typedef SMARTP<oahAtom> S_oahAtom;
EXP ostream& operator<< (ostream& os, const S_oahAtom& elt);

//______________________________________________________________________________
class oahOptionsUsageAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOptionsUsageAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahOptionsUsageAtom (
      string shortName,
      string longName,
      string description);

    virtual ~oahOptionsUsageAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsUsage (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<oahOptionsUsageAtom> S_oahOptionsUsageAtom;
EXP ostream& operator<< (ostream& os, const S_oahOptionsUsageAtom& elt);

//______________________________________________________________________________
class oahOptionsSummaryAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOptionsSummaryAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahOptionsSummaryAtom (
      string shortName,
      string longName,
      string description);

    virtual ~oahOptionsSummaryAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsSummary (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<oahOptionsSummaryAtom> S_oahOptionsSummaryAtom;
EXP ostream& operator<< (ostream& os, const S_oahOptionsSummaryAtom& elt);

//______________________________________________________________________________
class oahAtomWithVariableName : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahAtomWithVariableName> create (
      string shortName,
      string longName,
      string description,
      string variableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahAtomWithVariableName (
      string shortName,
      string longName,
      string description,
      string variableName);

    virtual ~oahAtomWithVariableName ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getVariableName () const
                              { return fVariableName; }

    // services
    // ------------------------------------------------------

    virtual int           fetchVariableNameLength () const
                              { return fVariableName.size (); }

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    string                fVariableName;
};
typedef SMARTP<oahAtomWithVariableName> S_oahAtomWithVariableName;
EXP ostream& operator<< (ostream& os, const S_oahAtomWithVariableName& elt);

//______________________________________________________________________________
class oahBooleanAtom : public oahAtomWithVariableName
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahBooleanAtom> create (
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahBooleanAtom (
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable);

    virtual ~oahBooleanAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setBooleanVariable (
                            bool value)
                              { fBooleanVariable = value; }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    bool&                 fBooleanVariable;
};
typedef SMARTP<oahBooleanAtom> S_oahBooleanAtom;
EXP ostream& operator<< (ostream& os, const S_oahBooleanAtom& elt);

//______________________________________________________________________________
class oahTwoBooleansAtom : public oahAtomWithVariableName
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahTwoBooleansAtom> create (
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable,
      bool&  booleanSecondaryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahTwoBooleansAtom (
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable,
      bool&  booleanSecondaryVariable);

    virtual ~oahTwoBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setTwoBooleansVariables (
                            bool value)
                              {
                                fBooleanVariable =
                                  value;
                                fBooleanSecondaryVariable =
                                  value;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    bool&                 fBooleanVariable;

    bool&                 fBooleanSecondaryVariable;
};
typedef SMARTP<oahTwoBooleansAtom> S_oahTwoBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahTwoBooleansAtom& elt);

//______________________________________________________________________________
class oahThreeBooleansAtom : public oahAtomWithVariableName
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahThreeBooleansAtom> create (
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable,
      bool&  booleanSecondaryVariable,
      bool&  booleanTertiaryVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahThreeBooleansAtom (
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable,
      bool&  booleanSecondaryVariable,
      bool&  booleanTertiaryVariable);

    virtual ~oahThreeBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setThreeBooleansVariables (
                            bool value)
                              {
                                fBooleanVariable =
                                  value;
                                fBooleanSecondaryVariable =
                                  value;
                                fBooleanTertiaryVariable =
                                  value;
                              }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    bool&                 fBooleanVariable;

    bool&                 fBooleanSecondaryVariable;
    bool&                 fBooleanTertiaryVariable;
};
typedef SMARTP<oahThreeBooleansAtom> S_oahThreeBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahThreeBooleansAtom& elt);

//______________________________________________________________________________
class oahCombinedBooleansAtom : public oahAtomWithVariableName
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahCombinedBooleansAtom> create (
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahCombinedBooleansAtom (
      string shortName,
      string longName,
      string description,
      string variableName,
      bool&  booleanVariable);

    virtual ~oahCombinedBooleansAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<S_oahBooleanAtom>&
                          getBooleanAtomsList ()
                              { return fBooleanAtomsList; }

    // services
    // ------------------------------------------------------

    void                  addBooleanAtom (
                            S_oahBooleanAtom booleanAtom);

    void                  addBooleanAtomByName (
                            string name);

    void                  setCombinedBooleanVariables (
                            bool value);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os);

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    bool&                 fBooleanVariable;

    list<S_oahBooleanAtom>
                          fBooleanAtomsList;
};
typedef SMARTP<oahCombinedBooleansAtom> S_oahCombinedBooleansAtom;
EXP ostream& operator<< (ostream& os, const S_oahCombinedBooleansAtom& elt);

//______________________________________________________________________________
class oahValuedAtom : public oahAtomWithVariableName
{
  public:

    // data types
    // ------------------------------------------------------

    enum oahValuedAtomKind { // JMI ???
      kAtomHasNoArgument,
      kAtomHasARequiredArgument,
      kAtomHasAnOptionsArgument };

    static string oahAtomKindAsString (
      oahValuedAtomKind oahAtomKind);

    // creation
    // ------------------------------------------------------

    static SMARTP<oahValuedAtom> create (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahValuedAtom (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName);

    virtual ~oahValuedAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    oahValuedAtomKind
                          getValuedAtomKind () const
                              { return fValuedAtomKind; }

    string                getValueSpecification () const
                              { return fValueSpecification; }

    void                  setValueIsOptional ()
                              { fValueIsOptional = true; }

    bool                  getValueIsOptional () const
                              { return fValueIsOptional; }

    // services
    // ------------------------------------------------------

    virtual void          handleValue (
                            string   theString,
                            ostream& os) = 0;

    virtual void          handleDefaultValue ();
                            // used only fValueIsOptional is true

    // print
    // ------------------------------------------------------

    virtual void          printValuedAtomEssentials (
                            ostream& os,
                            int      fieldWidth) const;

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os);

    virtual void          printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    oahValuedAtomKind     fValuedAtomKind; // JMI

    string                fValueSpecification;

    bool                  fValueIsOptional; // not yet supported JMI
};
typedef SMARTP<oahValuedAtom> S_oahValuedAtom;
EXP ostream& operator<< (ostream& os, const S_oahValuedAtom& elt);

//______________________________________________________________________________
class oahIntegerAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahIntegerAtom> create (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      int&   integerVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahIntegerAtom (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      int&   integerVariable);

  protected:


    virtual ~oahIntegerAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setIntegerVariable (
                            int value)
                              { fIntegerVariable = value; }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  private:

    // fields
    // ------------------------------------------------------

    int&                  fIntegerVariable;
};
typedef SMARTP<oahIntegerAtom> S_oahIntegerAtom;
EXP ostream& operator<< (ostream& os, const S_oahIntegerAtom& elt);

//______________________________________________________________________________
class oahFloatAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahFloatAtom> create (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      float& floatVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahFloatAtom (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      float& floatVariable);

    virtual ~oahFloatAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setFloatVariable (
                            float value)
                              { fFloatVariable = value; }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    float&                fFloatVariable;
};
typedef SMARTP<oahFloatAtom> S_oahFloatAtom;
EXP ostream& operator<< (ostream& os, const S_oahFloatAtom& elt);

//______________________________________________________________________________
class oahStringAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringAtom> create (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahStringAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable);

    virtual ~oahStringAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (
                            string value)
                              { fStringVariable = value; }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    string&               fStringVariable;
};
typedef SMARTP<oahStringAtom> S_oahStringAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringAtom& elt);

//______________________________________________________________________________
class oahStringWithDefaultValueAtom : public oahStringAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahStringWithDefaultValueAtom> create (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable,
      string  defaultStringValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahStringWithDefaultValueAtom (
      string  shortName,
      string  longName,
      string  description,
      string  valueSpecification,
      string  variableName,
      string& stringVariable,
      string  defaultStringValue);

    virtual ~oahStringWithDefaultValueAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setStringVariable (
                            string value)
                              { oahStringAtom::setStringVariable (value); }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  protected:

    // fields
    // ------------------------------------------------------

    string                fDefaultStringValue;
};
typedef SMARTP<oahStringWithDefaultValueAtom> S_oahStringWithDefaultValueAtom;
EXP ostream& operator<< (ostream& os, const S_oahStringWithDefaultValueAtom& elt);

//______________________________________________________________________________
class oahRationalAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahRationalAtom> create (
      string    shortName,
      string    longName,
      string    description,
      string    valueSpecification,
      string    variableName,
      rational& rationalVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahRationalAtom (
      string    shortName,
      string    longName,
      string    description,
      string    valueSpecification,
      string    variableName,
      rational& rationalVariable);

    virtual ~oahRationalAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setRationalVariable (
                            rational value)
                              { fRationalVariable = value;  }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    rational&             fRationalVariable;
};
typedef SMARTP<oahRationalAtom> S_oahRationalAtom;
EXP ostream& operator<< (ostream& os, const S_oahRationalAtom& elt);

//______________________________________________________________________________
class oahIntegersSetAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahIntegersSetAtom> create (
      string    shortName,
      string    longName,
      string    description,
      string    valueSpecification,
      string    variableName,
      set<int>& integersSetVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahIntegersSetAtom (
      string    shortName,
      string    longName,
      string    description,
      string    valueSpecification,
      string    variableName,
      set<int>& integersSetVariable);

    virtual ~oahIntegersSetAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setIntegersSetVariable (
                            set<int> value)
                              { fIntegersSetVariable = value; }

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    set<int>&             fIntegersSetVariable;
};
typedef SMARTP<oahIntegersSetAtom> S_oahIntegersSetAtom;
EXP ostream& operator<< (ostream& os, const S_oahIntegersSetAtom& elt);

//______________________________________________________________________________
class oahOptionNameHelpAtom : public oahStringWithDefaultValueAtom
{
/*
  This is where OAH is introspective:
    OAH provides a way to obtain help on any option name,
    be it one to print help about a group or subgroup,
    or the name of a 'regular' atom.
  This option is reflexive, since it can provide help about itself.
*/
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahOptionNameHelpAtom> create (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      string& stringVariable,
      string defaultOptionName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahOptionNameHelpAtom (
      string shortName,
      string longName,
      string description,
      string valueSpecification,
      string variableName,
      string& stringVariable,
      string defaultOptionName);

  protected:


    virtual ~oahOptionNameHelpAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    void                  handleValue (
                            string   theString,
                            ostream& os);

    void                  handleDefaultValue ();

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asLongNamedOptionString () const;

    void                  print (ostream& os) const;

    virtual void          printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;
  private:

    // fields
    // ------------------------------------------------------

    string                fValueSpecification;
};
typedef SMARTP<oahOptionNameHelpAtom> S_oahOptionNameHelpAtom;
EXP ostream& operator<< (ostream& os, const S_oahOptionNameHelpAtom& elt);

//_______________________________________________________________________________
class oahSubGroup : public oahOption
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahSubGroup> create (
      string                  subGroupHeader,
      string                  shortName,
      string                  longName,
      string                  description,
      oahOptionVisibilityKind optionVisibilityKind,
      S_oahGroup              groupUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahSubGroup (
      string                  subGroupHeader,
      string                  shortName,
      string                  longName,
      string                  description,
      oahOptionVisibilityKind optionVisibilityKind,
      S_oahGroup              groupUpLink);

    virtual ~oahSubGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setGroupUpLink (
                            S_oahGroup oahGroup)
                              { fGroupUpLink = oahGroup; }

    S_oahGroup            getGroupUpLink () const
                              { return fGroupUpLink; }

    string                getSubGroupHeader () const
                              { return fSubGroupHeader; }

    // services
    // ------------------------------------------------------

    void                  underlineSubGroupHeader (ostream& os) const;

    void                  registerSubGroupInHandler (
                            S_oahHandler handler);

    void                  appendAtom (
                            S_oahAtom oahAtom);

    S_oahOption           fetchOptionByName (
                            string name);

    void                  handleOptionName (
                            string   optionName,
                            ostream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os);

    void                  printHelpWithHeaderWidth (
                            ostream& os,
                            int      subGroupHeaderWidth);

    void                  printSubGroupHeader (ostream& os) const;
    void                  printSubGroupHeaderWithHeaderWidth (
                            ostream& os,
                            int      subGroupHeaderWidth) const;

    void                  printSubGroupHelp (
                            ostream& os) const;

    void                  printSubGroupAndAtomHelp (
                            ostream&  os,
                            S_oahAtom targetAtom) const;

    void                  printOptionsSummary (ostream& os) const;

    void                  printSubGroupSpecificHelpOrOptionsSummary (
                            ostream&      os,
                            S_oahSubGroup subGroup) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    S_oahGroup            fGroupUpLink;

    string                fSubGroupHeader;

    list<S_oahAtom>       fAtomsList;
};
typedef SMARTP<oahSubGroup> S_oahSubGroup;
EXP ostream& operator<< (ostream& os, const S_oahSubGroup& elt);

//_______________________________________________________________________________
class oahGroup : public oahOption
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahGroup> create (
      string                  header,
      string                  shortName,
      string                  longName,
      string                  description,
      oahOptionVisibilityKind optionVisibilityKind,
      S_oahHandler            groupHandlerUpLink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahGroup (
      string                  header,
      string                  shortName,
      string                  longName,
      string                  description,
      oahOptionVisibilityKind optionVisibilityKind,
      S_oahHandler            groupHandlerUpLink);

    virtual ~oahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getGroupHeader () const
                              { return fGroupHeader; }

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkOptionsConsistency ();

  public:

    // services
    // ------------------------------------------------------

    void                  underlineGroupHeader (ostream& os) const;

    void                  registerOptionsGroupInHandler (
                            S_oahHandler handler);

    void                  appendSubGroup (
                            S_oahSubGroup subGroup);

    S_oahOption           fetchOptionByName (
                            string name);

    virtual S_oahValuedAtom
                          handleAtom (
                            ostream&  os,
                            S_oahAtom atom);

    virtual void          handleAtomValue (
                            ostream&  os,
                            S_oahAtom atom,
                            string    theString);

    void                  handleOptionName (
                            string   optionName,
                            ostream& os);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printGroupHeader (ostream& os) const;

    void                  printHelp (ostream& os);

    void                  printGroupAndSubGroupHelp (
                            ostream&      os,
                            S_oahSubGroup targetSubGroup) const;

    void                  printGroupAndSubGroupAndAtomHelp (
                            ostream&      os,
                            S_oahSubGroup targetSubGroup,
                            S_oahAtom     targetAtom) const;

    void                  printOptionsSummary (ostream& os) const;

    void                  printGroupAndSubGroupSpecificHelp (
                            ostream&      os,
                            S_oahSubGroup subGroup) const;

    void                  printOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private fields
    // ------------------------------------------------------

    string                fGroupHeader;

  protected:

    // protected fields
    // ------------------------------------------------------

    list<S_oahSubGroup>   fSubGroupsList;
};
typedef SMARTP<oahGroup> S_oahGroup;
EXP ostream& operator<< (ostream& os, const S_oahGroup& elt);

//______________________________________________________________________________
class oahPrefix;
typedef SMARTP<oahPrefix> S_oahPrefix;

class oahPrefix : public smartable
/*
An options prefix 'trace' --> 'trace-' allows:
  -trace=abc,def,gh
to be developped into :
  -trace-abc -trace-def -trace-gh
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahPrefix> create (
      string prefixName,
      string prefixErsatz,
      string prefixDescription);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahPrefix (
      string prefixName,
      string prefixErsatz,
      string prefixDescription);

    virtual ~oahPrefix ();

  public:

    // set and get
    // ------------------------------------------------------

    string                getPrefixName () const
                              { return fPrefixName; }

    string                getPrefixErsatz () const
                              { return fPrefixErsatz; }

    string                getPrefixDescription () const
                              { return fPrefixDescription; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    string                prefixNames () const;
    string                prefixNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;

    string                prefixNamesBetweenParentheses () const;
    string                prefixNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    string                operator () () const
                              { return fPrefixErsatz; }

  public:

    // print
    // ------------------------------------------------------

    virtual void          printPrefixHeader (ostream& os) const;

    virtual void          printPrefixEssentials (
                            ostream& os,
                            int      fieldWidth) const;

    virtual void          print (ostream& os) const;

    virtual void          printHelp (ostream& os);

  protected:

    // fields
    // ------------------------------------------------------

    string                fPrefixName;
    string                fPrefixErsatz;
    string                fPrefixDescription;
};
EXP ostream& operator<< (ostream& os, const S_oahPrefix& elt);

//_______________________________________________________________________________
class EXP oahHandler : public oahOption
{
  public:

    // creation
    // ------------------------------------------------------
/* JMI this a pure virtual class
    static SMARTP<oahHandler> create (
      string           handlerHeader,
      string           handlerValuesHeader,
      string           optionHoahHandlerandlerHelpShortName,
      string           handlerLongName,
      string           handlerSummaryShortName,
      string           handlerSummaryLongName,
      string           handlerPreamble,
      string           handlerDescription,
      indentedOstream& handlerLogOstream);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahHandler (
      string           HandlerHeader,
      string           handlerValuesHeader,
      string           handlerShortName,
      string           handlerLongName,
      string           handlerSummaryShortName,
      string           handlerSummaryLongName,
      string           handlerPreamble,
      string           handlerDescription,
      indentedOstream& handlerLogOstream);

    virtual ~oahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    virtual void          initializeOptionsHandler (
                            string executableName) = 0;

  public:

    // set and get
    // ------------------------------------------------------

    string                getHandlerHeader () const
                              { return fHandlerHeader; }

    string                getOptionsHandlerValuesHeader () const
                              { return fHandlerValuesHeader; }

    string                getHandlerSummaryShortName () const
                              { return fHandlerSummaryShortName; }

    string                getHandlerSummaryLongName () const
                              { return fHandlerSummaryLongName; }

    string                getHandlerPreamble () const
                              { return fHandlerPreamble; }

    ostream&              getHandlerLogOstream ()
                              { return fHandlerLogOstream; }

    string                getExecutableName () const
                              { return fHandlerExecutableName; }

    const list<S_oahOption>&
                          getHandlerOptionsList () const
                              { return fHandlerOptionsList; }

    int                   getMaximumShortNameWidth () const
                              { return fMaximumShortNameWidth; }

    int                   getMaximumLongNameWidth () const
                              { return fMaximumLongNameWidth; }

    int                   getMaximumSubGroupsHeadersSize () const
                              { return fMaximumSubGroupsHeadersSize; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendPrefixToHandler (
                            S_oahPrefix prefix);

    void                  appendGroupToHandler (
                            S_oahGroup oahGroup);

    void                  registerOptionInHandler (
                            S_oahOption option);

    void                  registerHandlerInItself ();

    S_oahPrefix           fetchPrefixFromMap (
                            string name) const;

    S_oahOption           fetchOptionFromMap (
                            string name) const;

    const vector<string>  decipherOptionsAndArguments (
                            int   argc,
                            char* argv[]);

  public:

    // print
    // ------------------------------------------------------

    string                commandLineWithShortNamesAsString () const;
    string                commandLineWithLongNamesAsString () const;

    void                  print (ostream& os) const;

    void                  printHelp (ostream& os);

    void                  printOptionsSummary (ostream& os) const;
    void                  printOptionsSummary () const
                              { printOptionsSummary (fHandlerLogOstream); }

    void                  printHandlerAndGroupAndSubGroupSpecificHelp (
                            ostream&      os,
                            S_oahSubGroup subGroup) const;

    void                  printOptionSpecificHelp (
                            ostream& os,
                            string   name) const;

    void                  printAllOahValues (
                            ostream& os) const;

    void                  setOptionsHandlerFoundAHelpOption ()
                              { fHandlerFoundAHelpOption = true; }

    bool                  getOptionsHandlerFoundAHelpOption () const
                              { return fHandlerFoundAHelpOption; }

  private:

    // private services
    // ------------------------------------------------------

    string                handlerOptionNamesBetweenParentheses () const;

    void                  registerOptionNamesInHandler (
                            string      optionShortName,
                            string      optionLongName,
                            S_oahOption option);

    void                  printKnownPrefixes () const;
    void                  printOptionsDefaultValuesInformation () const;
    void                  printKnownOptions () const;

    S_oahOption           fetchOptionByName (
                            string name);

    void                  handleOptionName (
                            string name);

    void                  handleHandlerName (
                            S_oahHandler handler,
                            string       name);

    void                  handleGroupName (
                            S_oahGroup group,
                            string     groupName);

    void                  handleSubGroupName (
                            S_oahSubGroup subGroup,
                            string        subGroupName);

    void                  handleAtomName (
                            S_oahAtom atom,
                            string    atomName);

    void                  handleOptionsUsageAtomName (
                            S_oahOptionsUsageAtom optionsUsageAtom,
                            string                atomName);

    void                  handleOptionsSummaryAtomName (
                            S_oahOptionsSummaryAtom optionsSummaryAtom,
                            string                  atomName);

    void                  handleCombinedBooleansAtomName (
                            S_oahCombinedBooleansAtom combinedBooleansAtom,
                            string                    atomName);

    void                  handleBooleanAtomName (
                            S_oahBooleanAtom booleanAtom,
                            string           atomName);

    void                  handleTwoBooleansAtomName (
                            S_oahTwoBooleansAtom twoBooleansAtom,
                            string               atomName);

    void                  handleThreeBooleansAtomName (
                            S_oahThreeBooleansAtom threeBooleansAtom,
                            string                 atomName);

    void                  handleOptionNameHelpAtomName (
                            S_oahOptionNameHelpAtom optionNameHelpAtom,
                            string                  atomName);

    void                  handleIntegerAtomName (
                            S_oahIntegerAtom integerAtom,
                            string           atomName);

    void                  handleFloatAtomName (
                            S_oahFloatAtom floatAtom,
                            string         atomName);

    void                  handleStringAtomName (
                            S_oahStringAtom stringAtom,
                            string          atomName);

    void                  handleRationalAtomName (
                            S_oahRationalAtom rationalAtom,
                            string            atomName);

    void                  handleIntegersSetAtomName (
                            S_oahIntegersSetAtom integersSetAtom,
                            string              atomName);

    void                  handleOptionValueOrArgument (
                            string theString);

    void                  checkMissingPendingValuedAtomValue (
                            string context);

    virtual void          checkOptionsAndArguments () = 0;

  protected:

    // fields
    // ------------------------------------------------------

    string                fHandlerHeader;
    string                fHandlerValuesHeader;

    string                fHandlerSummaryShortName;
    string                fHandlerSummaryLongName;

    string                fHandlerPreamble;

    map<string, S_oahPrefix>
                          fHandlerPrefixesMap;

    list<S_oahGroup>      fHandlerGroupsList;

    map<string, S_oahOption>
                          fHandlerOptionsMap;

    vector<string>        fHandlerArgumentsVector;

    string                fHandlerExecutableName;

    list<S_oahOption>     fHandlerOptionsList;

    ostream&              fHandlerLogOstream;

    // this is needed to exit if the executable is launched
    // with one or more help options,
    // i.e. options that are only used to display help to the user
    bool                  fHandlerFoundAHelpOption;

  private:

    // private services
    // ------------------------------------------------------

    int                   getMaximumVariableNameWidth () const
                              { return fMaximumVariableNameWidth; }


  private:

    // private fields
    // ------------------------------------------------------

    int                   fMaximumSubGroupsHeadersSize;

    int                   fMaximumShortNameWidth;
    int                   fMaximumLongNameWidth;

    int                   fMaximumVariableNameWidth;

    S_oahValuedAtom       fPendingValuedAtom;
};
typedef SMARTP<oahHandler> S_oahHandler;
EXP ostream& operator<< (ostream& os, const S_oahHandler& elt);


}


#endif