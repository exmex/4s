
%{
#include <string.h>
#include <stdlib.h>
#include "cinterf.h"
#include "TCML.cpp.h"

int tcml_attr = 0;
int tcml_line = 0;
int tcml_buf_idx = 0;
unsigned char tcml_type = 0;
char tcml_buf[MAX_TCML_SYMBOL] = "";
char tcml_identifier[MAX_TCML_SYMBOL] = "";

void reset_lex()
{
	tcml_attr = 0;
	tcml_line = 0;
	tcml_buf_idx = 0;
	tcml_type = 0;
	memset( tcml_buf, 0, MAX_TCML_SYMBOL);
	memset( tcml_identifier, 0, MAX_TCML_SYMBOL);
}

void clean_lex_buf()
{
	if(yy_current_buffer)
	{
		free(yy_current_buffer->yy_ch_buf);
		free(yy_current_buffer);
	}	
}
%}

%x			IN_COMMENT
%x			IN_STRING


white		[ \t,]+
letter		[A-Za-z]
digit		[0-9]
integer		{digit}+
identifier	{letter}(_|{letter}|{digit})*


%option noyywrap

%%
"\n"				{
						tcml_line++;
					}
"/*"				{
						BEGIN IN_COMMENT;
					}
<IN_COMMENT>"*/"	{
						BEGIN 0;
					}
<IN_COMMENT>"\n"	{
						tcml_line++;
					}
<IN_COMMENT>.		{/*Eat up all string in comment*/}
\"					{
						tcml_buf_idx = 0;
						memset(tcml_buf, '\0', MAX_TCML_SYMBOL);
						BEGIN IN_STRING;
					}
<IN_STRING>\"		{			
						strcpy(TCMLlval.string_value, tcml_buf);	
						BEGIN 0;
						
						return STRING;
					}
<IN_STRING>.		{
						if(tcml_buf_idx < MAX_TCML_SYMBOL-1)
							tcml_buf[tcml_buf_idx++] = yytext[0];
					}
"<"					{return BRACKET_OPEN;}
">"					{return BRACKET_CLOSE;}
"="					{return SET;}
"/"					{return SLASH;}
"TCML"				{return TCML;}
"HEAD"				{return HEAD;}
"FRAME"				{return FRAME;}
"COMPONENT"			{return COMPONENT;}
"BUTTON"			{return BUTTON;}
"SCROLL"			{return SCROLL;}
"EDIT"				{return EDIT;}
"LIST"				{return LIST;}
"COMBO"				{return COMBO;}
"TABCTRL"			{return TABCTRL;}
"GAUGE"				{return GAUGE;}
"IMAGELIST"			{return IMAGELIST;}
"DEFINE"			{return DEFINE;}
"ID"				{return ID;}
"TEXT"				{return TEXT_DEF;}
"SOUND"				{return SOUND;}
"HMARGINE"			{return HMARGINE;}
"VMARGINE"			{return VMARGINE;}
"POSX"				{return POSX;}
"POSY"				{return POSY;}
"WIDTH"				{return WIDTH;}
"HEIGHT"			{return HEIGHT;}
"DEFAULT"			{return DEFAULT;}
"DISABLE"			{return DISABLE;}
"ENABLE"			{return ENABLE;}
"ALIGN"				{return ALIGN;}
"BLT"				{return BLT;}
"DOWN"				{return DOWN;}
"UP"				{return UP;}
"HOVER"				{return HOVER;}
"UL"				{return UPLEFT;}
"DR"				{return DOWNRIGHT;}
"BAR"				{return BAR;}
"CARET"				{return CARET;}
"ITEM"				{return ITEM;}
"VERTICAL"			{return VERTICAL;}
"DROP"				{return DROP;}
"DOWNLIST"			{return DOWNLIST;}
"LCLICK"			{return LCLICK;}
"RCLICK"			{return RCLICK;}
"DBLCLICK"			{return DBLCLICK;}
"LINEUP"			{return LINEUP;}
"LINEDOWN"			{return LINEDOWN;}
"VSCROLL"			{return VSCROLL;}
"HSCROLL"			{return HSCROLL;}
"SELCHANGE"			{return SELCHANGE;}
"TABACTIVE"			{return TABACTIVE;}
"ENTER"				{return ENTER;}
"ESC"				{return ESC;}
"FONT"				{return FONT;}
"TIPFONT"			{return TIPFONT;}
"COLOR"				{return COLOR;}
"ESCAPEMENT"		{return ESCAPEMENT;}
"ORIENTATION"		{return ORIENTATION;}
"WEIGHT"			{return WEIGHT;}
"ITALIC"			{return ITALIC;}
"UNDERLINE"			{return UNDERLINE;}
"STRIKEOUT"			{return STRIKEOUT;}
"CHARSET"			{return CHARSET;}
"OUTPRECISION"		{return OUTPRECISION;}
"CLIPPRECISION"		{return CLIPPRECISION;}
"QUALITY"			{return QUALITY;}
"PITCHANDFAMILY"	{return PITCHANDFAMILY;}
"FACENAME"			{return FACENAME;}
"FACECLR"			{return FACECLR;}
"METER"				{return METER;}
"SUB"				{return SUB;}
"SUPER"				{return SUPER;}
"SUPERLEVEL"		{return SUPERLEVEL;}
"TOOLTIP"			{return TOOLTIP;}
"PASSWORD"			{return PASSWORD;}
"STATEBUTTON"		{return STATEBUTTON;}
{white}				{/*Eat up all white characters*/}
{integer}			{TCMLlval.integer_value = atoi(TCMLtext); return INT_NUM;}
{identifier}		{strcpy(TCMLlval.string_value, yytext); return IDENTIFIER;}
%%
