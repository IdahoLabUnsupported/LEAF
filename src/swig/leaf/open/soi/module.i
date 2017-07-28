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



%module leaf_open_soi
%{
    #include <leaf/open/soi/Soils.h>
    #include <leaf/open/soi/MapUnit.h>
    #include <leaf/open/soi/Component.h>
    #include <leaf/open/soi/Corestriction.h>
    #include <leaf/open/soi/CHorizon.h>
    #include <leaf/open/soi/ChFrag.h>
    #include <leaf/open/soi/ChTextureGrp.h>
    #include <leaf/open/soi/ChTexture.h>
%}

%import <leaf/open/module.i>

%typemap( csimports ) SWIGTYPE
%{
    using System;
    using System.Runtime.InteropServices;

    using Poco;
    using Poco.Data;
    using Poco.Dynamic;

    using Object = leaf.open.Object;
%}

%include <leaf/open/soi/SoilsPtr.h>
%shared_ptr( leaf::open::soi::Soils )
%include <leaf/open/soi/MapUnitPtr.h>
%shared_ptr( leaf::open::soi::MapUnit )
%include <leaf/open/soi/ComponentPtr.h>
%shared_ptr( leaf::open::soi::Component )
%include <leaf/open/soi/CorestrictionPtr.h>
%shared_ptr( leaf::open::soi::Corestriction )
%include <leaf/open/soi/CHorizonPtr.h>
%shared_ptr( leaf::open::soi::CHorizon )
%include <leaf/open/soi/ChFragPtr.h>
%shared_ptr( leaf::open::soi::ChFrag )
%include <leaf/open/soi/ChTextureGrpPtr.h>
%shared_ptr( leaf::open::soi::ChTextureGrp )
%include <leaf/open/soi/ChTexturePtr.h>
%shared_ptr( leaf::open::soi::ChTexture )

%include <leaf/open/soi/ChTexture.h>
%include <leaf/open/soi/ChTextureGrp.h>
%include <leaf/open/soi/ChFrag.h>
%include <leaf/open/soi/CHorizon.h>
%include <leaf/open/soi/Corestriction.h>
%include <leaf/open/soi/Component.h>
%include <leaf/open/soi/MapUnit.h>
%include <leaf/open/soi/Soils.h>
