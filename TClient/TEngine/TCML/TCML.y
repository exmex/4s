
%{
#include "cinterf.h"
#include <stdio.h>

#include <string.h>

extern int TCMLerror(char* s);
extern int TCMLlex();

extern int tcml_attr;
extern unsigned char tcml_type;
extern char tcml_identifier[MAX_TCML_SYMBOL];

#ifdef _DEBUG
	#define TCML_TRACE				printf
#else
	#define TCML_TRACE				//
#endif
%}


/*
 * Declare the type of values in the grammar
 */

%union 
{
	char string_value[MAX_TCML_SYMBOL];
	int integer_value;
}



/*
 * Token types: These are returned by the lexer
 */

%token STRING INT_NUM
%token BRACKET_OPEN BRACKET_CLOSE SLASH IDENTIFIER SET
%token TCML HEAD FRAME COMPONENT BUTTON SCROLL EDIT LIST COMBO TAB TABCTRL GAUGE IMAGELIST METER
%token DEFINE FONT TIPFONT COLOR ESCAPEMENT ORIENTATION WEIGHT ITALIC UNDERLINE STRIKEOUT CHARSET OUTPRECISION CLIPPRECISION QUALITY PITCHANDFAMILY FACENAME FACECLR
%token ID LCLICK RCLICK DBLCLICK LINEUP LINEDOWN VSCROLL HSCROLL SELCHANGE TABACTIVE ENTER ESC TEXT_DEF SOUND HMARGINE VMARGINE POSX POSY WIDTH HEIGHT DEFAULT DISABLE ENABLE ALIGN BLT
%token DOWN UP HOVER UPLEFT DOWNRIGHT BAR CARET ITEM VERTICAL DROP DOWNLIST BTNTAB
%token SUPER SUB SUPERLEVEL TOOLTIP PASSWORD STATEBUTTON

/*
 * These are production names:
 */

%start root

%%

/*
 * Production starts here
 */

value				:STRING
					{
						TCML_TRACE("value			->STRING\n");
					}
					|INT_NUM
					{
						TCML_TRACE("value			->INT_NUM\n");
					}
					;
identifier          :IDENTIFIER
					{
						TCML_TRACE("identifier		->IDENTIFIER\n");
						strcpy(tcml_identifier, TCMLlval.string_value);
					
						TCML_TRACE(":identifier %s\n", tcml_identifier);
					}
					;

font_def_start		:BRACKET_OPEN FONT
					{
						add_tcml_logfont();
					}
					;
font_attr			:ID
					{
						tcml_attr = ID;	
					}
					|HEIGHT
					{
						tcml_attr = HEIGHT;	
					}
					|WIDTH
					{
						tcml_attr = WIDTH;	
					}
					|FACENAME
					{
						tcml_attr = FACENAME;	
					}
					|ESCAPEMENT
					{
						tcml_attr = ESCAPEMENT;	
					}
					|ORIENTATION
					{
						tcml_attr = ORIENTATION;	
					}
					|WEIGHT
					{
						tcml_attr = WEIGHT;	
					}
					|ITALIC
					{
						tcml_attr = ITALIC;	
					}
					|UNDERLINE
					{
						tcml_attr = UNDERLINE;	
					}
					|STRIKEOUT
					{
						tcml_attr = STRIKEOUT;	
					}
					|CHARSET
					{
						tcml_attr = CHARSET;	
					}
					|OUTPRECISION
					{
						tcml_attr = OUTPRECISION;	
					}
					|CLIPPRECISION
					{
						tcml_attr = CLIPPRECISION;	
					}
					|QUALITY
					{
						tcml_attr = QUALITY;	
					}
					|PITCHANDFAMILY					
					{
						tcml_attr = PITCHANDFAMILY;	
					}
					|FACECLR
					{
						tcml_attr = FACECLR;
					}
					;
set_font_attr		:font_attr SET value
					{
						if(0!=set_font_attr(tcml_attr))
						{
							return -5;
						}
					}
					;
set_font_attr_list	:set_font_attr
					{
					}
					|set_font_attr_list set_font_attr
					{
					}
					;
font_def			:font_def_start set_font_attr_list BRACKET_CLOSE
					{
						
					}
					; 
definition			:BRACKET_OPEN DEFINE identifier SET value BRACKET_CLOSE
					{
						TCML_TRACE("definition		->BRACKET_OPEN DEFINE IDENTIFIER SET value BRACKET_CLOSE\n");
						define(tcml_identifier, TCMLlval.integer_value);
					}
					|font_def
					{

					}
					;
definition_list		:definition
					{
						TCML_TRACE("definition_list		->definition\n");
					}
					|definition_list definition
					{
						TCML_TRACE("defintion_list		->definition_list definition\n");
					}
					;
start_header		:BRACKET_OPEN HEAD BRACKET_CLOSE
					{
						TCML_TRACE("start_header		->BRACKET_OPEN HEAD BRACKET_CLOSE\n");
					}
					;
end_header			:BRACKET_OPEN SLASH HEAD BRACKET_CLOSE
					{
						TCML_TRACE("end_header		->BRACKET_OPEN SLASH HEAD BRACKET_CLOSE\n");
					}
					;
header				:start_header definition_list end_header
					{
						TCML_TRACE("header			->start_header definition_list end_header\n");
					}
					;
attr				:ID
					{
						TCML_TRACE("attr			->ID\n");
						tcml_attr = ID;
					}
					|LCLICK
					{
						tcml_attr = LCLICK;
					}
					|RCLICK
					{
						tcml_attr = RCLICK;
					}
					|DBLCLICK
					{
						tcml_attr = DBLCLICK;
					}
					|LINEUP
					{
						tcml_attr = LINEUP;
					}
					|LINEDOWN
					{
						tcml_attr = LINEDOWN;
					}
					|VSCROLL
					{
						tcml_attr = VSCROLL;
					}
					|HSCROLL
					{
						tcml_attr = HSCROLL;
					}
					|SELCHANGE
					{
						tcml_attr = SELCHANGE;
					}
					|TABACTIVE
					{
						tcml_attr = TABACTIVE;
					}
					|ENTER
					{
						tcml_attr = ENTER;
					}
					|ESC
					{
						tcml_attr = ESC;
					}
					|TEXT_DEF
					{
						TCML_TRACE("attr			->TEXT_DEF\n");
						tcml_attr = TEXT_DEF;
					}
					|SOUND
					{
						TCML_TRACE("attr			->SOUND\n");
						tcml_attr = SOUND;
					}
					|HMARGINE
					{
						TCML_TRACE("attr			->HMARGINE\n");
						tcml_attr = HMARGINE;
					}
					|VMARGINE
					{
						TCML_TRACE("attr			->VMARGINE\n");
						tcml_attr = VMARGINE;
					}
					|POSX
					{
						TCML_TRACE("attr			->POSX\n");
						tcml_attr = POSX;
					} 
					|POSY
					{
						TCML_TRACE("attr			->POSY\n");
						tcml_attr = POSY;
					} 
					|WIDTH 
					{
						TCML_TRACE("attr			->WIDTH\n");
						tcml_attr = WIDTH;
					}
					|HEIGHT 
					{
						TCML_TRACE("attr			->HEIGHT\n");
						tcml_attr = HEIGHT;
					}
					|DEFAULT 
					{
						TCML_TRACE("attr			->DEFAULT\n");
						tcml_attr = DEFAULT;
					}
					|DISABLE 
					{
						TCML_TRACE("attr			->DISABLE\n");
						tcml_attr = DISABLE;
					}
					|ENABLE 
					{
						TCML_TRACE("attr			->ENABLE\n");
						tcml_attr = ENABLE;
					}
					|ALIGN 
					{
						TCML_TRACE("attr			->ALIGN\n");
						tcml_attr = ALIGN;
					}
					|BLT 
					{
						TCML_TRACE("attr			->BLT\n");
						tcml_attr = BLT;
					}
					|DOWN 
					{
						TCML_TRACE("attr			->DOWN\n");
						tcml_attr = DOWN;
					}
					|UP 
					{
						TCML_TRACE("attr			->UP\n");
						tcml_attr = UP;
					}
					|HOVER 
					{
						TCML_TRACE("attr			->HOVER\n");
						tcml_attr = HOVER;
					}
					|UPLEFT 
					{
						TCML_TRACE("attr			->UPLEFT\n");
						tcml_attr = UPLEFT;
					}
					|DOWNRIGHT 
					{
						TCML_TRACE("attr			->DOWNRIGHT\n");
						tcml_attr = DOWNRIGHT;
					}
					|BAR 
					{
						TCML_TRACE("attr			->BAR\n");
						tcml_attr = BAR;
					}
					|CARET 
					{
						TCML_TRACE("attr			->CARET\n");
						tcml_attr = CARET;
					}
					|ITEM 
					{
						TCML_TRACE("attr			->ITEM\n");
						tcml_attr = ITEM;
					}
					|VERTICAL 
					{
						TCML_TRACE("attr			->VERTICAL\n");
						tcml_attr = VERTICAL;
					}
				
					|DROP 
					{
						TCML_TRACE("attr			->DROP\n");
						tcml_attr = DROP;
					}
					|DOWNLIST 
					{
						TCML_TRACE("attr			->DOWNLIST\n");
						tcml_attr = DOWNLIST;
					}
					|BTNTAB
					{
						TCML_TRACE("attr			->BTNTAB\n");
						tcml_attr = BTNTAB;
					}
					|FONT
					{
						TCML_TRACE("attr			->FONT\n");
						tcml_attr = FONT;
					}
					|TIPFONT
					{
						TCML_TRACE("attr			->TIPFONT\n");
						tcml_attr = TIPFONT;
					}
					|COLOR
					{
						TCML_TRACE("attr			->COLOR\n");
						tcml_attr = COLOR;
					}
					|SUPER
					{
						TCML_TRACE("attr			->SUPER\n");
						tcml_attr = SUPER;
					}
					|SUB
					{
						TCML_TRACE("attr			->SUB\n");
						tcml_attr = SUB;
					}
					|SUPERLEVEL
					{
						TCML_TRACE("attr			->SUPERLEVEL\n");
						tcml_attr = SUPERLEVEL;
					}
					|TOOLTIP
					{
						TCML_TRACE("attr			->TOOLTIP\n");
						tcml_attr = TOOLTIP;
					}
					|PASSWORD
					{
						TCML_TRACE("attr			->PASSWORD\n");
						tcml_attr = PASSWORD;
					}
					|STATEBUTTON
					{
						TCML_TRACE("attr			->STATEBUTTON\n");
						tcml_attr = STATEBUTTON;
					}
					;
set_attr			:attr SET value
					{
						TCML_TRACE("set_attr		->attr SET value\n");
						{
							int ret;
							ret = set_attr(tcml_attr);
							switch(ret)
							{
							case -1:TCML_TRACE("Unknown attribute name\n");break;
							case -2:TCML_TRACE("Unknown attribute value\n");break;
							}

							if(0!=ret)
								return ret;
						}
					}
					;
set_attr_list		:set_attr
					{
						TCML_TRACE("set_attr_list		->set_attr\n");
					}
					|set_attr_list set_attr
					{
						TCML_TRACE("set_attr_list		->set_attr_list set_attr\n");
					}
					;
component_type		:COMPONENT
					{
						TCML_TRACE("component_type		->COMPONENT\n");
						tcml_type = TCML_TYPE_COMPONENT;
					}
					|BUTTON
					{
						TCML_TRACE("component_type		->BUTTON\n");
						tcml_type = TCML_TYPE_BUTTON;
					}
					|SCROLL
					{
						TCML_TRACE("component_type		->SCROLL\n");
						tcml_type = TCML_TYPE_SCROLL;
					}
					|EDIT
					{
						TCML_TRACE("component_type		->EDIT\n");
						tcml_type = TCML_TYPE_EDIT;
					}
					|LIST
					{
						TCML_TRACE("component_type		->LIST\n");
						tcml_type = TCML_TYPE_LIST;
					}
					|COMBO
					{
						TCML_TRACE("component_type		->COMBO\n");
						tcml_type = TCML_TYPE_COMBO;
					}					
					|TABCTRL
					{
						TCML_TRACE("component_type		->TABCTRL\n");
						tcml_type = TCML_TYPE_TABCTRL;
					}
					|GAUGE
					{
						TCML_TRACE("component_type		->GAUGE\n");
						tcml_type = TCML_TYPE_GAUGE;
					}
					|METER
					{
						TCML_TRACE("component_type		->METER\n");
						tcml_type = TCML_TYPE_METER;
					}
					|IMAGELIST
					{
						TCML_TRACE("component_type		->IMAGELIST\n");
						tcml_type = TCML_TYPE_IMAGELIST;
					}					
					;
start_component_sign:BRACKET_OPEN component_type
					{
						TCML_TRACE("start_component_sign	->BRACKET_OPEN component_type\n");
						if(0!=add_component(tcml_type))
						{
							TCML_TRACE("compile error 002\n");
							return -1;
						}
					}
					;
start_component		:start_component_sign set_attr_list BRACKET_CLOSE
					{
						TCML_TRACE("start_component		->start_component_sign set_attr_list BRACKET_CLOSE\n");
					}
					|start_component_sign BRACKET_CLOSE
					{
						TCML_TRACE("start_component		->start_component_sign BRACKET_CLOSE\n"); 
					}
					;
end_component_sign  :BRACKET_OPEN SLASH component_type
					{
						TCML_TRACE("end_component_sign	->BRACKET_OPEN SLASH component_type\n");
						if(0!=close_current_component(tcml_type))
						{
							TCML_TRACE("compile error 001\n");
							return -1;							
						}
					}
					;
end_component		:end_component_sign BRACKET_CLOSE
					{
						TCML_TRACE("end_component		->end_component_sign BRACKET_CLOSE\n");
					}
					;
component_def		:start_component component_def_list end_component
					{
						TCML_TRACE("component_def		->start_component component_def_list end_component\n");
					}
					|start_component end_component
					{
						TCML_TRACE("component_def		->start_component end_component\n");
					}
					;			
component_def_list	:component_def
					{
						TCML_TRACE("component_def_list	->component_def\n");
					}
					|component_def_list component_def
					{
						TCML_TRACE("component_def_list	->component_def_list component_def\n");
					}
					;
start_frame_sign	:BRACKET_OPEN FRAME
					{
						TCML_TRACE("start_frame_sign		->BRACKET_OPEN FRAME\n");
						if(0!=add_frame())
						{
							TCML_TRACE("compile error 004\n");
							return -1;
						}
					}
					;
start_frame			:start_frame_sign set_attr_list BRACKET_CLOSE
					{
						TCML_TRACE("start_frame_sign	->BRACKET_OPEN FRAME set_attr_list BRACKET_CLOSE\n");	
					}
					|start_frame_sign BRACKET_CLOSE
					{
						TCML_TRACE("start_frame_sign	->BRACKET_OPEN FRAME BRACKET_CLOSE\n");
					}
					;
end_frame			:BRACKET_OPEN SLASH FRAME BRACKET_CLOSE
					{
						TCML_TRACE("end_frame		->BRACKET_OPEN SLASH FRAME BRACKET_CLOSE\n");

					}
					;
frame_def			:start_frame component_def_list end_frame
					{
						TCML_TRACE("frame_def		->start_frame component_def_list end_frame\n");
					}
					|start_frame end_frame
					{
						TCML_TRACE("frame_def		->start_frame end_frame\n");
					}
					;
start_tcml			:BRACKET_OPEN TCML BRACKET_CLOSE
					{
						TCML_TRACE("start_tcml		->BRACKET_OPEN TCML BRACKET_CLOSE\n");
					}
					;
end_tcml			:BRACKET_OPEN SLASH TCML BRACKET_CLOSE
					{
						TCML_TRACE("end_tcml		->BRACKET_OPEN SLASH TCML BRACKET_CLOSE\n");
					}
					;
frame_def_list		:frame_def
					{
						TCML_TRACE("frame_def_list		->frame_def\n");
					}
					|frame_def_list frame_def
					{
						TCML_TRACE("frame_def_list		->frame_def_list frame_def\n");
					}
					;
root				:start_tcml header frame_def_list end_tcml
					{
						TCML_TRACE("root			->start_tcml header frame_def_list end_tcml\n");
						return 0;
					}				 	
					;
%%

/* programs */

/*
 * ???
 */
int
TCMLwrap()
{
  return 1;
}

/*
 * Report an error situation discovered in a production
 *
 * This does not do anything since we report all error situations through
 * idl_global->err() operations
 */
int
TCMLerror(char *s)
{
    fprintf(stderr, "Syntax error: %s\n", s);
	return 0;
}
