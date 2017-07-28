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


%module Poco
%{
    #include <Poco/Nullable.h>

    #include <Poco/Dynamic/Var.h>
%}

%import <Poco/Dynamic/module.i>

%typemap( csimports ) SWIGTYPE
%{
    using System;
    using System.Runtime.InteropServices;

    using Dynamic;
%}

%ignore Poco::Nullable::value() const;
%ignore operator bool&;
%ignore operator const bool&;
%ignore operator int&;
%ignore operator const int&;
%ignore operator long&;
%ignore operator const long&;
%ignore operator double&;
%ignore operator const double&;
%ignore operator std::string&;
%ignore operator const std::string&;
%ignore operator Poco::Dynamic::Var&;
%ignore operator const Poco::Dynamic::Var&;
%ignore operator NullType&;
%include <Poco/Nullable.h>
%template( NullableBool ) Poco::Nullable< bool >;
%template( NullableInt ) Poco::Nullable< int >;
%template( NullableLong ) Poco::Nullable< long >;
%template( NullableDouble ) Poco::Nullable< double >;
%template( NullableString ) Poco::Nullable< std::string >;
%template( NullableVar ) Poco::Nullable< Poco::Dynamic::Var >;
