 /*	
  *		Streams.h - stream family for MAXScript
  *
  *			Copyright © John Wainwright 1996
  *
  */
 
 #ifndef _H_STREAM
 #define _H_STREAM
 
 class Listener;
 class FileStream;
 class Parser;
 
#include "codetree.h"

 visible_class_debug_ok (CharStream)
 
 class CharStream : public Value
 {
 protected:
	 unsigned int	mlineNumber;
//	 bool			mTrippedLineFeed;
 public:
	ScripterExport CharStream();
	ScripterExport virtual ~CharStream();

 					classof_methods (CharStream, Value);
 			BOOL	_is_charstream() { DbgAssert(!is_sourcepositionwrapper(this)); return 1; }
 #	define			is_charstream(v) ((v)->_is_charstream())
 
 	virtual MCHAR	get_char() = 0;
 	virtual void	unget_char(MCHAR c) = 0;
 	virtual MCHAR	peek_char() = 0;
 	virtual int		at_eos() = 0;
 	virtual void	rewind() = 0;

	virtual unsigned int pos();
	virtual unsigned int line(); // Get the current line

//	bool			getTrippedLineFeed(bool reset = true); // get whether passed a line feed since last setTrippedLineFeed(false)/reset
//	void			setTrippedLineFeed(bool val); // set whether passed a line feed

	virtual void	seek(int pos);
	virtual void	flush_to_eol() = 0;
 	virtual void	flush_to_eobuf();
 	virtual void	flush_whitespace();
 
 	virtual MCHAR	putch(MCHAR c) { return c; }
	virtual const MCHAR*	puts(const MCHAR* str) { return str; }
 	virtual int		printf(const MCHAR *format, ...) { return 0; }
 	virtual void	close();
 	virtual void	flush();
 
 	virtual void	log_to(CharStream* log);
 	virtual void	close_log();
 	virtual Listener* get_listener() { return NULL; }
 	virtual CharStream* get_log() { return NULL; }

	virtual Value*	get_file_name() { return NULL; }
	
 };
 
 visible_class_debug_ok (FileStream)
 
 class FileStream : public CharStream
 {
 public:
 	Value*	file_name;
 	MCHAR*	mode;
 	FILE*	file;
 	CharStream*	log;
 	int		ungetchar_count;
	MCHAR	ungetchar_buf[8];
 	Parser* reader;
	DWORD	decrypt;
	// Constructor / Destructor
 			ScripterExport FileStream();
 		    ScripterExport ~FileStream();
 
 			classof_methods (FileStream, CharStream);
 #	define	is_filestream(v) ((DbgVerify(!is_sourcepositionwrapper(v)), (v))->tag == class_tag(FileStream))
	void	collect() { delete this; }
 	void	gc_trace();
 	ScripterExport void	sprin1(CharStream* s);
 
 #include "defimpfn.h"
 #	include "streampr.h"
 
 	/* internal char stream protocol */
 	ScripterExport MCHAR	get_char();
 	ScripterExport void		unget_char(MCHAR c);
 	ScripterExport MCHAR	peek_char();
 	ScripterExport int		at_eos();
 	ScripterExport unsigned int	pos();
	ScripterExport unsigned int line(); // Gets the current line number
 	ScripterExport void		seek(int pos);
	ScripterExport void		rewind();
 	ScripterExport void		flush_to_eol();
 
 	ScripterExport MCHAR	putch(MCHAR c);
 	ScripterExport const MCHAR*	puts(const MCHAR* str);
 	ScripterExport int		printf(const MCHAR *format, ...);
 
 	ScripterExport FileStream* open_decrypt(const MCHAR* ifile_name, int seed);
 	ScripterExport FileStream* open(const MCHAR* ifile_name, const MCHAR* imode);
 	ScripterExport void	flush();
 	ScripterExport void	close();
 	void	log_to(CharStream* log);
 	void	close_log();

	Value*	get_file_name() { return file_name; }
	void	undo_lookahead();

	void	check_readable();
	void	check_writeable();
 };
 
 visible_class_debug_ok (WindowStream)
 
 // Collectable::flags3 - bit 0 set if WindowStream is wrapping a MXS_Scintilla edit control. If not, standard
 // edit control is assumed
 class WindowStream : public CharStream
 {
 public:
 	HWND		window;
 	int			cursor;
 	MCHAR*		title;
 	Listener*	listener;
 	CharStream*	log;
	MCHAR		wputs_buf[512];			// edit control output buffer
	MCHAR*		wputs_p;
	HWND		echo;
 
	// Constructor / Destructor
 				WindowStream(HWND iwin);
 				WindowStream(MCHAR* title);		/* for background scripts; window with given title will open if output generated */
 			   ~WindowStream();
 
 				classof_methods (WindowStream, CharStream);
 	void		collect() { delete this; }
 	void		gc_trace();
 	ScripterExport void	sprin1(CharStream* s);
 
 #include "defimpfn.h"
 	def_generic(sprint,	  "sprint");
  	use_generic( coerce,  "coerce");
 	use_generic( flush,   "flush"); 
		
 	/* internal MCHAR stream protocol */
 	
 	MCHAR		get_char() { return 0; }
 	void		unget_char(MCHAR c) {}
 	MCHAR		peek_char() {return 0; }
 	int			at_eos() { return TRUE; }
 	void		rewind() {}
 	void		flush_to_eol() {}
 
 	ScripterExport MCHAR	putch(MCHAR c);
 	ScripterExport const MCHAR*	puts(const MCHAR* str);
 	ScripterExport int		printf(const MCHAR *format, ...);
 	ScripterExport void		flush();

 	void		ensure_window_open();
 
 	void		log_to(CharStream* log);
 	void		close_log();
 	Listener*	get_listener() { return listener; }
 	CharStream* get_log() { return log; }

	// edit control output primitives
	ScripterExport MCHAR*	wputs(const MCHAR *str);
	ScripterExport void		wflush();
	ScripterExport MCHAR	wputch(const MCHAR c);
	ScripterExport int		wprintf(const MCHAR *format, ...);
				   void		set_echo_window(HWND wnd) { echo = wnd; }
				   void		echo_cur_line();
 				   int		get_cur_line(MSTR& line);
};
 
 #endif
 
 
 
