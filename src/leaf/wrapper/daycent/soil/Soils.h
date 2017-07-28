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

#include <leaf/open/soi/Soils.h>

// --- Boost Includes --- //
#include <boost/scoped_ptr.hpp>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace soil
{

class Soils
{
public:
    ///
    Soils();

    ///
    ~Soils();

    ///
    bool LoadSQL(
        open::soi::Component const& component,
        std::string const& dir,
        std::string const& abbr );

    ///
    int GetNLayer();
    void SetNLayer(
        int const& layer );

    ///
    double GetSilt();
    void SetSilt(
        double const& silt );

    ///
    double GetSand();
    void SetSand(
        double const& sand );

    ///
    double GetClay();
    void SetClay(
        double const& clay );

    ///
    double GetBulkDensity();
    void SetBulkDensity(
        double const& bulkden );

    ///
    int m_nlayer;

    ///
    double m_silt;

    ///
    double m_sand;

    ///
    double m_clay;

    ///
    double m_bulkden;

protected:


private:
    ///
    class Layer
    {
    public:
        ///
        Layer(
            open::soi::CHorizon const& horizon );

        ///
        ~Layer();

        ///
        bool const& IsValid() const;
        void SetIsValid( 
            bool valid );

        ///
        double const& GetMinDepth() const;
        void SetMinDepth(
            double const& minDepth );

        ///
        double const& GetMaxDepth() const;
        void SetMaxDepth(
            double const& maxDepth );

        ///
        double const& GetBulkDens() const;

        ///
        double const& GetFldCpcty() const;

        ///
        double const& GetWltPnt() const;

        ///
        double const& GetEvapCoeff() const;
        void SetEvapCoeff(
            double const& evapCoeff );

        ///
        double const& GetTransCoeff() const;
        void SetTransCoeff(
            double const& transCoeff );

        ///
        double const& GetMinVolH2O() const;
        void SetMinVolH2O(
            double const& minVolH2O );

        ///
        double const& GetSandFrac() const;

        ///
        double const& GetClayFrac() const;

        ///
        double const& GetSiltFrac() const;

        ///
        double const& GetOm() const;

        ///
        double const& GetSatHydCond() const;

        ///
        double const& GetPh() const;

        ///
        bool operator ==(
            double const& s )
        {
            bool isEqual( false );
            isEqual = isEqual || ( m_minDepth == s );
            isEqual = isEqual || ( m_maxDepth == s );
            isEqual = isEqual || ( m_bulkDens == s );
            isEqual = isEqual || ( m_fldCpcty == s );
            isEqual = isEqual || ( m_wltPnt == s );
            //isEqual = isEqual || ( m_evapCoeff == s );
            //isEqual = isEqual || ( m_transCoeff == s );
            isEqual = isEqual || ( m_sandFrac == s );
            isEqual = isEqual || ( m_clayFrac == s );
            isEqual = isEqual || ( m_om == s );
            //isEqual = isEqual || ( m_minVolH2O == s );
            isEqual = isEqual || ( m_satHydCond == s );
            isEqual = isEqual || ( m_ph == s );
            return isEqual;
        }

        ///
        void operator *=(
            double const& s )
        {
            m_bulkDens *= s;
            m_fldCpcty *= s;
            m_wltPnt *= s;
            //m_evapCoeff *= s;
            //m_transCoeff *= s;
            m_sandFrac *= s;
            m_clayFrac *= s;
            m_om *= s;
            //m_minVolH2O *= s;
            m_satHydCond *= s;
            m_ph *= s;
        }

        ///
        Layer operator *(
            double const& s )
        {
            Layer result = *this;
            result *= s;
            return result;
        }

        ///
        void operator +=(
            Layer const& layer )
        {
            m_bulkDens += layer.GetBulkDens();
            m_fldCpcty += layer.GetFldCpcty();
            m_wltPnt += layer.GetWltPnt();
            //m_evapCoeff += layer.GetEvapCoeff();
            //m_transCoeff += layer.GetTransCoeff();
            m_sandFrac += layer.GetSandFrac();
            m_clayFrac += layer.GetClayFrac();
            m_om += layer.GetOm();
            //m_minVolH2O += layer.GetMinVolH2O();
            m_satHydCond += layer.GetSatHydCond();
            m_ph += layer.GetPh();
        }

        ///
        friend std::ostream& operator <<(
            std::ostream& os,
            Layer const& layer )
        {
            os << layer.GetMinDepth() << "\t";
            os << layer.GetMaxDepth() << "\t";
            os << layer.GetBulkDens() << "\t";
            os << layer.GetFldCpcty() << "\t";
            os << layer.GetWltPnt() << "\t";
            os << layer.GetEvapCoeff() << "\t";
            os << layer.GetTransCoeff() << "\t";
            os << layer.GetSandFrac() << "\t";
            os << layer.GetClayFrac() << "\t";
            os << layer.GetOm() << "\t";
            os << layer.GetMinVolH2O() << "\t";
            os << layer.GetSatHydCond() << "\t";
            os << layer.GetPh() << std::endl;
            return os;
        }

    protected:

    private:
        ///
        bool m_isValid;

        ///Column 1 - Minimum depth of soil layer (cm)
        double m_minDepth;

        ///Column 2 - Maximum depth of soil layer (cm)
        double m_maxDepth;

        ///Column 3 - Bulk density of soil layer (g/cm^3)
        double m_bulkDens;

        ///Column 4 - Field capacity of soil layer, volumetric
        double m_fldCpcty;

        ///Column 5 - Wilting point of soil layer, volumetric
        double m_wltPnt;

        ///Column 6 - Evaporation coefficient for soil layer
        ///(currently not being used)
        double m_evapCoeff;

        ///Column 7 - % of roots in soil layer, these values must sum to 1
        double m_transCoeff;

        ///Column 8 - Fraction of sand in soil layer, 0.0 - 1.0
        double m_sandFrac;

        ///Column 9 - Fraction of clay in soil layer, 0.0 - 1.0
        double m_clayFrac;

        ///Fraction of silt in soil layer, 0.0 - 1.0
        double m_siltFrac;

        ///Column 10 - Organic matter in soil layer, fraction 0.0 - 1.0
        double m_om;

        ///Column 11 - Min. volumetric soil water content below wilting point,
        ///soil water content will not be allowed to drop below this value
        double m_minVolH2O;

        ///Column 12 - Saturated hydraulic conductivity of soil layer in cm/s
        ///Need to check units of ksat in SSURGO database
        double m_satHydCond;

        ///Column 13 - pH of soil layer
        double m_ph;

    };
    typedef boost::scoped_ptr< Layer > LayerPtr;
    typedef std::vector< Layer > Layers;

    ///
    static std::set< unsigned int > const ADEP;

    ///
    static std::set< unsigned int > const LAYER;

    ///
    static std::vector< double > const EVAP_COEFF;

    ///
    static std::vector< double > const TRANS_COEFF;

    ///
    static std::vector< double > const MIN_VSWC;

};




} //end soil
} //end daycent
} //end wrapper
} //end leaf
