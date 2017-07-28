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
#include <leaf/LEAFConfig.h>

#include <leaf/util/Template.h>

// --- Java Includes --- //
#include <jni.h>

// --- Boost Includes --- //
#include <boost/shared_ptr.hpp>

// --- STL Includes --- //
#include <string>
#include <map>

namespace leaf
{
namespace wrapper
{
namespace java
{

namespace lang
{
class Throwable;
}

///
typedef std::map< std::string const, jmethodID const > JMIDMAP;

///
typedef std::map< std::string const, jfieldID const > JFIDMAP;

///
bool CheckJNIFunctionReturn(
    jint ret );

namespace VM
{

///
JavaVM* CreateVM();

///
LEAF_JAVA_EXPORTS
JavaVM& Instance(
    bool const& erase = false );

}

namespace ENV
{

///
JNIEnv& Instance();

}

///
LEAF_JAVA_EXPORTS
jint MonitorEnter(
    jobject obj );

///
LEAF_JAVA_EXPORTS
jint MonitorExit(
    jobject obj );

///
LEAF_JAVA_EXPORTS
jboolean ExceptionCheck();

///
LEAF_JAVA_EXPORTS
void ExceptionDescribe();

///
LEAF_JAVA_EXPORTS
boost::shared_ptr< lang::Throwable > ExceptionOccurred();

///
LEAF_JAVA_EXPORTS
void ExceptionClear();

///
LEAF_JAVA_EXPORTS
void HandleException();

///
LEAF_JAVA_EXPORTS
jobject NewGlobalRef(
    jobject ref );

///
LEAF_JAVA_EXPORTS
void DeleteGlobalRef(
    jobject ref );

///
LEAF_JAVA_EXPORTS
void DeleteLocalRef(
    jobject ref );

///
LEAF_JAVA_EXPORTS
jobject GlobalFromLocal(
    jobject lobject );

///
template< typename T >
T GlobalFromLocal(
    jobject lobject )
{
    BOOST_STATIC_ASSERT( ( boost::is_base_of<
        _jobject, boost::remove_pointer< T >::type >::value ) );

    return static_cast< T >( GlobalFromLocal( lobject ) );
}

///
LEAF_JAVA_EXPORTS
jclass FindClass(
    std::string const& name );

///
LEAF_JAVA_EXPORTS
jboolean IsInstanceOf(
    jobject obj,
    jclass clazz );

///
LEAF_JAVA_EXPORTS
jmethodID GetMethodID(
    jclass clazz,
    std::string const& name,
    std::string const& sig );

///
LEAF_JAVA_EXPORTS
jmethodID GetStaticMethodID(
    jclass clazz,
    std::string const& name,
    std::string const& sig );

///
LEAF_JAVA_EXPORTS
jfieldID GetFieldID(
    jclass clazz,
    std::string const& name,
    std::string const& sig );

///
LEAF_JAVA_EXPORTS
jfieldID GetStaticFieldID(
    jclass clazz,
    std::string const& name,
    std::string const& sig );

///
template< typename T >
T CallMethod(
    jobject obj,
    jmethodID methodID,
    ... );

///
template<> LEAF_JAVA_EXPORTS
void CallMethod< void >(
    jobject obj,
    jmethodID methodID,
    ... );

///
template<> LEAF_JAVA_EXPORTS
jobject CallMethod< jobject >(
    jobject obj,
    jmethodID methodID,
    ... );

///
template<> LEAF_JAVA_EXPORTS
jboolean CallMethod< jboolean >(
    jobject obj,
    jmethodID methodID,
    ... );

///
template<> LEAF_JAVA_EXPORTS
jint CallMethod< jint >(
    jobject obj,
    jmethodID methodID,
    ... );

///
template<> LEAF_JAVA_EXPORTS
jlong CallMethod< jlong >(
    jobject obj,
    jmethodID methodID,
    ... );

///
template<> LEAF_JAVA_EXPORTS
jdouble CallMethod< jdouble >(
    jobject obj,
    jmethodID methodID,
    ... );

///
template< typename T >
T CallStaticMethod(
   jclass clazz,
   jmethodID methodID,
   ... );

///
template<> LEAF_JAVA_EXPORTS
void CallStaticMethod< void >(
   jclass clazz,
   jmethodID methodID,
   ... );

///
template<> LEAF_JAVA_EXPORTS
jobject CallStaticMethod< jobject >(
   jclass clazz,
   jmethodID methodID,
   ... );

///
template<> LEAF_JAVA_EXPORTS
jboolean CallStaticMethod< jboolean >(
   jclass clazz,
   jmethodID methodID,
   ... );

///
LEAF_JAVA_EXPORTS
jobject NewObject(
    jclass clazz,
    jmethodID methodID,
    ... );

///
LEAF_JAVA_EXPORTS
jobjectArray NewObjectArray(
    jsize len,
    jclass clazz,
    jobject init );

///
LEAF_JAVA_EXPORTS
jstring NewString(
    std::string const& str );

///
LEAF_JAVA_EXPORTS
char const* GetStringChars(
    jstring jstr,
    jboolean isCopy = JNI_FALSE );

///
LEAF_JAVA_EXPORTS
void ReleaseStringChars(
    jstring jstr,
    char const* chars );

///
template< typename T >
T GetField(
    jobject obj,
    jfieldID fieldID );

///
template<> LEAF_JAVA_EXPORTS
jobject GetField< jobject >(
    jobject obj,
    jfieldID fieldID );

///
template<> LEAF_JAVA_EXPORTS
jboolean GetField< jboolean >(
    jobject obj,
    jfieldID fieldID );

///
template<> LEAF_JAVA_EXPORTS
jint GetField< jint >(
    jobject obj,
    jfieldID fieldID );

///
template<> LEAF_JAVA_EXPORTS
jdouble GetField< jdouble >(
    jobject obj,
    jfieldID fieldID );

///
template< typename T >
T GetStaticField(
    jclass clazz,
    jfieldID fieldID );

///
template<> LEAF_JAVA_EXPORTS
jobject GetStaticField< jobject >(
    jclass clazz,
    jfieldID fieldID );

///
template<> LEAF_JAVA_EXPORTS
jboolean GetStaticField< jboolean >(
    jclass clazz,
    jfieldID fieldID );

///
template<> LEAF_JAVA_EXPORTS
jint GetStaticField< jint >(
    jclass clazz,
    jfieldID fieldID );

///
template<> LEAF_JAVA_EXPORTS
jdouble GetStaticField< jdouble >(
    jclass clazz,
    jfieldID fieldID );

///
LEAF_JAVA_EXPORTS
jsize GetArrayLength(
    jarray array );

///
template< typename T >
T GetArrayElement(
    jarray array,
    jsize index );

///
template<> LEAF_JAVA_EXPORTS
jobject GetArrayElement< jobject >(
    jarray array,
    jsize index );

///
template< typename T >
void SetField(
    jobject obj,
    jfieldID fieldID,
    T );

///
template<> LEAF_JAVA_EXPORTS
void SetField< jobject >(
    jobject obj,
    jfieldID fieldID,
    jobject val );

///
template<> LEAF_JAVA_EXPORTS
void SetField< jboolean >(
    jobject obj,
    jfieldID fieldID,
    jboolean val );

///
template<> LEAF_JAVA_EXPORTS
void SetField< jint >(
    jobject obj,
    jfieldID fieldID,
    jint val );

///
template<> LEAF_JAVA_EXPORTS
void SetField< jdouble >(
    jobject obj,
    jfieldID fieldID,
    jdouble val );

///
template< typename T >
void SetStaticField(
    jclass clazz,
    jfieldID fieldID,
    T );

///
template<> LEAF_JAVA_EXPORTS
void SetStaticField< jobject >(
    jclass clazz,
    jfieldID fieldID,
    jobject val );

///
template< typename T >
void SetArrayElement(
    jarray array,
    jsize index,
    T val );

///
template<> LEAF_JAVA_EXPORTS
void SetArrayElement< jobject >(
    jarray array,
    jsize index,
    jobject val );

///
template<> LEAF_JAVA_EXPORTS
void SetArrayElement< jboolean >(
    jarray array,
    jsize index,
    jboolean val );

///
template<> LEAF_JAVA_EXPORTS
void SetArrayElement< bool >(
    jarray array,
    jsize index,
    bool val );

///
template<> LEAF_JAVA_EXPORTS
void SetArrayElement< jdouble >(
    jarray array,
    jsize index,
    jdouble val );

} //end java
} //end wrapper
} //end leaf
