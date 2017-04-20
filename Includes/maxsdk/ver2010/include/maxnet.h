//-----------------------------------------------------------------------------
// -------------------
// File ....: maxnet.h
// -------------------
// Author...: Gus J Grubba
// Date ....: February 2000
//
// Descr....: 3D Studio MAX Network Interface
//
// History .: Feb, 07 2000 - Started
//            
//-----------------------------------------------------------------------------

#ifndef _MAXNET_H_
#define _MAXNET_H_

#include <strbasic.h>
#include "maxheap.h"

#ifdef WIN32
#ifndef MAXNETEXPORT
#define MAXNETEXPORT __declspec( dllimport )
#endif
#else
#define MAXNETEXPORT
#endif

//-----------------------------------------------------------------------------
//-- MaxNet Errors

typedef enum {
	MAXNET_ERR_NONE = 0,
	MAXNET_ERR_CANCEL,
	MAXNET_ERR_NOMEMORY,
	MAXNET_ERR_FILEIO,
	MAXNET_ERR_BADARGUMENT,
	MAXNET_ERR_NOTCONNECTED,
	MAXNET_ERR_NOTREADY,
	MAXNET_ERR_IOERROR,
	MAXNET_ERR_CMDERROR,
	MAXNET_ERR_HOSTNOTFOUND,
	MAXNET_ERR_BADSOCKETVERSION,
	MAXNET_ERR_WOULDBLOCK,
	MAXNET_ERR_SOCKETLIMIT,
	MAXNET_ERR_CONNECTIONREFUSED,
	MAXNET_ERR_ACCESSDENIED,
	MAXNET_ERR_TIMEOUT,
	MAXNET_ERR_BADADDRESS,
	MAXNET_ERR_HOSTUNREACH,
	MAXNET_ERR_DUPLICATE_JOB_NAME,
	MAXNET_ERR_UNKNOWN
} maxnet_error_t;

//---------------------------------------------------------
//-- Special Types

/*! \sa  Class MaxNetManager
\remarks This structure is available in release 4.0 and later only. \n\n
This structure is used by the Network Rendering API as a handle to a server.
*/
struct HSERVER: public MaxHeapOperators {
	/*! The handle to a server by it’s mac (Ethernet) address. */
	BYTE addr[8];
};

#define HBSERVER	(BYTE *)(void *) 
typedef DWORD		HJOB;

//---------------------------------------------------------
//-- Server Work Schedule

#define HIGHPRIORITY		0
#define LOWPRIORITY 		1
#define IDLEPRIORITY		2

/*! \sa Class MaxNetManager, Structure WeekSchedule
\remarks This structure is available in release 4.0 and later only.\n\n
This structure is used by the Network Rendering API to store hourly scheduling information.
*/
struct Schedule: public MaxHeapOperators {
	/*! This bit map represents the hourly schedule where 24 bits represent the hours.
	A bit set to 0 indicates it’s allowed to work, a bit set to 1 indicates
	it’s not allowed to work.
	*/
	DWORD hour;
};

/*! \sa Class MaxNetManager, Structure Schedule
\remarks This structure is available in release 4.0 and later only. \n\n
This structure is used by the Network Rendering API to store weekly scheduling information.
*/
struct WeekSchedule: public MaxHeapOperators {
	/*! The hourly schedule configuration for each day of the week. */
	Schedule	day[7];
	/*! The attended priority value which is one of the following values: HIGHPRIORITY, LOWPRIORITY, or IDLEPRIORITY. */
	int		AttendedPriority;
	/*! The unattended priority value which is one of the following values: HIGHPRIORITY, LOWPRIORITY, or IDLEPRIORITY. */
	int		UnattendedPriority;
};

//---------------------------------------------------------
//-- Network Status
//

/*! \sa Class MaxNetManager
\remarks This structure is available in release 4.0 and later only. \n\n
This structure is used by the Network Rendering API to store network status information.
*/
struct NetworkStatus: public MaxHeapOperators {
	/*! The number of packets dropped due to buffer overflows. */
	DWORD		dropped_packets;
	/*! The number of bad formed packets. */
	DWORD		bad_packets;
	/*! The total number of TCP requests since boot time. */
	DWORD		tcprequests;
	/*! The total number of UDP requests since boot time. */
	DWORD		udprequests;
	/*! The system boot time. */
	SYSTEMTIME	boot_time;
	/*! Reserved for future use. */
	char		reserved[32];
};

//---------------------------------------------------------
//-- Station Configuration Block
//

/*! \sa Class MaxNetManager, Structure ManagerInfo, Structure ServerInfo, Structure ClientInfo
\remarks This structure is available in release 4.0 and later only. \n\n
This structure is used by the Network Rendering API to store information about the system.
*/
struct ConfigurationBlock: public MaxHeapOperators {
	/*! The system’s total physical memory, retrieved through GlobalMemoryStatus(). */
	SIZE_T dwTotalPhys;			   //-- GlobalMemoryStatus();
	/*! The number of processors in the system, retrieved through GetSystemInfo(). */
	DWORD	dwNumberOfProcessors;	//-- GetSystemInfo();
	/*! The system’s major version, retrieved through GetVersionEx(). */
	DWORD	dwMajorVersion;			//-- GetVersionEx();
	/*! The system’s minor version, retrieved through GetVersionEx(). */
	DWORD	dwMinorVersion;			//-- GetVersionEx();
	/*! The system’s build number, retrieved through GetVersionEx(). */
	DWORD	dwBuildNumber;			//-- GetVersionEx();
	/*! The system’s platform ID, retrieved through GetVersionEx(). */
	DWORD	dwPlatformId;			//-- GetVersionEx();
	/*! The system’s CSD version, retrieved through GetVersionEx(). */
	MCHAR	szCSDVersion[128];		//-- GetVersionEx();
	/*! The logged in user’s name, retrieved through GetUserName(). */
	MCHAR	user[MAX_PATH];			//-- GetUserName();
	/*! The temporary directory, retrieved through ExpandEnvironmentStrings(). */
	MCHAR	tempdir[MAX_PATH];		//-- ExpandEnvironmentStrings()
	/*! The name of the computer, retrieved through GetComputerName(). */
	MCHAR	name[MAX_PATH];			//-- GetComputerName()
	/*! The disk, indexed by drive letter, used for server files such as
	incoming jobs. The index is alphabetical, disk A = 0, disk B = 1, etc. */
	char	workDisk;				//-- Disk used for Server files (incomming jobs, etc. A = 0, B = 1, etc)
	/*! The bit map representing the available disks. 
	Disk A = 0x1, B = 0x2, C = 0x4, etc. */
	DWORD	disks;					//-- Available disks (bitmap A=0x1, B=0x2, C=0x4, etc)
	/*! The space available on disks in MegaBytes, indexed by 
	drive letter. Disk A = diskSpace[0], B = diskSpace[1], etc. */
	DWORD	diskSpace[26];			//-- Space available on disks in MegaBytes (A=diskSpace[0], B=diskSpace[1], etc.)
	/*! The computer NIC hardware address (00:00:00:00:00:00), 
	which is 6 bytes and 2 padding bytes. */
	BYTE	mac[8];					//-- Computer NIC address (00:00:00:00:00:00) 6 bytes + 2 padding
	/*! Reserved for future use. */
	char	reserved[32];
};

//---------------------------------------------------------
//-- Manager Info
//

#define _MANAGER_INFO_VERSION 400

/*! \sa Class MaxNetManager, Structure NetworkStatus, Structure ConfigurationBlock
\remarks This structure is available in release 4.0 and later only. \n\n
This structure is used by the Network Rendering API to store information about a Manager.
*/
struct ManagerInfo: public MaxHeapOperators {
	/*! The size of the structure, being sizeof(ManagerInfo). */
	DWORD			size;
	/*! The version information, defined by _MANAGER_INFO_VERSION. */
	DWORD			version;
	/*! The network system configuration data. Refer to this structure for more information. */
	ConfigurationBlock	cfg;
	/*! The network status information. */
	NetworkStatus		net_status;
	/*! The number of servers registered. */
	int			servers;
	/*! The number of jobs. */
	int			jobs;
	/*! Reserved for future use. */
	char			reserved[32];
};

//---------------------------------------------------------
//-- Server Info
//

#define _SERVER_INFO_VERSION 400

/*! \sa Class MaxNetManager, Structure NetworkStatus, Structure ConfigurationBlock
\remarks This structure is available in release 4.0 and later only. \n\n
This structure is used by the Network Rendering API to store information about a Server.
*/
struct ServerInfo: public MaxHeapOperators {
	/*! The size of the structure, being sizeof(ServerInfo). */
	DWORD				size;
	/*! The version information, defined by _SERVER_INFO_VERSION. */
	DWORD				version;
	/*! The total number of frames rendered. */
	DWORD				total_frames;
	/*! The total time spent rendering, in hours. */
	float				total_time;
	/*! Performance index */
	float				index;
	/*! The network system configuration data. Refer to this structure for more information. */
	ConfigurationBlock	cfg;
	/*! The network status information. */
	NetworkStatus		net_status;
	/*! Reserved for future use. */
	char				reserved[32];
};

//---------------------------------------------------------
//-- Client Info
//

#define _CLIENT_INFO_VERSION 400

/*! \sa Class MaxNetManager, Structure ConfigurationBlock
\remarks This structure is available in release 4.0 and later only. \n\n
This structure is used by the Network Rendering API to store information about a Client.
*/
struct ClientInfo: public MaxHeapOperators {
	/*! The size of the structure, being sizeof(ClientInfo). */
	DWORD				size;
	/*! The version information, defined by _CLIENTINFO_VERSION. */
	DWORD				version;
	/*! The network system configuration data. Refer to this structure for more information. */
	ConfigurationBlock	cfg;
	/*! TRUE if the Client is currently controlling the queue. */
	bool				controller;
	/*! The UDP port being used for network communications. */
	short		  		udp_port;
	/*! Reserved for future use. */
	char				reserved[32];
};

//-------------------------------------------------------------------
//-- Global Server State
//

#define SERVER_STATE_ABSENT    0
#define SERVER_STATE_IDLE      1
#define SERVER_STATE_BUSY      2
#define SERVER_STATE_ERROR     3
#define SERVER_STATE_SUSPENDED 4

/*! \sa Class MaxNetManager, Structure HSERVER, Structure ServerInfo
\remarks This structure is available in release 4.0 and later only.\n\n
This structure is used by the Network Rendering API to store global 
server state information.\n\n
If <b>hJob</b> is a valid handle (i.e. non 0) and <b>frame</b> holds a <b>NO_FRAME</b>,
this means this server has just be assigned a job or it is in between
frames (no frames assigned). Most likely it is loading 3ds Max. The
transition in between frames is in the nanosecond level, but seeing
as it is possible, it cannot be discarded.
*/
struct ServerList: public MaxHeapOperators {
	/*! The handle to the server. */
	HSERVER		hServer;
	/*! The current state of the server which is one of the following values:
	- SERVER_STATE_ABSENT   \n
	The server is absent.
	- SERVER_STATE_IDLE \n
	The server is idle.
	- SERVER_STATE_BUSY \n
	The server is busy.
	- SERVER_STATE_ERROR \n
	The server is experiencing an error.
	- SERVER_STATE_SUSPENDED \n
	The server has been suspended. */
	WORD		state;
	/*! The server information structure containing the server details. */
	ServerInfo	info;
	//-- Current Task
	/*! The handle to the job that the server is currently working on, if there is one. */
	HJOB  		hJob;			//-- It will be 0 if no current task is defined
	/*! The frame the server is currently rendering, if there is one. */
	int		frame;			//-- It will be NO_FRAME if loading job (no frames yet assigned)
	/*! The time the server had the frame assigned. */
	SYSTEMTIME	frame_started;
};

//---------------------------------------------------------
//-- Server Statistics
//

struct Statistics: public MaxHeapOperators {
	float		tseconds;
	int			frames;
};

//-------------------------------------------------------------------
//-- Servers in Job Queue -------------------------------------------
//
//   Server Information for a given Job

#define JOB_SRV_IDLE		0		//-- Idle
#define JOB_SRV_BUSY		1		//-- Busy
#define JOB_SRV_FAILED		2		//-- Render Error
#define JOB_SRV_ABSENT		3		//-- Absent
#define JOB_SRV_SUSPENDED	4		//-- Out of work schedule
#define JOB_SRV_BUSYOTHER	5		//-- Busy with another job
#define JOB_SRV_ERROR		6		//-- Connection Error
#define JOB_SRV_COOL_OFF	7		//-- In Error Recovery

/*! \sa Class MaxNetManager, Structure HSERVER
\remarks This structure is available in release 4.0 and later only. \n\n
This structure is used by the Network Rendering API to store information about the servers in the job queue.
*/
struct JobServer: public MaxHeapOperators {
	/*! The handle to the server. */
	HSERVER		hServer;
	/*! The current state of the server, which is one of the following values:
	- JOB_SRV_IDLE \n
	The server is idle.
	- JOB_SRV_BUSY \n
	The server is busy.
	- JOB_SRV_FAILED  \n
	The server is has failed (rendering error).
	- JOB_SRV_ABSENT \n
	The server is absent.
	- JOB_SRV_SUSPENDED \n
	The server is suspended, out of work schedule.
	- JOB_SRV_BUSYOTHER    \n
	The server is busy with another job.
	- JOB_SRV_ERROR \n
	The server experienced a connection error.
	- JOB_SRV_COOL_OFF \n
	The server is "cooling off" (i.e. in error recovery).
	*/
	char	  	status;
	/*! This variable is used internally. */
	bool		failed;
	/*! Indicates that the server is active in the job. */
	bool		active;
	/*! The frame which is currently being rendered. */
	int		cur_frame;			
	/*! The total hours the server has spent rendering. */
	float		thours;
	/*! The total number of frames the server has rendered. */
	int		frames;
};

//-----------------------------------------------------------------------------
//-- Job Frame State

#define NO_FRAME				0x0FFFFFFF

#define FRAME_WAITING   		0
#define FRAME_ASSIGNED  		1
#define FRAME_COMPLETE  		2

/*! \sa Class MaxNetManager, Structure HSERVER
\remarks This structure is available in release 4.0 and later only. \n\n
This structure is used by the Network Rendering API to store a job’s frame progress information.
*/
struct JOBFRAMES: public MaxHeapOperators {
	/*! The current state of this frame which is one of the following values:
	- FRAME_WAITING \n
	The frame is waiting to be assigned and rendered.
	- FRAME_ASSIGNED  \n
	The frame is assigned to the server.
	- FRAME_COMPLETE  \n
	The frame has completed rendering.
	- NO_FRAME \n
	There are no frames to be rendered.
	*/
	char	state;
	/*! The frame number. */
	int	frame;
	/*! The handle to the server rendering this frame. */
	HSERVER	hServer;
	/*! The time it took to render this frame, in milliseconds. */
	DWORD	elapsed;
};

//-----------------------------------------------------------------------------
//-- MaxNet Class (Exception Handler)
//

/*! \sa  Class MaxNetManager, <a href="ms-its:listsandfunctions.chm::/idx_R_list_of_maxnet_errors.html">List of MaxNet Errors</a>\n\n
\par Description:
This class is available in release 4.0 and later only.\n\n
The MaxNet class serves as the base class for MaxNetManager and should be
treated as the exception handler when using the Network Rendering API.  */
class MAXNETEXPORT MaxNet: public MaxHeapOperators {
	protected:
		int	gerror;
		maxnet_error_t	error;
		maxnet_error_t	TranslateError	(int err);
	public:
						/*! \remarks Constructor */
						MaxNet			();
		/*! \remarks This method returns the MaxNet error. See the list of
		MaxNet error codes for details. */
		maxnet_error_t	GetError		();
		/*! \remarks This method returns the MaxNet error. See the list of
		MaxNet error codes for details. */
		int				GetGError		();
		/*! \remarks This method returns the MaxNet error description string.
		*/
		const MCHAR*	GetErrorText	();
};

#include <maxnet_platform.h>
#include <maxnet_job.h>
#include <maxnet_file.h>
#include <maxnet_archive.h>
#include <maxnet_manager.h>

//-----------------------------------------------------------------------------
//-- Interface
/*! \remarks This method will create and return a new instance of the
MaxNetManager class. */
MAXNETEXPORT MaxNetManager*	CreateManager			( );
/*! \remarks This method will destroy an instance of the MaxNetManager class.
\par Parameters:
<b>MaxNetManager* mgr</b>\n\n
Points to the MaxNetManager object to destroy.*/
MAXNETEXPORT void			DestroyManager			(MaxNetManager* mgr);
//-- Initializes a "Job" structure
/*! \remarks This function will initialize a job structure using the data read
from a specified 3ds Max scene file. If you would want to submit a job based on
a 3ds Max file alone, you would call this function passing it to the file
specified. This function takes care of filling all the fields so you can turn
around and just send the job to the queue. An example of this can be found in
the SDK under <b>/MAXSDK/SAMPLES/NETRENDER/JOBASSIGN</b>.
\par Parameters:
<b>char* filename</b>\n\n
The filename of the 3ds Max scene file (*.max).\n\n
<b>Job* job</b>\n\n
A pointer to an empty job structure which will be initialized by the method.
Because the function will initialize the structure any values present prior to
calling this method will be reset.\n\n
<b>CJobText\& jobText</b>\n\n
A reference to an empty CJobText class which will be initialized by the
function. Just like the job structure, any values present prior to calling this
function will be reset.
\return  TRUE if reading the properties was successful, otherwise FALSE. */
MAXNETEXPORT bool			jobReadMAXProperties	(char* max_filename, Job* job, CJobText& jobText);
//-- Reads Render Data from a *.max file and fills in a Job structure
/*! \remarks This function will set the default values for the given Job
structure. You can use this function to prevent frequent resetting of the
structure and its default fields such as size, version, etc.
\par Parameters:
<b>Job *job</b>\n\n
The job structure containing the information about the job. See the Job
structure description for an explanation. */
MAXNETEXPORT void			jobSetJobDefaults		(Job* job);
//-- Utilities
MAXNETEXPORT void			NumberedFilename		(MCHAR* infile, MCHAR* outfile, int number);
MAXNETEXPORT bool			IsMacNull				(BYTE *addr);
MAXNETEXPORT bool			GetMacAddress			(BYTE* addr);
MAXNETEXPORT bool			MatchMacAddress			(BYTE* addr1, BYTE* addr2);
MAXNETEXPORT void			Mac2String				(BYTE* addr, MCHAR* string );
MAXNETEXPORT void			Mac2StringCondensed		(BYTE* addr, MCHAR* string );
MAXNETEXPORT void			StringCondensed2Mac		(MCHAR* string, BYTE* addr);
MAXNETEXPORT void			InitConfigurationInfo	(ConfigurationBlock &cb, MCHAR workdisk = 0);
MAXNETEXPORT bool			MatchServers			(HSERVER srv1, HSERVER srv2);
/*! \remarks This function creates a Network Rendering archive. This is the
archive sent to the manager when submitting a new job. Note that even though
you can use whatever name you feel like, the Manager and Servers will look for
a "jobname.maz" file.
\par Parameters:
<b>MCHAR* archivename</b>\n\n
The full path and filename of the archive you want to create.\n\n
<b>MCHAR* file_list</b>\n\n
A list of NULL terminated filenames to include in the archive. You should
provide a full path and filename. However, only the file names will be saved in
the archive and all files are going to be un-archived in the same
directory.\n\n
An example: file_list[] =
{"c:\\path\\file.maz/0c:\\anotherpath\\maps.tga/0/0"};\n\n
<b>DWORD* filesize</b>\n\n
Optional pointer to a variable to receive the accumulated size of all files
included in the archive. This is the "uncompressed" size. You can use this to
compute the disk space necessary to uncompress the archive.
\return  TRUE if the archive was successfully created, otherwise FALSE. */
MAXNETEXPORT bool			Maz						(MCHAR* archivename, MCHAR* file_list, DWORD* filesize = 0);
/*! \remarks This function is the opposite of <b>Maz()</b> and will un-archive
the given archive into the specified directory.
\par Parameters:
<b>MCHAR* archivename</b>\n\n
The full path and filename of the archive you want to un-archive.\n\n
<b>MCHAR* output_path</b>\n\n
The path you want the files extracted to.
\return  TRUE if the archive was successfully extracted, otherwise FALSE. */
MAXNETEXPORT bool			UnMaz					(MCHAR* archivename, MCHAR* output_path);
//-- Localization Resources
MAXNETEXPORT MCHAR*			ResString				(int id, MCHAR* buffer = 0);
//-- Backburner helper
MAXNETEXPORT bool			ConvertOldJobFile		(MCHAR* oldFile, MCHAR* newFile);

enum MAXNET_PLATFORM { 
	MAXNET_PLATFORM_NATIVE = 0,
	MAXNET_PLATFORM_32BIT = 32,
	MAXNET_PLATFORM_64BIT =64
};
#ifndef NO_MAXNET_PLATFORM_OVERRIDE
	MAXNETEXPORT MaxNetManager* CreateManager		(int platform);
	MAXNETEXPORT bool			ConvertOldJobFile	(MCHAR* oldFile, MCHAR* newFile, int platform);
#endif

#endif

//-- EOF: maxnet.h ------------------------------------------------------------
