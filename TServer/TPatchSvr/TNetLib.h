#if !defined __TNETLIB_H
#define __TNETLIB_H

#include <winsock2.h>
#include <mswsock.h>
#include <atlbase.h>
#include <atlstr.h>

// STL header
#include <string>
#include <vector>
#include <queue>
#include <map>

using namespace ATL;
using namespace std;

// DB Access header
#include <SQL.h>
#include <SQLExt.h>

#include <SqlDatabase.h>
#include <SqlBase.h>
#include <BindDesc.h>
#include <SqlQuery.h>
#include <SqlDirect.h>

// Network access header
#include <Packet.h>
#include <Session.h>
#include <TNetDef.h>
#include <ErrorCode.h>

#endif // !defined __TNETLIB_H
