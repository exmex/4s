/*	
 *		macrorec.h - Interface to MAXScript macro recorder for MAX
 *
 *	The macro recorder 					
 *
 *			Copyright © Autodesk, Inc, 1998.  John Wainwright.
 *
 */

#ifndef _H_MACRORECORD
#define _H_MACRORECORD
#include "maxheap.h"

#ifndef ScripterExport
#	ifdef BLD_MAXSCRIPT
#		define ScripterExport __declspec( dllexport )
#	else
#		define ScripterExport __declspec( dllimport )
#	endif
#endif

class ParamBlock2;

/*! \sa  Class ClassDesc, Class INode, Class IParamBlock, Class IParamBlock2, Class Matrix3, Class ReferenceTarget.\n\n
\par Description:
This class is available in release 3.0 and later only.\n\n
This class provides various methods to emit pieces of script to the Macro
Recorder. There are also methods to specify the nature of the scripts generated
by the user operating 3ds Max. Additionally there are methods to enable or
disable the recording of scripts.\n\n
This class may be used by plug-ins but is also used internally as part of the
3ds Max Macro Recorder. Inside key areas of 3ds Max macro scripts are emitted
corresponding to the operation being performed. For example when a call goes to
a parameter block to set a value 3ds Max internally call this classes method
<b>ParamBlock2SetValue(...)</b> to emit script to record the change. Thus, many
operations a plug-in performs are recorded automatically. There are however
operations a plug-in can perform which won't be recorded automatically. In
these cases methods of this class may be used to emit script to record these
operations.\n\n
Several of the functions in this class use an ellipsis argument (<b>...</b>).
This ellipsis is the var-args style of passing information to a method. This
allows a developer to pass a variable number of values to the method. These are
typically a set of tag values followed by some C++ types. The sample code shown
with many of the methods shows how this is done. For a starting point for more
general information on variable argument lists see <b>va_arg</b> in the Window
help file.\n\n
Developers use the following global instance of this class to call these
methods:\n\n
<b>MacroRecorder *macroRecorder;</b>  */
class MacroRecorder : public BaseInterfaceServer
{
public:
	class MacroRecorderDisable;

	// script constructors
	/*! \remarks You would use this method if implementing a custom creation
	manager. This method starts a special accumulation 'mode' in which certain
	other Macro Recorder calls are treated specially until the next
	<b>EmitScript()</b>. <b>BeginCreate()</b> effectively signals the start of
	a scene node creation and enters a mode in which calls to
	<b>SetNodeTM()</b>, <b>SetProperty()</b>, <b>ParamBlockXSetValue()</b> and
	<b>SetSelProperty()</b> all generate keyword parameters to the current
	constructor, rather than emitting stand-alone property assignment scripts.
	Outside the 'create' mode, a call to <b>SetNodeTM()</b> would generate
	something like:\n\n
	<b>move $foo [10,0,0]</b>\n\n
	but when in the mode would add a pos: argument to the constructor:\n\n
	<b>sphere radius:20 pos:[10,0,0]</b>
	\par Parameters:
	<b>ClassDesc* cd</b>\n\n
	Points to the class descriptor for the plug-in.
	\return  This tells you whether MAXScript successfully entered the
	BeginCreate mode. It will fail if MAXScript can't create the object
	described by the ClassDesc, so you might use it in some generic situations
	to decided whether to call the closing EmitScript(). */
	virtual BOOL BeginCreate(ClassDesc* cd, int flags = 0) = 0;
	/*! \remarks This method is for internal use only. */
	virtual void SetNodeTM(INode* n, Matrix3 m) = 0;
	/*! \remarks This method is for internal use only. */
	virtual void ParamBlockSetValue(ParamBlock* pb, int i, BYTE type, ...) = 0;
	/*! \remarks This method is for internal use only. */
	virtual void ParamBlock2SetValue(IParamBlock2* pb, int i, int tabIndex, BYTE type, ...) = 0;
	/*! \remarks This method is for internal use only. */
	virtual void ParamBlock2SetCount(IParamBlock2* pb, int i, int n) = 0;
	/*! \remarks This provides a simple way to emit a properly assignment
	script. An example of this type of script is shown below:\n\n
	<b>$sphere01.radius = 50</b>
	\par Parameters:
	<b>ReferenceTarget* targ</b>\n\n
	Points to the object whose property is changing.\n\n
	<b>MCHAR* prop_name</b>\n\n
	The string which is the name of the property. This is the fixed
	machine-parsable name.\n\n
	<b>BYTE type</b>\n\n
	One of the type tags from
	<a href="ms-its:listsandfunctions.chm::/idx_R_list_of_macro_recorder_value_types.html">List of Macro
	Recorder Value Types</a>.\n\n
	<b>...</b>\n\n
	This ellipsis is the var-args style of passing information to a method.
	This allows a developer to pass a variable number of values to a method.
	These are typically a set of 'tags' followed by some C++ data types. */
	virtual void SetProperty(ReferenceTarget* targ, MCHAR* prop_name, BYTE type, ...) = 0;
	/*! \remarks This method is used to signal that an mr_reftarg argument in
	the currently accumulating script should be emitted as a copy. For example,
	when maps or materials are dragged onto sub-map/mtl buttons in the material
	editor, an instance/copy requester dialog is presented and depending on the
	choice, either a direct assignment or assignment of a copy is
	appropriate:\n\n
	<b>meditMaterials[4].materialList[2] = meditMaterials[11] --
	instance</b>\n\n
	<b>meditMaterials[4].materialList[2] = copy meditMaterials[11] --
	copy</b>\n\n
	The actual assignment script is set up using a
	<b>macroRecorder-\>SetProperty()</b> call with the dropped map/material
	supplied as an <b>mr_reftarg</b> argument. In situations where the
	copy/instance status is known, you can emit directly an <b>mr_funcall</b>
	argument for the copy, but there may be situations in which this choice is
	decided in some piece of remote code or control flow and so you can use
	this method to condition the emitted script to apply a 'copy ' call.
	\par Parameters:
	<b>ReferenceTarget* to_copy</b>\n\n
	The ReferenceTarget* object which should be copied. */
	virtual void SetCopy(ReferenceTarget* to_copy) = 0;
	/*! \remarks This provides a simple way to emit a properly assignment
	script for the current selection set.
	\par Parameters:
	<b>MCHAR* prop_name</b>\n\n
	The name of the property to set.\n\n
	<b>BYTE type</b>\n\n
	One of the type tags from
	<a href="ms-its:listsandfunctions.chm::/idx_R_list_of_macro_recorder_value_types.html">List of Macro
	Recorder Value Types</a>.\n\n
	<b>...</b>\n\n
	This ellipsis is the var-args style of passing information to a method.
	This allows a developer to pass a variable number of values to a method.
	These are typically a set of 'tags' followed by some C++ types.
	\par Sample Code:
	<b>macroRecorder-\>SetSelProperty(_M("material"), mr_reftarg,
	(Mtl*)dropThis);</b> */
	virtual void SetSelProperty(MCHAR* prop_name, BYTE type, ...) = 0;
	/*! \remarks This method is used to build a MAXScript function call. In
	the general case, such a call may have positional arguments followed by
	keyword arguments.
	\par Parameters:
	<b>MCHAR* op_name</b>\n\n
	The name of the function to call.\n\n
	<b>int arg_count</b>\n\n
	The number of positional arguments in the varargs section.\n\n
	<b>int keyarg_count</b>\n\n
	The number of keyword arguments in the varargs section.\n\n
	<b>...</b>\n\n
	This ellipsis is the var-args style of passing information to a method.
	This allows a developer to pass a variable number of values to a method.
	These are typically a set of 'tags' followed by some C++ types.\n\n
	See <a href="ms-its:listsandfunctions.chm::/idx_R_list_of_macro_recorder_value_types.html">List of Macro
	Recorder Value Types</a> for a list of potential tags and arguments.
	\par Sample Code:
	<b>macroRecorder-\>FunctionCall(_M("addModifier"), 2, 1, mr_reftarg, this,
	mr_create, mod-\>ClassID(), mod-\>SuperClassID(), 0, _M("before"), mr_int,
	before);</b>\n\n
	This generates an addModifier() function call, such as:\n\n
	<b>addModifier $foo (bend()) before:3</b>\n\n
	The call has 2 positional arguments and 1 keyword argument (hence the <b>2,
	1</b>). The first positional is given as <b>mr_reftarg, this</b> which
	refers to the current node, the second positional is given as <b>mr_create,
	mod-\>ClassID(), mod-\>SuperClassID(), 0</b> which causes a 0-argument
	constructor to be emitted for the modifier, and finally the single keyword
	argument is given as <b>_M("before"), mr_int, before</b> which is the
	keyword name followed by the arg value. */
	virtual void FunctionCall(MCHAR* op_name, int arg_count, int keyarg_count, ...) = 0;
	/*! \remarks Emits a piece of macro script as a literal string. To
	understand when this is used consider the following example. Say you have a
	button in your user interface which does a certain thing but there is no
	way using the other macro recorder calls of constructing the piece of
	script that you need emitted. For instance the button may invoke a for
	loop. In such a case you can use this method to emit a macro script string
	which does a for loop. This is a string, just as you would type it into the
	MAXScript Listener.
	\par Parameters:
	<b>MCHAR* s</b>\n\n
	The string to emit. */
	virtual void ScriptString(MCHAR* s) = 0;
	/*! \remarks This method is called to emit script to record the assignment
	to a MAXScript variable.
	\par Parameters:
	<b>MCHAR* var_name</b>\n\n
	The variable name.\n\n
	<b>BYTE type</b>\n\n
	One of the type tags from
	<a href="ms-its:listsandfunctions.chm::/idx_R_list_of_macro_recorder_value_types.html">List of Macro
	Recorder Value Types</a>.\n\n
	<b>...</b>\n\n
	This ellipsis is the var-args style of passing information to a method.
	This allows a developer to pass a variable number of values to a method.
	These are typically a set of 'tags' followed by some C++ types. */
	virtual void Assign(MCHAR* var_name, BYTE type, ...) = 0;
	virtual void Assign(BYTE type, ...) = 0;
	virtual void OpAssign(MCHAR* op, BYTE type, ...) = 0;
	virtual void OperandSequence(int count, BYTE type, ...) = 0;
	/*! \remarks This method is for internal use only. */
	virtual BOOL BeginSelectNode() = 0;
	/*! \remarks This method is for internal use only. */
	virtual void Select(INode*) = 0;
	/*! \remarks This method is for internal use only. */
	virtual void DeSelect(INode*) = 0;
	/*! \remarks This method is for internal use only. */
	virtual void MAXCommand(int com) = 0;
	virtual void AddComment(MCHAR* str) = 0;
	/*! \remarks This cancels and clears the currently accumulating script.
	This would be used for example, if the operation that is being accumulated
	can be canceled by the user, such as right-clicking out of a transform or a
	geometry node create. There are calls to <b>macroRecorder()-\>Cancel()</b>
	in the <b>MOUSE_ABORT</b> processing in the default creation manager. */
	virtual void Cancel() = 0;
	/*! \remarks This signals the completion of an accumulating script,
	causing it to be frozen in the recorder pane and any new calls that might
	have been folded into the current script will cause a new one to be
	started. For example, when you drag the time slider, the sliderTime
	assignment script accumulates the changes, but when you let go of the mouse
	button, an EmitScript() is called, so that subsequent drags will start a
	separate script fragment. Same with interactive transforms and node
	creation. */
	virtual void EmitScript() = 0;
	// scripter info extractors
	/*! \remarks Returns the property name of the 'i-th' sub-material of the
	specified material.
	\par Parameters:
	<b>Mtl* m</b>\n\n
	The material whose 'i-th' sub-material property name is returned.\n\n
	<b>int i</b>\n\n
	The zero based index of the sub-material. */
	virtual MSTR GetSubMtlPropName(Mtl* m, int i) = 0;
	/*! \remarks Returns the property name of the 'i-th' sub-texmap of the
	specified material.
	\par Parameters:
	<b>ReferenceTarget* m</b>\n\n
	The material or texmap whose 'i-th' sub-texmap property name is returned.\n\n
	<b>int i</b>\n\n
	The zero based index of the sub-texmap. */
	virtual MSTR GetSubTexmapPropName(ReferenceTarget* m, int i) = 0;
	// nestable disable/enable
	/*! \remarks Enables the Macro Recorder. This call is 'nestable', i.e. it
	uses a use counter internally so recursive or nested code can manage local
	enables and disable states. */
	virtual void Enable() = 0;
	/*! \remarks Disables the Macro Recorder. This allows the developer to
	disable the automatic macro recording. */
	virtual void Disable() = 0;
	/*! \remarks Returns TRUE if the Macro Recroder is enabled (via
	<b>Enable()</b> above); otherwise FALSE. */
	virtual BOOL Enabled() = 0;
	// master enable and option controls
	/*! \remarks In the MAXScript Listener Window Macro Recorder pulldown menu
	choice is an option to enable or disable the Macro Recorder. This method
	corresponds to that state. It returns TRUE if enabled; FALSE if disabled.
	*/
	virtual BOOL MasterEnable() = 0;
	/*! \remarks In the MAXScript Listener Window Macro Recorder pulldown menu
	choice is an option to enable or disable the Macro Recorder. This method
	sets this state.
	\par Parameters:
	<b>BOOL onOff</b>\n\n
	TRUE for enabled; FALSE for disabled. */
	virtual void MasterEnable(BOOL onOff) = 0;
	/*! \remarks Returns TRUE if code is emitted when command panels are
	changed; FALSE if code is not emitted. */
	virtual BOOL ShowCommandPanelSwitch() = 0;
	/*! \remarks Determines if the macro recorder will emit script for command
	panel mode changes.
	\par Parameters:
	<b>BOOL onOff</b>\n\n
	TRUE to record command panel changes; FALSE to ignore them. */
	virtual void ShowCommandPanelSwitch(BOOL onOff) = 0;
	/*! \remarks Returns TRUE if the macro recorder will emit script for 3ds
	Max toolbar tool selections; otherwise FALSE. */
	virtual BOOL ShowToolSelections() = 0;
	/*! \remarks Determines if the macro recorder will emit script for 3ds Max
	toolbar selections.
	\par Parameters:
	<b>BOOL onOff</b>\n\n
	TRUE to record toolbar selections; FALSE to ignore them. */
	virtual void ShowToolSelections(BOOL onOff) = 0;
	/*! \remarks Returns TRUE if the macro recorder will emit script for 3ds
	Max menu selecitons; otherwise FALSE. */
	virtual BOOL ShowMenuSelections() = 0;
	/*! \remarks Determines if the macro recorder will emit script for 3ds Max
	menu selections.
	\par Parameters:
	<b>BOOL onOff</b>\n\n
	TRUE to record menu selections; FALSE to ignore them. */
	virtual void ShowMenuSelections(BOOL onOff) = 0;
	/*! \remarks Returns TRUE if specific node names are used in the generated
	code; FALSE if the current selection is used. */
	virtual BOOL EmitAbsoluteSceneNames() = 0;
	/*! \remarks This controls whether the code generated refers to the exact
	node names being operated or or simply the current selection.
	\par Parameters:
	<b>BOOL onOff</b>\n\n
	TRUE to record absolute scene names; FALSE to use the selection set. */
	virtual void EmitAbsoluteSceneNames(BOOL onOff) = 0;
	/*! \remarks Returns TRUE if recording absolute sub-object numbers; FALSE
	if using the selection set. */
	virtual BOOL EmitAbsoluteSubObjects() = 0;
	/*! \remarks Determines if code generated is relative to the current
	sub-object selection state or if explicit sub-object numbers are generated.
	\par Parameters:
	<b>BOOL onOff</b>\n\n
	TRUE to record explicit, absolute sub-object numbers; FALSE to use the
	selection set. */
	virtual void EmitAbsoluteSubObjects(BOOL onOff) = 0;
	/*! \remarks Returns TRUE if code is generated using absolute transform
	assignments; FALSE if relative transforms operations are generated. */
	virtual BOOL EmitAbsoluteTransforms() = 0;
	/*! \remarks Sets if code is generated using absolute transform
	assignments.
	\par Parameters:
	<b>BOOL onOff</b>\n\n
	TRUE for on; FALSE for off. */
	virtual void EmitAbsoluteTransforms(BOOL onOff) = 0;
//	virtual BOOL EmitExplicitCoordinates() = 0;   // deferred in Shiva
//	virtual void EmitExplicitCoordinates(BOOL onOff) = 0;
};
   
#if defined(BLD_CORE) || defined(BLD_PARAMBLK2)
	extern MacroRecorder *macroRecorder;
#else
	extern ScripterExport MacroRecorder *macroRecorder;
#endif

/*! \remarks This global function is used internally to inialize the macro
recorder and should not be called by plug-in developers. */
extern ScripterExport void InitMacroRecorder();

// value types:
enum { mr_int, mr_float, mr_string, mr_bool,			// basic C types
	   mr_point3, mr_color, mr_angaxis, mr_quat,		// MAX SDK types...
	   mr_time, mr_reftarg, mr_bitarray, mr_pbbitmap,
	   mr_matrix3, mr_nurbssel, mr_meshselbits, 
	   mr_meshsel, mr_subanim, mr_animatable,
	   mr_classid, mr_nurbsselbits, 
	   mr_point4, mr_acolor,
	   mr_sel, mr_funcall, mr_varname, mr_create,		// MAXScript types
	   mr_angle, mr_percent, mr_index, mr_prop,
	   mr_name, 
	   mr_dimfloat, mr_dimpoint3,						// explicitly dimensioned float & point
	};

#define macroRec GetCOREInterface()->GetMacroRecorder()

//==============================================================================
// class MacroRecorder::MacroRecorderDisable
//
//! \brief Class for easily & safely disabling the macro recorder.
/*! This class disables the macro recorder in its constructor and re-enables it
in its destructor. It is therefore a fool-proof (and exception safe) way of
temporarily disabling the macro recorder
*/
class MacroRecorder::MacroRecorderDisable : public MaxHeapOperators {
public:
	//! Constructor, enables the macro recorder automatically.
	MacroRecorderDisable() {
		MacroRecorder* recorder = macroRec;
		if(recorder != NULL)
			recorder->Disable();
	}

	//! Destructor, disables the macro recorder automatically.
	~MacroRecorderDisable() {
		MacroRecorder* recorder = macroRec;
		if(recorder != NULL)
			recorder->Enable();
	}
};

#endif
