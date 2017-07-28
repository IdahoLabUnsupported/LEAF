//! @file  api-rome.h
//! @brief Interface file for functions exported for the Rome DLL interface.
//!
//! This file defines the Rome API, the interface for using Rusle2 calculations
//!   from a DLL.
//! Copyright (C) 2000-2009 University of Tennessee.
//! All rights reserved.

#ifndef API_ROME_H_INCLUDED
#define API_ROME_H_INCLUDED
#pragma once

/////////////////////////////////////////////////////////////////////////////
// Types used by this API.
// All custom Rome types begin with prefix "RT_".

typedef __int32			RT_BOOL;		//!< (32 bits) Boolean value (can use -1 for failure...)
typedef double			RT_DATE;		//!< (32 bits) date type used by OLE functions
typedef char            RT_CHAR;        //!< ( 8 bits) character type (ASCII == UTF8)
typedef const RT_CHAR*	RT_CNAME;		//!< (32 bits) ptr to const name string
typedef const RT_CHAR*	RT_CSTR;		//!< (32 bits) ptr to const string data
typedef __int32			RT_INT;			//!< (32 bits) signed integer
typedef unsigned __int32 RT_UINT;		//!< (32 bits) unsigned integer
typedef double          RT_REAL;        //!< (64 bits) floating point value (IEEE double)
typedef __int16			RT_SHORT;		//!< (16 bits) short integer
typedef void			RT_VOID;		//!<  --       void type
typedef RT_CHAR*        RT_PCHAR;		//!< (32 bits) pointer to character
typedef RT_VOID*		RT_PVOID;		//!< (32 bits) pointer to void
typedef const RT_VOID*	RT_PCVOID;		//!< (32 bits) const pointer to void

// Boolean (RT_BOOL) values used by Rome API functions.
// Constants used by the Rome API begin with prefix "RX_".
// Note that most functions which return RT_BOOL may return a failure value.
#define RX_TRUE         (1)             //!< True
#define RX_FALSE        (0)             //!< False
#define RX_FAILURE      (-1)            //!< Failure

/////////////////////////////////////////////////////////////////////////////
// Classes used by interface functions
// These are "opaque" objects. This API doesn't allow access to their contents.
// These are typdef'd to concrete internal Rusle2 classes in "rtypes.h".

// @def R2_OPAQUE_TYPEDEF()
// Define symbol class to be a C++ class when building the Rome code.
// Otherwise, it should be a void, and is only useable as a void pointer.
//
#if defined(__cplusplus) && defined(BUILD_ROMEDLL)
//	#define R2_OPAQUE_TYPEDEF(ClassName)   class ClassName;
	#define CLASS class
#else  // C interface
//	#define R2_OPAQUE_TYPEDEF(ClassName)   typedef void ClassName;
	#define CLASS struct
#endif // _cplusplus


#ifndef RTYPES_H_INCLUDED
/*
	R2_OPAQUE_TYPEDEF(RT_Attr)        //!< Opaque type: Rome attrs
	R2_OPAQUE_TYPEDEF(RT_App)         //!< Opaque type: Rome interface
	R2_OPAQUE_TYPEDEF(RT_Database)    //!< Opaque type: Rome database
	R2_OPAQUE_TYPEDEF(RT_DBFIND)      //!< Opaque type: Rome find result set
	R2_OPAQUE_TYPEDEF(RT_Engine)      //!< Opaque type: Rome engine
	R2_OPAQUE_TYPEDEF(RT_FileObj)     //!< Opaque type: Rome file
	R2_OPAQUE_TYPEDEF(RT_Files)       //!< Opaque type: Rome filesystem
	R2_OPAQUE_TYPEDEF(RT_Statusbar)   //!< Opaque type: Rome statusbar
	R2_OPAQUE_TYPEDEF(RT_SubObj)      //!< Opaque type: Rome subobject
*/
	typedef CLASS RT_Attr		    RT_Attr;        //!< Opaque pointer type: Rome attrs
	typedef CLASS RT_App		    RT_App;         //!< Opaque pointer type: Rome interface
	typedef CLASS RT_Database	    RT_Database;    //!< Opaque pointer type: Rome database
	typedef CLASS RT_DBFIND		    RT_DBFIND;      //!< Opaque pointer type: Rome find result set
	typedef CLASS RT_Engine		    RT_Engine;      //!< Opaque pointer type: Rome engine
	typedef CLASS RT_FileObj	    RT_FileObj;     //!< Opaque pointer type: Rome file
	typedef CLASS RT_Files		    RT_Files;       //!< Opaque pointer type: Rome filesystem
	typedef CLASS RT_Statusbar	    RT_Statusbar;   //!< Opaque pointer type: Rome statusbar
	typedef CLASS RT_SubObj		    RT_SubObj;      //!< Opaque pointer type: Rome subobject
#endif // RTYPES_H_INCLUDED

/////////////////////////////////////////////////////////////////////////////
// Modifiers used to export functions, classes, and data.

#ifndef AFX_API_EXPORT
    #define AFX_API_EXPORT	__declspec(dllexport)
    #define AFX_API_IMPORT	__declspec(dllimport)
#endif

#ifndef ROME_API
	#ifdef BUILD_ROMEDLL
		#define ROME_API		extern "C" AFX_API_EXPORT   //!< Qualifier for symbols exported as part of the Rome API.
	#else
		#define ROME_API		extern "C" AFX_API_IMPORT   //!< Qualifier for symbols exported as part of the Rome API.
	#endif
#endif

// Obtain a lock on access to the Rome API.
// These locks are used inside the Rome API functions to make them thread-safe.
// It is only necessary to use them outside these functions when they must run exclusively of Rome API functions.
#ifdef USE_ROME_API_LOCKING
#define ROME_API_TYPEDLOCK(type)		TYPEDLOCK3(&App.Engine.m_RomeApiLock, __LINE__, type)
#else
#define ROME_API_TYPEDLOCK(type)
#endif // USE_ROME_API_LOCKING
//
#define ROME_API_NOLOCK()				//!< For documentation purposes
#define ROME_API_LOCK()			        /*ASSERT(!App.Engine.IsTaskThread());*/ \
                                        /*ASSERT(!App.Engine.IsUpdateThread());*/ \
                                        ROME_API_TYPEDLOCK(READWRITE_LOCK_WRITE)

/////////////////////////////////////////////////////////////////////////////
// Application level functions
// These require an RT_App* pointer acquired from RomeInit().

// Flags used by RomeGetPropertyStr().
#define RX_PROPERTYSTR_APPFULLNAME				1	//!< e.g. "C:\Program Files\Rusle2\Binaries\Rusle2.exe".
#define RX_PROPERTYSTR_APPNAME					2	//!< e.g. "Rusle2".
#define RX_PROPERTYSTR_APPPATH					3	//!< e.g. "C:\Program Files\Rusle2\Binaries".
//
#define RX_PROPERTYSTR_DBAUTHOR					4	//!< Author name for entire database.
#define RX_PROPERTYSTR_DBCOMMENTS				5	//!< Comments for entire database.
#define RX_PROPERTYSTR_DBDATE					6	//!< Datestamp for entire database.
#define RX_PROPERTYSTR_DBFULLNAME				7	//!< e.g. "C:\Program Files\Rusle2\moses.gdb".
#define RX_PROPERTYSTR_DBNAME					8	//!< e.g. "moses.gdb".
#define RX_PROPERTYSTR_DBPATH					9	//!< e.g. "C:\Program Files\Rusle2".

// Flags used by RomeSetTitle().
#define RX_TITLES_USER                      (1<<0)  //!< Affect user  titles
#define RX_TITLES_FIXED                     (1<<1)  //!< Affect fixed titles (loaded)
#define RX_TITLES_INTERNAL                  (1<<10) //!< Affect internal titles (generated internally - e.g. "#APPVERSION").


//! Prototype for the callback function used for Rome notifications.
//! @param nCallbackType  The notification type.
//! @param pCallbackArg   Optional dditional callback information which depends on the notification type.
typedef RT_PCVOID (*RT_RomeCallback)(enum RomeNotificationType nCallbackType, RT_PCVOID pCallbackArg);

ROME_API RT_Database*	RomeGetDatabase(RT_App* pApp);
ROME_API RT_CSTR		RomeGetDirectory(RT_App* pApp, RT_CSTR pszName = NULL);
ROME_API RT_VOID		RomeGetDirectoryF(RT_PCHAR pBuf, RT_UINT nBufLen, RT_App* pApp, RT_CSTR pszName = NULL);
ROME_API RT_Engine*		RomeGetEngine(RT_App* pApp);
ROME_API RT_Files*		RomeGetFiles(RT_App* pApp);
ROME_API RT_CSTR		RomeGetPropertyStr(RT_App* pApp, RT_UINT nProp);
ROME_API RT_VOID		RomeGetPropertyStrF(RT_PCHAR pBuf, RT_UINT nBufLen, RT_App* pApp, RT_UINT nProp);
ROME_API RT_UINT		RomeGetScienceVersion(RT_App* pApp);
ROME_API RT_Statusbar*	RomeGetStatusbar(RT_App* pApp);

ROME_API RT_CSTR		RomeGetTitle(RT_App* pApp, RT_CSTR pszKey);
ROME_API RT_VOID		RomeGetTitleF(RT_PCHAR pBuf, RT_UINT nBufLen, RT_App* pApp, RT_CSTR pszKey);
ROME_API RT_BOOL		RomeSetTitle(RT_App* pApp, RT_CSTR pszKey, RT_CSTR pszTitle, RT_UINT nFlags);

ROME_API RT_CSTR        RomeGetLastError(RT_App* pApp);
ROME_API RT_BOOL        RomeSetLastError(RT_App* pApp, RT_CSTR pszInfo);

ROME_API RT_BOOL		RomeExit(RT_App* pApp);
ROME_API RT_App*		RomeInit(RT_CSTR pszArgs);

ROME_API RT_BOOL        RomeTemplateLoad(RT_App* pApp, RT_CSTR pszFilename);
ROME_API RT_BOOL        RomeTemplateSave(RT_App* pApp, RT_CSTR pszFilename = NULL);

/////////////////////////////////////////////////////////////////////////////
//! @name Rome Database functions
//! These require an RT_Database* pointer acquired from RomeGetDatabase().
//! @note This will be a non-NULL pointer, even if a database hasn't been opened yet.
//!   The pointer represents the abstract interface itself, not the database connection.
//! @{

// Info types used by RomeDatabaseFileInfo()
#define RX_DBFILEINFO_QUERY		 0 //!< (string result of query)
#define RX_DBFILEINFO_NAME		 1 //!<                name
#define RX_DBFILEINFO_PATH		 2 //!<         path
#define RX_DBFILEINFO_RIGHT		 3 //!<         path \ name
#define RX_DBFILEINFO_TABLE		 4 //!< table
#define RX_DBFILEINFO_OUTER		 5 //!< table \        name
#define RX_DBFILEINFO_LEFT		 6 //!< table \ path
#define RX_DBFILEINFO_FULL		 7 //!< table \ path \ name
#define RX_DBFILEINFO_OWNER		 8 //!< Owner field
#define RX_DBFILEINFO_GROUP		 9 //!< group field
#define RX_DBFILEINFO_PERMS		10 //!< perms field
#define RX_DBFILEINFO_DATE		11 //!< Date  field
#define RX_DBFILEINFO_DATA		12 //!< Data  field
#define RX_DBFILEINFO_FOLDER	13 //!< is this a folder? return "0" or "1".

ROME_API RT_BOOL		RomeDatabaseFileDelete(RT_Database* pDatabase, RT_CSTR pszPathname, RT_UINT nFlags);
ROME_API RT_CSTR		RomeDatabaseFileInfo(RT_Database* pDatabase, RT_CSTR pszFilename, RT_UINT nInfoType);
ROME_API RT_VOID		RomeDatabaseFileInfoF(RT_PCHAR pBuf, RT_UINT nBufLen, RT_Database* pDatabase, RT_CSTR pszFilename, RT_UINT nInfoType);
ROME_API RT_App*        RomeDatabaseGetApp(RT_Database* pDatabase);
ROME_API RT_BOOL		RomeDatabaseGetReadOnly(RT_Database* pDatabase);
ROME_API RT_BOOL		RomeDatabaseOpen(RT_Database* pDatabase, RT_CSTR pszDatabase);
ROME_API RT_BOOL		RomeDatabaseClose(RT_Database* pDatabase, RT_CSTR pszDatabase = NULL);

// Flags affecting the type of files/folders to match in DbFindOpen().
// These are identical to the DBSYS_FIND_... flags used internally.
#define RX_DBFIND_FILES			1<<0	//!< Match file names
#define RX_DBFIND_FOLDERS		1<<1	//!< Match folder names
#define RX_DBFIND_BOTH			(RX_DBFIND_FILES | RX_DBFIND_FOLDERS)	//!< Match both file and folder names
#define RX_DBFIND_RECURSE		1<<2	//!< Recurse into subdirectories.
#define RX_DBFIND_ADDROOT		1<<3	//!< Add the root to the search results
#define RX_DBFIND_EXACT			1<<4	//!< Find single record matching argument string
//
#define RX_DBFIND_FLAGBITS        10    //!< Number of legal flags in RomeDatabaseFindOpen().
#define RX_DBFIND_FLAGMASK ((1<<RX_DBFIND_FLAGBITS)-1)  //!< Mask of legal flag values in RomeDatabaseFindOpen().
//
// Values in this section are not flags. They are sequential values.
// These values do not combine with flag values.
#define RX_DBFIND_TABLES		1<<10	//!< Find all tables in the database
#define RX_DBFIND_QUERY			2<<10	//!< The pattern string is a SQL query.
//
//! Verify that flags used by RomeDatabaseFindOpen() are legal. The flags must either be a combination of flags OR'd together
//!   as part of the #RX_DBFIND_FLAGMASK range, or an integer shifted above that range.
#define RX_DBFIND_LEGAL_FLAGS(N) (((N) <= RX_DBFIND_FLAGMASK) || ((N) & RX_DBFIND_FLAGMASK) == 0)
#define RX_DBFIND_ASSERT_LEGAL_FLAGS(N) ASSERT(RX_DBFIND_LEGAL_FLAGS(N));

ROME_API RT_DBFIND*     RomeDatabaseFindOpen(RT_Database* pDatabase, RT_CSTR pszPattern, RT_UINT nFindFlags);
ROME_API RT_VOID        RomeDatabaseFindClose(RT_DBFIND* pDbFind);
ROME_API RT_INT         RomeDatabaseFindCount(RT_DBFIND* pDbFind);
ROME_API RT_CSTR        RomeDatabaseFindInfo(RT_DBFIND* pDbFind, RT_INT nIndex, RT_UINT nInfoType);
ROME_API RT_VOID        RomeDatabaseFindInfoF(RT_PCHAR pBuf, RT_UINT nBufLen, RT_DBFIND* pDbFind, RT_INT nIndex, RT_UINT nInfoType);


//! @}
/////////////////////////////////////////////////////////////////////////////
//! @name Date and Time functions
//! @{

// Formats used by Rome Date and Time functions
//
#define RX_DTFMT_R2		0	//!< 2002-12-01 23:15:01
							//!< This format sorts correctly using string sorting.
#define RX_DTFMT_OLE	1	//!< 12/01/2002 11:15:01 PM
							//!< Was the default output of COleDateTime::Format().
							//!< Now this uses 4-digit years and is a hard-coded format.
#define RX_DTFMT_BUG	2	//!< 2002-12-01 23h15m01
							//!< Outputs string legal in Win32 filename, for bug reports.
#define RX_DTFMT_ISO    3	//!< 2002-12-01T23:15:01 (ISO 8601 format - note 'T')
					        //!< This format sorts correctly using string sorting.

ROME_API RT_DATE		DateTimeCurrent();
ROME_API RT_CSTR		DateTimeCurrentStr(RT_UINT nType);
ROME_API RT_CSTR		DateTimeFormat(RT_DATE dt, RT_UINT nType);
ROME_API RT_DATE		DateTimeParse(RT_CSTR pszDate);


//! @}
/////////////////////////////////////////////////////////////////////////////
//! @name Computational engine
//! These require an RT_Engine* pointer acquired from RomeGetEngine().
//! @{

ROME_API RT_BOOL		RomeEngineFinishUpdates(RT_Engine* pEngine);
ROME_API RT_BOOL		RomeEngineGetAutorun(RT_Engine* pEngine);
ROME_API RT_BOOL		RomeEngineIsLocked(RT_Engine* pEngine);
ROME_API RT_INT			RomeEngineLockUpdate(RT_Engine* pEngine);
ROME_API RT_BOOL		RomeEngineRun(RT_Engine* pEngine);
ROME_API RT_INT			RomeEngineUnlockUpdate(RT_Engine* pEngine);
ROME_API RT_VOID		RomeEngineSetAutorun(RT_Engine* pEngine, RT_BOOL bAutorun);
ROME_API RT_BOOL		RomeEngineShowStatus(RT_Engine* pEngine, RT_BOOL bShowMessages);

//! @}
/////////////////////////////////////////////////////////////////////////////
//! @name Rome File functions
//! These require an RT_FileObj* pointer acquired from RomeFilesAdd(), RomeFilesGetItem() or RomeFilesOpen().
//! @{

// Values used by RomeFileSaveAsEx().
#define RX_FILE_SAVEASEX_NONE           0       //!< No flags
#define RX_FILE_SAVEASEX_CALC	     1<<5       //!< Save calculated data in \<Calc> tags
#define RX_FILE_SAVEASEX_PRIVATE     1<<7       //!< Create a private file (see #FSF_PRIVATE, #OMF_PRIVATE)

// "Variants" used when setting/getting floating point data.
#define RX_VARIANT_CATALOG      ((RT_UINT)-2)   //!< Variant used by catalog (currently always #RX_VARIANT_INTERVAL)
#define RX_VARIANT_TEMPLATE     ((RT_UINT)-1)   //!< Variant used by param pref in current user template
#define RX_VARIANT_NONE         ((RT_UINT) 0)   //!< None / Unspecified / Error
//
#define RX_VARIANT_INTERVAL     ((RT_UINT)70)   //!< Normal values
#define RX_VARIANT_CUMULATIVE   ((RT_UINT)71)   //!< Cumulative values
//#define RX_VARIANT_DIFFERENCE                 // (unimplemented)
//#define RX_VARIANT_FROMEND                    // (unimplemented)
//#define RX_VARIANT_MULTIPLE                   // (unimplemented)
//#define RX_VARIANT_PERCENT                    // (unimplemented)
//
#define RX_VARIANT_DEFAULT      RX_VARIANT_INTERVAL     //!< Default variant used by the catalog

ROME_API RT_BOOL		RomeFileClose(RT_FileObj* pFile);
ROME_API RT_BOOL		RomeFileDelete(RT_FileObj* pFile);
ROME_API RT_Attr*		RomeFileGetAttr(RT_FileObj* pFile, RT_CNAME pszAttrName);
ROME_API RT_SHORT		RomeFileGetAttrSize(RT_FileObj* pFile, RT_CNAME pszAttr);
ROME_API RT_INT         RomeFileGetAttrSizeEx(RT_FileObj* pFile, RT_CNAME pszAttr);
ROME_API RT_CSTR		RomeFileGetAttrValue(RT_FileObj* pFile, RT_CNAME pszAttr, RT_SHORT nIndex);
ROME_API RT_VOID		RomeFileGetAttrValueF(RT_PCHAR pBuf, RT_UINT nBufLen, RT_FileObj* pFile, RT_CNAME pszAttr, RT_SHORT nIndex);
ROME_API RT_CSTR		RomeFileGetFullname(RT_FileObj* pFile);
ROME_API RT_VOID		RomeFileGetFullnameF(RT_PCHAR pBuf, RT_UINT nBufLen, RT_FileObj* pFile);
ROME_API RT_INT			RomeFileSave(RT_FileObj* pFile);
ROME_API RT_BOOL		RomeFileSaveAs(RT_FileObj* pFile, RT_CSTR pszNewName);
ROME_API RT_BOOL		RomeFileSaveAsEx(RT_FileObj* pFile, RT_CSTR pszNewName, RT_UINT nFlags);
ROME_API RT_SHORT		RomeFileSetAttrSize(RT_FileObj* pFile, RT_CNAME pszAttr, RT_SHORT nNewSize);
ROME_API RT_SHORT		RomeFileSetAttrValue(RT_FileObj* pFile, RT_CNAME pszAttr, RT_CSTR pszValue, RT_SHORT nIndex);

ROME_API RT_BOOL		RomeFileGetFloatArray(RT_FileObj* pFile, RT_CNAME pszAttr, RT_REAL* pArray, RT_INT* pSize, RT_UINT nVariant = RX_VARIANT_CATALOG, RT_CNAME pszUnit = "");

//! @} // Rome File functions
/////////////////////////////////////////////////////////////////////////////
//! @name Rome Filesystem functions
//! @{
//! These require an RT_Files* pointer acquired from RomeGetFiles().

// Flags for closing files used by RomeFilesCloseAll().
#define RX_CLOSEALL_SAVE		    (1<<0)		//!< Allow saving modified [and temp] files
#define RX_CLOSEALL_CANCEL	        (1<<1)		//!< Allow canceling the operation
#define RX_CLOSEALL_INTERNAL	    (1<<2)		//!< Internally generated command (controls logging).
#define RX_CLOSEALL_TEMP		    (1<<3)		//!< Close temporary files as well
#define RX_CLOSEALL_USED		    (1<<4)		//!< Close files still being used (independent of #RX_CLOSEALL_UNUSED)
#define RX_CLOSEALL_NOCLOSE	        (1<<5)		//!< Close internal #OBJT_NOCLOSE files
#define RX_CLOSEALL_NOMODIFIED		(1<<6)		//!< Don't close modified files
#define RX_CLOSEALL_NOUPDATE		(1<<7)		//!< Don't allow engine to finish before closing files
#define RX_CLOSEALL_NOUNUSED 		(1<<8)      //!< Don't close unused top-level files (independent of #RX_CLOSEALL_NOUSED)
#define RX_NOCLOSE_FILE				(1<<9)		//!< Don't close the file (usually because am inside it!)
#define RX_CLOSEALL_FORCE           (1<<10)		//!< Force the file to close, even if not top level, without reopening it.
//
// Flags for default behaviors.
#define RX_CLOSEALL_NOSAVE			(   0)      //!< Don't allow saving
#define RX_CLOSEALL_NOCANCEL		(   0)		//!< Don't allow user to cancel
#define RX_CLOSEALL_EXTERNAL	    (   0)		//!< Externally generated command.
#define RX_CLOSEALL_NOTEMP			(   0)		//!< Don't close temporary files
#define RX_CLOSEALL_NOUSED			(   0)		//!< Don't close files still in use (independent of #RX_CLOSEALL_NOUNUSED)
#define RX_CLOSEALL_MODIFIED	    (   0)		//!< Allow closing modified files
#define RX_CLOSEALL_UPDATE		    (   0)		//!< Allow engine to finish before closing files
#define RX_CLOSEALL_UNUSED          (   0)      //!< Close unused top-level files (independent of #RX_CLOSEALL_USED)
#define RX_CLOSEALL_NOFORCE         (   0)		//!< Don't force the file to close, even if not top level, without reopening it.
//
// Composite flags, used to re-implement internal functions of the same name.
#define RX_CLOSEALL_DeleteAllFiles	(RX_CLOSEALL_NOSAVE | RX_CLOSEALL_NOCANCEL | RX_CLOSEALL_TEMP | RX_CLOSEALL_USED | RX_CLOSEALL_NOCLOSE | RX_CLOSEALL_MODIFIED)
                                    //!< Close all files.
#define RX_CLOSEALL_DeleteUnused    (RX_CLOSEALL_SAVE | RX_CLOSEALL_CANCEL)
                                    //!< Close all unused files.
#define RX_CLOSEALL_CloseTempFiles  (RX_CLOSEALL_SAVE | RX_CLOSEALL_CANCEL | RX_CLOSEALL_TEMP)
                                    //!< Close all temporary files.

// File open mode flags used by RomeFilesOpen().
#define RX_FILESOPEN_USE_OPEN		(1<<0)		//!< Use an existing open copy
#define RX_FILESOPEN_NO_CREATE		(1<<1)		//!< Don't create if not found in the database
#define RX_FILESOPEN_TEMP			(1<<2)		//!< Create a temporary file
#define RX_FILESOPEN_NO_DEFAULT		(1<<3)		//!< Don't open user selected default instead of literal default
#define RX_FILESOPEN_SHOW_VIEW		(1<<4)		//!< Open a view for the file (not used by all fns)
#define RX_FILESOPEN_NO_KNOWN		(1<<5)		//!< Don't use map of known bad files
#define RX_FILESOPEN_SAVE_EDITED	(1<<6)		//!< Save an edited file before reopening (if flag #RX_FILESOPEN_USE_OPEN not set)
#define RX_FILESOPEN_CLONE			(1<<7)		//!< Return a clone of the file asked for.
#define RX_FILESOPEN_USERRENAME		(1<<8)		//!< Allow user renaming the opened file
#define RX_FILESOPEN_PRIVATE		(1<<9)		//!< Open as private file (don't allow save, excludes flag #RX_FILESOPEN_TEMP)
#define RX_FILESOPEN_LOG_HIST		(1<<10) 	//!< Log this action to the history log (commands.xml)
#define RX_FILESOPEN_CMD_USER		(1<<11)	    //!< Caused by a user action (for logging purposes)
#define RX_FILESOPEN_FORMAT_CFF		(1<<12) 	//!< Data is in CFF format (for some fns only)
#define RX_FILESOPEN_FORMAT_XML		(    0) 	//!< Data is in XML format (for some fns only)
#define RX_FILESOPEN_NO_REGISTER	(1<<13) 	//!< Don't add the filename to the map of open files
#define RX_FILESOPEN_NO_LOAD 		(1<<14) 	//!< Don't load a file from the database
#define RX_FILESOPEN_INTERNAL		RX_FILESOPEN_PRIVATE

// Pragma types
#define RX_PRAGMA_DB_SYNCH_ON		0	//!< Turn disk synchronization on
#define RX_PRAGMA_DB_SYNCH_OFF		1	//!< Turn disk synchronization off
#define RX_PRAGMA_DB_TRANS_BEGIN	2	//!< Begin transaction
#define RX_PRAGMA_DB_TRANS_END		3	//!< End transaction
#define RX_PRAGMA_DB_CLEAR_CACHE	4	//!< Clear the cache of known records

ROME_API RT_FileObj*	RomeFilesAdd(RT_Files* pFiles, RT_CNAME pszObjType, RT_CSTR pszFullname);
ROME_API RT_VOID		RomeFilesCloseAll(RT_Files* pFiles, RT_UINT nFlags = 0);
ROME_API RT_INT			RomeFilesGetCount(RT_Files* pFiles);
ROME_API RT_FileObj*	RomeFilesGetItem(RT_Files* pFiles, RT_INT nItem);
ROME_API RT_FileObj*	RomeFilesOpen(RT_Files* pFiles, RT_CSTR pszFilename, RT_UINT nFlags);
ROME_API RT_INT			RomeFilesPragma(RT_Files* pFiles, RT_UINT nPragma, RT_VOID* pExtra);

//! @} // Rome Filesystem functions
/////////////////////////////////////////////////////////////////////////////
// Rome Notification functions
//
//! Rome Notifications are for events that happen in the Rome server.
//! These are custom "messages" sent from the Rome server to be
//!   handled by the client.<br>
//! They do not specify what action this should trigger in the client.
//!   Rome notifications can generally be safely ignored by the client.<br>
//! Handlers should return #RX_TRUE if successfully handled.
//! If a notification is not handled, the server may use a default handler.
//!
enum RomeNotificationType
{
	RX_NOTIFY_APP_APPNAME			= 0x000,	//!< Name of application (e.g "Rusle2").
	RX_NOTIFY_APP_EXENAME			= 0x001,	//!< Name of EXE without extension (e.g "RUSLE2").
    //
	RX_NOTIFY_ACCESS_LOAD			= 0x100,	//!< Current access level changed (@p lParam is new access name)
	RX_NOTIFY_ACCESS_SAVE			= 0x101,	//!< Current access level saved (@p lParam is new access name)
    //
	RX_NOTIFY_FILE_CLOSE			= 0x200,	//!< The file is closing.
	RX_NOTIFY_FILE_SAVE				= 0x201,	//!< The file has been saved.
	RX_NOTIFY_FILE_NEEDSHUFFLE		= 0x202,	//!< The file/obj had a structure change and requires regenerating visuals.
	RX_NOTIFY_FILE_SHOWVIEW			= 0x203,	//!< Show the view for this file/obj (@p lParam is obj ptr)
	RX_NOTIFY_FILE_INVALIDATE		= 0x204,	//!< Invalidate the view for this file/obj (@p lParam is obj ptr)
	RX_NOTIFY_FILE_TITLECHANGED		= 0x205,	//!< The file/object title has changed
	RX_NOTIFY_FILE_EDITED   		= 0x206,	//!< The file/object edited state has changed
    //
	RX_NOTIFY_REGISTRY_GETVALUE		= 0x300,	//!< Get value of registry sub-key value (@p lParam is sub-key)
	RX_NOTIFY_REGISTRY_SETVALUE		= 0x301,	//!< Set value of registry sub-key value (@p lParam is sub-key xFF value)
	RX_NOTIFY_REGISTRY_DELVALUE		= 0x302,	//!< Delete value of registry sub-key value (@p lParam is sub-key)
    //
	RX_NOTIFY_TEMPLATE_LOAD			= 0x400,	//!< Current template  (@p lParam is filename)
	RX_NOTIFY_TEMPLATE_DELETE		= 0x401,	//!< Current template is being destroyed.
	RX_NOTIFY_TEMPLATE_SAVE			= 0x402,	//!< Current template  (@p lParam is filename)
    //
	RX_NOTIFY_REPORT_ERROR			= 0x500,	//!< Error: show bug report (@p lParam is cause)
	RX_NOTIFY_REPORT_SYSERROR		= 0x501,	//!< Error: show sytem error dialog (@p lParam is cause)
    //
	RX_NOTIFY_UNITS_CHANGE			= 0x600,	//!< Unit system changed (@p lParam is new system)
	RX_NOTIFY_UNITS_ATTR_CHANGE		= 0x601,	//!< Attr unit changed (@p lParam is param name)
    //
	RX_NOTIFY_PROGRESS_WAIT_BEGIN	= 0x700,	//!< Begin a long wait operation (show busy cursor)
	RX_NOTIFY_PROGRESS_WAIT_END		= 0x701,	//!< End   a long wait operation (hide busy cursor)
	RX_NOTIFY_PROGRESS_STATUSMSG    = 0x702,	//!< Display a status [bar] message
    //
	RX_NOTIFY_DIALOG_MESSAGEBOX		= 0x800,	//!< Message box dialog. (@p lParam is concatenated encoded arguments)
    //
	RX_NOTIFY_XML_FILE_COMPARE		= 0x900,	//!< Compare 2 external Rusle2 XML files and return #RX_TRUE if different
    //
    RX_NOTIFY_TASK_FINISHED         = 0xA00,    //!< Engine task has finished

	// Temporary notifications.
	// These need to be renamed and redesigned to behave better.
	// Notifications should *alert* the client to a state change, not
	//   mandate a specific activity that the core code shouldn't know
	//   anything about.
	RX_NOTIFY_GRAPH_ATTRCHANGE      = 0x1001,	//!< Redraw Graph because of change to attr. (@p lParam is RT_Attr*)
	RX_NOTIFY_GRAPH_ATTRDELETE      = 0x1002,	//!< Destroy graph window because attr is being destroyed. (@p lParam is RT_Attr*)
};

/////////////////////////////////////////////////////////////////////////////
//! @name Rome Statusbar functions
//! These require an RT_Statusbar* pointer acquired from RomeGetStatusbar().
//! @{

ROME_API RT_BOOL		RomeProgressCreate(RT_Statusbar* pStatus, RT_INT nLower = 0, RT_INT nUpper = 100, RT_INT nStep = 1);
ROME_API RT_BOOL		RomeProgressSetRange(RT_Statusbar* pStatus, RT_INT nLower = 0, RT_INT nUpper = 100);
ROME_API RT_BOOL		RomeProgressSetStep(RT_Statusbar* pStatus, RT_INT nStep = 1);
ROME_API RT_BOOL		RomeProgressStepIt(RT_Statusbar* pStatus);
ROME_API RT_BOOL		RomeProgressDestroy(RT_Statusbar* pStatus);
ROME_API RT_BOOL		RomeStatusbarMessage(RT_Statusbar* pStatus, RT_CSTR lpszNewText, RT_BOOL bUpdate = RX_TRUE);

//! @}
/////////////////////////////////////////////////////////////////////////////

#endif // API_ROME_H_INCLUDED

