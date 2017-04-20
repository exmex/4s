/*	
 *		imacroscript.h - Public interface to Macro script manager for the MAX CUI
 *
 *	Macro scripts (or macros) are scripts that live in buttons and menus in the new 
 *  customizable UI.  The macro script manager keeps a directory of all known macros 
 *  and provides an API for running and editing macros and for accessing and 
 *  updating the directory.
 *
 *  The directory is used by the CUI to provide a list of available macros in the 
 *  toolbar/menu/shelf editor.  The API also provides a way for the CUI
 *  to open a macro editor to allow on-the-fly creation of macro scripts. 
 *
 *  Macros are normally entered into the directory by the MAXScript compiler as a
 *  side-effect of compiling a macro () definition.  Anyone using MAXScript can at
 *  any time eval a macro definition and thereby add CUI macro scripts.
 *  Consequently, macros can be stored in any script file and be loaded just by
 *  executing the file.  The macro definition syntax permits any number of macros per
 *  file.  
 *
 *  Most macros will be stored in files in a special CUI macro or config
 *  directory so that a user can take all his custom UI stuff with him by copying
 *  directories.  (this directory supports recursive scanning of sub-dirs,
 *  so users can organize their macros)
 *  On-the-fly macro creation in the CUI editor or by text drag-and-
 *  drop onto the shelf or by evaling a definition in the listener will generate 
 *  a file in this directory to provide permanent storage.
 *
 *			Copyright © Autodesk, Inc, 1998.  John Wainwright.
 *
 */

#ifndef _H_IMACROSCRIPT
#define _H_IMACROSCRIPT

#ifndef ScripterExport
#	ifdef BLD_MAXSCRIPT
#		define ScripterExport __declspec( dllexport )
#	else
#		define ScripterExport __declspec( dllimport )
#	endif
#endif

#include "iFnPub.h"

typedef short MacroID;

class Value;

/* ----------------  macro directory -------------------------- */

/*! \sa  Class MacroDir, Class MacroButtonData.\n\n
\par Description:
This class is available in release 3.0 and later only.\n\n
This class provides access to a single macro entry. There are methods provided
to access the macro ID, name, category, file name, tooltip, UI button text, and
the UI button icon. MacroEntries are returned from methods of <b>class
MacroDir</b>.\n\n
Macro scripts (or macros) are scripts that live in buttons and menus in the new
customizable UI. The macro script manager keeps a directory of all known macros
and provides an API for running and editing macros and for accessing and
updating the directory.\n\n
All macro scripts have a name and a category. The category is used to organize
the macros into groupings and is given to the macro script at definition time.
If you look at the macro scripts list in the UI Customize dialog, you see a
Category dropdown with things like 'Cameras', 'DragandDrop', 'LightTools',
etc., which is derived from the all categories present in the currently-defined
macroscripts. Note that the normal way to define a macroScript is through
MAXScript, the .mcr files in the UI/MacroScripts directory contain examples,
and they all have category definitions.\n\n
Note: In order to use these methods you need to <b>#include
"IMACROSCRIPT.H"</b> and link to <b>"MAXSCRPT.LIB"</b>.  */
class MacroEntry : public BaseInterfaceServer
{
public:
	// access
	/*! \remarks Returns the ID for this macro script. Note: <b>typedef short
	MacroID;</b> */
	virtual MacroID		GetID() = 0;
	/*! \remarks Returns the name for this macro script. */
	virtual MSTR&		GetName() = 0;
	/*! \remarks Returns the category for this macro script. */
	virtual MSTR&		GetCategory() = 0;
	virtual MSTR&		GetInternalCategory() = 0;
	virtual void		SetInternalCategory(const MCHAR* ic) = 0;
	/*! \remarks Returns the file name of the script source text for this
	script. */
	virtual MSTR&		GetFileName() = 0;
	/*! \remarks Sets the file name for the script source.
	\par Parameters:
	<b>MCHAR* fn</b>\n\n
	The file name to set. */
	virtual void		SetFileName(const MCHAR* fn) = 0;
	/*! \remarks Returns the offset for this macro script entry. See
	<b>SetOffset()</b> above. */
	virtual long		GetOffset() = 0;
	/*! \remarks Sets the offset for this macro script entry. There can be any
	number of macroScripts in a single source file and the offset keeps track
	of the beginning of its definition in the file.
	\par Parameters:
	<b>int o</b>\n\n
	The offset to set. */
	virtual void		SetOffset(int o) = 0;
	/*! \remarks When the macroScript is defined, only its source file and
	source offset are registered. When the user first runs it, the MAXScript
	compiler is used to compile the definition into executable code, which is
	then cached and used for any later executions and is what this method
	returns. If this returns NULL, the macro hasn't been compiled or run yet.
	Another way to run the macro is via the <b>MacroEntry::Execute()</b> and
	this causes the code to be cached as a side effect also. Normally,
	developers only ever need to use the <b>Execute()</b> method, but if they
	are using the MAXScript SDK, they can grab the code using <b>GetCode()</b>
	and work with it directly. */
	virtual Value*		GetCode() = 0;
	virtual void		SetCode(Value* code) = 0;
	/*! \remarks Returns the tooltip text for the UI button. */
	virtual MSTR&		GetToolTip() = 0;
	/*! \remarks Sets the tooltip text for the UI button.
	\par Parameters:
	<b>MCHAR* tt</b>\n\n
	The tooltip text to set. */
	virtual void		SetToolTip(const MCHAR* tt) = 0;
	/*! \remarks Returns the UI button text (for label buttons). */
	virtual MSTR&		GetButtonText() = 0;
	/*! \remarks Sets the UI button text (for label buttons).
	\par Parameters:
	<b>MCHAR* bt</b>\n\n
	The button text. */
	virtual void		SetButtonText(const MCHAR* bt) = 0;
	/*! \remarks Sets the UI button icon via a base icon file name and index
	into the specified BMP bitmap.
	\par Parameters:
	<b>MCHAR* icnf</b>\n\n
	The file name of the BMP file. See the remarks in
	Class CUIFrameMgr for details on
	the naming scheme.\n\n
	<b>int indx</b>\n\n
	The zero based index of the icon in the BMP file. The first icon is 0, the
	second is 1, etc. */
	virtual void		SetButtonIcon(const MCHAR* icnf, int indx) = 0;
	/*! \remarks Returns the file name of the icon file. */
	virtual MSTR&		GetButtonIconFile() = 0;
	/*! \remarks Returns the zero based index of the icon in the icon file. */
	virtual int			GetButtonIconIndex() = 0;
	/*! \remarks Sets the specified flags.
	\par Parameters:
	<b>short mask</b>\n\n
	The flags to set. One or more of the following values:\n\n
	<b>ME_DROPPED_SCRIPT</b>\n\n
	Macro made from some drag-and-dropped text.\n\n
	<b>ME_SILENT_ERRORS</b>\n\n
	Macro won't report any runtime errors. */
	virtual void		SetFlags(short mask) = 0;
	/*! \remarks Clears the specifed flags.
	\par Parameters:
	<b>short mask</b>\n\n
	The flags to clear. One or more of the following values:\n\n
	<b>ME_DROPPED_SCRIPT</b>\n\n
	Macro made from some drag-and-dropped text.\n\n
	<b>ME_SILENT_ERRORS</b>\n\n
	Macro won't report any runtime errors. */
	virtual void		ClearFlags(short mask) = 0;
	/*! \remarks Returns the state of the specified flags.
	\par Parameters:
	<b>short mask</b>\n\n
	The flags to get. One or more of the following values:\n\n
	<b>ME_DROPPED_SCRIPT</b>\n\n
	Macro made from some drag-and-dropped text.\n\n
	<b>ME_SILENT_ERRORS</b>\n\n
	Macro won't report any runtime errors. */
	virtual short		GetFlags(short mask) = 0;

	// execution
	/*! \remarks Executes this macro entry.
	\return  A pointer to the result of executing the macro. If a developer
	does't care about the result of executing a macro script, which is usually
	the case, then the <b>Value*</b> returned from this method can just be
	ignored. If a developer does care, then the necessary information about
	working with <b>Value*</b>'s is in the MAXScript SDK documentation. */
	virtual Value*		Execute() = 0;
	virtual BOOL		Compile() = 0;
	virtual void		Reset() = 0;

	// cleanup
	virtual void		DeleteThis() = 0;

	// R4 extension to support body structured into handlers
	virtual Value*		CallHandler(Value* handler_or_name, Value** arg_list, int count, BOOL hold = TRUE)=0;
	virtual FPStatus	CallHandler(const MCHAR* handler_name, FPParams* params, FPValue& result, BOOL hold = TRUE)=0;
	virtual Value*		GetHandler(Value* handler_name)=0;
	virtual BOOL		HasHandler(const MCHAR* handler_name)=0;

	virtual MaxIcon*	GetIcon()=0;
};

#define ME_DROPPED_SCRIPT	0x0001		// macro made from some drag-and-dropped text
#define ME_SILENT_ERRORS	0x0002		// macro won't report any runtime errors
#define ME_HAS_EXECUTE		0x0004		// has execute handler
#define ME_TEMPORARY		0x0008		// temporary dropScript
#define ME_NO_AUTO_UNDO		0x0010		// execution of the macroscript is not automatically wrapped in a thehold begin/accept
#define ME_NEEDS_COMPILE	0x0020		// macro should be compiled when maxscript starts up

#define BAD_MACRO_ID		-1			// illegal macroID 

/*! \sa  Class MacroEntry, Class MacroButtonData.\n\n
\par Description:
This class is available in release 3.0 and later only.\n\n
This class provides access to Macro scripts. Macro scripts (or macros) are
scripts that live in buttons and menus in the customizable UI. Methods of this
class are availalble to access macros using IDs or category and name strings,
methods to edit macro scripts, methods to execute macros, and methods for
directory scanning and loading.\n\n
The directory instance (access via the global function
<b>GetMacroScriptDir()</b>) is used by the CUI to provide a list of available
macros in the toolbar/menu editor. The API also provides a way for the CUI to
open a macro editor to allow on-the-fly creation of macro scripts.\n\n
The macro script manager keeps a directory of all known macros and provides an
API for running and editing macros and for accessing and updating the
directory.\n\n
Macros are normally entered into the directory by the MAXScript compiler as a
side-effect of compiling a macro definition. Anyone using MAXScript can at any
time eval a macro definition and thereby add CUI macro scripts.\n\n
Consequently, macros can be stored in any script file and be loaded just by
executing the file. The macro definition syntax permits any number of macros
per file.\n\n
Most macros will be stored in files in a special CUI macro or config directory
so that a user can take all his custom UI stuff with him by copying
directories. (This directory supports recursive scanning of sub-dirs, so users
can organize their macros). On-the-fly macro creation in the CUI editor or by
text drag-and-drop onto the shelf or by evaling a definition in the listener
will generate a file in this directory to provide permanent storage.\n\n
Note: <b>typedef short MacroID;</b>\n\n
Note: In order to use these methods you need to <b>#include
"IMACROSCRIPT.H"</b> and link to <b>"MAXSCRPT.LIB"</b>. */
class MacroDir : public InterfaceServer
{
public:

	// access by ID or category & name strings
	/*! \remarks Returns a pointer to the MacroEntry for the macro script
	whose ID is passed.
	\par Parameters:
	<b>MacroID mid</b>\n\n
	The ID of the macro. */
	virtual MacroEntry*	GetMacro(MacroID mid) = 0;
	/*! \remarks Returns a pointer to the MacroEntry corresponding to the
	given category and name strings passed (or NULL if not found).
	\par Parameters:
	<b>MCHAR* category</b>\n\n
	The category name.\n\n
	<b>MCHAR* name</b>\n\n
	The macro script name. */
	virtual MacroEntry*	FindMacro(const MCHAR* category, const MCHAR* name) = 0;
	/*! \remarks Returns TRUE if the macro ID is valid (or unused); otherwise
	FALSE.
	\par Parameters:
	<b>MacroID mid</b>\n\n
	The ID to check. */
	virtual BOOL		ValidID(MacroID mid) = 0;

	// iteration 
	/*! \remarks Returns the number of macro entries in this macro directory.
	*/
	virtual int			Count() = 0;
	/*! \remarks Returns a pointer to the macro entry whose index in the
	directory is passed.
	\par Parameters:
	<b>int index</b>\n\n
	The zero based index of the entry. This is a value between 0 and
	<b>Count()-1</b>. */
	virtual MacroEntry*	GetMacro(int index) = 0;

	// macro entry management

// LAM - 10/15/01 - when SDK can change, remove the following and uncomment the versions where internalCategory
// is passed in. Internally, only the verions using internalCategory are used.
	/*! \remarks Adds the macro whose parameters are passed and returns a
	pointer to the new macro entry. This form allows you to define a macro that
	is already in a file, by giving a source file name and seek offset into
	that file. This is typically used by the MAXScript compiler and .mcr file
	scanner to register macro definitions they come across.
	\par Parameters:
	<b>MCHAR* category</b>\n\n
	The category for the macro.\n\n
	<b>MCHAR* name</b>\n\n
	The name for the macro.\n\n
	<b>MCHAR* tooltip</b>\n\n
	The tooltip text.\n\n
	<b>MCHAR* buttonText</b>\n\n
	The button text.\n\n
	<b>MCHAR* sourceFile</b>\n\n
	The source file name.\n\n
	<b>int sourceOffset</b>\n\n
	The line number of the first line of the script in the source file. */
	virtual MacroEntry*	AddMacro(const MCHAR* category, const MCHAR* internalCategory, const MCHAR* name, 
								 const MCHAR* tooltip, const MCHAR* buttonText,
								 const MCHAR* sourceFile, int sourceOffset) = 0;
	/*! \remarks Add or replaces a macro using given source text as the body
	of the macro. In this overload, name can be NULL in which case a unique
	name is generated.\n\n
	This form takes the body of the actual macro script as the
	<b>sourceText</b> argument and places it in a newly-created file in the UI
	directory and registers that file and a zero offset as the macro
	definition. All macroScripts need to be in files somewhere so that they are
	persistent if referenced in a CUI toolbar that the user saves. This form is
	used, for example, by the toolbar manager when you drag a piece of selected
	text onto a toolbar to cause a script button to be created.
	\par Parameters:
	<b>MCHAR* category</b>\n\n
	The category for the macro.\n\n
	<b>MCHAR* name</b>\n\n
	The name for the macro or NULL to generate a unique name.\n\n
	<b>MCHAR* tooltip</b>\n\n
	The tooltip text.\n\n
	<b>MCHAR* buttonText</b>\n\n
	The button text.\n\n
	<b>MCHAR* sourceText</b>\n\n
	The source text. */
	virtual MacroEntry*	AddMacro(const MCHAR* category, const MCHAR* internalCategory, const MCHAR* name, 
								 const MCHAR* tooltip, const MCHAR* buttonText,
								 const MCHAR* sourceText) = 0;

    /*! \remarks Sets the parameters for the macro entry whose ID is passed.
    \par Parameters:
    <b>MacroID mid</b>\n\n
    The macro ID.\n\n
    <b>MCHAR* tooltip</b>\n\n
    The tooltip text.\n\n
    <b>MCHAR* btnText</b>\n\n
    The button text.\n\n
    <b>MCHAR* sourceFile</b>\n\n
    The source file name.\n\n
    <b>int sourceOffset</b>\n\n
    The sorce offset.
    \return  TRUE if set; FALSE if the ID was not found. */
    virtual BOOL		SetMacro(MacroID mid, const MCHAR* tooltip, const MCHAR* btnText, const MCHAR* sourceFile, int sourceOffset) = 0;
	/*! \remarks This method modifies the string in place to be a valid macro
	name (no punctuations other than spaces).
	\par Parameters:
	<b>MCHAR* s</b>\n\n
	The name string. */
	virtual MCHAR*		MakeNameValid(MCHAR* s) = 0;
	/*! \remarks This method modifies the string in place to be a valid
	category name (no punctuations other than spaces).
	\par Parameters:
	<b>MCHAR* s</b>\n\n
	The category string. */
	virtual MCHAR*		MakeCategoryValid(MCHAR* s) = 0;
	
	// editing
	/*! \remarks This methods brings up the editor for editing the specified
	macro script text.
	\par Parameters:
	<b>MacroID mid</b>\n\n
	The ID of the macro script to edit. */
	virtual BOOL		EditMacro(MacroID mid) = 0;
	
	// execution
	/*! \remarks Executes the macro script whose ID is passed.
	\par Parameters:
	<b>MacroID mid</b>\n\n
	The ID of the macro to execute.
	\return  A pointer to the result of executing the macro. If a developer
	does't care about the result of executing a macroScript, which is usually
	the case, then the <b>Value*</b> returned from this method can just be
	ignored. If a developer does care, then the necessary information about
	working with <b>Value*</b>'s is in the MAXScript SDK documentation. */
	virtual Value*		Execute(MacroID mid) = 0;
	// R4 extension to support body structured into handlers
	virtual Value*		CallHandler(MacroID mid, Value* handler_or_name, Value** arg_list, int count, BOOL hold = TRUE)=0;
	virtual FPStatus	CallHandler(MacroID mid, const MCHAR* handler_name, FPParams* params, FPValue& result, BOOL hold = TRUE)=0;
	virtual Value*		GetHandler(MacroID mid, Value* handler_name)=0;

	// macro-script file directory scanning & loading
	/*! \remarks This method loads all the macro scripts found in the
	specified path and optionally its sub-directories. You can point this
	method at any directory and it will scan it for .mcr files and scan those
	for macroScript definitions. 3ds Max uses this during startup to scan the
	UI directory (recursively) for .mcr files.
	\par Parameters:
	<b>MCHAR* path_name = NULL</b>\n\n
	The path to check. If NULL the default path is used.\n\n
	<b>BOOL recurse = TRUE</b>\n\n
	If TRUE nested sub-directories are scanned and loaded as well. */
	virtual void		LoadMacroScripts(const MCHAR* path_name = NULL, BOOL recurse = TRUE) = 0;
	virtual MacroEntry*	LoadMacroScript(const MCHAR* file_name) = 0;

	// set & get default path for storing/searching macro script files
	/*! \remarks Sets the default path for storing / searching macro script
	files.
	\par Parameters:
	<b>MCHAR* path_name</b>\n\n
	The path to set. */
	virtual void		SetMacroScriptPath(const MCHAR* path_name) = 0;
	/*! \remarks Returns the default path for storing / searching macro script
	files. */
	virtual const MCHAR*		GetMacroScriptPath() = 0;
};

#if defined(BLD_CORE)
	/*! \remarks This global function is available in release 3.0 and later
	only.\n\n
	Returns a reference to the macro script directory. */
	extern MacroDir& GetMacroScriptDir();
#else
	/*! \remarks This global function is available in release 3.0 and later
	only.\n\n
	Returns a reference to the macro script directory. */
	extern ScripterExport MacroDir& GetMacroScriptDir();
	/*! \remarks This global function is available in release 3.0 and later
	only.\n\n
	This is an internal only function used by 3ds Max during startup to get the
	macroScript system initialized.  */
	extern ScripterExport void InitMacroScriptDir();
#endif

// ActionTableIds
const ActionTableId   kActionMacroScripts = 647394;
const ActionContextId kActionMacroScriptsContext = 647394;

#endif    // _H_IMACROSCRIPT
