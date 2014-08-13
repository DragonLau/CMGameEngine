#ifndef HGL_ATOMIC_INCLUDE
#define HGL_ATOMIC_INCLUDE

#include<hgl/platform/Platform.h>

// #ifdef HGL_ATOMIC_CPP11
// 	#include<atomic>
// #else
	#if HGL_OS == HGL_OS_Windows
		#include<hgl/thread/atomic/AtomicWin.h>
	#elif HGL_OS == HGL_OS_MacOSX
		#include<hgl/thread/atomic/AtomicOSX.h>
	#elif defined(__GNUC__)
		#if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1) || __GNUC__ > 4) && (defined(__x86_64__) || defined(__i386__) || defined(__powerpc__))
			#include<hgl/thread/atomic/AtomicGNU.h>
		#else
			#include<hgl/thread/atomic/AtomicAPR.h>
		#endif//
	#endif//
// #endif//C++11 ATOMIC

//ps.1���Ͼɵ�Linux/32bit��ԭ�ӽ�֧��24λ���������趨Ϊ��֧�־ɵ�Linux
//ps.2��ʹ��GCC 4.1���ú�ʵ�ֵ�AtomicGNU�Ĳ�֧��doubel�ʹ�������֧�֣��򾡿��ܲ�Ҫ��atom_double

//ps..........GCC4.7/4.8/4.9�����ʹ��c++11��atomic�����һЩvalgrind-memcheck���������ݲ�ʹ�á���valgrind����

namespace hgl
{
	#if (HGL_OS == HGL_OS_Windows)//&&(!defined(HGL_ATOMIC_CPP11))
		typedef atom_win32<int		> atom_int;
		typedef atom_win32<uint		> atom_uint;
		typedef atom_win32<int32	> atom_int32;
		typedef atom_win32<uint32	> atom_uint32;
		typedef atom_win64<int64	> atom_int64;
		typedef atom_win64<uint64	> atom_uint64;
		typedef atom_win32<float	> atom_float;
// 		typedef atom_win64<double	> atom_double;

		typedef atom_win32<char		> atom_char;
		typedef atom_win32<uchar	> atom_uchar;
//		typedef atom_win32<wchar_t	> atom_wchar;
		typedef atom_win32<char16_t	> atom_char16;
//		typedef atom_win32<char32_t	> atom_char32;
	#else
// 		#ifdef HGL_ATOMIC_CPP11
// 			template<typename T> using atom=std::atomic<T>;
// 		#endif//HGL_ATOMIC_CPP11

		typedef atom<bool		> atom_bool;
		typedef atom<int		> atom_int;
		typedef atom<uint		> atom_uint;
		typedef atom<int32		> atom_int32;
		typedef atom<uint32		> atom_uint32;
		typedef atom<int64		> atom_int64;
		typedef atom<uint64		> atom_uint64;
		typedef atom<float		> atom_float;
// 		typedef atom<double		> atom_double;

		typedef atom<char		> atom_char;
		typedef atom<uchar		> atom_uchar;
//		typedef atom<wchar_t	> atom_wchar;
		typedef atom<char16_t	> atom_char16;
//		typedef atom<char32_t	> atom_char32;
	#endif//windows & !c++11
}//namespace hgl
#endif//HGL_ATOMIC_INCLUDE
