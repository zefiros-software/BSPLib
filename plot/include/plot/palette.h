/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2016 Koen Visscher, Paul Visscher and individual contributors.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @endcond
 */

#pragma once
#ifndef __PALETTE_H__
#define __PALETTE_H__

#include <stdint.h>
#include <sstream>
#include <tuple>

static const char *const __PlotLibColours[] =
{
    "deep",
    "muted",
    "bright",
    "pastel",
    "dark",
    "colourBlind",

    "viridis", "inferno", "plasma", "magma",

    "Blues", "BuGn", "BuPu",
    "GnBu", "Greens", "Greys", "Oranges", "OrRd",
    "PuBu", "PuBuGn", "PuRd", "Purples", "RdPu",
    "Reds", "YlGn", "YlGnBu", "YlOrBr", "YlOrRd",

    "afmhot", "autumn", "bone", "cool",
    "copper", "gist_heat", "gray", "hot",
    "pink", "spring", "summer", "winter",

    "BrBG", "bwr", "coolwarm", "PiYG", "PRGn", "PuOr",
    "RdBu", "RdGy", "RdYlBu", "RdYlGn", "Spectral",
    "seismic",

    "Accent", "Dark2", "Paired", "Pastel1",
    "Pastel2", "Set1", "Set2", "Set3",

    "gist_earth", "terrain", "ocean", "gist_stern",
    "brg", "CMRmap", "cubehelix",
    "gnuplot", "gnuplot2", "gist_ncar",
    "nipy_spectral", "jet", "rainbow",
    "gist_rainbow", "hsv", "flag", "prism"
};

class Palette
{
public:

    enum class Type
    {
        Reset,
        Custom,
        Integer,
        Other,
        ColorPallette
    };

    enum class ColourType
    {
        None,
        Reverse,
        Dark
    };

    enum class Colour : size_t
    {
        // seaborn
        Deep,
        Muted,
        Bright,
        Pastel,
        Dark,
        ColourBlind,

        // uniform
        Viridis,
        Inferno,
        Plasma,
        Magma,

        // sequential
        Blues,
        BuGn,
        BuPu,
        GnBu,
        Greens,
        Greys,
        Oranges,
        OrRd,
        PuBu,
        PuBuGn,
        PuRd,
        Purples,
        RdPu,
        Reds,
        YlGn,
        YlGnBu,
        YlOrBr,
        YlOrRd,

        // sequential 2
        Afmhot,
        Autumn,
        Bone,
        Cool,
        Copper,
        GistHeat,
        Gray,
        Hot,
        Pink,
        Spring,
        Summer,
        Winter,

        // diverging
        BrBG,
        Bwr,
        Coolwarm,
        PiYG,
        PRGn,
        PuOr,
        RdBu,
        RdGy,
        RdYlBu,
        RdYlGn,
        Spectral,
        Seismic,

        // qualitative
        Accent,
        Dark2,
        Paired,
        Pastel1,
        Pastel2,
        Set1,
        Set2,
        Set3,

        // misc
        GistEarth,
        Terrain,
        Ocean,
        GistStern,
        Brg,
        CMRmap,
        CubeHelix,
        GnuPlot,
        GnuPlot2,
        GistNcar,
        NipySpectral,
        Jet,
        Rainbow,
        GistRainbow,
        Hsv,
        Flag,
        Prism
    };

    enum class Uniform
    {
        Viridis = ( size_t )Colour::Viridis,
        Inferno = ( size_t )Colour::Inferno,
        Plasma = ( size_t )Colour::Plasma,
        Magma = ( size_t )Colour::Magma
    };

    enum class Sequential
    {
        Blues = ( size_t )Colour::Blues,
        BuGn = ( size_t )Colour::BuGn,
        BuPu = ( size_t )Colour::BuPu,
        GnBu = ( size_t )Colour::GnBu,
        Greens = ( size_t )Colour::Greens,
        Greys = ( size_t )Colour::Greys,
        Oranges = ( size_t )Colour::Oranges,
        OrRd = ( size_t )Colour::OrRd,
        PuBu = ( size_t )Colour::PuBu,
        PuBuGn = ( size_t )Colour::PuBuGn,
        PuRd = ( size_t )Colour::PuRd,
        Purples = ( size_t )Colour::Purples,
        RdPu = ( size_t )Colour::RdPu,
        Reds = ( size_t )Colour::Reds,
        YlGn = ( size_t )Colour::YlGn,
        YlGnBu = ( size_t )Colour::YlGnBu,
        YlOrBr = ( size_t )Colour::YlOrBr,
        YlOrRd = ( size_t )Colour::YlOrRd
    };

    enum class Sequential2
    {
        Afmhot = ( size_t )Colour::Afmhot,
        Autumn = ( size_t )Colour::Autumn,
        Bone = ( size_t )Colour::Bone,
        Cool = ( size_t )Colour::Cool,
        Copper = ( size_t )Colour::Copper,
        GistHeat = ( size_t )Colour::GistHeat,
        Gray = ( size_t )Colour::Gray,
        Hot = ( size_t )Colour::Hot,
        Pink = ( size_t )Colour::Pink,
        Spring = ( size_t )Colour::Spring,
        Summer = ( size_t )Colour::Summer,
        Winter = ( size_t )Colour::Winter
    };

    enum class Diverging
    {
        BrBG = ( size_t )Colour::BrBG,
        Bwr = ( size_t )Colour::Bwr,
        Coolwarm = ( size_t )Colour::Coolwarm,
        PiYG = ( size_t )Colour::PiYG,
        PRGn = ( size_t )Colour::PRGn,
        PuOr = ( size_t )Colour::PuOr,
        RdBu = ( size_t )Colour::RdBu,
        RdGy = ( size_t )Colour::RdGy,
        RdYlBu = ( size_t )Colour::RdYlBu,
        RdYlGn = ( size_t )Colour::RdYlGn,
        Spectral = ( size_t )Colour::Spectral,
        Seismic = ( size_t )Colour::Seismic
    };

    enum class Qualitative
    {
        Accent = ( size_t )Colour::Accent,
        Dark2 = ( size_t )Colour::Dark2,
        Paired = ( size_t )Colour::Paired,
        Pastel1 = ( size_t )Colour::Pastel1,
        Pastel2 = ( size_t )Colour::Pastel2,
        Set1 = ( size_t )Colour::Set1,
        Set2 = ( size_t )Colour::Set2,
        Set3 = ( size_t )Colour::Set3
    };

    enum class Miscellaneous
    {
        GistEarth = ( size_t )Colour::GistEarth,
        Terrain = ( size_t )Colour::Terrain,
        Ocean = ( size_t )Colour::Ocean,
        GistStern = ( size_t )Colour::GistStern,
        Brg = ( size_t )Colour::Brg,
        CMRmap = ( size_t )Colour::CMRmap,
        CubeHelix = ( size_t )Colour::CubeHelix,
        GnuPlot = ( size_t )Colour::GnuPlot,
        GnuPlot2 = ( size_t )Colour::GnuPlot2,
        GistNcar = ( size_t )Colour::GistNcar,
        NipySpectral = ( size_t )Colour::NipySpectral,
        Jet = ( size_t )Colour::Jet,
        Rainbow = ( size_t )Colour::Rainbow,
        GistRainbow = ( size_t )Colour::GistRainbow,
        Hsv = ( size_t )Colour::Hsv,
        Flag = ( size_t )Colour::Flag,
        Prism = ( size_t )Colour::Prism
    };

    Palette( Type type )
        : mType( type )
    {
    }

    Palette( const std::string &type )
        : mCustomType( type ),
          mType( Type::Custom )
    {
    }

    template< typename tT >
    Palette( tT colour, int32_t amount = 8, ColourType type = ColourType::None )
        : mCustomType( "sns.color_palette( '" + GetColour( colour, type ) +
                       "', " + std::to_string( amount ) + " )" ),
          mType( Type::ColorPallette )
    {
    }

    template< typename tT >
    Palette( tT colour, ColourType type = ColourType::None )
        : Palette( "sns.color_palette( '" + GetColour( colour, type ) + "'" )
    {
    }

    std::string ToString() const
    {
        return GetPalette( mType );
    }

    std::string GetArguments() const
    {
        return mArguments.str();
    }

    Palette &SetColours( size_t nColours )
    {
        mArguments << ", n_colors = " << nColours;
        return *this;
    }

    Palette &SetDesaturation( double desat )
    {
        mArguments << ", desat = " << desat;
        return *this;
    }

    Palette &UseColourCodes( bool colourCodes )
    {
        mArguments << ", color_codes = " << ( colourCodes ? "True" : "False" );
        return *this;
    }

    Palette &SetColourMap( bool map )
    {
        mArguments << ", as_cmap = " << ( map ? "True" : "False" );
        return *this;
    }

protected:

    std::stringstream mStream;
    std::stringstream mArguments;
    std::string mCustomType;
    Type mType;

    template< typename tT >
    static std::string GetColour( tT colour, ColourType type )
    {
        std::string col = __PlotLibColours[static_cast<size_t>( colour )];

        if ( type != ColourType::None )
        {
            if ( type == ColourType::Dark )
            {
                col += "_d";
            }
            else
            {
                col += "_r";
            }
        }

        return col;
    }

    std::string GetPalette( Type palette ) const
    {
        switch ( palette )
        {
        case Type::Reset:
            return "'None'";

        case Type::Custom:
            return "'" + mCustomType + "'";

        case Type::ColorPallette:
            return mCustomType;

        case Type::Other:
            {
                std::string arguments = mArguments.str();

                if ( arguments.size() > 0 && *arguments.begin() == ',' )
                {
                    arguments = std::string( arguments.begin() + 1, arguments.end() );
                }

                return mStream.str() + arguments + " )";
            }
        }

        return "";
    }
};

class HUSLPalette
    : public Palette
{
public:

    HUSLPalette()
        : Palette( Type::Other )
    {
        mStream << "sns.husl_palette( ";
    }

    HUSLPalette &SetColours( size_t nColours )
    {
        mArguments << ", n_colors = " << nColours;
        return *this;
    }

    HUSLPalette &SetHue( double h )
    {
        mArguments << ", h = " << h;
        return *this;
    }

    HUSLPalette &SetSaturation( double s )
    {
        mArguments << ", s = " << s;
        return *this;
    }

    HUSLPalette &SetLightness( double l )
    {
        mArguments << ", l = " << l;
        return *this;
    }
};

class HLSPalette
    : public Palette
{
public:

    HLSPalette()
        : Palette( Type::Other )
    {
        mArguments << "sns.hls_palette( ";
    }

    HLSPalette &SetColours( size_t nColours )
    {
        mArguments << ", n_colors = " << nColours;
        return *this;
    }

    HLSPalette &SetHue( double h )
    {
        mArguments << ", h = " << h;
        return *this;
    }

    HLSPalette &SetSaturation( double s )
    {
        mArguments << ", s = " << s;
        return *this;
    }

    HLSPalette &SetLightness( double l )
    {
        mArguments << ", l = " << l;
        return *this;
    }
};

class CubeHelixPalette
    : public Palette
{
public:

    CubeHelixPalette()
        : Palette( Type::Other )
    {
        mStream << "sns.cubehelix_palette( ";
    }

    CubeHelixPalette &SetColours( size_t nColours )
    {
        mArguments << ", n_colors = " << nColours;
        return *this;
    }

    CubeHelixPalette &SetStart( double start )
    {
        mArguments << ", start = " << start;
        return *this;
    }

    CubeHelixPalette &SetRot( double rot )
    {
        mArguments << ", rot = " << rot;
        return *this;
    }

    CubeHelixPalette &SetGamma( double gamma )
    {
        mArguments << ", gamma = " << gamma;
        return *this;
    }

    CubeHelixPalette &SetHue( double hue )
    {
        mArguments << ", hue = " << hue;
        return *this;
    }

    CubeHelixPalette &SetDark( double dark )
    {
        mArguments << ", dark = " << dark;
        return *this;
    }

    CubeHelixPalette &SetLight( double light )
    {
        mArguments << ", light = " << light;
        return *this;
    }

    CubeHelixPalette &SetReverse( bool reverse )
    {
        mArguments << ", reverse = " << ( reverse ? "True" : "False" );
        return *this;
    }
};

class DarkPalette
    : public Palette
{
public:

    enum class Input
    {
        RGB,
        HLS,
        HUSL,
        XKCD
    };

    template< typename tT >
    DarkPalette( tT colour, ColourType type = ColourType::None )
        : Palette( Type::Other )
    {
        mArguments << "sns.dark_palette( '" << GetColour( colour, type ) << "'";
    }

    DarkPalette( std::tuple< uint8_t, uint8_t, uint8_t > colour, Input input )
        : Palette( Type::Other )
    {
        assert( input != Input::XKCD );
        mArguments << "sns.dark_palette( ( " << std::get<0>( colour ) << ", "
                   << std::get<1>( colour ) << ", "
                   << std::get<2>( colour ) << " ), input = " << GetInput( input );
    }

    DarkPalette( const std::string &colour, Input input )
        : Palette( Type::Other )
    {
        assert( input == Input::XKCD );
        mArguments << "sns.dark_palette( " << colour << ", input = " << GetInput( input );
    }

    DarkPalette &SetColours( size_t nColours )
    {
        mArguments << ", n_colors = " << nColours;
        return *this;
    }

    DarkPalette &SetReverse( bool reverse )
    {
        mArguments << ", reverse = " << ( reverse ? "True" : "False" );
        return *this;
    }

protected:

    DarkPalette( Type type )
        : Palette( type )
    {
    }

    std::string GetInput( Input input )
    {
        switch ( input )
        {
        case Input::RGB:
            return "'rgb'";

        case Input::HLS:
            return "'hls'";

        case Input::HUSL:
            return "'huls'";

        case Input::XKCD:
            return "'xkcd'";
        }

        return "";
    }
};

class LightPalette
    : public DarkPalette
{
public:

    template< typename tT >
    LightPalette( tT colour, ColourType type = ColourType::None )
        : DarkPalette( Type::Other )
    {
        mArguments << "sns.light_palette( '" << GetColour( colour, type ) << "'";
    }

    LightPalette( std::tuple< uint8_t, uint8_t, uint8_t > colour, Input input )
        : DarkPalette( Type::Other )
    {
        assert( input != Input::XKCD );
        mArguments << "sns.light_palette( ( " << std::get<0>( colour ) << ", "
                   << std::get<1>( colour ) << ", "
                   << std::get<2>( colour ) << " ), input = " << GetInput( input );
    }

    LightPalette( const std::string &colour, Input input )
        : DarkPalette( Type::Other )
    {
        assert( input == Input::XKCD );
        mArguments << "sns.light_palette( " << colour << ", input = " << GetInput( input );
    }
};

class DivergingPalette
    : public Palette
{
public:

    enum class Center
    {
        Light,
        Dark
    };

    DivergingPalette( double hNeg, double hPos )
        : Palette( Type::Other )
    {
        assert( hNeg >= 0 && hNeg <= 359 );
        assert( hPos >= 0 && hPos <= 359 );
        mArguments << "sns.diverging_palette( " << hNeg << ", " << hPos;
    }

    DivergingPalette &SetSaturation( double sat )
    {
        mArguments << ", s = " << sat;
        return *this;
    }

    DivergingPalette &SetLightness( double light )
    {
        mArguments << ", l = " << light;
        return *this;
    }

    DivergingPalette &SetColours( size_t nColours )
    {
        mArguments << ", n_colors = " << nColours;
        return *this;
    }

    DivergingPalette &SetCenter( Center center )
    {
        mArguments << ", center = " << ( center == Center::Dark ? "'dark'" : "light'" );
        return *this;
    }
};

#endif