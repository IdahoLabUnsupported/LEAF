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


// --- LEAF Includes --- //
#include <leaf/wrapper/java/api-java.h>
#include <leaf/wrapper/java/ThreadSpecificJNIEnv.h>

#include <leaf/wrapper/java/lang/Thread.h>
#include <leaf/wrapper/java/lang/Throwable.h>

// --- Boost Includes --- //
#include <boost/filesystem.hpp>

#include <boost/thread/tss.hpp>

namespace fs = boost::filesystem;

// --- STL Includes --- //
#include <sstream>

namespace leaf
{
namespace wrapper
{
namespace java
{

////////////////////////////////////////////////////////////////////////////////
lang::ThreadPtr MainThread;
boost::thread_specific_ptr< ThreadSpecificJNIEnv > TSJNIEnvPtr;
////////////////////////////////////////////////////////////////////////////////
std::string const JarDeps =
    "-Djava.class.path="
    "jar/absolutelayout.jar;"
    "jar/axis_REV609819.jar;"
    "jar/axis-ant.jar;"
    "jar/beansbinding_1.2.1.jar;"
    "jar/commons-beanutils_1.7.0.jar;"
    "jar/commons-beanutils-bean-collections_1.7.0.jar;"
    "jar/commons-beanutils-core_1.7.0.jar;"
    "jar/commons-collections_3.2.1.jar;"
    "jar/commons-digester_1.8.jar;"
    "jar/commons-discovery_0.2.jar;"
    "jar/commons-lang_2.4.jar;"
    "jar/commons-logging_1.1.1.jar;"
    "jar/geoapi_2.3-M1.jar;"
    "jar/geoapi-pending_2.3-M1.jar;"
    "jar/glazedlists_1.7.0.jar;"
    "jar/gt-api_2.6.5.jar;"
    "jar/gt-coverage_2.6.5.jar;"
    "jar/gt-cql_2.6.5.jar;"
    "jar/gt-data_2.6.5.jar;"
    "jar/gt-epsg-wkt_2.6.5.jar;"
    "jar/gt-main_2.6.5.jar;"
    "jar/gt-metadata_2.6.5.jar;"
    "jar/gt-referencing_2.6.5.jar;"
    "jar/gt-render_2.6.5.jar;"
    "jar/gt-shapefile_2.6.5.jar;"
    "jar/h2-1.3.157.jar;"
    "jar/itext_2.1.7.jar;"
    "jar/jackcess_1.1.15.jar;"
    "jar/jai-codec.jar;"
    "jar/jai-core.jar;"
    "jar/jasperreports_4.0.2.jar;"
    "jar/jasperreports-fonts_4.0.2.jar;"
    "jar/javamail_1.4.1.jar;"
    "jar/javolution_5.3.1.jar;"
    "jar/jaxrpc.jar;"
    "jar/jcchart_6.3.jar;"
    "jar/jcchart3dj3d_6.3.jar;"
    "jar/jcelements_6.3.jar;"
    "jar/jcfield_6.3.jar;"
    "jar/jcommon_1.0.16.jar;"
    "jar/jcpagelayout_6.3.jar;"
    "jar/jctable_6.3.jar;"
    "jar/jdic.jar;"
    "jar/jdom_1.1.jar;"
    "jar/jfreechart_1.0.13.jar;"
    "jar/jh_2.0_05.jar;"
    "jar/jhall_2.0_05.jar;"
    "jar/jmock_1.1.0.jar;"
    "jar/jscience.jar;"
    "jar/jsearch_2.0_05.jar;"
    "jar/jsr-275_1.0-beta-2.jar;"
    "jar/jts_1.10.jar;"
    "jar/loadingcircle.jar;"
    "jar/log4_1.2.15.jar;"
    "jar/mantisconnect-client-api_1.1.1.0.jar;"
    "jar/org-openide-awt.jar;"
    "jar/org-openide-util.jar;"
    "jar/org-openide-util-lookup.jar;"
    "jar/propertyboilerplate.jar;"
    "jar/resolver_2.9.0.jar;"
    "jar/saaj.jar;"
    "jar/serializer_2.9.0.jar;"
    "jar/swing-layout_1.0.3.jar;"
    "jar/truezip_6.6.jar;"
    "jar/vecmath_1.3.2.jar;"
    "jar/weps.gui.jar;"
    "jar/wizard.jar;"
    "jar/wsdl4j_1.5.1.jar;"
    "jar/xerces_2.9.0.jar;"
    "jar/xml-apis_2.9.0.jar;";
////////////////////////////////////////////////////////////////////////////////
bool CheckJNIFunctionReturn(
    jint ret )
{
    switch( ret )
    {
        //0: Success
        case JNI_OK:
        {
            return true;
        }
        //-1: Unknown error
        case JNI_ERR:
        {
            return false;
        }
        //-2: Thread detached from the VM
        case JNI_EDETACHED:
        {
            return false;
        }
        //-3: JNI version error
        case JNI_EVERSION:
        {
            return false;
        }
        //-4: Not enough memory
        case JNI_ENOMEM:
        {
            return false;
        }
        //-5: VM already created
        case JNI_EEXIST:
        {
            return false;
        }
        //-6: Invalid arguments
        case JNI_EINVAL:
        {
            return false;
        }
        default:
        {
            return false;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
JavaVM* VM::CreateVM()
{
    fs::path leafDataDir( getenv( "LEAF_DATA_DIR" ) );
    fs::path weps( leafDataDir/"weps" );
    std::string user_dir( "-Duser.dir=" );
    user_dir.append( weps.string() );
    std::string user_home( "-Duser.home=" );
    user_home.append( weps.string() );
    std::string weps_dbs( "WEPS_DATABASES=" );
    weps_dbs.append( weps.string() );
#ifdef _WIN32
    _putenv( weps_dbs.c_str() );
    //_putenv( &std::vector< char >( weps_dbs.begin(), weps_dbs.end() )[ 0 ] );
#else
    putenv( &std::vector< char >( weps_dbs.begin(), weps_dbs.end() )[ 0 ] );
#endif

    JavaVMOption options[ 6 ];
    //Disable JIT
    options[ 0 ].optionString = const_cast< char* >( "-Djava.compiler=NONE" );
    //User classes
    options[ 1 ].optionString = const_cast< char* >( JarDeps.c_str() );
    //User dirs
    options[ 2 ].optionString = const_cast< char* >( user_dir.c_str() );
    options[ 3 ].optionString = const_cast< char* >( user_home.c_str() );
    //Extended checking
    options[ 4 ].optionString = const_cast< char* >( "-Xcheck:jni" );
    //Maximum java heap size
    options[ 5 ].optionString = const_cast< char* >( "-Xmx196M" );
    //Garbage collection
    //options[  ].optionString = const_cast< char* >( "-verbose:gc" );
    //options[  ].optionString = const_cast< char* >( "-XX:+PrintGCTimeStamps" );
    //options[  ].optionString = const_cast< char* >( "-XX:+PrintGCDetails" );
    //Debugging
    //options[  ].optionString = const_cast< char* >( "-Xdebug" );
    //options[  ].optionString = const_cast< char* >( "-Xnoagent" );
    //options[  ].optionString = const_cast< char* >(
        //"-Xrunjdwp:transport=dt_socket,server=y,suspend=n,address=8800" );

    JavaVMInitArgs vm_args;
    //JDK version. This indicates version 1.6
    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 6;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = JNI_FALSE;

    JavaVM* jvm( NULL );
    JNIEnv* env( NULL );
    jint ret = JNI_CreateJavaVM( &jvm, (void**)&env, &vm_args );
    assert( CheckJNIFunctionReturn( ret ) );
    TSJNIEnvPtr.reset( new ThreadSpecificJNIEnv( env ) );
    MainThread = lang::Thread::CurrentThread();

    return jvm;
}
////////////////////////////////////////////////////////////////////////////////
JavaVM& VM::Instance(
    bool const& erase )
{
    static JavaVM* jvm = CreateVM();

    if( erase )
    {
        MainThread.reset();
        TSJNIEnvPtr.reset( NULL );
        jvm->DestroyJavaVM();
        jvm = NULL;
    }

    return *jvm;
}
////////////////////////////////////////////////////////////////////////////////
JNIEnv& ENV::Instance()
{
    ThreadSpecificJNIEnv* tsje = TSJNIEnvPtr.get();
    if( tsje == NULL )
    {
        tsje = new ThreadSpecificJNIEnv();
        TSJNIEnvPtr.reset( tsje );

        //http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=4489399
        lang::ThreadPtr currentThread = lang::Thread::CurrentThread();
        assert( MainThread->GetId() != currentThread->GetId() );
        currentThread->SetContextClassLoader(
            *MainThread->GetContextClassLoader() );
    }

    return *tsje->GetJNIEnv();
}
////////////////////////////////////////////////////////////////////////////////
jint MonitorEnter(
    jobject obj )
{
    jint ret = ENV::Instance().MonitorEnter( obj );
    HandleException();
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
jint MonitorExit(
    jobject obj )
{
    jint ret = ENV::Instance().MonitorExit( obj );
    HandleException();
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
jboolean ExceptionCheck()
{
    return ENV::Instance().ExceptionCheck();
}
////////////////////////////////////////////////////////////////////////////////
void ExceptionDescribe()
{
    ENV::Instance().ExceptionDescribe();
}
////////////////////////////////////////////////////////////////////////////////
lang::ThrowablePtr ExceptionOccurred()
{
    return lang::Object::Create< lang::Throwable >(
        ENV::Instance().ExceptionOccurred() );
}
////////////////////////////////////////////////////////////////////////////////
void ExceptionClear()
{
    ENV::Instance().ExceptionClear();
}
////////////////////////////////////////////////////////////////////////////////
void HandleException()
{
    if( ExceptionCheck() == JNI_FALSE )
    {
        return;
    }

    lang::ThrowablePtr throwable = ExceptionOccurred();
    ExceptionClear();

    throwable->ThrowRuntimeException();
}
////////////////////////////////////////////////////////////////////////////////
jobject NewGlobalRef(
    jobject ref )
{
    jobject globalRef = ENV::Instance().NewGlobalRef( ref );
    HandleException();
    return globalRef;
}
////////////////////////////////////////////////////////////////////////////////
void DeleteGlobalRef(
    jobject ref )
{
    ENV::Instance().DeleteGlobalRef( ref );
    HandleException();
}
////////////////////////////////////////////////////////////////////////////////
void DeleteLocalRef(
    jobject ref )
{
    ENV::Instance().DeleteLocalRef( ref );
    HandleException();
}
////////////////////////////////////////////////////////////////////////////////
jobject GlobalFromLocal(
    jobject lobject )
{
    jobject gobject = NewGlobalRef( lobject );
    DeleteLocalRef( lobject );
    return gobject;
}
////////////////////////////////////////////////////////////////////////////////
jclass FindClass(
    std::string const& name )
{
    jclass lclass = ENV::Instance().FindClass( name.c_str() );
    HandleException();
    return GlobalFromLocal< jclass >( lclass );
}
////////////////////////////////////////////////////////////////////////////////
jboolean IsInstanceOf(
    jobject obj,
    jclass clazz )
{
    jboolean val = ENV::Instance().IsInstanceOf( obj, clazz );
    HandleException();
    return val;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID GetMethodID(
    jclass clazz,
    std::string const& name,
    std::string const& sig )
{
    jmethodID jmid =
        ENV::Instance().GetMethodID( clazz, name.c_str(), sig.c_str() );
    HandleException();
    return jmid;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID GetStaticMethodID(
    jclass clazz,
    std::string const& name,
    std::string const& sig )
{
    jmethodID jmid =
        ENV::Instance().GetStaticMethodID( clazz, name.c_str(), sig.c_str() );
    HandleException();
    return jmid;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID GetFieldID(
    jclass clazz,
    std::string const& name,
    std::string const& sig )
{
    jfieldID jfid =
        ENV::Instance().GetFieldID( clazz, name.c_str(), sig.c_str() );
    HandleException();
    return jfid;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID GetStaticFieldID(
    jclass clazz,
    std::string const& name,
    std::string const& sig )
{
    jfieldID jfid =
        ENV::Instance().GetStaticFieldID( clazz, name.c_str(), sig.c_str() );
    HandleException();
    return jfid;
}
////////////////////////////////////////////////////////////////////////////////
template<>
void CallMethod< void >(
    jobject obj,
    jmethodID methodID,
    ... )
{
    va_list args;
    va_start( args, methodID );
    ENV::Instance().CallVoidMethodV( obj, methodID, args );
    HandleException();
    va_end( args );
}
////////////////////////////////////////////////////////////////////////////////
template<>
jobject CallMethod< jobject >(
    jobject obj,
    jmethodID methodID,
    ... )
{
    va_list args;
    va_start( args, methodID );
    jobject lobject = ENV::Instance().CallObjectMethodV( obj, methodID, args );
    HandleException();
    va_end( args );
    return GlobalFromLocal( lobject );
}
////////////////////////////////////////////////////////////////////////////////
template<>
jboolean CallMethod< jboolean >(
    jobject obj,
    jmethodID methodID,
    ... )
{
    va_list args;
    va_start( args, methodID );
    jboolean val = ENV::Instance().CallBooleanMethodV( obj, methodID, args );
    HandleException();
    va_end( args );
    return val;
}
////////////////////////////////////////////////////////////////////////////////
template<>
jint CallMethod< jint >(
    jobject obj,
    jmethodID methodID,
    ... )
{
    va_list args;
    va_start( args, methodID );
    jint val = ENV::Instance().CallIntMethodV( obj, methodID, args );
    HandleException();
    va_end( args );
    return val;
}
////////////////////////////////////////////////////////////////////////////////
template<>
jlong CallMethod< jlong >(
    jobject obj,
    jmethodID methodID,
    ... )
{
    va_list args;
    va_start( args, methodID );
    jlong val = ENV::Instance().CallLongMethodV( obj, methodID, args );
    HandleException();
    va_end( args );
    return val;
}
////////////////////////////////////////////////////////////////////////////////
template<>
jdouble CallMethod< jdouble >(
    jobject obj,
    jmethodID methodID,
    ... )
{
    va_list args;
    va_start( args, methodID );
    jdouble val = ENV::Instance().CallDoubleMethodV( obj, methodID, args );
    HandleException();
    va_end( args );
    return val;
}
////////////////////////////////////////////////////////////////////////////////
template<>
void CallStaticMethod< void >(
   jclass clazz,
   jmethodID methodID,
   ... )
{
    va_list args;
    va_start( args, methodID );
    ENV::Instance().CallStaticVoidMethodV( clazz, methodID, args );
    HandleException();
    va_end( args );
}
////////////////////////////////////////////////////////////////////////////////
template<>
jobject CallStaticMethod< jobject >(
   jclass clazz,
   jmethodID methodID,
   ... )
{
    va_list args;
    va_start( args, methodID );
    jobject lobject =
        ENV::Instance().CallStaticObjectMethodV( clazz, methodID, args );
    HandleException();
    va_end( args );
    return GlobalFromLocal( lobject );
}
////////////////////////////////////////////////////////////////////////////////
template<>
jboolean CallStaticMethod< jboolean >(
   jclass clazz,
   jmethodID methodID,
   ... )
{
    va_list args;
    va_start( args, methodID );
    jboolean val =
        ENV::Instance().CallStaticBooleanMethodV( clazz, methodID, args );
    HandleException();
    va_end( args );
    return val;
}
////////////////////////////////////////////////////////////////////////////////
jobject NewObject(
    jclass clazz,
    jmethodID methodID,
    ... )
{
    va_list args;
    va_start( args, methodID );
    jobject lobject = ENV::Instance().NewObjectV( clazz, methodID, args );
    HandleException();
    va_end( args );
    return GlobalFromLocal( lobject );
}
////////////////////////////////////////////////////////////////////////////////
jobjectArray NewObjectArray(
    jsize len,
    jclass clazz,
    jobject init )
{
    jobjectArray ljoarr = ENV::Instance().NewObjectArray( len, clazz, init );
    HandleException();
    return GlobalFromLocal< jobjectArray >( ljoarr );
}
////////////////////////////////////////////////////////////////////////////////
jstring NewString(
    std::string const& str )
{
    jstring ljstr = ENV::Instance().NewStringUTF( str.c_str() );
    HandleException();
    return GlobalFromLocal< jstring >( ljstr );
}
////////////////////////////////////////////////////////////////////////////////
char const* GetStringChars(
    jstring jstr,
    jboolean isCopy )
{
    char const* chars = ENV::Instance().GetStringUTFChars( jstr, &isCopy );
    HandleException();
    return chars;
}
////////////////////////////////////////////////////////////////////////////////
void ReleaseStringChars(
    jstring jstr,
    char const* chars )
{
    ENV::Instance().ReleaseStringUTFChars( jstr, chars );
    HandleException();
}
////////////////////////////////////////////////////////////////////////////////
template<>
jobject GetField< jobject >(
    jobject obj,
    jfieldID fieldID )
{
    jobject lobject = ENV::Instance().GetObjectField( obj, fieldID );
    HandleException();
    return GlobalFromLocal( lobject );
}
////////////////////////////////////////////////////////////////////////////////
template<>
jboolean GetField< jboolean >(
    jobject obj,
    jfieldID fieldID )
{
    jboolean val = ENV::Instance().GetBooleanField( obj, fieldID );
    HandleException();
    return val;
}
////////////////////////////////////////////////////////////////////////////////
template<>
jint GetField< jint >(
    jobject obj,
    jfieldID fieldID )
{
    jint val = ENV::Instance().GetIntField( obj, fieldID );
    HandleException();
    return val;
}
////////////////////////////////////////////////////////////////////////////////
template<>
jdouble GetField< jdouble >(
    jobject obj,
    jfieldID fieldID )
{
    jdouble val = ENV::Instance().GetDoubleField( obj, fieldID );
    HandleException();
    return val;
}
////////////////////////////////////////////////////////////////////////////////
template<>
jobject GetStaticField< jobject >(
    jclass clazz,
    jfieldID fieldID )
{
    jobject lobject = ENV::Instance().GetStaticObjectField( clazz, fieldID );
    HandleException();
    return GlobalFromLocal( lobject );
}
////////////////////////////////////////////////////////////////////////////////
template<>
jboolean GetStaticField< jboolean >(
    jclass clazz,
    jfieldID fieldID )
{
    jboolean val = ENV::Instance().GetStaticBooleanField( clazz, fieldID );
    HandleException();
    return val;
}
////////////////////////////////////////////////////////////////////////////////
template<>
jint GetStaticField< jint >(
    jclass clazz,
    jfieldID fieldID )
{
    jint val = ENV::Instance().GetStaticIntField( clazz, fieldID );
    HandleException();
    return val;
}
////////////////////////////////////////////////////////////////////////////////
template<>
jdouble GetStaticField< jdouble >(
    jclass clazz,
    jfieldID fieldID )
{
    jdouble val = ENV::Instance().GetStaticDoubleField( clazz, fieldID );
    HandleException();
    return val;
}
////////////////////////////////////////////////////////////////////////////////
jsize GetArrayLength(
    jarray array )
{
    jsize val = ENV::Instance().GetArrayLength( array );
    HandleException();
    return val;
}
////////////////////////////////////////////////////////////////////////////////
template<>
jobject GetArrayElement< jobject >(
    jarray array,
    jsize index )
{
    jobject lobject = ENV::Instance().GetObjectArrayElement(
        static_cast< jobjectArray >( array ), index );
    HandleException();
    return GlobalFromLocal( lobject );
}
////////////////////////////////////////////////////////////////////////////////
template<>
void SetField< jobject >(
    jobject obj,
    jfieldID fieldID,
    jobject val )
{
    ENV::Instance().SetObjectField( obj, fieldID, val );
    HandleException();
}
////////////////////////////////////////////////////////////////////////////////
template<>
void SetField< jboolean >(
    jobject obj,
    jfieldID fieldID,
    jboolean val )
{
    ENV::Instance().SetBooleanField( obj, fieldID, val );
    HandleException();
}
////////////////////////////////////////////////////////////////////////////////
template<>
void SetField< jint >(
    jobject obj,
    jfieldID fieldID,
    jint val )
{
    ENV::Instance().SetIntField( obj, fieldID, val );
    HandleException();
}
////////////////////////////////////////////////////////////////////////////////
template<>
void SetField< jdouble >(
    jobject obj,
    jfieldID fieldID,
    jdouble val )
{
    ENV::Instance().SetDoubleField( obj, fieldID, val );
    HandleException();
}
////////////////////////////////////////////////////////////////////////////////
template<>
void SetStaticField< jobject >(
    jclass clazz,
    jfieldID fieldID,
    jobject val )
{
    ENV::Instance().SetStaticObjectField( clazz, fieldID, val );
    HandleException();
}
////////////////////////////////////////////////////////////////////////////////
template<>
void SetArrayElement< jobject >(
    jarray array,
    jsize index,
    jobject val )
{
    ENV::Instance().SetObjectArrayElement(
        static_cast< jobjectArray >( array ), index, val );
    HandleException();
}
////////////////////////////////////////////////////////////////////////////////
template<>
void SetArrayElement< jboolean >(
    jarray array,
    jsize index,
    jboolean val )
{
    ENV::Instance().SetBooleanArrayRegion(
        static_cast< jbooleanArray >( array ), index, 1, &val );
    HandleException();
}
////////////////////////////////////////////////////////////////////////////////
template<>
void SetArrayElement< bool >(
    jarray array,
    jsize index,
    bool val )
{
    SetArrayElement< jboolean >( array, index, val );
}
////////////////////////////////////////////////////////////////////////////////
template<>
void SetArrayElement< jdouble >(
    jarray array,
    jsize index,
    jdouble val )
{
    ENV::Instance().SetDoubleArrayRegion(
        static_cast< jdoubleArray >( array ), index, 1, &val );
    HandleException();
}
////////////////////////////////////////////////////////////////////////////////

} //end java
} //end wrapper
} //end leaf
