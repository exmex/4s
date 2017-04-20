#ifndef TCML_C_INTERFACE
#define TCML_C_INTERFACE


#define MAX_TCML_FACE_NAME						32
#define MAX_TCML_SYMBOL							1024
#define MAX_STACK_COMPDESC_DEPTH				1024

//Common styles
#define TS_NONE								((unsigned int)0x00000000)
#define TS_FOCUSABLE						((unsigned int)0x00000001)
#define TS_CUSTOM_COLOR						((unsigned int)0x00000002)

//Button styles
#define TBS_STATE_BUTTON					((unsigned int)0x00000010)
#define TBS_NOTIFY_DBLCLICK					((unsigned int)0x00000020)

//Scroll styles
#define TSS_VSCROLL							((unsigned int)0x00000100)
#define TSS_HSCROLL							((unsigned int)0x00000200)

//Gauge styles
#define TGS_GROW_RIGHT						((unsigned int)0x00001000)
#define TGS_GROW_LEFT						((unsigned int)0x00002000)
#define TGS_GROW_UP							((unsigned int)0x00004000)
#define TGS_GROW_DOWN						((unsigned int)0x00008000)

//Meter styles
#define TMS_GROW_RIGHT						TGS_GROW_RIGHT
#define TMS_GROW_LEFT						TGS_GROW_LEFT
#define TMS_GROW_UP							TGS_GROW_UP
#define TMS_GROW_DOWN						TGS_GROW_DOWN

//Frame styles
#define TFS_FLOAT							((unsigned int)0x00010000)

//Edit styles 
#define	TES_PASSWORD						((unsigned int)0x00100000)
#define	TES_NOCONMODE					((unsigned int)0x00200000)

#pragma warning(disable:4786)

/*
	Component descriptor
*/
#define TCML_ID_NULL						((unsigned int)-1)
#define TCML_FLAG_NULL						((unsigned char)0x00)

#define ALIGN_CUSTOM						((unsigned char)0x01)
#define ALIGN_LEFT							((unsigned char)0x02)
#define ALIGN_RIGHT							((unsigned char)0x04)
#define ALIGN_CENTER						((unsigned char)0x08)

#define TCML_BLT_FIT						((unsigned char)0x01)
#define TCML_BLT_CENTER						((unsigned char)0x02)
#define TCML_BLT_TILE						((unsigned char)0x04)

#define TCML_IDX_DEFAULT					0
#define TCML_IDX_ENABLE						0
#define TCML_IDX_DISABLE					1

#define TCML_TYPE_COMPONENT					((unsigned char)0x00)
#define TCML_TYPE_BUTTON					((unsigned char)0x01)
#define TCML_TYPE_SCROLL					((unsigned char)0x02)
#define TCML_TYPE_EDIT						((unsigned char)0x03)
#define TCML_TYPE_COMBO						((unsigned char)0x04)
#define TCML_TYPE_LIST						((unsigned char)0x05)
#define TCML_TYPE_GAUGE						((unsigned char)0x06)
#define TCML_TYPE_TABCTRL					((unsigned char)0x08)
#define TCML_TYPE_FRAME						((unsigned char)0x09)
#define TCML_TYPE_IMAGELIST					((unsigned char)0x0A)
#define TCML_TYPE_METER						((unsigned char)0x0B)
#define TCML_TYPE_HTTPCTRL					((unsigned char)0x0C)

/*
	Component notify message
*/
#define TNM_BASE							((unsigned int) 0x0000)
#define TNM_LCLICK							((unsigned int) TNM_BASE + 1)
#define TNM_RCLICK							((unsigned int) TNM_BASE + 2)
#define TNM_DBLCLICK						((unsigned int) TNM_BASE + 3)
#define TNM_LINEUP							((unsigned int) TNM_BASE + 4)
#define TNM_LINEDOWN						((unsigned int) TNM_BASE + 5)
#define TNM_VSCROLL							((unsigned int) TNM_BASE + 6)
#define TNM_HSCROLL							((unsigned int) TNM_BASE + 7)
#define TNM_SEL_CHANGE						((unsigned int) TNM_BASE + 8)
#define TNM_TAB_ACTIVE						((unsigned int) TNM_BASE + 9)
#define TNM_ENTER							((unsigned int) TNM_BASE + 10)
#define TNM_ESC								((unsigned int) TNM_BASE + 11)

#define TCML_MENU_COUNT						(TNM_ESC + 1)


typedef struct _tagButtonDesc
{
	unsigned int up;
	unsigned int down;	
	unsigned int hover;
}BUTTONDESC, *LP_BUTTONDESC;

typedef struct _tagScrollDesc
{
	unsigned int bar;
	unsigned int ul;
	unsigned int dr;
}SCROLLDESC, *LP_SCROLLDESC;

typedef struct _tagEditDesc
{
	unsigned int caret;
}EDITDESC, *LP_EDITDESC;

#define MAX_LIST_COLUMN						15
typedef struct _tagListDesc
{
	unsigned int item[MAX_LIST_COLUMN];
	unsigned int column;
	unsigned int vs;
}LISTDESC, *LP_LISTDESC;

typedef struct _tagComboDesc
{
	unsigned int drop;
	unsigned int downlist;
}COMBODESC, *LP_COMBODESC;

#define MAX_GAUGE_BAR							10
typedef struct _tagGauge
{
	unsigned int bar[MAX_GAUGE_BAR];
	unsigned int count;
}GAUGEDESC, *LP_GAUGEDESC;

#define MAX_METER_SUBLEVEL						7

typedef struct _tagMeter
{
	unsigned int sub[MAX_METER_SUBLEVEL];
	unsigned int super;
	unsigned int sublevel;
	unsigned int superlevel;	
}METERDESC, *LP_METERDESC;

typedef union _tagTypeSpec
{		
	GAUGEDESC		gauge;
	LISTDESC		list;
	EDITDESC		edit;
	METERDESC		meter;
	COMBODESC		combo;
	SCROLLDESC		scroll;
	BUTTONDESC		button;
}TSATR;//Component type specific attributes

typedef struct _tagCompDesc
{
	unsigned int		id;
	unsigned char		type;						//Component type

	unsigned int		sound;
	unsigned int		color;
	unsigned int		menu[TCML_MENU_COUNT];		//Game command
	unsigned int		images[2];					//Default images id -Enable, Disable

	int					hmargine;
	int					vmargine;
	int					x;							//Coordinates...
	int					y;
	int					width;
	int					height;

	unsigned char		align;
	unsigned char		display;	

	char				text[MAX_TCML_SYMBOL];		//Component text
	char				tooltip[MAX_TCML_SYMBOL];	//Tooltip
	unsigned int		face;						//Font ID
	unsigned int		tipface;					//Tooltip Font ID

	unsigned int		style;						//Type specific styles
	TSATR				ex;							//Type specific attributes

	struct _tagCompDesc *child;
	struct _tagCompDesc *next;	
}COMPDESC,*LP_COMPDESC;

typedef struct _tagStackCompDesc
{
	int	top;
	LP_COMPDESC can[MAX_STACK_COMPDESC_DEPTH];
}STACK_COMPDESC;

typedef struct _tagIdDefinition
{
	unsigned int				id;
	char						name[MAX_TCML_SYMBOL];
	struct _tagIdDefinition *	next;
}IDEF, *LPIDEF;

typedef struct _tagTcmlLogFont
{
	long tlfHeight;
	long tlfWidth;
	long tlfEscapement;
	long tlfOrientation; 
	long tlfWeight; 
	char tlfItalic; 
	char tlfUnderline; 
	char tlfStrikeOut; 
	char tlfCharSet; 
	char tlfOutPrecision ; 
	char tlfClipPrecision ; 
	char tlfQuality; 
	char tlfPitchAndFamily; 
	char tlfFaceName[32];

	unsigned long tlfId;
	unsigned long tlfColor;
	struct _tagTcmlLogFont *next;
}TCML_LOGFONT,* LP_TCML_LOGFONT;

int InitTCMLInterface();
void FiniTCMLInterface();

LP_COMPDESC CompDescNew();
void ZeroCompDesc(LP_COMPDESC ptr);
void CompDescDel(LP_COMPDESC pDesc);

LPIDEF IDefNew();
void IDefDel(LPIDEF ptr);

LP_COMPDESC cds_pop();
int cds_push(LP_COMPDESC ptr);

int add_frame();
int set_attr(int attr);
void SetFrameAsTCMLCurrent();
int add_component(unsigned char type);
int close_current_component(unsigned char type);

void add_tcml_logfont();
int set_font_attr(int attr);

void define(char *name, unsigned int id);
unsigned int find_idef(char *name);

void reset_lex();
void clean_lex_buf();

#endif
