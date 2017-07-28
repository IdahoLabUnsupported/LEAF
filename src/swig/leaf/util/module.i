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


%module leaf_util
%{
    #include <leaf/util/RotationHelper.h>
    #include <leaf/util/SoilHelper.h>
%}

%import <boost/module.i>

%import <Poco/Data/module.i>

%import <leaf/LEAFConfig.h>

%typemap( csimports ) SWIGTYPE
%{
    using System;
    using System.Runtime.InteropServices;

    using Poco;
    using Poco.Data;
    using Poco.Dynamic;

    using boost;
%}

%include <leaf/util/RotationHelper.h>
%template( CROPYEAR_PAIR ) std::pair< leaf::util::Crop::Type, leaf::util::Year::Enum >;
%template( CROP_MAP ) std::map< leaf::util::Crop::Type, Poco::Dynamic::Var >;
%template( CROPYEAR_MAP ) std::map< leaf::util::CROPYEAR_PAIR, Poco::Dynamic::Var >;

%template( CROPVAR_PAIR ) std::pair< leaf::util::Crop::Type, Poco::Dynamic::Var >;
%template( CROPVAR_VEC ) std::vector< leaf::util::CROPVAR_PAIR >;

%include <leaf/util/SoilHelper.h>
