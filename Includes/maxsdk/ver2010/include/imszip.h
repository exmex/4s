/*	
 *		iMSZip.h - Public interface to MAXScript Zip Package Manager
 *
 *			Copyright © Autodesk, Inc, 2000.  John Wainwright.
 *
 */

#ifndef _H_IMSZIP
#define _H_IMSZIP

class IMSZipMgr;

#define MSZIP_MGR_INTERFACE   Interface_ID(0x26492c82, 0x10fc5210)
inline IMSZipMgr* GetMSZipMgr() { return (IMSZipMgr*)GetCOREInterface(MSZIP_MGR_INTERFACE); }

// class IMSZipMgr
//    MAXScript Zip Package manager interface 
/*! \sa  Class FPStaticInterface,  Class MacroEntry\n\n
\par Description:
This class is available in release 4.0 and later only.\n\n
This class represents the interface to the MAXScript Zip Package Manager. You
can obtain a pointer to the Browser Manager interface using; <b>IMSZipMgr*
GetMSZipMgr()</b>. This macro will return
<b>(IMSZipMgr*)GetCOREInterface(MSZIP_MGR_INTERFACE).</b>\n\n
All methods of this class are Implemented by the System.  */
class IMSZipMgr : public FPStaticInterface 
{
public:
	// function IDs 
	enum { fileInPackage,
		   unloadPackage,
		}; 

	/*! \remarks This method will unload and run a zip package and return the
	extract_dir.
	\par Parameters:
	<b>MCHAR* file_name</b>\n\n
	The file name of the ZIP script package.\n\n
	<b>MSTR\& extract_dir</b>\n\n
	The directory in which the files were extracted.
	\return  TRUE if successful, otherwise FALSE. */
	virtual BOOL FileInPackage(const MCHAR* file_name, MSTR& extract_dir)=0;	// unload & run zip package, return extract-to directory
	/*! \remarks This method will unload the package while ignoring any drop or
	run commands and return the extract_dir and any primary drop file. If the
	primary dropFile is a *.ds, compile it in context of the package and return the
	MacroEntry in dropScript.
	\par Parameters:
	<b>MCHAR* file_name</b>\n\n
	The file name of the ZIP script package.\n\n
	<b>MSTR\& extract_dir</b>\n\n
	The directory in which the files were extracted.\n\n
	<b>MSTR\& dropFile</b>\n\n
	The primary drop file.\n\n
	<b>MacroEntry*\& dropScript</b>\n\n
	The resulting MacroEntry.
	\return  TRUE if successful, otherwise FALSE. */
	virtual BOOL UnloadPackage(const MCHAR* file_name, MSTR& extract_dir, 
							   MSTR& dropFile, MacroEntry*& dropScript)=0; // just unload the package, ignore any drop or run commands,
																	    //   return extract-to directory and any primary drop file
																		//   if the primary dopFile is a .ds, compile it in context of
																		//   package and return MAcroEntry* for it in 'dropScript'
}; 

#endif
