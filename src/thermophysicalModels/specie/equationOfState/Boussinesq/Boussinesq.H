/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     | Version:     4.1
    \\  /    A nd           | Web:         http://www.foam-extend.org
     \\/     M anipulation  | For copyright notice see file Copyright
-------------------------------------------------------------------------------
License
    This file is part of foam-extend.

    foam-extend is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    foam-extend is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::Boussinesq

Description
    Perfect gas equation of state.

SourceFiles
    BoussinesqI.H
    Boussinesq.C

\*---------------------------------------------------------------------------*/

#ifndef Boussinesq_H
#define Boussinesq_H

#include "specie.H"
#include "autoPtr.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class Boussinesq Declaration
\*---------------------------------------------------------------------------*/

class Boussinesq
:
    public specie
{

    // Private data
        //- Reference density, reference temperature, and thermal expansion
        scalar rho0_;
        scalar T0_;
        scalar beta_;



public:

    // Constructors

        //- Construct from components
        inline Boussinesq(const specie& sp);

        //- Construct from Istream
        Boussinesq(Istream&);

        //- Construct as named copy
        inline Boussinesq(const word& name, const Boussinesq&);

        //- Construct and return a clone
        inline autoPtr<Boussinesq> clone() const;

        // Selector from Istream
        inline static autoPtr<Boussinesq> New(Istream& is);


    // Member functions

        inline scalar rho0() const;

        inline scalar T0() const;

        inline scalar beta() const;


        //- Return density [kg/m^3]
        inline scalar rho(scalar p, scalar T) const;

        //- Return compressibility rho/p [s^2/m^2]
        inline scalar psi(scalar p, scalar T) const;

        //- Return compression factor []
        inline scalar Z(scalar p, scalar T) const;


    // Member operators

        inline void operator+=(const Boussinesq&);
        inline void operator-=(const Boussinesq&);

        inline void operator*=(const scalar);


    // Friend operators

        inline friend Boussinesq operator+
        (
            const Boussinesq&,
            const Boussinesq&
        );

        inline friend Boussinesq operator-
        (
            const Boussinesq&,
            const Boussinesq&
        );

        inline friend Boussinesq operator*
        (
            const scalar s,
            const Boussinesq&
        );

        inline friend Boussinesq operator==
        (
            const Boussinesq&,
            const Boussinesq&
        );


    // Ostream Operator

        friend Ostream& operator<<(Ostream&, const Boussinesq&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "BoussinesqI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
