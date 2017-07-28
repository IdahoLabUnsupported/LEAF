
#if defined( SWIGPYTHON )
%warnfilter(
    SWIGWARN_IGNORE_OPERATOR_EQ,
    SWIGWARN_IGNORE_OPERATOR_INDEX,
    SWIGWARN_IGNORE_OPERATOR_PLUSPLUS,
    SWIGWARN_IGNORE_OPERATOR_MINUSMINUS,
    SWIGWARN_IGNORE_OPERATOR_LAND,
    SWIGWARN_IGNORE_OPERATOR_LOR );
#endif

#if !defined( SWIGLUA ) && !defined( SWIGR )
%rename( Equal ) operator =;
%rename( PlusEqual ) operator +=;
%rename( MinusEqual ) operator -=;
%rename( MultiplyEqual ) operator *=;
%rename( DivideEqual ) operator /=;
%rename( PercentEqual ) operator %=;
%rename( Plus ) operator +;
%rename( Minus ) operator -;
%rename( Multiply ) operator *;
%rename( Divide ) operator /;
%rename( Percent ) operator %;
%rename( Not ) operator !;
%rename( IndexIntoConst ) operator [] const;
%rename( IndexInto ) operator [];
%rename( Functor ) operator ();
%rename( EqualEqual ) operator ==;
%rename( NotEqual ) operator !=;
%rename( LessThan ) operator <;
%rename( LessThanEqual ) operator <=;
%rename( GreaterThan ) operator >;
%rename( GreaterThanEqual ) operator >=;
%rename( And ) operator &&;
%rename( Or ) operator ||;
%rename( PlusPlusPrefix ) operator ++();
%rename( PlusPlusPostfix ) operator ++( int );
%rename( MinusMinusPrefix ) operator --();
%rename( MinusMinusPostfix ) operator --( int );
#endif

//Some languages have a %rename *::operator[] already in place,
//which seems to takes precedence over the above %rename operator[]
%rename( IndexInto ) *::operator []( unsigned idx );

#ifdef SWIGPHP
%rename( AndOperator ) operator &&;
%rename( OrOperator ) operator ||;
#endif

#ifdef SWIG_ALLEGRO_CL
%{
#include <stdio.h>
%}
#endif

#ifdef SWIGD
//Due to the way operator overloading is implemented in D1 and D2, the prefix
//increment/decrement operators (D1) resp. the postfix ones (D2) are ignored
%warnfilter(
    SWIGWARN_IGNORE_OPERATOR_PLUSPLUS,
    SWIGWARN_IGNORE_OPERATOR_MINUSMINUS );
#endif

%rename( IntCast ) operator int();
%rename( DoubleCast ) operator double();
