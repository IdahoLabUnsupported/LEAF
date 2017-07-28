// Copyright 2017 Battelle Energy Alliance, LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#pragma once

// --- LEAF Includes --- //
#include <leaf/wrapper/java/lang/Object.h>
#include <leaf/wrapper/java/lang/String.h>

#include <leaf/wrapper/weru/weps/RunFileBean.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace weps
{

class RunFileData;
typedef boost::shared_ptr< RunFileData > RunFileDataPtr;

class RunFileData : public java::lang::Object
{
public:
    ///
    RunFileData(
        bool const& isStatic = false );

    ///
    RunFileData(
        jobject const& o );

    ///
    virtual ~RunFileData();

    ///
    static void Static();

    ///
    static java::lang::String const& AverageSlope();

    ///
    static java::lang::String const& CalibFileName();

    ///
    static java::lang::String const& CycleCount();

    ///
    static java::lang::String const& ManageFile();

    ///
    static java::lang::String const& RotationYears();

    ///
    static java::lang::String const& RunTypeDisp();

    ///
    static java::lang::String const& SoilFile();

    ///
    static java::lang::String const& WaterErosionLoss();

    ///
    void FireAll() const;

    ///
    RunFileBeanPtr GetBean() const;

    ///
    java::lang::StringPtr GetData(
        java::lang::String const& idxstr ) const;

    ///
    void Initialize() const;

    ///
    void SetData(
        java::lang::String const& idxstr,
        java::lang::String const& newData ) const;

    ///
    void SetRotationYears(
        java::lang::String const& manageFile ) const;

    ///
    void UpdateDates() const;

    ///
    void WriteRunFile(
        java::lang::String const& runDirPath ) const;

    ///
    static jclass const& GetJclass();

protected:

private:
    ///
    static RunFileData& Instance(
        bool const& erase = false );

    ///
    static jmethodID const& GetJmid(
        std::string const& name );

    ///
    static jfieldID const& GetJfid(
        std::string const& name );

    ///
    java::lang::StringPtr m_AverageSlope;

    ///
    java::lang::StringPtr m_CalibFileName;

    ///
    java::lang::StringPtr m_CycleCount;

    ///
    java::lang::StringPtr m_ManageFile;

    ///
    java::lang::StringPtr m_RotationYears;

    ///
    java::lang::StringPtr m_RunTypeDisp;

    ///
    java::lang::StringPtr m_SoilFile;

    ///
    java::lang::StringPtr m_WaterErosionLoss;

};

} //end weps
} //end weru
} //end wrapper
} //end leaf
