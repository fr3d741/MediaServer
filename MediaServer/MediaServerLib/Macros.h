#pragma once

#ifdef _WIN32
    #ifdef MEDIASERVER_LIB
        #define DLL __declspec( dllexport )
    #else
        #define DLL __declspec( dllimport )
    #endif
#else
    #define DLL
#endif

#pragma warning ( disable : 4251 )
