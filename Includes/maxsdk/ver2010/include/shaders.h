//////////////////////////////////////////////////////////////////////////////
//
//		Shader plug-ins
//
//		Created: 8/18/98 Kells Elmquist
//
#ifndef	SHADERS_H
#define SHADERS_H

#include "iparamb2.h"
#include "stdmat.h"
#include "buildver.h"

//#define STD2_NMAX_TEXMAPS	24
#define		N_ID_CHANNELS	16		// number of ids in stdMat

class Shader;

#define OPACITY_PARAM	0

#define DEFAULT_SOFTEN	0.1f

/////////////////////////////////////////////////////////////////////////////
//
//	Shader param dialog
//
// Returned by a shader when it is asked to put up its rollup page.
/*! \sa  Class ParamDlg, Class StdMat2, Class Shader.\n\n
\par Description:
This class is available in release 3.0 and later only.\n\n
A pointer to an instance of this class is returned by a Shader when it is asked
to put up its rollup page.  */
class ShaderParamDlg : public ParamDlg {
	public:
		/*! \remarks Returns the unique Class_ID of this object. */
		virtual Class_ID ClassID()=0;
		/*! \remarks This method sets the current shader being edited to the shader passed.
		\par Parameters:
		<b>ReferenceTarget *m</b>\n\n
		The Shader to set as current. */
		virtual void SetThing(ReferenceTarget *m)=0;
		/*! \remarks This method sets the current Standard material (and its shader) being
		edited to the ones passed.
		\par Parameters:
		<b>StdMtl2* pMtl</b>\n\n
		The Standard material to set as current.\n\n
		<b>Shader* pShader</b>\n\n
		The Shader to set as current. */
		virtual void SetThings( StdMat2* pMtl, Shader* pShader )=0;
		/*! \remarks Returns the a pointer to the current <b>material</b>
		being edited. Note that in most of the Get/SetThing() methods in the
		SDK the 'Thing' is the actual plug-in. In this case it's not. It the
		material which is using this Shader. */
		virtual ReferenceTarget* GetThing()=0;
		/*! \remarks This method returns a pointer to the current Shader. */
		virtual Shader* GetShader()=0;
		/*! \remarks This method is called when the current time has changed.
		This gives the developer an opportunity to update any user interface
		data that may need adjusting due to the change in time.
		\par Parameters:
		<b>TimeValue t</b>\n\n
		The new current time.
		\par Default Implementation:
		<b>{}</b> */
		virtual void SetTime(TimeValue t) {}		
		/*! \remarks This method is called to delete this instance of the class.\n\n
		For dynamically created global utility plugins, this method has to be
		implemented and should have a implementation like <b>{ delete this;
		}</b> */
		virtual void DeleteThis()=0;		
		/*! \remarks This is the dialog procedure for the user interface controls of	the Shader.
		\par Parameters:
		<b>HWND hwndDlg</b>\n\n
		The window handle of the rollup page.\n\n
		<b>UINT msg</b>\n\n
		The message to process.\n\n
		<b>WPARAM wParam</b>\n\n
		The first dialog parameter.\n\n
		<b>LPARAM lParam</b>\n\n
		The second dialog parameter.
		\return  Except in response to the WM_INITDIALOG message, the procedure should
		return nonzero if it processes the message, and zero if it does not. In
		response to a WM_INITDIALOG message, the dialog box procedure should return
		zero if it calls the SetFocus function to set the focus to one of the controls
		in the dialog. Otherwise, it should return nonzero, in which case the system
		sets the focus to the first control in the dialog that can be given the focus.*/
		virtual INT_PTR PanelProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam )=0; 
		/*! \remarks This method is used to load the user interface controls
		with their current values.
		\par Parameters:
		<b>int draw</b>\n\n
		This parameter is not currently used. */
		virtual void LoadDialog( int draw )=0;
		virtual void UpdateDialog( ParamID paramId )=0;
		/*! \remarks This method returns the window handle of the rollup
		panel. */
		virtual HWND GetHWnd()=0;
		/*! \remarks This method returns the index of the sub-texmap
		corresponding to the window whose handle is passed. If the handle is
		not valid return -1.
		\par Parameters:
		<b>HWND hw</b>\n\n
		The window handle to check. */
		virtual int  FindSubTexFromHWND(HWND hw)=0;
		/*! \remarks This method is called to update the opacity parameter of
		the plug-in in the user interface. */
		virtual void UpdateOpacity()=0;
		/*! \remarks This method is called to update the map buttons in the
		user interface. For example it can put a <b>" "</b> or <b>"m"</b> or
		<b>"M"</b> on the button face based on the state of the map. */
		virtual void UpdateMapButtons()=0;
};
 
///////////////////////////////////sh flags //////////////////////////////////////
#define SELFILLUM_CLR_ON	(1<<16) // can be or'd w/ mtl, not sure it's necessary

/*********
// post mapping params for shader
class IllumParams {
public:
	// these are the inputs to the shader, mostly textured channels
	ULONG mtlFlags;
	Shader* pShader;	// for render elements to call the shader & mtl again, may be null
	Mtl* pMtl;			// max mtl being shaded, null if bgnd
//	Point3 N, V;
	Color channels[ STD2_NMAX_TEXMAPS ];

	float falloffOpac;		// textured opacity w/ stdfalloff (reg opac in channels)
	float kR;				// combined reflection.a * amt 
	ULONG hasComponents;	// bits for active components(e.g.currently has active refl map)
	ULONG stdParams;
	int*  stdIDToChannel;	// combined shader & mtl

	// these are the component-wise outputs from the shading process
	Color ambIllumOut, diffIllumOut, transIllumOut, selfIllumOut; // the diffuse clrs
	Color specIllumOut, reflIllumOut;	// the specular colors

	// User Illumination outputs for render elements, name matched
	int	nUserIllumOut;		// one set of names for all illum params instances
	MCHAR** userIllumNames;  // we just keep ptr to shared name array, not destroyed
	Color* userIllumOut;	// the user illum color array, new'd & deleted w/ the class

	float diffIllumIntens; // used only by reflection dimming, intensity of diffIllum prior to color multiply
	float finalOpac; // for combining components

	// these are the outputs of the combiner
	Color finalC;	// final clr: combiner composites into this value.
	Color finalT;	// shader transp clr out

public:
	// Illum params are allocated by materials during shading process to
	// communicate with shaders & render elements
	// So materials need to know how many userIllum slots they will use
	// most materials know this, but std2 will have to get it from the shader
	IllumParams( int nUserOut = 0, MCHAR** pUserNames = NULL ){ 
		nUserIllumOut = nUserOut;
		userIllumOut = ( nUserOut )? new Color[ nUserOut ] : NULL;
		userIllumNames = pUserNames;
	}

//	IllumParams(){ 
//		nUserIllumOut = 0;
//		userIllumOut = NULL;
//		userIllumNames = NULL;
//	}


	~IllumParams(){
	// We Dont destroy the name array as it's shared by all
		if( userIllumOut )
			delete [] userIllumOut;
	}

	// returns number of user illum channels for this material
	int nUserIllumChannels(){ return nUserIllumOut; }

	// returns null if no name array specified
	MCHAR* GetUserIllumName( int n ) { 
		DbgAssert( n < nUserIllumOut );
		if( userIllumNames )
			return userIllumNames[n];
		return NULL;
	}

	// render elements, mtls & shaders can use this to find the index associated with a name
	// returns -1 if it can't find the name
	int FindUserIllumName( MCHAR* name ){
		int n = -1;
		for( int i = 0; i < nUserIllumOut; ++i ){
			DbgAssert( userIllumNames );
			if( strcmp( name, userIllumNames[i] )){
				n = i;
				break;
			}
		}
		return n;
	}

	// knowing the index, these set/get the user illum output color
	void SetUserIllumOutput( int n, Color& out ){
		DbgAssert( n < nUserIllumOut );
		userIllumOut[n] = out;
	}
	void SetUserIllumOutput( MCHAR* name, Color& out ){
		for( int i = 0; i < nUserIllumOut; ++i ){
			DbgAssert( userIllumNames );
			if( strcmp( name, userIllumNames[i] )){
				userIllumOut[i] = out;
				break;
			}
		}
		DbgAssert( i < nUserIllumOut );
	}

	Color GetUserIllumOutput( int n ){
		DbgAssert( n < nUserIllumOut );
		return userIllumOut[n];
	}
	Color GetUserIllumOutput( MCHAR* name, int n ){
		for( int i = 0; i < nUserIllumOut; ++i ){
			DbgAssert( userIllumNames );
			if( strcmp( name, userIllumNames[i] )){
				return userIllumOut[i];
			}
		}
		return Color(0,0,0);
	}

	void ClearOutputs() { 
		finalC = finalT = ambIllumOut=diffIllumOut=transIllumOut=selfIllumOut=
		specIllumOut=reflIllumOut= Color( 0.0f, 0.0f, 0.0f ); 
		finalOpac = diffIllumIntens = 0.0f;
		for( int i=0; i < nUserIllumOut; ++i )
			userIllumOut[i] = finalC;
	}

	void ClearInputs() { 
		mtlFlags = stdParams = hasComponents = 0;
		pShader = NULL; pMtl = NULL;
		stdIDToChannel = NULL;
		kR = 0.0f; 
		for( int i=0; i < STD2_NMAX_TEXMAPS; ++i )
			channels[ i ] = Color( 0, 0, 0 );
	}
};
********/
 
/////////// Components defines
#define HAS_BUMPS				0x01L
#define HAS_REFLECT				0x02L
#define HAS_REFRACT				0x04L
#define HAS_OPACITY				0x08L
#define HAS_REFLECT_MAP			0x10L
#define HAS_REFRACT_MAP			0x20L
#define HAS_MATTE_MTL			0x40L


////////// Texture channel type flags
#define UNSUPPORTED_CHANNEL		0x01L
#define CLR_CHANNEL				0x02L
#define MONO_CHANNEL			0x04L
#define BUMP_CHANNEL			0x08L
#define REFL_CHANNEL			0x10L
#define REFR_CHANNEL			0x20L
#define DISP_CHANNEL			0x40L
#define SLEV_CHANNEL			0x80L
#define ELIMINATE_CHANNEL		0x8000L

#define SKIP_CHANNELS	(UNSUPPORTED_CHANNEL+BUMP_CHANNEL+REFL_CHANNEL+REFR_CHANNEL)

/////////// Class Id upper half for loading the Pre 3.0 shaders
#define  DEFAULT_SHADER_CLASS_ID BLINNClassID 

#ifndef USE_LIMITED_STDMTL // orb 01-14-2002
#define  PHONGClassID (STDSHADERS_CLASS_ID+2)
#define  METALClassID (STDSHADERS_CLASS_ID+4)
#endif // USE_LIMITED_STDMTL

#define  BLINNClassID (STDSHADERS_CLASS_ID+3)

class ParamBlockDescID;
class IParamBlock;

/*! \sa  Class SpecialFX, Class ShaderParamDlg, Class ShadeContext, Class IllumParams, Class IMtlParams, Class StdMat2,  Class Mtl,  Class Color, Class ILoad,  Class ISave.\n\n
\par Description:
This class is available in release 3.0 and later only.\n\n
This is one of the base classes for the creation of Shaders which plug-in to
the Standard material. Note: Developers should derive their plug-in Shader from
Class Shader rather than this class directly
since otherwise the interactive renderer won't know how to render the Shader in
the viewports.\n\n
Developers of this plug-in type need to understand how the Standard material
and the Shader work together.\n\n
Every material has a Shader. The Shader is the piece of code which controls how
light is reflected off the surface. The Standard material is basically the
mapping mechanism. It handles all the texturing for the material. It also
manages the user interface. This simplifies things so the Shader plug-in only
needs to worry about the interaction of light on the surface.\n\n
Prior to release 3 developers could write Material plug-ins that performed
their own shading, however ths was usually a major programming task. Release 3
provides the simpler Shader plug-in that would benefit from sharing all the
common capabilities. The Standard material, with its 'blind' texturing
mechanism, makes this possible. It doesn't know what it is texturing -- it
simply texturing 'stuff'. The shader names the channels (map), fills in the
initial values, specifies if they are a single channel (mono) or a triple
channel (color). The Standard material handles the rest including managing the
user interface.\n\n
Most of the code in a Shader has to do with supplying this information to a
Standard material. The values are passed and received back in class
<b>IllumParams</b>. There is a single method in a shader which actually does
the shading. This is the <b>Illum()</b> method.
\par Plug-In Information:
Class Defined In SHADER.H\n\n
Super Class ID SHADER_CLASS_ID\n\n
Standard File Name Extension DLB\n\n
Extra Include File Needed SHADERS.H
\par Method Groups:
See Method Groups for Class BaseShader.
*/
class BaseShader : public SpecialFX {
	public:
		RefResult NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
	         PartID& partID,  RefMessage message) {return REF_SUCCEED;}

		SClass_ID SuperClassID() {return SHADER_CLASS_ID;}
		BOOL BypassPropertyLevel() { return TRUE; }  // want to promote shader props to material level

		/*! \remarks Returns the requirements of the Shader for the specified
		sub-material. Many objects in the rendering pipeline use the
		requirements to tell the renderer what data needs to be available. The
		Shader's requirements are OR'd with the combined map requirements and
		returned to the renderer via the Stdmtl2's <b>GetRequirements()</b>
		function.
		\par Parameters:
		<b>int subMtlNum</b>\n\n
		This parameter is not used.
		\return  One or more of the following flags:\n\n
		See \ref materialRequirementsFlags. */
		virtual ULONG GetRequirements(int subMtlNum)=0;

		// Put up a dialog that lets the user edit the plug-ins parameters.
		/*! \remarks This method creates and returns a pointer to a<b>ShaderParamDlg</b> 
		object and puts up the dialog which lets the user edit the Shader's parameters.
		\par Parameters:
		<b>HWND hOldRollup</b>\n\n
		The window handle of the old rollup. If non-NULL the IMtlParams method
		ReplaceRollup method is usually used instead of AddRollup() to present the
		rollup.\n\n
		<b>HWND hwMtlEdit</b>\n\n
		The window handle of the material editor.\n\n
		<b>IMtlParams *imp</b>\n\n
		The interface pointer for calling methods in 3ds Max.\n\n
		<b>StdMtl2* theMtl</b>\n\n
		Points to the Standard material being edited.\n\n
		<b>int rollupOpen</b>\n\n
		TRUE to have the UI rollup open; FALSE if closed.\n\n
		<b>int n=0</b>\n\n
		This parameter is available in release 4.0 and later only.\n\n
		Specifies the number of the rollup to create. Reserved for future use with multiple rollups. */
		virtual ShaderParamDlg* CreateParamDialog(
			HWND hOldRollup, 
			HWND hwMtlEdit, 
			IMtlParams *imp, 
			StdMat2* theMtl, 
			int rollupOpen, 
			int n=0) = 0;

		/*! \remarks This method is available in release 4.0 and later
		only.\n\n
		Returns the number of rollups this shader is requesting.
		\par Default Implementation:
		<b>{ return 1; }</b> */
		virtual int NParamDlgs(){ return 1; }
		/*! \remarks Returns a pointer to the <b>ShaderParamDlg</b> object
		which manages the user interface.
		\par Parameters:
		<b>int n=0</b>\n\n
		This parameter is available in release 4.0 and later only.\n\n
		Specifies the rollup to get <b>ShaderParamDlg</b> for. Reserved for
		future use with multiple rollups. */
		virtual ShaderParamDlg* GetParamDlg(int n=0 )=0;
		/*! \remarks Sets the <b>ShaderParamDlg</b> object which manages the
		user interface to the one passed.
		\par Parameters:
		<b>ShaderParamDlg* newDlg</b>\n\n
		Points to the new ShaderParamDlg object.\n\n
		<b>int n=0</b>\n\n
		This parameter is available in release 4.0 and later only.\n\n
		Specifies the rollup to set <b>ShaderParamDlg</b> for. Reserved for
		future use with multiple rollups.\n\n
		  */
		virtual void SetParamDlg( ShaderParamDlg* newDlg, int n=0 )=0;

		// Saves and loads name. These should be called at the start of
		// a plug-in's save and load methods.
		/*! \remarks Saves the plug-in's name. This should be called at the
		start of a plug-in's <b>Save()</b> method.
		\par Parameters:
		<b>ISave *isave</b>\n\n
		An interface for saving data. */
		IOResult Save(ISave *isave) { return SpecialFX::Save(isave); }
		/*! \remarks Loads the plug-in's name. This should be called at the
		start of a plug-in's <b>Load()</b> method.
		\par Parameters:
		<b>ILoad *iload</b>\n\n
		An interface for loading data. */
		IOResult Load(ILoad *iload) { return SpecialFX::Load(iload); }

		// std parameter support
		/*! \remarks Returns a value which indicates which of the standard
		parameters are supported.
		\return  See
		<a href="ms-its:listsandfunctions.chm::/idx_R_list_of_shader_standard_parameter.html">List of Shader
		Standard Parameter Flags</a>. */
		virtual ULONG SupportStdParams()=0;

		// this method only req'd for R2.5 shaders, to convert stdmtl1 paramblks to current
		/*! \remarks This method is only required for R2.5 shaders to convert
		the previous Standard material parameter blocks to the current version.
		\par Parameters:
		<b>ParamBlockDescID *descOld</b>\n\n
		Points to the old parameter block descriptor.\n\n
		<b>int oldCount</b>\n\n
		The number in the array of parameters above.\n\n
		<b>IParamBlock *oldPB</b>\n\n
		Points to the old parameter block. */
		virtual void ConvertParamBlk( ParamBlockDescID *descOld, int oldCount, IParamBlock *oldPB ){};

		// LOCAL vars of mtl for possible mapping prior to being given to back to illum
		/*! \remarks This method updates the <b>channels</b>(as well as other) data
		member of the <b>IllumParams</b> object passed to it with the <b>local</b>
		variables of the material for possible mapping prior to being given to the
		Shader's <b>Illum()</b> method. The shader plug-in copies the state of all its
		parameters (at their current animation state) into the data members of the
		<b>IllumParams</b> passed.
		\par Parameters:
		<b>IllumParams* ip</b>\n\n
		Points to the IllumParams to update. */
		virtual void GetIllumParams( ShadeContext &sc, IllumParams& ip )=0;

		// actual shader
		/*! \remarks This is the illumination function for the Shader.\n\n
		Developers will find it very helpful to review the <b>Mtl::Shade()</b>
		method of the Standard material. This is the main method of the
		material which computes the color of the point being rendered. This
		code is available in <b>/MAXSDK/SAMPLES/MATERIALS/STDMTL2.CPP</b>. This
		code shows how the Standard calls <b>Shader::GetIllumParams()</b>, sets
		up mapping channels, calls this <b>Illum()</b> method, and calls the
		<b>Shader::CombineComponents()</b> method when all done.
		\par Parameters:
		<b>ShadeContext \&sc</b>\n\n
		The ShadeContext which provides information on the pixel being
		shaded.\n\n
		<b>IllumParams \&ip</b>\n\n
		The object whose data members provide communication between 3ds Max and
		the shader. Input values are read from here and output values are
		stored here. Note that the <b>XOut</b> (<b>ambIllumout</b>, etc) data
		members of this class are initialized to 0 before the first call to
		this method.\n\n
		The input to this method has the textured illumination parameters, the
		bump perturbed normal, the view vector, the raw (unattenuated) colors
		in the reflection and refraction directions, etc.
		\par Sample Code:
		Below is a brief analysis of the standard Blinn shader Illum() method.
		This is the standard 'computer graphics look' type shader supplied with
		3ds Max. The entire method follows:\n\n
		\code
		void Blinn2::Illum(ShadeContext &sc, IllumParams &ip)
		{
			LightDesc *l;
			Color lightCol;
		
		// Blinn style phong
			BOOL is_shiny= (ip.channels[ID_SS].r > 0.0f) ? 1:0;
			double phExp = pow(2.0, ip.channels[ID_SH].r * 10.0) *	4.0;
		
			for (int i=0; i<sc.nLights; i++) {
				l = sc.Light(i);
				register float NL, diffCoef;
				Point3 L;
				if (l->Illuminate(sc,ip.N,lightCol,L,NL,diffCoef)) {
					if (l->ambientOnly) {
						ip.ambIllumOut += lightCol;
						continue;
					}
					if (NL<=0.0f)
						continue;
		
		// diffuse
					if (l->affectDiffuse)
						ip.diffIllumOut += diffCoef * lightCol;
		
		// specular (Phong2)
					if (is_shiny&&l->affectSpecular) {
						Point3 H = Normalize(L-ip.V);
						float c = DotProd(ip.N,H);
						if (c>0.0f) {
							if (softThresh!=0.0 && diffCoef<softThresh) {
								c *= Soften(diffCoef/softThresh);
							}
							c = (float)pow((double)c, phExp);
							ip.specIllumOut += c * ip.channels[ID_SS].r * lightCol;
						}
					}
				}
			}
		
		// Apply mono self illumination
			if ( ! selfIllumClrOn ) {
				float si = ip.channels[ID_SI].r;
				ip.diffIllumOut = (si>=1.0f) ? Color(1.0f,1.0f,1.0f):ip.diffIllumOut * (1.0f-si) + si;
			}
			else {
		// colored self illum,
				ip.selfIllumOut += ip.channels[ID_SI];
			}
		// now we can multiply by the clrs,
			ip.ambIllumOut *= ip.channels[ID_AM];
			ip.diffIllumOut *= ip.channels[ID_DI];
			ip.specIllumOut *= ip.channels[ID_SP];
		
		// the following is applicable only in R4
			int chan = ip.stdIDToChannel[ ID_RL ];
			ShadeTransmission(sc, ip, ip.channels[chan], ip.refractAmt);
			chan = ip.stdIDToChannel[ ID_RR ];
			ShadeReflection( sc, ip, ip.channels[chan] );
			CombineComponents( sc, ip );
		}
		\endcode 
		Some of the key parts of this method are discussed below:\n\n
		The <b>is_shiny</b> line sets a boolean based on if the Shader has a
		shininess setting \> 0. Note that the Blinn shader uses the same
		channel ordering as the original Standard material so it can index its
		channels using the standard ID <b>ID_SS</b>.\n\n
		<b>BOOL is_shiny= (ip.channels[ID_SS].r \> 0.0f) ? 1:0;</b>\n\n
		Next the 'Phong Exponent' is computed. This is just a function that is
		used to give a certain look. It uses 2^(Shinniness *10) * 4. This
		calculation simply 'feels right' and gives a good look.\n\n
		<b>double phExp = pow(2.0, ip.channels[ID_SH].r * 10.0) * 4.0;</b>\n\n
		The following loop sums up the effect of each light on this point on
		surface.\n\n
		<b>for (int i=0; i\<sc.nLights; i++) {</b>\n\n
		Inside the loop, the light descriptor is grabbed from the
		ShadeContext:\n\n
		<b>  l = sc.Light(i);</b>\n\n
		The <b>LightDesc::Illuminate()</b> method is then called to compute
		some data:\n\n
		<b>  if (l-\>Illuminate(sc,ip.N,lightCol,L,NL,diffCoef))
		{</b>\n\n
		To <b>Illuminate()</b> is passed the ShadeContext (<b>sc</b>), and the
		normal to the surface (<b>ip.N</b>) (pointing away from the surface
		point).\n\n
		The method returns the light color (<b>lightCol</b>), light vector
		(<b>L</b>) (which points from the surface point to the light), the dot
		product of N and L (<b>NL</b>) and the diffuse coefficient
		(<b>diffCoef</b>). The diffuse coefficient is similar to NL but has the
		atmosphere between the light and surface point taken into account as
		well.\n\n
		The next piece of code checks if the light figures into the
		computations:\n\n
		<b>   if (NL\<=0.0f)</b>\n\n
		<b>    continue;</b>\n\n
		If NL\<0 then the cosine of the vectors is greater than 90 degrees.
		This means the light is looking at the back of the surface and is
		therefore not to be considered.\n\n
		The next statement checks if the light affect the diffuse channel
		(lights may toggle on or off their ability to affect the diffuse and
		specular channels.)\n\n
		<b>   if (l-\>affectDiffuse)</b>\n\n
		<b>    ip.diffIllumOut += diffCoef *
		lightCol;</b>\n\n
		If the light affects the diffuse channel then the diffuse illumination
		output component of the <b>IllumParams</b> is added to. This is done by
		multiplying the diffuse coefficient (returned by
		<b>LightDesc::Illuminate()</b>) times the light color (also returned by
		<b>LightDesc::Illuminate()</b>). Notice that the <b>diffIllumOut</b> is
		being accumulated with each pass of the light loop.\n\n
		The next section of code involves the specular component. If the light
		is shiny, and it affects the specular channel a vector <b>H</b> is
		computed.\n\n
		<b>   if (is_shiny\&\&l-\>affectSpecular) {</b>\n\n
		Note the following about this <b>H</b> vector computation. Most vectors
		are considered pointing <b>from</b> the point on the surface. The view
		vector (<b>IllumParams::V</b>) does not follow this convention. It
		points from the 'eye' towards the surface. Thus it's reversed from the
		usual convention.\n\n
		<b>H</b> is computed to be the average of <b>L</b> and <b>V</b>. This
		is <b>(L+V)/2</b>. Since we normalize this we don't have to divide by
		the 2. So, if <b>V</b> followed the standard convention this would be
		simply <b>L+V</b>. Since it doesn't it is <b>L+(-ip.V)</b> or
		<b>L-ip.V</b>.\n\n
		<b>    Point3 H = Normalize(L-ip.V);</b>\n\n
		Next the dot product of <b>N</b> and <b>H</b> is computed and stored in
		<b>c</b>. When you take the dot product of two normalized vectors what
		is returned is the cosine of the angle between the vectors.\n\n
		<b>    float c = DotProd(ip.N,H); </b>\n\n
		If c\>0 and the diffuse coefficient is less than the soften threshold
		then <b>c</b> is modified by a 'softening' curve.
		\code
		if (c>0.0f)
		{
			if (softThresh!=0.0 &&
			diffCoef<softThresh) {
				c *=
					Soften(diffCoef/softThresh);
			}
		}
		\endcode  
		Note that the <b>Soften()</b> function is defined in
		<b>/MAXSDK/SAMPLES/MATERIALS/SHADERUTIL.CPP</b> and developers can copy
		this code.
		\code
		c = (float)pow((double)c,phExp);
		\endcode 
		Next, <b>c</b> is raised to the power of the Phong exponent. This is
		effectively taking a cosine (a smooth S curve) and raising it to a
		power. As it is raised to a power it becomes a sharper and sharper S
		curve. This is where the shape of the highlight curve in the Materials
		Editor UI comes from.\n\n
		That completes the pre computations for the specular function. Then
		<b>c</b> is multiplied by the specular strength
		(<b>ip.channels[ID_SS].r</b>) times the light color (<b>lightCol</b>).
		The result is summed in specular illumination out
		(<b>ip.specIllumOut</b>).\n\n
		\code
		ip.specIllumOut += c * ip.channels[ID_SS].r * lightCol;
		\endcode 
		That completes the light loop. It happens over and over for each
		light.\n\n
		Next the self illumunation is computed. If the Self Illumination Color
		is not on, then the original code for doing mono self illumination is
		used.
		\code
		// Apply mono self illumination
		if ( ! selfIllumClrOn )
		{
			float si = ip.channels[ID_SI].r;
			ip.diffIllumOut = (si>=1.0f) ? Color(1.0f,1.0f,1.0f)
				:
			ip.diffIllumOut * (1.0f-si) + si;
		}
		else
		{
		// Otherwise the self illumination color is summed in to the Self Illumination 
			Out (ip.selfIllumOut).
		// colored self illum,
			ip.selfIllumOut += ip.channels[ID_SI];
		}
		\endcode 
		Then, we multiply by the colors for ambient, diffuse and specular.\n\n
		\code
		ip.ambIllumOut *= ip.channels[ID_AM];
		ip.diffIllumOut *= ip.channels[ID_DI];
		ip.specIllumOut *= ip.channels[ID_SP];
		\endcode 
		The results are <b>ambIllumOut</b>, <b>diffIllumOut</b>, and
		<b>specIllumOut</b>. Note that these components are not summed. In R3
		and earlier these results would be returned to the Standard material.
		However, R4 introduces a couple extra steps.\n\n
		Finally, we call <b>ShadeTransmission()</b> and
		<b>ShadeReflection()</b> to apply the transmission/refraction and
		reflection models. The core implementation of 3ds Max provides the
		standard models, but the shader can override these methods to compute
		its own models.\n\n
		\code
		int chan = ip.stdIDToChannel[ ID_RL ];
		ShadeTransmission(sc, ip, ip.channels[chan],ip.refractAmt);
		chan = ip.stdIDToChannel[ ID_RR ];
		ShadeReflection( sc, ip, ip.channels[chan] );
		\endcode 
		In R4, It is a shader's responsibility to combine the components of the
		shading process prior to exiting <b>Illum()</b> (in R3, this was the
		responsibility of the Standard material). In order to combine these
		values together to produce the final color for that point on the
		surface (<b>IllumParams.finalC</b>), the shader should call
		<b>CombineComponents()</b> method. The Shader base class provides a
		default implementation which simply sums everything together and
		multiplies by the opacity.\n\n
		\code
		virtual void CombineComponents( IllumParams& ip )
		{
			ip.finalC = ip.finalOpac * (ip.ambIllumOut + ip.diffIllumOut + 	ip.selfIllumOut)
				+ ip.specIllumOut + ip.reflIllumOut + ip.transIllumOut ;
		}
		\endcode */
		virtual void Illum(ShadeContext &sc, IllumParams &ip)=0;

// begin - ke/mjm - 03.16.00 - merge reshading code
		// these support the pre-shade/reshade protocol
//		virtual void PreIllum(ShadeContext &sc, IReshadeFragment* pFrag){}
//		virtual void PostIllum(ShadeContext &sc, IllumParams &ip, IReshadeFragment* pFrag ){ Illum(sc,ip); }

		// >>>> new for V4, one call superceded, 2 new ones added
		/*! \remarks This method is available in release 4.0 and later
		only.\n\n
		Compute the reflected color from the <b>sc</b>, <b>ip</b>, and
		reflection map (or ray) color. The core implementation of this provides
		the standard 3ds Max reflection model. To support the standard
		reflection model, a shader may call this default implementation.
		\par Parameters:
		<b>ShadeContext\& sc</b>\n\n
		The context which provides information on the pixel being shaded.\n\n
		<b>IllumParams\& ip</b>\n\n
		The object whose data members provide communication between 3ds Max and
		the shader.\n\n
		<b>Color \&mapClr</b>\n\n
		The input reflection (or ray) color is passed in here and the resulting
		'affected' color is stored here. */
		virtual void ShadeReflection(ShadeContext &sc, IllumParams &ip, Color &mapClr){}
		/*! \remarks This method is available in release 4.0 and later
		only.\n\n
		Compute the transmission/refraction color for the sample.. The core
		implementation of this provides the standard 3ds Max reflection model.
		To support the standard transmission/refraction model, a shader may
		call this default implementation.
		\par Parameters:
		<b>ShadeContext\& sc</b>\n\n
		The context which provides information on the pixel being shaded.\n\n
		<b>IllumParams\& ip</b>\n\n
		The object whose data members provide communication between 3ds Max and
		the shader.\n\n
		<b>Color \&mapClr</b>\n\n
		The input refraction (or ray) color is passed in here and the resulting
		'affected' color is stored here.\n\n
		<b>float amount</b>\n\n
		The level of the amount spinner for the refraction channel. */
		virtual void ShadeTransmission(ShadeContext &sc, IllumParams &ip, Color &mapClr, float amount){}
		// orphaned, replaced by ShadeReflection()
		/*! \remarks Note: This method has been superceded by
		<b>ShadeReflection()</b> and is <b>obsolete</b> in release 4.0 and
		later.\n\n
		This method provides the shader with an opportunity to affect the
		reflection code.
		\par Parameters:
		<b>ShadeContext \&sc</b>\n\n
		The ShadeContext which provides information on the pixel being
		shaded.\n\n
		<b>IllumParams \&ip</b>\n\n
		The object whose data members provide communication between 3ds Max and
		the shader.\n\n
		<b>Color \&rcol</b>\n\n
		The input reflection color is passed in here and the resulting
		'affected' color is stored here.
		\par Sample Code:
		A simple example like Phong does the following:\n\n
		\code
		void AffectReflection(ShadeContext &sc, IllumParams &ip, Color &rcol)
		{
			rcol *= ip.channels[ID_SP];
		};
		\endcode 
		If a color can affect the reflection of light off a surface than it can
		usually affect the reflection of other things off a surface. Thus some
		shaders influence the reflection color using the specular color and
		specular level. For instance the Multi Layer Shader does the
		following:\n\n
		\code
		#define DEFAULT_GLOSS2 0.03f
		void MultiLayerShader::AffectReflection(ShadeContext &sc, IllumParams &ip, Color &rcol)
		{
			float axy = DEFAULT_GLOSS2;
			float norm = 1.0f / (4.0f * PI * axy );
			rcol *= ip.channels[_SPECLEV1].r * ip.channels[_SPECCLR1] * norm;
		}
		\endcode  */
		virtual void AffectReflection(ShadeContext &sc, IllumParams &ip, Color &rcol){}

// end - ke/mjm - 03.16.00 - merge reshading code

		/*! \remarks This method does the final compositing of the various
		illumination components. A default implementation is provided which
		simply adds the components together. Developers who want to do other
		more specialized composition can override this method. For example, a
		certain Shader might want to composited highlights over the underlying
		diffuse component since the light is reflected and the diffuse color
		wouldn't fully show through. Such a Shader would provide its own
		version of this method.
		\par Parameters:
		<b>ShadeContext \&sc</b>\n\n
		The ShadeContext which provides information on the pixel being
		shaded.\n\n
		<b>IllumParams\& ip</b>\n\n
		The illumination parameters to composite and store.
		\par Default Implementation:
		\code
		virtual void CombineComponents(IllumParams& ip)
		{
			ip.finalC = ip.finalOpac * (ip.ambIllumOut + ip.diffIllumOut + ip.selfIllumOut)
				+ ip.specIllumOut + ip.reflIllumOut + ip.transIllumOut;
		}
		\endcode  */
		virtual void CombineComponents( ShadeContext &sc, IllumParams& ip ){};

		// texture maps
		/*! \remarks Returns the number of texture map map channels supported
		by this Shader. */
		virtual long nTexChannelsSupported()=0;
		/*! \remarks Returns the name of the specified texture map channel.
		\par Parameters:
		<b>long nTextureChan</b>\n\n
		The zero based index of the texture map channel whose name is returned.
		*/
		virtual MSTR GetTexChannelName( long nTextureChan )=0;
		/*! \remarks Returns the internal name of the specified texture map.
		The Standard material uses this to get the fixed, parsable internal
		name for each texture channel it defines.
		\par Parameters:
		<b>long nTextureChan</b>\n\n
		The zero based index of the texture map whose name is returned.
		\par Default Implementation:
		<b>{ return GetTexChannelName(nTextureChan); }</b> */
		virtual MSTR GetTexChannelInternalName( long nTextureChan ) { return GetTexChannelName(nTextureChan); }
		/*! \remarks Returns the channel type for the specified texture map
		channel. There are four channels which are part of the Material which
		are not specific to the Shader. All other channels are defined by the
		Shader (what they are and what they are called.) The four which are not
		the province of the Shader are Bump, Reflection, Refraction and
		Displacement. For example, Displacement mapping is really a geometry
		operation and not a shading one. The channel type returned from this
		method indicates if the specified channel is one of these, or if it is
		a monochrome channel, a color channel, or is not a supported channel.
		\par Parameters:
		<b>long nTextureChan</b>\n\n
		The zero based index of the texture map whose name is returned.
		\return  Texture channel type flags. One or more of the following
		values:\n\n
		<b>UNSUPPORTED_CHANNEL</b>\n\n
		Indicates the channel is not supported (is not used).\n\n
		<b>CLR_CHANNEL</b>\n\n
		A color channel. The <b>Color.r</b>, <b>Color.g</b> and <b>Color.b</b>
		parameters are used.\n\n
		<b>MONO_CHANNEL </b>\n\n
		A monochrome channel. Only the <b>Color.r</b> is used.\n\n
		<b>BUMP_CHANNEL</b>\n\n
		The bump mapping channel.\n\n
		<b>REFL_CHANNEL</b>\n\n
		The reflection channel.\n\n
		<b>REFR_CHANNEL</b>\n\n
		The refraction channel.\n\n
		<b>DISP_CHANNEL</b>\n\n
		The displacement channel.\n\n
		<b>ELIMINATE_CHANNEL</b>\n\n
		Indicates that the channel is not supported. For example, a certain
		Shader might not support displacement mapping for some reason. If it
		didn't, it could use this channel type to eliminate the support of
		displacement mapping for itself. It would be as if displacement mapping
		was not included in the material. None of the 3ds Max shaders use
		this.\n\n
		<b>SKIP_CHANNELS</b>\n\n
		This is used internally to indicate that the channels to be skipped. */
		virtual long ChannelType( long nTextureChan )=0;
		// map StdMat Channel ID's to the channel number
		/*! \remarks Returns the index of this Shader's channels which
		corresponds to the specified Standard materials texture map ID. This
		allows the Shader to arrange its channels in any order it wants in the
		<b>IllumParams::channels</b> array but enables the Standard material to
		access specific ones it needs (for instance the Bump channel or
		Reflection channel).
		\par Parameters:
		<b>long stdID</b>\n\n
		The ID whose corresponding channel to return. See \ref Material_TextureMap_IDs "List of Material Texture Map Indices".
		\return  The zero based index of the channel. If there is not a
		corresponding channel return -1.
		\par Sample Code:
		This can be handled similar to below where an array is initialized with
		the values of this plug-in shader's channels that correspond to each of
		the standard channels. Then this method just returns the correspond
		index from the array.\n\n
		\code
		static int stdIDToChannel[N_ID_CHANNELS] =
		{
			0, 1, 2, 5, 4, -1, 7, 8, 9, 10, 11, 12
		};
		long StdIDToChannel(long stdID){ return stdIDToChannel[stdID]; }
		\endcode 
		*/
		virtual long StdIDToChannel( long stdID )=0;

		// Shader Uses these UserIllum output channels 
		virtual long nUserIllumOut(){ return 0; } // number of channels it will use
		// static name array for matching by render elements
		virtual MCHAR** UserIllumNameArray(){ return NULL; } // static name of each channel

		/*! \remarks This method is called when the Shader is first activated
		in the dropdown list of Shader choices. The Shader should reset itself
		to its default values. */
		virtual void Reset()=0;	//reset to default values

	};

// Chunk IDs saved by base class
#define SHADERBASE_CHUNK	0x39bf
#define SHADERNAME_CHUNK	0x0100


/////////////////////////////////////////////////////////////////////////////
//
//	Standard params for shaders
//
// combination of these is returned by Shader.SupportStdParams()
#define STD_PARAM_NONE			(0)
#define STD_PARAM_ALL			(0xffffffffL)
#define STD_PARAM_METAL			(1)
#define STD_PARAM_LOCKDS		(1<<1)
#define STD_PARAM_LOCKAD		(1<<2)
#define STD_PARAM_LOCKADTEX		(1<<3)
#define STD_PARAM_SELFILLUM		(1<<4)
#define STD_PARAM_SELFILLUM_CLR	(1<<5)
#define STD_PARAM_AMBIENT_CLR	(1<<6)
#define STD_PARAM_DIFFUSE_CLR	(1<<7)
#define STD_PARAM_SPECULAR_CLR	(1<<8)
#define STD_PARAM_FILTER_CLR	(1<<9)
#define STD_PARAM_GLOSSINESS	(1<<10)
#define STD_PARAM_SOFTEN_LEV	(1<<11)
#define STD_PARAM_SPECULAR_LEV	(1<<12)
#define STD_PARAM_DIFFUSE_LEV	(1<<13)
#define STD_PARAM_DIFFUSE_RHO	(1<<14)
#define STD_PARAM_ANISO			(1<<15)
#define STD_PARAM_ORIENTATION	(1<<16)
#define STD_PARAM_REFL_LEV		(1<<17)
#define STD_PARAM_SELFILLUM_CLR_ON		(1<<18)

#define STD_BASIC2_DLG			(1<<20)
#define STD_EXTRA_DLG			(1<<21)

// not including these 3 in yr param string disables the relevant params 
// in extra params dialog
#define STD_EXTRA_REFLECTION	(1<<22)
#define STD_EXTRA_REFRACTION	(1<<23)
#define STD_EXTRA_OPACITY		(1<<24)

#define STD_EXTRA	(STD_EXTRA_DLG \
					+STD_EXTRA_REFLECTION+STD_EXTRA_REFRACTION \
					+STD_EXTRA_OPACITY )

#define STD_BASIC	(0x00021ffeL | STD_BASIC2_DLG)

#ifndef USE_LIMITED_STDMTL // orb 01-14-2002
#define STD_BASIC_METAL	(0x00021fffL | STD_BASIC2_DLG)
#define STD_ANISO	(0x0002cffe)
#define STD_MULTILAYER	(0x0002fffe)
#define STD_ONB		(0x00023ffe)
#define STD_WARD	(0x00000bce)
#endif // USE_LIMITED_STDMTL


///////////////////////////////////////////////////////////////////////////////

/*! \sa  Class BaseShader, Class MacroRecorder.\n\n
\par Description:
This class is available in release 3.0 and later only.\n\n
This is the class that developers use to create Shader plug-ins. Developers
must implement the methods of this class to provide data to the 3ds Max
interactive renderer so it can properly reflect the look of the shader in the
viewports. The methods associated with the actual Shader illumination code are
from the base class <b>BaseShader</b>.\n\n
There are various Get and Set methods defined in this class. Plug-in developers
provide implementations for the 'Get' methods which are used by the interactive
renderer. The implementations of the 'Set' methods are used when switching
between shaders types in the Materials Editor. This is used to transfer the
corresponding colors between the old Shader and the new one.\n\n
Note that some shaders may not have the exact parameters as called for in the
methods. In those case an approximate value may be returned from the 'Get'
methods. For example, the Strauss Shader doesn't have an Ambient channel. In
that case the Diffuse color is taken and divided by 2 and returned as the
Ambient color. This gives the interactive renderer something to work with that
might not be exact but is somewhat representative.  */
class Shader : public BaseShader, public IReshading  {
	public:
	/*! \remarks This method copies the standard shader parameters from
	<b>pFrom</b> to this object. Note that plug-ins typically disable the macro
	recorder during this operation as the Get and Set methods are called. See
	the sample code for examples.
	\par Parameters:
	<b>Shader* pFrom</b>\n\n
	The source parameters. */
	virtual void CopyStdParams( Shader* pFrom )=0;
	// these are the standard shader params
	/*! \remarks Sets the state of the Diffuse / Specular lock to on or off.
	\par Parameters:
	<b>BOOL lock</b>\n\n
	TRUE for on; FALSE for off. */
	virtual void SetLockDS(BOOL lock)=0;
	/*! \remarks Returns TRUE if the Diffuse / Specular lock is on; otherwise
	FALSE. */
	virtual BOOL GetLockDS()=0;
	/*! \remarks Sets the state of the Ambient / Diffuse lock to on or off.
	\par Parameters:
	<b>BOOL lock</b>\n\n
	TRUE for on; FALSE for off. */
	virtual void SetLockAD(BOOL lock)=0;
	/*! \remarks Returns TRUE if the Ambient / Diffuse lock is on; otherwise
	FALSE. */
	virtual BOOL GetLockAD()=0;
	/*! \remarks Sets the state of the Ambient / Diffuse Texture lock to on or
	off.
	\par Parameters:
	<b>BOOL lock</b>\n\n
	TRUE for on; FALSE for off. */
	virtual void SetLockADTex(BOOL lock)=0;
	/*! \remarks Returns TRUE if the Ambient / Diffuse Texture lock is on;
	otherwise FALSE. */
	virtual BOOL GetLockADTex()=0;

	/*! \remarks Sets the Self Illumination parameter to the specified value
	at the time passed.
	\par Parameters:
	<b>float v</b>\n\n
	The value to set.\n\n
	<b>TimeValue t</b>\n\n
	The time to set the value. */
	virtual void SetSelfIllum(float v, TimeValue t)=0;		
	/*! \remarks Sets the Self Illumination Color On/Off state.
	\par Parameters:
	<b>BOOL on</b>\n\n
	TRUE for on; FALSE for off. */
	virtual void SetSelfIllumClrOn( BOOL on )=0;
	/*! \remarks Sets the Self Illumination Color at the specified time.
	\par Parameters:
	<b>Color c</b>\n\n
	The color to set.\n\n
	<b>TimeValue t</b>\n\n
	The time to set the color. */
	virtual void SetSelfIllumClr(Color c, TimeValue t)=0;		

	/*! \remarks Sets the Ambient Color at the specified time.
	\par Parameters:
	<b>Color c</b>\n\n
	The color to set.\n\n
	<b>TimeValue t</b>\n\n
	The time to set the color. */
	virtual void SetAmbientClr(Color c, TimeValue t)=0;		
	/*! \remarks Sets the Diffuse Color at the specified time.
	\par Parameters:
	<b>Color c</b>\n\n
	The color to set.\n\n
	<b>TimeValue t</b>\n\n
	The time to set the color. */
	virtual void SetDiffuseClr(Color c, TimeValue t)=0;		
	/*! \remarks Sets the Specular Color at the specified time.
	\par Parameters:
	<b>Color c</b>\n\n
	The color to set.\n\n
	<b>TimeValue t</b>\n\n
	The time to set the color. */
	virtual void SetSpecularClr(Color c, TimeValue t)=0;
	/*! \remarks Sets the Glossiness parameter to the specified value at the
	time passed.
	\par Parameters:
	<b>float v</b>\n\n
	The value to set.\n\n
	<b>TimeValue t</b>\n\n
	The time to set the value. */
	virtual void SetGlossiness(float v, TimeValue t)=0;		
	/*! \remarks Sets the Specular Level parameter to the specified value at
	the time passed.
	\par Parameters:
	<b>float v</b>\n\n
	The value to set.\n\n
	<b>TimeValue t</b>\n\n
	The time to set the value. */
	virtual void SetSpecularLevel(float v, TimeValue t)=0;		
	/*! \remarks Sets the Soften Specular Highlights Level to the specified
	value at the time passed.
	\par Parameters:
	<b>float v</b>\n\n
	The value to set.\n\n
	<b>TimeValue t</b>\n\n
	The time to set the value. */
	virtual void SetSoftenLevel(float v, TimeValue t)=0;
		
	/*! \remarks Returns TRUE if the Self Illumination Color setting is on
	(checked); FALSE if off.
	\par Parameters:
	The parameters to this method are not applicable and may safely be ignored.
	*/
	virtual BOOL IsSelfIllumClrOn(int mtlNum, BOOL backFace)=0;
	/*! \remarks Returns the Ambient Color.
	\par Parameters:
	The parameters to this method are not applicable and may safely be ignored.
	*/
	virtual Color GetAmbientClr(int mtlNum, BOOL backFace)=0;		
    /*! \remarks Returns the Diffuse Color.
    \par Parameters:
    The parameters to this method are not applicable and may safely be ignored.
    */
    virtual Color GetDiffuseClr(int mtlNum, BOOL backFace)=0;		
	/*! \remarks Returns the Specular Color.
	\par Parameters:
	The parameters to this method are not applicable and may safely be ignored.
	*/
	virtual Color GetSpecularClr(int mtlNum, BOOL backFace)=0;
	/*! \remarks Returns the Self Illumination Color.
	\par Parameters:
	The parameters to this method are not applicable and may safely be ignored.
	*/
	virtual Color GetSelfIllumClr(int mtlNum, BOOL backFace)=0;
	/*! \remarks Returns the Self Illumination Amount.
	\par Parameters:
	The parameters to this method are not applicable and may safely be ignored.
	*/
	virtual float GetSelfIllum(int mtlNum, BOOL backFace)=0;
	/*! \remarks Returns the Glossiness Level.
	\par Parameters:
	The parameters to this method are not applicable and may safely be ignored.
	*/
	virtual float GetGlossiness(int mtlNum, BOOL backFace)=0;	
	/*! \remarks Returns the Specular Level.
	\par Parameters:
	The parameters to this method are not applicable and may safely be ignored.
	*/
	virtual float GetSpecularLevel(int mtlNum, BOOL backFace)=0;
	/*! \remarks Returns the Soften Level as a float.
	\par Parameters:
	The parameters to this method are not applicable and may safely be ignored.
	*/
	virtual float GetSoftenLevel(int mtlNum, BOOL backFace)=0;

	/*! \remarks Returns TRUE if the Self Illumination Color setting is on
	(checked); FALSE if off. */
	virtual BOOL IsSelfIllumClrOn()=0;
	/*! \remarks Returns the Ambient Color at the specified time.
	\par Parameters:
	<b>TimeValue t</b>\n\n
	The time at which to return the color. */
	virtual Color GetAmbientClr(TimeValue t)=0;		
	/*! \remarks Returns the Diffuse Color at the specified time.
	\par Parameters:
	<b>TimeValue t</b>\n\n
	The time at which to return the color. */
	virtual Color GetDiffuseClr(TimeValue t)=0;		
	/*! \remarks Returns the Specular Color at the specified time.
	\par Parameters:
	<b>TimeValue t</b>\n\n
	The time at which to return the color. */
	virtual Color GetSpecularClr(TimeValue t)=0;
	/*! \remarks Returns the Glossiness value at the specified time.
	\par Parameters:
	<b>TimeValue t</b>\n\n
	The time at which to return the value. */
	virtual float GetGlossiness( TimeValue t)=0;		
	/*! \remarks Returns the Specular Level at the specified time.
	\par Parameters:
	<b>TimeValue t</b>\n\n
	The time at which to return the value. */
	virtual float GetSpecularLevel(TimeValue t)=0;
	/*! \remarks Returns the Soften Specular Highlights setting at the
	specified time.
	\par Parameters:
	<b>TimeValue t</b>\n\n
	The time at which to return the value. */
	virtual float GetSoftenLevel(TimeValue t)=0;
	/*! \remarks Returns the Self Illumination Amount at the specified time.
	\par Parameters:
	<b>TimeValue t</b>\n\n
	The time at which to return the value. */
	virtual float GetSelfIllum(TimeValue t)=0;		
	/*! \remarks Returns the Self Illumination Color at the specified time.
	\par Parameters:
	<b>TimeValue t</b>\n\n
	The time at which to return the color. */
	virtual Color GetSelfIllumClr(TimeValue t)=0;		

	/*! \remarks This method is called to evaluate the hightlight curve that
	appears in the Shader user interface.\n\n
	Note: This gets called from the <b>DrawHilite()</b> function which is
	available to developers in
	<b>/MAXSDK/SAMPLES/MATERIALS/SHADER/SHADERUTIL.CPP</b>. <b>DrawHilite()</b>
	get called from the window proc <b>HiliteWndProc()</b> in the same file.
	This code is available to developers to use in their Shader dialog procs.
	\par Parameters:
	<b>float x</b>\n\n
	The input value.
	\return  The output value on the curve. A value of 1.0 represents the top
	of the curve as it appears in the UI. Values greater than 1.0 are okay and
	simply appear off the top of the graph.
	\par Default Implementation:
	<b>{ return 0.0f; }</b> */
	virtual float EvalHiliteCurve(float x){ return 0.0f; }
	/*! \remarks This is the highlight curve function for the two highlight
	curves which intersect and appear in the UI, for instance in the Anistropic
	shader.
	\par Parameters:
	<b>float x</b>\n\n
	The x input value.\n\n
	<b>float y</b>\n\n
	The y input value.\n\n
	<b>int level = 0</b>\n\n
	This is used by multi-layer shaders to indicate which layer to draw. The
	draw highlight curve routines use this when redrawing the graph.
	\return  The output value of the curve.
	\par Default Implementation:
	<b>{ return 0.0f; }</b> */
	virtual float EvalHiliteCurve2(float x, float y, int level = 0 ){ return 0.0f; }

	// the Max std way of handling reflection and Transmission is
	// implemented here to provide the default implementation.
	CoreExport void ShadeReflection(ShadeContext &sc, IllumParams &ip, Color &mapClr);
	CoreExport void ShadeTransmission(ShadeContext &sc, IllumParams &ip, Color &mapClr, float amount);

	// Reshading
	void PreShade(ShadeContext& sc, IReshadeFragment* pFrag){}
	void PostShade(ShadeContext& sc, IReshadeFragment* pFrag, int& nextTexIndex, IllumParams* ip){ Illum( sc, *ip ); }

	// [dl | 13march2003] Adding this inlined definition to resolve compile errors
    BaseInterface* GetInterface(Interface_ID id) { return BaseShader::GetInterface(id); }
	void* GetInterface(ULONG id){
		if( id == IID_IReshading )
			return (IReshading*)( this );
	//	else if ( id == IID_IValidityToken )
	//		return (IValidityToken*)( this );
		else
			return BaseShader::GetInterface(id);
	}
};



#endif