//! @file rxencode.h
//! @brief Rome string encoding/decoding functions.
//!
//! This file contains utility code which isn't Rusle2 specific.
//! These string functions are used to encode and decode strings when they
//!   are stored in different formats: XML files, XML stored in a SQL database,
//!   and Win32 filenames.
//!
//! Copyright (C) 2001-2005 University of Tennessee.
//! All rights reserved.

#ifndef RXENCODE_H_INCLUDED
#define RXENCODE_H_INCLUDED
#pragma once

////////////////////////////////////////////////////////////////////////////

//! Flags used by XMLEncodeAux().
enum XMLAuxFlags
{
	ENCODE_NONE			= 0<<0, //!< No encoding
	ENCODE_XML			= 1<<0,	//!< Encode chars invalid in XML
	ENCODE_SQLITE		= 1<<1,	//!< Encode chars invalid in SQLite
	ENCODE_FILENAME		= 1<<2,	//!< Encode chars invalid in Win32 filenames
	ENCODE_NO_ASSERTS	= 1<<3, //!< Don't trigger ASSERTs
	ENCODE_GET_TYPE		= 1<<4, //!< Restrict legal encodings to the given flags
	ENCODE_FORCEOUTPUT  = 1<<5, //!< Force output to string buffer even if no changes made
};

ROME_API LPCSTR		XMLDecode(LPCSTR pszEncoded);
ROME_API LPCSTR		XMLDecodeAux(LPCSTR pszEncoded, UINT nFlags);
         LPCSTR		XMLDecodeAux(LPCSTR pszEncoded, CString* pDecoded, UINT nFlags);
ROME_API LPCSTR		XMLEncode(LPCSTR pszDecoded);
ROME_API LPCSTR		XMLEncodeAux(LPCSTR pszDecoded, UINT nFlags);
         LPCSTR		XMLEncodeAux(LPCSTR pszDecoded, CString* pEncoded, UINT nFlags);
ROME_API long		XMLFindVersion(LPCSTR xml);
ROME_API bool		XMLIsDecoded(LPCSTR pszDecoded);
ROME_API bool		XMLIsDecodedAux(LPCSTR pszDecoded, UINT nFlags);
ROME_API bool		XMLIsEncoded(LPCSTR pszEncoded);
ROME_API bool		XMLIsEncodedAux(LPCSTR pszEncoded, UINT nFlags);

#if RUN_UNIT_TESTS
	BOOL			UnitTestStringEncoding();
#endif

////////////////////////////////////////////////////////////////////////////

#endif // RXENCODE_H_INCLUDED

