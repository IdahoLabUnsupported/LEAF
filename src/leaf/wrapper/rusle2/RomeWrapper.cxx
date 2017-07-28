/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 *
 * Copyright 2012-2013 by Ames Laboratory
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.rb END do not edit this line> ***************/


// --- LEAF Includes --- //
#include <leaf/wrapper/rusle2/RomeWrapper.h>

// --- ROME Includes --- //
#include <cstdlib> //This must be included before api-rome.h
#if __RUSLE2_version <= 2000004
#define AFX_API_IMPORT
#endif //__RUSLE2_version
#include <api-rome.h>

// --- Boost Includes --- //
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

// --- STL Includes --- //
#include <iostream>
#include <sstream>

namespace leaf
{
namespace wrapper
{
namespace rusle2
{

////////////////////////////////////////////////////////////////////////////////
RomeWrapper::RomeWrapper()
    :
    m_rtApp( NULL ),
    m_rtEngine( NULL ),
    m_rtDatabase( NULL ),
    m_rtFiles( NULL ),
    m_pProfile( NULL ),
    m_pContour( NULL ),
    m_pClimate( NULL ),
    m_pSoil( NULL ),
    m_pStripBarrier( NULL ),
    m_pManagement( NULL ),
    m_sciVer( 0 ),
    m_sciVerStr( "" )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
RomeWrapper::~RomeWrapper()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void RomeWrapper::Exit()
{
    m_pProfile = NULL;
    m_pContour = NULL;
    m_pClimate = NULL;
    m_pSoil = NULL;
    m_pStripBarrier = NULL;
    m_pManagement = NULL;

    CloseAllFiles();

    //Shutdown connection to RomeDLL
    RT_BOOL success = RomeExit( m_rtApp );
    if( !success )
    {
        OutputErrorInfo();
    }

    m_rtFiles = NULL;
    m_rtDatabase = NULL;
    m_rtEngine = NULL;
    m_rtApp = NULL;
}
////////////////////////////////////////////////////////////////////////////////
RomeWrapper& RomeWrapper::Instance(
    bool const& erase )
{
    static RomeWrapper* sRomeWrapper = new RomeWrapper();
    if( erase )
    {
        sRomeWrapper->Exit();
        delete sRomeWrapper;
        sRomeWrapper = NULL;
    }

    return *sRomeWrapper;
}
////////////////////////////////////////////////////////////////////////////////
void RomeWrapper::Init(
    std::string const& dirRoot,
    std::string const& dbPath )
{
    std::stringstream arguments;
    arguments << "RomeDLL "
              << "\"/DirRoot=" << dirRoot << "\" "
              << "\"/Path:Session=" << dirRoot << "\" "
              << "\"/Path:Users=" << dirRoot << "\" "
              << "\"/UnitSystem=US\"";

    //Create connection to the Rome DLL
    m_rtApp = RomeInit( arguments.str().c_str() );
    if( !m_rtApp )
    {
        OutputErrorInfo();
    }

    //Turn off Autorun
    m_rtEngine = RomeGetEngine( m_rtApp );
    if( !m_rtEngine )
    {
        OutputErrorInfo();
    }
    RomeEngineSetAutorun( m_rtEngine, RX_FALSE );

    //Get a pointer to the Rome database
    m_rtDatabase = RomeGetDatabase( m_rtApp );
    if( !m_rtDatabase )
    {
        OutputErrorInfo();
    }

    //"http"
    //"internal"
    //"sqlite2"
    //"file"
    //"msaccess"
    //std::string dbPath = "file://" + boost::filesystem::current_path().string();
    RT_BOOL success = RomeDatabaseOpen( m_rtDatabase, dbPath.c_str() );
    if( !success )
    {
        OutputErrorInfo();
    }

    //Get a pointer to the Rome file system
    m_rtFiles = RomeGetFiles( m_rtApp );
    if( !m_rtFiles )
    {
        OutputErrorInfo();
    }

    Reset();

    //Get science version
    m_sciVer = RomeGetScienceVersion( m_rtApp );
    m_sciVerStr = boost::lexical_cast< std::string >( m_sciVer );
}
////////////////////////////////////////////////////////////////////////////////
RT_FileObj* RomeWrapper::AddFile(
    ObjectType const& objType,
    std::string const& fullName )
{
    RT_FileObj* fileObj = RomeFilesAdd(
        m_rtFiles, NAME( objType ).c_str(), fullName.c_str() );
    if( !fileObj )
    {
        OutputErrorInfo();
    }

    return fileObj;
}
////////////////////////////////////////////////////////////////////////////////
bool RomeWrapper::CloseFile(
    RT_FileObj* fileObj )
{
    RT_BOOL retVal = RomeFileClose( fileObj );

    //return RX_TRUE    ( 1) if the file was closed/reloaded
    //       RX_FALSE   ( 0) on failure
    //       RX_FAILURE (-1) on error
    switch( retVal )
    {
        case 1:
            return true;
        case 0:
            return false;
        default:
        {
            OutputErrorInfo();
            return false;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void RomeWrapper::CloseAllFiles(
    unsigned int const& nFlags )
{
    //RX_CLOSEALL_SAVE : Allow saving modified [and temp] files
      //If this flag isn't set, flag #RX_CLOSEALL_CANCEL will have no effect
    //RX_CLOSEALL_CANCEL : Allow canceling this operation
      //The [Cancel] button won't be shown unless this flag is set
      //Note: This only works in applications which handle dialog notifications
    //RX_CLOSEALL_TEMP : Close temporary files
      //Normally temporary files remain open until the app closes
    //RX_CLOSEALL_USED : Close files still being used
    //RX_CLOSEALL_NOCLOSE : Close internal #OBJT_NOCLOSE files
      //This is normally only done on app shutdown
    //RX_CLOSEALL_NOMODIFIED : Don't close modified files
    //RX_CLOSEALL_NOUPDATE : Don't allow engine to finish before closing files
    //RX_CLOSEALL_NOUNUSED : Don't close unused top-level files
      //This is independent of #RX_CLOSEALL_NOUSED
    RomeFilesCloseAll( m_rtFiles, nFlags );
}
////////////////////////////////////////////////////////////////////////////////
bool RomeWrapper::DeleteFile(
    RT_FileObj* fileObj )
{
    RT_BOOL retVal = RomeFileDelete( fileObj );

    //return RX_TRUE    ( 1) if the file was closed/reloaded
    //       RX_FALSE   ( 0) on failure
    //       RX_FAILURE (-1) on error
    switch( retVal )
    {
        case 1:
            return true;
        case 0:
            return false;
        default:
        {
            OutputErrorInfo();
            return false;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
bool RomeWrapper::FinishUpdates()
{
    RT_BOOL retVal = RomeEngineFinishUpdates( m_rtEngine );

    //return RX_TRUE    ( 1) on success
    //       RX_FALSE   ( 0) on failure
    //       RX_FAILURE (-1) on error
    switch( retVal )
    {
        case 1:
            return true;
        case 0:
        default:
        {
            OutputErrorInfo();
            return false;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
std::string RomeWrapper::GetFullname(
    RT_FileObj* fileObj )
{
    RT_CSTR attrVal = RomeFileGetFullname( fileObj );
    if( !attrVal )
    {
        OutputErrorInfo();
    }

    return attrVal;
}
////////////////////////////////////////////////////////////////////////////////
std::string RomeWrapper::GetAttrVal(
    RT_FileObj* fileObj,
    std::string const& attrName,
    unsigned int const& index )
{
    RT_CSTR attrVal = RomeFileGetAttrValue( fileObj, attrName.c_str(), index );
    if( !attrVal )
    {
        OutputErrorInfo();
    }

    return attrVal;
}
////////////////////////////////////////////////////////////////////////////////
short int RomeWrapper::GetAttrSize(
    RT_FileObj* fileObj,
    std::string const& attrName )
{
    RT_SHORT attrSize = RomeFileGetAttrSize( fileObj, attrName.c_str() );
    if( attrSize == -1 )
    {
        OutputErrorInfo();
    }

    return attrSize;
}
////////////////////////////////////////////////////////////////////////////////
unsigned int const& RomeWrapper::GetSciVer() const
{
    return m_sciVer;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& RomeWrapper::GetSciVerStr() const
{
    return m_sciVerStr;
}
////////////////////////////////////////////////////////////////////////////////
RT_FileObj*& RomeWrapper::OBJECT(
    ObjectType const& objType )
{
    switch( objType )
    {
        case OBJTYP_SLOPE:
        {
            return m_pProfile;
        }
        case OBJTYP_CONTOUR:
        {
            return m_pContour;
        }
        case OBJTYP_CLIMATE:
        {
            return m_pClimate;
        }
        case OBJTYP_SOIL:
        {
            return m_pSoil;
        }
        case OBJTYP_STRIPBARRIER:
        {
            return m_pStripBarrier;
        }
        case OBJTYP_MANAGEMENT:
        {
            return m_pManagement;
        }
        default:
            return m_pProfile;
    }
}
////////////////////////////////////////////////////////////////////////////////
RT_FileObj* RomeWrapper::OpenFile(
    std::string const& filePath,
    unsigned int const& nFlags )
{
    //File open mode flags used by RomeFilesOpen()
    //RX_FILESOPEN_USE_OPEN : Use an existing open copy
    //RX_FILESOPEN_NO_CREATE : Don't create if not found in the database
    //RX_FILESOPEN_TEMP : Create a temporary file
    //RX_FILESOPEN_NO_DEFAULT : Don't open user default if not found -- use model
    //RX_FILESOPEN_SHOW_VIEW : Open a view for the file (not used by all fns)
    //RX_FILESOPEN_NO_KNOWN : Don't use map of known bad files
    //RX_FILESOPEN_SAVE_EDITED : Save an edited file before reopening (if flag #RX_FILESOPEN_USE_OPEN not set)
    //RX_FILESOPEN_CLONE : Return a clone of the file asked for
    //RX_FILESOPEN_USERRENAME : Allow user renaming the opened file
    //RX_FILESOPEN_PRIVATE : Open as private file (don't allow save, excludes flag #RX_FILESOPEN_TEMP)
    //RX_FILESOPEN_LOG_HIST : Log this action to the history log (commands.xml)
    //RX_FILESOPEN_CMD_USER : Caused by a user action (for logging purposes)
    //RX_FILESOPEN_FORMAT_CFF : Data is in CFF format (for some fns only)
    //RX_FILESOPEN_FORMAT_XML : Data is in XML format (for some fns only)
    //RX_FILESOPEN_NO_REGISTER : Don't add the filename to the map of open files
    //RX_FILESOPEN_NO_LOAD : Don't load a file from the database
    //RX_FILESOPEN_INTERNAL : RX_FILESOPEN_PRIVATE
    RT_FileObj* rtFileObj =
        RomeFilesOpen( m_rtFiles, filePath.c_str(), nFlags );
    if( !rtFileObj ) OutputErrorInfo();

    return rtFileObj;
}
////////////////////////////////////////////////////////////////////////////////
void RomeWrapper::Reset()
{
    m_pProfile = NULL;
    m_pContour = NULL;
    m_pClimate = NULL;
    m_pSoil = NULL;
    m_pStripBarrier = NULL;
    m_pManagement = NULL;

    CloseAllFiles();

#if __RUSLE2_version >= 2002005
    //RX_FILESOPEN_INTERNAL is important to allow modification of attributes
    m_pProfile = OpenFile( TBL( OBJTYP_SLOPE ) +
        "\\" + "#ENTRY_DEFAULT", RX_FILESOPEN_INTERNAL );
#else
    m_pProfile = OpenFile( TBL( OBJTYP_SLOPE ) +
        "\\" + "#ENTRY_DEFAULT" );
    GetAttrVal( m_pProfile, PTR( OBJTYP_CONTOUR ) );
    GetAttrVal( m_pProfile, PTR( OBJTYP_CLIMATE ) );
    GetAttrVal( m_pProfile, PTR( OBJTYP_SOIL ) );
    GetAttrVal( m_pProfile, PTR( OBJTYP_STRIPBARRIER ) );
    GetAttrVal( m_pProfile, PTR( OBJTYP_MANAGEMENT ) );
#endif //__RUSLE2_version
}
////////////////////////////////////////////////////////////////////////////////
bool RomeWrapper::Run()
{
    //Force calculation of outputs whose inputs have changed
    //NOTE: This is necessary because we have set Autorun to FALSE above
    //NOTE: This call does not cause unnecessary calculation of outputs
    //      that are already current
    RT_BOOL retVal = RomeEngineRun( m_rtEngine );

    //return RX_TRUE    ( 1) on success
    //       RX_FALSE   ( 0) on failure
    //       RX_FAILURE (-1) on error
    switch( retVal )
    {
        case 1:
            return true;
        case 0:
        default:
        {
            OutputErrorInfo();
            return false;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
bool RomeWrapper::SaveFile(
    RT_FileObj* fileObj,
    std::string const& filePath )
{
    RT_BOOL retVal = RomeFileSaveAs( fileObj, filePath.c_str() );

    //return RX_TRUE    ( 1) on success
    //       RX_FALSE   ( 0) on failure
    //       RX_FAILURE (-1) on error
    switch( retVal )
    {
        case 1:
            return true;
        case 0:
        default:
        {
            OutputErrorInfo();
            return false;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
bool RomeWrapper::SetAttrVal(
    RT_FileObj* fileObj,
    std::string const& attrName,
    std::string const& attrVal,
    unsigned int const& index )
{
    RT_SHORT retVal = RomeFileSetAttrValue(
        fileObj, attrName.c_str(), attrVal.c_str(), index );

    //return RX_TRUE    ( 1) if the value changed
    //       RX_FALSE   ( 0) if unchanged
    //       RX_FAILURE (-1) on error
    switch( retVal )
    {
        case 1:
        case 0:
            return true;
        default:
        {
            OutputErrorInfo();
            return false;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
bool RomeWrapper::SetAttrSize(
    RT_FileObj* fileObj,
    std::string const& attrName,
    short int const& attrVal )
{
    RT_SHORT retVal = RomeFileSetAttrSize( fileObj, attrName.c_str(), attrVal );

    //return RX_TRUE    ( 1) if the value changed
    //       RX_FALSE   ( 0) if unchanged
    //       RX_FAILURE (-1) on error
    switch( retVal )
    {
        case 1:
        case 0:
            return true;
        default:
        {
            OutputErrorInfo();
            return false;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void RomeWrapper::OutputErrorInfo()
{
    //Check for Rome error info
    //This will work even if pRome is NULL
    RT_CSTR pszRomeError = RomeGetLastError( m_rtApp );
    if( !pszRomeError ) return;
    throw std::runtime_error( pszRomeError );
}
////////////////////////////////////////////////////////////////////////////////

} //end rusle2
} //end wrapper
} //end leaf
