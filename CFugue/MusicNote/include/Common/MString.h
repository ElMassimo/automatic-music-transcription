/*
	This is part of CFugue, the C++ Runtime for MIDI Score Programming
	Copyright (C) 2009 Gopalakrishna Palem

	For links to further information, or to contact the author,
	see <http://musicnote.sourceforge.net/>.
*/

#ifndef __MSTRING_H__2B50AFA1_EFB9_428a_A397_3FFEA175FA33__
#define __MSTRING_H__2B50AFA1_EFB9_428a_A397_3FFEA175FA33__

#include "_TChar.h"	// On Non win32 platforms we use a local TChar.h
#include <string>
#include <string.h>
#include <wchar.h>

namespace MusicNoteLib
{
	/// <Summary> Helper class for simple string manipulations </Summary>
	class MString : public 
#ifdef UNICODE
		std::wstring
#else
		std::string
#endif // UNICODE
	{
#ifdef UNICODE
		typedef std::wstring base;
#else
		typedef std::string base;
#endif // UNICODE
	public:
		inline MString() : base()
        { }
		inline MString(const TCHAR* sz) : base(sz) 
        { }
		inline MString(const base& sz) : base(sz) 
        { }
		inline MString& operator += (const TCHAR* sz)
		{
			base::operator += (sz);
			return *this;
		}	
		inline MString operator + (const TCHAR* sz) const
		{
			return (base&)(*this) + sz;
		}
		inline MString operator + (const TCHAR ch) const
		{
			return (base&)(*this) + ch;
		}
		inline MString operator + (const MString& other) const
		{
			return (*this) + (const TCHAR*)other;
		}
		inline friend MString operator + ( const TCHAR* sz, const MString& obj)
		{
			return MString(sz) + obj;
		}
		inline operator const TCHAR* () const {	return c_str(); }

#if defined UNICODE || defined _UNICODE
		inline MString(const char* arg)
        { 
            size_t nLen = strlen(arg);
            wchar_t *pSz = new wchar_t[nLen +2];
            mbstowcs(pSz, arg, nLen+1);
            *this = pSz;
            delete[] pSz;
        }
		inline MString operator + (const char* sz) const
		{
			return std::move((base&)(*this) + MString(sz));
		}
#else
		inline MString(const wchar_t* argw) 
        {  
            size_t nLen = wcslen(argw);
            char* pSz = new char[nLen + 2];
            wcstombs(pSz, argw, nLen+1);
            *this = pSz;
            delete pSz;
        }
		inline MString operator + (const wchar_t* sz) const
		{
			return std::move((base&)(*this) + MString(sz));
		}
#endif
	}; // class MString

} // namespace MusicNoteLib

#endif // __MSTRING_H__2B50AFA1_EFB9_428a_A397_3FFEA175FA33__