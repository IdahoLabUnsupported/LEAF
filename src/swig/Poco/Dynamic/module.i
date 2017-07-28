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


%module Poco_Dynamic
%{
    #include <Poco/Dynamic/Var.h>
    #include <Poco/Dynamic/Struct.h>
%}

%import <std/module.i>

%import <Poco/Foundation.h>

%import <Poco/Dynamic/VarHolder.h>

%ignore Poco::Dynamic::operator ==( char const*, Var const& );
%ignore Poco::Dynamic::operator !=( char const*, Var const& );
%ignore Poco::Dynamic::Var::operator []( std::string const& ) const;
%ignore Poco::Dynamic::Var::operator []( char const* ) const;
%ignore Poco::Dynamic::Var::operator []( char const* );
%ignore Poco::Dynamic::Var::type() const;
%ignore Poco::Dynamic::Var::convert< bool >( bool& ) const;
%ignore Poco::Dynamic::Var::convert< int >( int& ) const;
%ignore Poco::Dynamic::Var::convert< double >( double& ) const;
%ignore Poco::Dynamic::Var::convert< std::string >( std::string& ) const;
%include <Poco/Dynamic/Var.h>
//Create conversion constructors
%template( Var ) Poco::Dynamic::Var::Var< bool >;
%template( Var ) Poco::Dynamic::Var::Var< int >;
%template( Var ) Poco::Dynamic::Var::Var< double >;
%template() Poco::Dynamic::Var::operator =< bool >;
%template() Poco::Dynamic::Var::operator =< int >;
%template() Poco::Dynamic::Var::operator =< double >;
%template() Poco::Dynamic::Var::operator =< std::string >;
%template( ExtractBool ) Poco::Dynamic::Var::extract< bool >;
%template( ExtractInt ) Poco::Dynamic::Var::extract< int >;
%template( ExtractDouble ) Poco::Dynamic::Var::extract< double >;
%template( ExtractString ) Poco::Dynamic::Var::extract< std::string >;
%template( ConvertBool ) Poco::Dynamic::Var::convert< bool >;
%template( ConvertInt ) Poco::Dynamic::Var::convert< int >;
%template( ConvertDouble ) Poco::Dynamic::Var::convert< double >;
%template( ConvertString ) Poco::Dynamic::Var::convert< std::string >;

%ignore Poco::Dynamic::Struct< std::string >::operator []( std::string const& ) const;
%ignore Poco::Dynamic::Struct< std::string >::find( std::string const& ) const;
%ignore Poco::Dynamic::Struct< std::string >::end() const;
%ignore Poco::Dynamic::Struct< std::string >::begin() const;
%include <Poco/Dynamic/Struct.h>
%template( DynamicStruct ) Poco::Dynamic::Struct< std::string >;
%template( StructData ) std::map< std::string, Poco::Dynamic::Var >;
//%template( StructNameSet ) std::set< std::string >;
