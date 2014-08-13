#ifndef HGL_COMPILER_MICROSOFT_INCLUDE
#define HGL_COMPILER_MICROSOFT_INCLUDE
//--------------------------------------------------------------------------------------------------
#define HGL_COMPILER_NAME	OS_TEXT("Microsoft C/C++")

#define HGL_USE_APR			//ʹ��Apache Portable Runtime
//#define HGL_USE_C_IO		//ʹ��C IO��

#if _MSC_VER < 1500							//Visual C++ 2008(9.0)
	#error Please upgrade your compiler or development tools to Microsoft C/C++ 15.0 (Visual C++ 2008) or later.
#else
	#if _MSC_VER == 1800
		#define HGL_LIB_COMPILER_NAME		OS_TEXT("VC2013")
		#define HGL_LIB_COMPILER_VERSION	OS_TEXT("18")
	#elif _MSC_VER == 1700
		#define HGL_LIB_COMPILER_NAME		OS_TEXT("VC2012")
		#define HGL_LIB_COMPILER_VERSION	OS_TEXT("17")
	#elif _MSC_VER == 1600
		#if _MSC_FULL_VER < 160040219		//Visual C++ 2010 SP1
			#error Please install Visual C++ 2010 Service Pack 1 and Windows SDK v7.1.
		#endif//_MSC_FULL_VER

		#define HGL_LIB_COMPILER_NAME		OS_TEXT("VC2010")
		#define HGL_LIB_COMPILER_VERSION	OS_TEXT("16")
	#elif _MSC_VER == 1500
		#if _MSC_FULL_VER < 150030729		//Visual C++ 2008 SP1
			#error Please install Visual C++ 2008 Service Pack 1 and Windows SDK v7.1.
		#endif//_MSC_FULL_VER

		#define HGL_LIB_COMPILER_NAME		OS_TEXT("VC2008")
		#define HGL_LIB_COMPILER_VERSION	OS_TEXT("15")
	#else
		#define HGL_LIB_COMPILER_NAME		OS_TEXT("VC_UnknownVersion")
		#define HGL_LIB_COMPILER_VERSION	OS_TEXT("Unknown")
	#endif//_MSC_VER

	#define enum_uint(name)	enum name:unsigned int
	#define enum_int(name)	enum name:int
#endif//_MSC_VER

//--------------------------------------------------------------------------------------------------
#if _MSC_VER < 1600
	#ifndef _HAS_CHAR16_T_LANGUAGE_SUPPORT
	#define _HAS_CHAR16_T_LANGUAGE_SUPPORT 0
	#endif /* _HAS_CHAR16_T_LANGUAGE_SUPPORT */

	#if _HAS_CHAR16_T_LANGUAGE_SUPPORT
	#else /* _HAS_CHAR16_T_LANGUAGE_SUPPORT */
		#if !defined(_CHAR16T)
			#define _CHAR16T
			typedef unsigned short char16_t;
			typedef unsigned int char32_t;
		#endif /* !defined(_CHAR16T) */
	#endif /* _HAS_CHAR16_T_LANGUAGE_SUPPORT */

	#define nullptr NULL
#else
	#include<cstdint>
#endif//_MSC_VER < 1600

#define HGL_FORCE_INLINE __forceinline

#include<hgl/platform/compiler/DataTypeTypedef.h>
#include<hgl/platform/compiler/Property.h>

#ifdef HGL_CPP11
	#if _MSC_VER>=1800
		#define HGL_VARIADIC_TEMPLATES				//�䳤����ģ��
		#define HGL_INITIALIZER_LIST				//��ʼ���б�
	#endif//VC2013

	#if _MSC_VER>=1700
// 		#define HGL_ATOMIC_CPP11					//C++11ԭ��ģ��
		#define HGL_CONSTRUCTION_REUSE				//���캯������
		#define HGL_DEFAULT_MEMFUNC 	=default
		#define HGL_OVERRIDE			override
	#else
		#define HGL_DEFAULT_MEMFUNC		{}
		#define HGL_OVERRIDE
	#endif//VC2012
#else
	#define HGL_DEFAULT_MEMFUNC			{}
	#define HGL_OVERRIDE
#endif//C++11

#if _MSC_VER < 1700 // VC2012
	#define _mm_set_ss set1_ps
#endif

#if _MSC_VER < 1800 // VC2013
#endif//

#define HGL_FORCE_INLINE __forceinline

#define HGL_THREAD	__declspec(thread)

#define HGL_FMT_I64				"%I64d"
#define HGL_FMT_U64				"%I64u"
//�ο��ĵ�������֧�ְ汾ΪVC2013����ַ��http://msdn.microsoft.com/en-us/library/tcxf1dw6.aspx
//--------------------------------------------------------------------------------------------------
#define _USE_MATH_DEFINES				// ʹ����ѧ��������
//--------------------------------------------------------------------------------------------------
#pragma warning(disable:4819)			// ansi -> unicode
#pragma warning(disable:4311)			// ģ�徯��
#pragma warning(disable:4800)			// -> bool ������ʧ����
#pragma warning(disable:4244)			// -> int ���ȶ�ʧ����
#pragma warning(disable:4804)			// ����ȫ�����ͱȽ�
#pragma warning(disable:4805)			// ����ȫ�����ͱȽ�

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif//

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif//

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif//

#ifdef _DLL
	#define HGL_LIB_CRT	"Dynamic"
#else
	#define HGL_LIB_CRT	"Static"
#endif//_DLL

#define HGL_LIB_FRONT	HGL_LIB_OS "_" HGL_LIB_COMPILER_NAME "_" HGL_LIB_DEBUG_NAME "_" HGL_LIB_CRT "_"

#define HGL_LIB_END		".LIB"
//--------------------------------------------------------------------------------------------------
#endif//HGL_COMPILER_MICROSOFT_INCLUDE

