#pragma once

#ifdef MEDIASERVER_LIB
    #define DLL __declspec( dllexport )
#else
    #define DLL __declspec( dllimport )
#endif

#pragma warning ( disable : 4251 )
