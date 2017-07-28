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



%module leaf_open_man
%{
    #include <leaf/open/man/Management.h>
    #include <leaf/open/man/Operation.h>
    #include <leaf/open/man/Crop.h>
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

%include <leaf/open/man/ManagementPtr.h>
%shared_ptr( leaf::open::man::Management )
%include <leaf/open/man/CropPtr.h>
%shared_ptr( leaf::open::man::Crop )

%include <leaf/open/man/Crop.h>
%include <leaf/open/man/Operation.h>
%include <leaf/open/man/Management.h>
