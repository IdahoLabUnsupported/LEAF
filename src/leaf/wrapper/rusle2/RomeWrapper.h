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


#pragma once

// --- LEAF Includes --- //
#include <leaf/wrapper/rusle2/Common.h>

// --- ROME Includes --- //
struct RT_App;
struct RT_Engine;
struct RT_Database;
struct RT_Files;
struct RT_FileObj;

// --- Boost Includes --- //
#include <boost/noncopyable.hpp>
#include <boost/assign.hpp>

namespace leaf
{
namespace wrapper
{
namespace rusle2
{

class LEAF_RUSLE2_EXPORTS RomeWrapper : public boost::noncopyable
{
public:
    ///
    void Exit();

    ///
    static RomeWrapper& Instance(
        bool const& erase = false );

    ///
    void Init(
        std::string const& dirRoot,
        std::string const& dbPath );

    ///
    RT_FileObj* AddFile(
        ObjectType const& objType,
        std::string const& fullName );

    ///
    bool CloseFile(
        RT_FileObj* fileObj );

    ///
    void CloseAllFiles(
        unsigned int const& nFlags = 0 );

    ///Deprecated
    bool DeleteFile(
        RT_FileObj* fileObj );

    ///
    bool FinishUpdates();

    ///
    std::string GetFullname(
        RT_FileObj* fileObj );

    ///
    std::string GetAttrVal(
        RT_FileObj* fileObj,
        std::string const& attrName,
        unsigned int const& index = 0 );

    ///
    short int GetAttrSize(
        RT_FileObj* fileObj,
        std::string const& attrName );

    ///
    unsigned int const& GetSciVer() const;

    ///
    std::string const& GetSciVerStr() const;

    ///
    RT_FileObj*& OBJECT(
        ObjectType const& objType );

    ///
    RT_FileObj* OpenFile(
        std::string const& filePath,
        unsigned int const& nFlags = 0 );

    ///
    void Reset();

    ///
    bool Run();

    ///
    bool SaveFile(
        RT_FileObj* fileObj,
        std::string const& filePath );

    ///
    bool SetAttrVal(
        RT_FileObj* fileObj,
        std::string const& attrName,
        std::string const& attrVal,
        unsigned int const& index = 0 );

    ///
    bool SetAttrSize(
        RT_FileObj* fileObj,
        std::string const& attrName,
        short int const& attrVal );

protected:
    ///
    RomeWrapper();

    ///
    ~RomeWrapper();

private:
    ///
    void OutputErrorInfo();

    ///
    RT_App* m_rtApp;

    ///
    RT_Engine* m_rtEngine;

    ///
    RT_Database* m_rtDatabase;

    ///
    RT_Files* m_rtFiles;

    ///SLOPE object
    RT_FileObj* m_pProfile;

    ///CONTOUR object
    RT_FileObj* m_pContour;

    ///CLIMATE object
    RT_FileObj* m_pClimate;

    ///SOIL object
    RT_FileObj* m_pSoil;

    ///STRIP_BARRIER object
    RT_FileObj* m_pStripBarrier;

    ///MANAGEMENT object
    RT_FileObj* m_pManagement;

    ///
    unsigned int m_sciVer;

    ///
    std::string m_sciVerStr;

};

} //end rusle2
} //end wrapper
} //end leaf
