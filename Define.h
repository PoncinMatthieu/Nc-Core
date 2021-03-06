
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, The 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        02/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

                    define all constants of 3DNovac Core

-----------------------------------------------------------------------------*/


#ifndef NC_CORE_SYSTEM_DEFINE_H_
#define NC_CORE_SYSTEM_DEFINE_H_

    #include <climits>
    #include <queue>
    #include <map>
    #include <list>
    #include <string>

    //#define NC_DYNAMIC_LINK
    //#define NC_EXPORT_CORE

// define Operating System
	#if !defined(SYSTEM_WINDOWS) && !defined(SYSTEM_LINUX) && !defined(SYSTEM_MACOS) && !defined(SYSTEM_FREEBSD)
		#if defined(_WIN32) || defined(__WIN32__) || defined(__WIN64) || defined(__WIN64__)     // Windows
			#define SYSTEM_WINDOWS
		#elif defined(linux) || defined(__linux)                                                // Linux
			#define SYSTEM_LINUX
		#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh) // MacOS
			#define SYSTEM_MACOS
		#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
			#define SYSTEM_FREEBSD
		#else                                                                                   // Unsupported system
			#error This operating system is not supported on the API_NAME
		#endif
	#endif

// define compilo option for Window
	#ifdef SYSTEM_WINDOWS
		#ifndef _WINDOWS_
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
		#undef WIN32_LEAN_AND_MEAN
		#endif

		#pragma warning(disable : 4244 4251 4996 4275)
		#include <winsock2.h>
	#endif

// linking option (static or shared)
    #ifdef SYSTEM_WINDOWS                                       // windows
        #ifdef NC_DYNAMIC_LINK                                  // dynamic
            #ifdef NC_EXPORT_CORE
                #define LIB_NC_CORE       __declspec(dllexport)
            #else
                #define LIB_NC_CORE       __declspec(dllimport)
            #endif
        #else                                                   // static (do nothing)
            #define LIB_NC_CORE
        #endif
    #else                                                       // others os (do nothing)
        #define LIB_NC_CORE               ///< Linking option of Nc-Core module
    #endif

// Define LOG
    #ifndef LOG
        #if !defined(_DEBUG_LOG_DEF)
            #if defined(_DEBUG) && defined(_DEBUG_LOG)
                #define _DEBUG_LOG_DEF  __FILE__, __LINE__,
            #else
                #define _DEBUG_LOG_DEF
            #endif
        #endif

        #define LOG             Nc::Utils::Logger::Log(_DEBUG_LOG_DEF 0)
        #define LOG_ERROR       Nc::Utils::Logger::Log(_DEBUG_LOG_DEF 1)
        #ifdef _DEBUG
            #define LOG_DEBUG       Nc::Utils::Logger::Log(_DEBUG_LOG_DEF 2)
        #else
            #define LOG_DEBUG       Nc::Utils::Logger::Log(_DEBUG_LOG_DEF -1)
        #endif
    #endif

// includes interne
    #include "Debug.h"

    namespace Nc
    {
        // portable fixed size
        // 8 bit interger type
            typedef char            Int8;
            typedef unsigned char   UInt8;

        // 16 bits integer type
        #if USHRT_MAX == 0xFFFF
            typedef short           Int16;
            typedef unsigned short  UInt16;
        #elif UINT_MAX == 0xFFFF
            typedef int             Int16;
            typedef unsigned int    UInt16;
        #elif ULONG_MAX == 0xFFFF
            typedef long            Int16;
            typedef unsigned long   UInt16;
        #else
            #error No 16 bits integer type for this platform
        #endif

        // 32 bits integer type
        #if USHRT_MAX == 0xFFFFFFFF
            typedef short           Int32;
            typedef unsigned short  UInt32;
        #elif UINT_MAX == 0xFFFFFFFF
            typedef int             Int32;
            typedef unsigned int    UInt32;
        #elif ULONG_MAX == 0xFFFFFFFF
            typedef long            Int32;
            typedef unsigned long   UInt32;
        #else
            #error No 32 bits integer type for this platform
        #endif

        namespace Utils
        {
            namespace Metaprog
            {
                struct Nop;
            }

            namespace Xml
            {
                class   Object;

                typedef std::list<Object*>                  ListObject;
                typedef std::map<std::string, std::string>  MapParam;
            }
        }

        namespace Math
        {
            // define the classes to typedef basics type
            template<typename T = unsigned char>    struct Pixel;
            template<typename T>                    struct Box2D;
            template<typename T>                    struct Box3D;
            template<typename T>                    struct Vector2D;
            template<typename T>                    struct Vector3D;
            template<typename T>                    class Matrix4x4;
            template<typename T>                    class Plane;

            // Typedefs
            typedef Pixel<float>                Color;  // a pixel could be see as a color rgba ^^ (float for opengl)
            typedef Box2D<int>                  Box2i;
            typedef Box2D<unsigned int>         Box2ui;
            typedef Box2D<float>                Box2f;
            typedef Box2D<double>               Box2d;
            typedef Box3D<int>                  Box3i;
            typedef Box3D<unsigned int>         Box3ui;
            typedef Box3D<float>                Box3f;
            typedef Box3D<double>               Box3d;
            typedef Vector2D<int>               Vector2i;
            typedef Vector2D<unsigned int>      Vector2ui;
            typedef Vector2D<float>             Vector2f;
            typedef Vector2D<double>            Vector2d;
            typedef Vector3D<int>               Vector3i;
            typedef Vector3D<float>             Vector3f;
            typedef Vector3D<double>            Vector3d;
            typedef Matrix4x4<float>            TMatrix;
            typedef Plane<float>                Planef;
            typedef Plane<double>               Planed;
        }

        namespace System
        {
            struct Event;

            typedef std::queue<Event>    EventQueue;
        }

        namespace Graph
        {
            template<typename T, typename NodeType, class Allocator>
            struct ListNodePolitic;
            template<typename T>
            class NodeList;
        }
    }

#endif // DEFINE_H_INCLUDED
