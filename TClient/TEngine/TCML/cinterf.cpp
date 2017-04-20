#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cinterf.h"
#include "TCMLParser.h"

LPIDEF TCMLIDef =0;
LP_COMPDESC TCMLFrame=0;
STACK_COMPDESC TCMLStack;
LP_COMPDESC TCMLCurrent = 0;
unsigned char TCMLCurrentType = 0;

LP_TCML_LOGFONT TCMLFont = 0;
LP_TCML_LOGFONT TCMLFontCur = 0;

LP_TCML_LOGFONT TcmlLogFontNew()
{
	LP_TCML_LOGFONT ptr = (LP_TCML_LOGFONT)malloc(sizeof(TCML_LOGFONT));
	memset(ptr, 0, sizeof(TCML_LOGFONT));

	return ptr;
}

void add_tcml_logfont()
{
	LP_TCML_LOGFONT ptr = TcmlLogFontNew();
	if(ptr)
	{
		LP_TCML_LOGFONT it = TCMLFont;
		if(!it)
			TCMLFont = ptr;
		else
		{
			LP_TCML_LOGFONT last = 0;
			while(it)
			{
				last = it;
				it = it->next;
			}
			if(last)
				last->next = ptr;
		}

		TCMLFontCur = ptr;
	}	
}

int InitTCMLInterface()
{
	memset(&TCMLStack, 0, sizeof(STACK_COMPDESC));
	TCMLStack.top = -1;	

	return 0;
}

void SetTCMLCurrent(LP_COMPDESC ptr)
{
	TCMLCurrent = ptr;
	TCMLCurrentType = ptr->type;
}

void FontDel(LP_TCML_LOGFONT ptr)
{
	while(ptr)
	{
		LP_TCML_LOGFONT next = ptr->next;
		free(ptr);
		ptr = next;
	}
}

void FiniTCMLInterface()
{
	reset_lex();
	clean_lex_buf();

	IDefDel(TCMLIDef);
	FontDel(TCMLFont);
	CompDescDel(TCMLFrame);
}

LP_COMPDESC cds_pop()
{
	if( TCMLStack.top >= 0 && 
		TCMLStack.top < MAX_STACK_COMPDESC_DEPTH)
	{
		return TCMLStack.can[TCMLStack.top--];
	}
	return NULL;
}

int cds_push(LP_COMPDESC ptr)
{
	if( TCMLStack.top < MAX_STACK_COMPDESC_DEPTH -1)
	{
		TCMLStack.can[++TCMLStack.top] = ptr;
		return TCMLStack.top +1;
	}

	return -1;
}

LP_COMPDESC CompDescNew()
{
	LP_COMPDESC ptr = (LP_COMPDESC)malloc(sizeof(COMPDESC));
	if(!ptr) return NULL;

	ZeroCompDesc(ptr);
	return ptr;	
}

void ZeroCompDesc( LP_COMPDESC ptr)
{
	memset( ptr, 0, sizeof(COMPDESC));

	ptr->id = TCML_ID_NULL;
	for( int i=0; i<TCML_MENU_COUNT; i++)
		ptr->menu[i] = TCML_ID_NULL;

	for(int i=0; i<2; i++)
		ptr->images[i] = TCML_ID_NULL;

	memset( &ptr->ex, TCML_ID_NULL, sizeof(TSATR));
}

void CompDescDel(LP_COMPDESC pDesc)
{
	if(pDesc && pDesc->child)
		CompDescDel(pDesc->child);
	if(pDesc && pDesc->next)
		CompDescDel(pDesc->next);

	if(pDesc)
		free(pDesc);
}

int add_frame()
{
	LP_COMPDESC ptr = CompDescNew();
	if(!ptr) return -1;
	
	ptr->type = TCML_TYPE_FRAME; 

	LP_COMPDESC it = TCMLFrame;
	if(!it)
		TCMLFrame = ptr;
	else
	{
		LP_COMPDESC last = 0;
		while(it)
		{
			last = it;
			it = it->next;
		}

		if(last)
			last->next = ptr;
	}

	SetTCMLCurrent(ptr);
	return 0;
}

int add_component(unsigned char type)
{
	LP_COMPDESC ptr = CompDescNew();
	if(!ptr) return -1;
	ptr->type = type;

	if(TCMLCurrent)
	{
		LP_COMPDESC it = TCMLCurrent->child;
		if(!it)
			TCMLCurrent->child = ptr;
		else
		{
			LP_COMPDESC last = 0;
			while(it)
			{	
				last = it;
				it = it->next;
			}

			if(last)
				last->next = ptr;
		}

		cds_push(TCMLCurrent);
	}
	
	SetTCMLCurrent(ptr);	
	return 0;
}

int close_current_component(unsigned char type)
{
	LP_COMPDESC ptr = cds_pop();
	if(!ptr)
		return -1;

	SetTCMLCurrent(ptr);
	return 0;
}

LPIDEF IDefNew()
{
	LPIDEF ptr = (LPIDEF) malloc(sizeof(IDEF));

	if(ptr)
		memset(ptr, 0, sizeof(IDEF));

	return ptr;
}

void IDefDel(LPIDEF ptr)
{
	while(ptr)
	{
		LPIDEF next = ptr->next;
		free(ptr);
		ptr = next;
	}
}

void define(char *name, unsigned int id)
{
	LPIDEF ptr = IDefNew();
	if(ptr)
	{		
		if(!TCMLIDef)
		    TCMLIDef = ptr;
		else
		{
			LPIDEF it = TCMLIDef;
			LPIDEF last = 0;
			while(it)
			{
				last = it;
				it = it->next;
			}

			last->next = ptr;
		}

		ptr->id = id;
		strcpy(ptr->name, name);
	}
}

unsigned int find_idef(char *name)
{
	LPIDEF it = TCMLIDef;
	while(it)
	{
		if(0 == strcmp(it->name, name))
			return it->id;

		it = it->next;
	}

	return TCML_ID_NULL;
}

int set_font_attr(int attr)
{
	if(0==TCMLFontCur) return -1;
	switch(attr)
	{
	case ID				: TCMLFontCur->tlfId = find_idef(TCMLlval.string_value);					break;
	case HEIGHT			: TCMLFontCur->tlfHeight = TCMLlval.integer_value;							break;
	case WIDTH			: TCMLFontCur->tlfWidth = TCMLlval.integer_value;							break;
	case ESCAPEMENT		: TCMLFontCur->tlfEscapement = TCMLlval.integer_value;						break;
	case ORIENTATION	: TCMLFontCur->tlfOrientation = TCMLlval.integer_value;						break;
	case WEIGHT			: TCMLFontCur->tlfWeight = TCMLlval.integer_value;							break;
	case ITALIC			: TCMLFontCur->tlfItalic = (char)TCMLlval.integer_value;					break;
	case UNDERLINE		: TCMLFontCur->tlfUnderline = (char)TCMLlval.integer_value;					break;
	case STRIKEOUT		: TCMLFontCur->tlfStrikeOut = (char)TCMLlval.integer_value;					break;
	case CHARSET		: TCMLFontCur->tlfCharSet = (char)TCMLlval.integer_value;					break;
	case OUTPRECISION	: TCMLFontCur->tlfOutPrecision = (char)TCMLlval.integer_value;				break;
	case CLIPPRECISION	: TCMLFontCur->tlfClipPrecision = (char)TCMLlval.integer_value;				break;
	case QUALITY		: TCMLFontCur->tlfQuality = (char)TCMLlval.integer_value;					break;
	case PITCHANDFAMILY	: TCMLFontCur->tlfPitchAndFamily = (char)TCMLlval.integer_value;			break;
	case FACENAME		: strcpy(TCMLFontCur->tlfFaceName, TCMLlval.string_value);					break;
	case FACECLR		: TCMLFontCur->tlfColor = (unsigned long)TCMLlval.integer_value;			break;
	default				: return -1;
	}

	return 0;
}

int set_attr(int attr)
{
	if(0==TCMLCurrent) return-1;

	//printf(":set_attr %d %s\n", attr, TCMLlval.string_value);
	switch(attr)
	{
	case ID:		TCMLCurrent->id = find_idef(TCMLlval.string_value);								break;
	case LCLICK:	TCMLCurrent->menu[TNM_LCLICK] = find_idef(TCMLlval.string_value);				break;
	case RCLICK:	TCMLCurrent->menu[TNM_RCLICK] = find_idef(TCMLlval.string_value);				break;
	case DBLCLICK:	TCMLCurrent->menu[TNM_DBLCLICK] = find_idef(TCMLlval.string_value);				break;
	case LINEUP:	TCMLCurrent->menu[TNM_LINEUP] = find_idef(TCMLlval.string_value);				break;
	case LINEDOWN:	TCMLCurrent->menu[TNM_LINEDOWN] = find_idef(TCMLlval.string_value);				break;
	case VSCROLL:	TCMLCurrent->menu[TNM_VSCROLL] = find_idef(TCMLlval.string_value);				break;
	case HSCROLL:	TCMLCurrent->menu[TNM_HSCROLL] = find_idef(TCMLlval.string_value);				break;
	case SELCHANGE:	TCMLCurrent->menu[TNM_SEL_CHANGE] = find_idef(TCMLlval.string_value);			break;
	case TABACTIVE:	TCMLCurrent->menu[TNM_TAB_ACTIVE] = find_idef(TCMLlval.string_value);			break;
	case ENTER:		TCMLCurrent->menu[TNM_ENTER] = find_idef(TCMLlval.string_value);				break;
	case ESC:		TCMLCurrent->menu[TNM_ESC] = find_idef(TCMLlval.string_value);					break;
	case SOUND:		TCMLCurrent->sound = TCMLlval.integer_value;									break;
	case HMARGINE:	TCMLCurrent->hmargine = TCMLlval.integer_value;									break;
	case VMARGINE:	TCMLCurrent->vmargine = TCMLlval.integer_value;									break;
	case POSX:		TCMLCurrent->x = TCMLlval.integer_value;										break;
	case POSY:		TCMLCurrent->y = TCMLlval.integer_value;										break;
	case WIDTH:		TCMLCurrent->width = TCMLlval.integer_value;									break;
	case HEIGHT:	TCMLCurrent->height = TCMLlval.integer_value;									break;
	case ENABLE:
	case DEFAULT:	TCMLCurrent->images[TCML_IDX_DEFAULT] = find_idef(TCMLlval.string_value);		break;
	case DISABLE:	TCMLCurrent->images[TCML_IDX_DISABLE] = find_idef(TCMLlval.string_value);		break;
	case ALIGN:	
		if(0==strcmp(TCMLlval.string_value, "ALIGN_CUSTOM"))
			TCMLCurrent->align = ALIGN_CUSTOM;
		else if(0==strcmp(TCMLlval.string_value, "ALIGN_LEFT"))
			TCMLCurrent->align = ALIGN_LEFT;
		else if(0==strcmp(TCMLlval.string_value, "ALIGN_RIGHT"))
			TCMLCurrent->align = ALIGN_RIGHT;
		else if(0==strcmp(TCMLlval.string_value, "ALIGN_CENTER"))
			TCMLCurrent->align = ALIGN_CENTER;
		else 
			return -2;
		
		break;
	case BLT: 
		if(0==strcmp(TCMLlval.string_value, "TCML_BLT_FIT"))
			TCMLCurrent->display = TCML_BLT_FIT;
		else if(0==strcmp(TCMLlval.string_value, "TCML_BLT_CENTER"))
			TCMLCurrent->display = TCML_BLT_CENTER;
		else if(0==strcmp(TCMLlval.string_value, "TCML_BLT_TILE"))
			TCMLCurrent->display = TCML_BLT_TILE;
		else
			return -2;
		break;
	case FONT:			TCMLCurrent->face = find_idef(TCMLlval.string_value);							break;
	case TIPFONT:		TCMLCurrent->tipface = find_idef(TCMLlval.string_value);						break;
	case COLOR:
		{
			TCMLCurrent->color = TCMLlval.integer_value;
			TCMLCurrent->style |= TS_CUSTOM_COLOR;
		}

		break;

	//Button
	case UP:			TCMLCurrent->ex.button.up = find_idef(TCMLlval.string_value);					break;
	case DOWN:			TCMLCurrent->ex.button.down = find_idef(TCMLlval.string_value);					break;	
	case HOVER:			TCMLCurrent->ex.button.hover = find_idef(TCMLlval.string_value);				break;
	case STATEBUTTON:
		if( TCMLlval.integer_value )
			TCMLCurrent->style |= TBS_STATE_BUTTON;
		else
			TCMLCurrent->style &= (~TBS_STATE_BUTTON);
		break;
	//Scroll
	case UPLEFT:		TCMLCurrent->ex.scroll.ul = find_idef(TCMLlval.string_value);					break;
	case DOWNRIGHT:		TCMLCurrent->ex.scroll.dr = find_idef(TCMLlval.string_value);					break;
	case BAR: 			
		if( TCMLCurrentType == TCML_TYPE_SCROLL)
			TCMLCurrent->ex.scroll.bar = find_idef(TCMLlval.string_value);	
		else if( TCMLCurrentType == TCML_TYPE_GAUGE)
		{
			if( TCMLCurrent->ex.gauge.count == TCML_ID_NULL )
				TCMLCurrent->ex.gauge.count = 0;
			if( TCMLCurrent->ex.gauge.count >= MAX_GAUGE_BAR )
				return -2;
			TCMLCurrent->ex.gauge.bar[TCMLCurrent->ex.gauge.count++] = find_idef(TCMLlval.string_value);
		}
		break;

	//Edit
	case CARET:			TCMLCurrent->ex.edit.caret = find_idef(TCMLlval.string_value);					break;
	case PASSWORD:		
		if( TCMLlval.integer_value )
			TCMLCurrent->style |= TES_PASSWORD;
		break;
	//List
	case ITEM:		
		if(TCMLCurrent->ex.list.column == TCML_ID_NULL)
			TCMLCurrent->ex.list.column = 0;
		if(TCMLCurrent->ex.list.column >= MAX_LIST_COLUMN )
			return -2;
		TCMLCurrent->ex.list.item[TCMLCurrent->ex.list.column++] = find_idef(TCMLlval.string_value);
		break;
	case VERTICAL: 	TCMLCurrent->ex.list.vs = find_idef(TCMLlval.string_value);						break;
	//Combo
	case DROP: 		TCMLCurrent->ex.combo.drop = find_idef(TCMLlval.string_value);					break;
	case DOWNLIST:	TCMLCurrent->ex.combo.downlist  = find_idef(TCMLlval.string_value);				break;
	//METER
	case SUB:
		if(TCMLCurrent->ex.meter.sublevel == TCML_ID_NULL) 
			TCMLCurrent->ex.meter.sublevel = 0;
		if(TCMLCurrent->ex.meter.sublevel >= MAX_METER_SUBLEVEL) 
			return -2;
		TCMLCurrent->ex.meter.sub[TCMLCurrent->ex.meter.sublevel++] = find_idef(TCMLlval.string_value);	
		break;
	case SUPER:		TCMLCurrent->ex.meter.super = find_idef(TCMLlval.string_value);					break;
	case SUPERLEVEL:TCMLCurrent->ex.meter.superlevel = TCMLlval.integer_value;						break;
	case TOOLTIP:	strcpy(TCMLCurrent->tooltip, TCMLlval.string_value);							break;
	case TEXT_DEF:	strcpy(TCMLCurrent->text, TCMLlval.string_value);								break;
	default:	return -1;
	}

	return 0;
}
