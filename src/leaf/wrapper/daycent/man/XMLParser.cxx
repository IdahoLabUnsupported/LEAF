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


#include "XMLParser.h"

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>
#include <errno.h>

using namespace xercesc;
using namespace std;

/**
 *  Constructor initializes xerces-C libraries.
 *  The XML tags and attributes which we seek are defined.
 *  The xerces-C DOM parser infrastructure is initialized.
 */

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace man
{

XMLParser::XMLParser(void)
{
    try
    {
        XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
    }
    catch( XMLException& e )
    {
        char* message = XMLString::transcode( e.getMessage() );
        cerr << "XML toolkit initialization error: " << message << endl;
        XMLString::release( &message );
        // throw exception here to return ERROR_XERCES_INIT
    }

    // Tags and attributes used in XML file.
    // Can't call transcode till after Xerces Initialize()

    TAG_RotationYears = XMLString::transcode("rotationyears");

    m_ConfigFileParser = new XercesDOMParser;
}

XMLParser::~XMLParser(void)
{
    // Free memory

    delete m_ConfigFileParser;

    try
    {
        XMLString::release( &TAG_RotationYears );
    }
    catch( ... )
    {
        cerr << "Unknown exception encountered in TagNamesdtor" << endl;
    }

    // Terminate Xerces

    try
    {
        XMLPlatformUtils::Terminate();  // Terminate after release of memory
    }
    catch( xercesc::XMLException& e )
    {
        char* message = xercesc::XMLString::transcode( e.getMessage() );

        cerr << "XML ttolkit teardown error: " << message << endl;
        XMLString::release( &message );
    }
}

void XMLParser::readConfigFile( const std::string& configFile )
{
    /**
    *  This function:
    *  - Tests the access and availability of the XML configuration file.
    *  - Configures the xerces-c DOM parser.
    *  - Reads and extracts the pertinent information from the XML config file.
    *
    *  @param in configFile The text string name of the HLA configuration file.
    */
// Test to see if the file is ok.

   struct stat fileStatus;

   int iretStat = stat(configFile.c_str(), &fileStatus);
   if( iretStat == ENOENT )
      throw ( std::runtime_error("Path file_name does not exist, or path is an empty string.") );
   else if( iretStat == ENOTDIR )
      throw ( std::runtime_error("A component of the path is not a directory."));
   //else if( iretStat == ELOOP )
   //   throw ( std::runtime_error("Too many symbolic links encountered while traversing the path."));
   else if( iretStat == EACCES )
      throw ( std::runtime_error("Permission denied."));
   else if( iretStat == ENAMETOOLONG )
      throw ( std::runtime_error("File can not be read\n"));

   // Configure DOM parser.

   m_ConfigFileParser->setValidationScheme( XercesDOMParser::Val_Never );
   m_ConfigFileParser->setDoNamespaces( false );
   m_ConfigFileParser->setDoSchema( false );
   m_ConfigFileParser->setLoadExternalDTD( false );

   try
   {
      m_ConfigFileParser->parse( configFile.c_str() );

      // no need to free this pointer - owned by the parent parser object
      DOMDocument* xmlDoc = m_ConfigFileParser->getDocument();

      // Get the top-level element: NAme is "root". No attributes for "root"

      DOMElement* elementRoot = xmlDoc->getDocumentElement();
      if( !elementRoot ) throw(std::runtime_error( "empty XML document" ));

      // Parse XML file for tags of interest: "ApplicationSettings"
      // Look one level nested within "root". (child of root)

      DOMNodeList*      children = elementRoot->getChildNodes();
      const  XMLSize_t nodeCount = children->getLength();

      // For all nodes, children of "root" in the XML tree.
      //std::cout << "nodeCount " << nodeCount << std::endl;

      for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
      {
         int date(0);
         std::vector< std::pair< std::string, std::string > > tempOp;
         DOMNode* currentNode = children->item(xx);
         if( currentNode->getNodeType() == DOMNode::ELEMENT_NODE )
         {
             std::string nodeName = XMLString::transcode(currentNode->getNodeName());
             if( nodeName == "rotationyears" || nodeName == "duration" )
             {
                 m_RotYears = boost::lexical_cast< int >(
                     XMLString::transcode(currentNode->getTextContent() ) );
                 std::cout << "rotationyears" << "=" << m_RotYears << std::endl;
             }
            //std::cout << "NodeName " << XMLString::transcode(currentNode->getNodeName()) <<
             //    " NodeType: " << currentNode->getNodeType() << std::endl;
             //std::cout << "NodeContent " << XMLString::transcode(currentNode->getTextContent() ) << std::endl;
             if( currentNode->hasChildNodes() )
             {
                // Found node which is an Element. Re-cast node as element
                DOMElement* currentElement
                            = dynamic_cast< xercesc::DOMElement* >( currentNode );
                //std::cout << "Node " << xx << " Name: " << XMLString::transcode(currentElement->getNodeName()) << std::endl;

                DOMNodeList* list = currentElement->getChildNodes();
                std::vector< std::pair< std::string, std::string > > temp_pair;
                XMLSize_t len = list->getLength();
                for( XMLSize_t i=0; i<len; ++i )
                {
                    DOMNode* subNode = list->item(i);
                    if( subNode->getNodeType() == DOMNode::ELEMENT_NODE )
                    {
                        DOMElement* subElement = dynamic_cast< xercesc::DOMElement* >( subNode );
                        std::cout << "ElementName " << XMLString::transcode(subElement->getNodeName()) <<
                            " ElementType: " << subElement->getNodeType() << std::endl;
                        if( subElement->hasChildNodes() )
                        {
                            std::vector< std::string > results;
                            results = GetSubElements( subElement );

                            //Convert date to day of year
                            size_t pos = results.at(0).rfind( "/" );
                            std::string sub( results.at(0).substr(pos+1) );
                            if( sub.length() == 1 )
                            {
                                results.at(0).insert( pos+1, "200" );
                            }
                            else if( sub.length() == 2 )
                            {
                                results.at(0).insert( pos+1, "20" );
                            }

                            boost::gregorian::date dt( boost::gregorian::from_us_string( results.at(0) ) );
                            std::string opdate = boost::lexical_cast< std::string >( dt.day_of_year() );
                            unsigned int opyear = dt.year();
                            std::cout << opyear << std::endl;
                            //std::cout << "Day of Year: " << dt.day_of_year() << std::endl;

                            std::pair< std::string, std::string > temp;

                            for( size_t j=1; j<results.size(); ++j )
                            {
                                //if( boost::lexical_cast< int >(opdate) < date )
                                if( boost::lexical_cast< int >(opyear) > date )
                                {
                                    m_Operations.push_back( tempOp );
                                    tempOp.clear();
                                }
                                temp.first = opdate;
                                temp.second = results.at(j);
                                tempOp.push_back( temp );
                                //date = boost::lexical_cast< int >(opdate);
                                date = opyear;
                            }
                        }
                        else
                        {
                            //std::cout << "Subelement Name: " << XMLString::transcode(subElement->getNodeName()) << std::endl;
                        }
                    }
                }
                //Capture last year of rotation
                if( !tempOp.empty() )
                {
                    m_Operations.push_back( tempOp );
                }
            }
         }
      }
   }
   catch( xercesc::XMLException& e )
   {
      char* message = xercesc::XMLString::transcode( e.getMessage() );
      ostringstream errBuf;
      errBuf << "Error parsing file: " << message << flush;
      XMLString::release( &message );
   }

   //Quick fix for removing extra empty vector at end
   std::vector< ::vector< std::pair< std::string, std::string > > >::iterator iter=m_Operations.begin();
   for( ; iter != m_Operations.end(); ++iter )
   {
       if( iter->empty() )
       {    m_Operations.erase( iter );}
   }
}

int XMLParser::GetRotYears()
{
    return m_RotYears;
}

std::vector< std::string > XMLParser::GetSubElements( xercesc::DOMNode* element )
{
    std::vector< std::string > data;

    DOMNodeList* sublist = element->getChildNodes();
    XMLSize_t sublen = sublist->getLength();
    for( XMLSize_t j=0; j<sublen; ++j )
    {
        DOMNode* subsubNode= sublist->item(j);
        if( subsubNode->getNodeType() == DOMNode::ELEMENT_NODE )
        {
            DOMElement* subsubElement = dynamic_cast< xercesc::DOMElement* >( subsubNode );
            /*std::string nodeName = XMLString::transcode(subsubElement->getNodeName());
            if( nodeName == "veg" )
            {
                std::string crop( XMLString::transcode( subsubElement->getTextContent() ) );
                if( std::string::npos != crop.find( "Corn" ) )
                {
                    m_GDD = true;
                }
                else
                {
                    m_GDD = false;
                }
            }*/
            if( subsubElement->hasChildNodes() )
            {
                //std::cout << "Subsubelement Name: " << XMLString::transcode(subsubElement->getNodeName()) << std::endl;
                //std::cout << "Subsubelement Value: " << XMLString::transcode(subsubElement->getTextContent()) << std::endl;
            }
            data.push_back( std::string( XMLString::transcode(subsubElement->getTextContent())) );
        }
    }
    std::vector< std::vector< std::string > > test;
    test.push_back(data);

    return data;
}

std::vector< std::vector< std::pair< std::string, std::string > > >
    XMLParser::GetRotations( std::vector< std::pair< std::string, std::string > > mgnt )
{
    std::vector< std::vector< std::pair< std::string, std::string > > > rotation;

    return rotation;
}
std::string XMLParser::ConvertDate( std::string date )
{
    std::string temp;
    return temp;
}

/*void XMLParser::SetManagement(
        std::vector< std::pair< std::string, std::string > > mngt)
{
    m_Operations = mngt;
}
*/
std::vector< std::vector< std::pair< std::string, std::string > > >
    XMLParser::GetManagement()
{
    return m_Operations;
}

void XMLParser::SetRotYear( std::string year )
{
    m_RotYear = year;
}

const std::string& XMLParser::GetRotYear()
{
    return m_RotYear;
}

bool XMLParser::GetGDD()
{
    return m_GDD;
}

} //end man
} //end daycent
} //end wrapper
} //end leaf