#ifndef __DEFINES_H
#define __DEFINES_H

#define ROBOIO  // enable specific constraints for RoBoard


#if defined(__BORLANDC__) && !defined(WIN32)
    #define RB_BC_DOS

    #if __BORLANDC__ == 0x0410
        #define RB_BC30
    #endif

    typedef int bool;
    #define true  (1==1)
    #define false (1==0)
    
    #define __FUNCB__(x)    #x              // for stringizing __LINE__
    #define __FUNCA__(x)    __FUNCB__(x)    // for stringizing __LINE__
    #define __FUNCTION__    (__FILE__ "[line " __FUNCA__(__LINE__) "]")
#endif

#if defined (DJGPP)
    #define RB_DJGPP
#endif

#if defined(__GNUC__) && defined(linux) //&& defined(i386)
    #define RB_LINUX
#endif

#if defined(_MSC_VER) //&& defined(_M_IX86)
    #if defined   (WINCE)
        #define RB_MSVC_WINCE
    #elif defined (WIN32)
        #define RB_MSVC_WIN32
    #endif
#endif


#if defined __cplusplus
    #define _RB_INLINE  static inline
#elif defined(DJGPP)
    #define _RB_INLINE  __INLINE__
#elif defined(__GNUC__)
    #define _RB_INLINE  __inline__
#elif defined(_MSC_VER)
    #define _RB_INLINE  __inline
#else
    #define _RB_INLINE  static
#endif


#if defined(RB_MSVC_WIN32) || defined(RB_MSVC_WINCE)
    //#define ROBOIO_DLL
	//#define DLL_EXPORTING
#endif

#ifdef ROBOIO_DLL
    #ifdef DLL_EXPORTING
        #define RBAPI(rtype)    __declspec(dllexport) rtype __stdcall
        #define _RBAPI_C(rtype) __declspec(dllexport) rtype __cdecl
    #else
        #define RBAPI(rtype)    __declspec(dllimport) rtype __stdcall
        #define _RBAPI_C(rtype) __declspec(dllimport) rtype __cdecl
    #endif
    
    #define RB_INLINE
#else
    #define RBAPI(rtype)    rtype
    #define _RBAPI_C(rtype) rtype
    #define RB_INLINE       _RB_INLINE
#endif //ROBOIO_DLL

#endif

