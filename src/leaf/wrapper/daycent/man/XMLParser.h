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


#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <string>
#include <vector>
#include <stdexcept>

// Error codes

enum {
   ERROR_ARGS = 1,
   ERROR_XERCES_INIT,
   ERROR_PARSE,
   ERROR_EMPTY_DOCUMENT
};

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace man
{

class XMLParser
{
public:
    XMLParser(void);
    ~XMLParser(void);

    void readConfigFile(const std::string& file);

    int GetRotYears();

    std::vector< std::string > GetSubElements( xercesc::DOMNode* element );

    std::string ConvertDate( std::string date );

    //void SetManagement( std::vector< std::pair< std::string, std::string > >  mngt);

    std::vector< std::vector< std::pair< std::string, std::string > > > GetManagement();

    std::vector< std::vector< std::pair< std::string, std::string > > > m_Operations;

    void SetRotYear( std::string year );

    const std::string& GetRotYear();

    std::vector< std::vector< std::pair< std::string, std::string > > >
        GetRotations( std::vector< std::pair< std::string, std::string > > mgnt );

    bool GetGDD();

private:
    xercesc::XercesDOMParser *m_ConfigFileParser;

    std::string m_RotYear;

    int m_RotYears;

    bool m_GDD;

    // Internal class use only. Hold Xerces data in UTF-16 SMLCh type.

    XMLCh* TAG_RotationYears;

};

} //end man
} //end daycent
} //end wrapper
} //end leaf

#endif
