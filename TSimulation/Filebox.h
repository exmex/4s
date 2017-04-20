#ifndef __FILEBOX
#define __FILEBOX

extern HINSTANCE hInstFileBox;

BOOL SaveBox(HWND, char *, char *);
UINT CALLBACK SaveBoxProc(HWND, unsigned, WPARAM, LPARAM);
BOOL OpenBox(HWND, char *, char *);
UINT CALLBACK OpenBoxProc(HWND, unsigned, WPARAM, LPARAM);
BOOL PathBox(HWND, HINSTANCE, char *, char *);
UINT CALLBACK PathBoxProc(HWND, unsigned, WPARAM, LPARAM);
#endif