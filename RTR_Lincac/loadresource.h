#pragma once

#ifndef _LOADRESOURCE_H
#define _LOADRESOURCE_H

#include"Help.h"
#include <windows.h>  
#include <commdlg.h> 

extern unsigned int LoadSkyBox(std::string path);
extern std::string OpenFileDialog();
extern std::string TCHAR2STRING(TCHAR* STR);

#endif // !_LOADRESOURCE_H

