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


%module leaf_wrapper_rusle2
%{
    #include <leaf/wrapper/rusle2/Common.h>
    #include <leaf/wrapper/rusle2/api-rusle2.h>
%}

%import <leaf/util/module.i>

%import <leaf/open/ero/module.i>
%import <leaf/open/man/module.i>
%import <leaf/open/soi/module.i>

%typemap( csimports ) SWIGTYPE
%{
    using System;
    using System.Runtime.InteropServices;

    using std;

    using Poco;
    using Poco.Data;
    using Poco.Dynamic;

    using leaf.util;

    using leaf.open.ero;
    using leaf.open.man;
    using leaf.open.scn;
    using leaf.open.soi;
%}
%pragma( csharp ) moduleimports=
%{
    using System;
    using System.Runtime.InteropServices;

    using std;

    using Poco;
    using Poco.Data;
    using Poco.Dynamic;

    using leaf.util;

    using leaf.open.ero;
    using leaf.open.man;
    using leaf.open.scn;
    using leaf.open.soi;
%}

%include <leaf/wrapper/rusle2/Common.h>

%include <leaf/wrapper/rusle2/api-rusle2.h>
