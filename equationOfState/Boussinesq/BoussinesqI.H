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

\*---------------------------------------------------------------------------*/

#include "Boussinesq.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Construct from components
inline Boussinesq::Boussinesq
(
    const specie& sp
)
:
    specie(sp)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct as named copy
inline Boussinesq::Boussinesq(const word& name, const Boussinesq& bq)
:
    specie(name, bq),
    rho0_(bq.rho0_),
    T0_(bq.T0_),
    beta_(bq.beta_)
{}


// Construct and return a clone
inline autoPtr<Boussinesq> Boussinesq::clone() const
{
    return autoPtr<Boussinesq>(new Boussinesq(*this));
}


// Selector from Istream
inline autoPtr<Boussinesq> Boussinesq::New(Istream& is)
{
    return autoPtr<Boussinesq>(new Boussinesq(is));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline scalar Boussinesq::rho0() const
{
    return rho0_;
}

inline scalar Boussinesq::T0() const
{
    return T0_;
}

inline scalar Boussinesq::beta() const
{
    return beta_;
}

//- Return density [kg/m^3]
inline scalar Boussinesq::rho(scalar p, scalar T) const
{
    //return p/(R()*T);
    //return rho0_;
    return rho0_*(scalar(1.0) - beta_*(T-T0_));
}

//- Return compressibility rho/p [s^2/m^2]
inline scalar Boussinesq::psi(scalar p, scalar T) const
{
    return rho(p, T)/(p);
    //return 1.0/(R()*T);
    //return 0.0;
}

//- Return compression factor []
inline scalar Boussinesq::Z(scalar, scalar) const
{
    return 1.0;
    //return 0.0;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void Boussinesq::operator+=(const Boussinesq& bq)
{
    specie::operator+=(bq);
}

inline void Boussinesq::operator-=(const Boussinesq& bq)
{
    specie::operator-=(bq);
}

inline void Boussinesq::operator*=(const scalar s)
{
    specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline Boussinesq operator+
(
    const Boussinesq& bq1,
    const Boussinesq& bq2
)
{
    return Boussinesq
    (
        static_cast<const specie&>(bq1)
      + static_cast<const specie&>(bq2)
    );
}


inline Boussinesq operator-
(
    const Boussinesq& bq1,
    const Boussinesq& bq2
)
{
    return Boussinesq
    (
        static_cast<const specie&>(bq1)
      - static_cast<const specie&>(bq2)
    );
}


inline Boussinesq operator*
(
    const scalar s,
    const Boussinesq& bq
)
{
    return Boussinesq(s*static_cast<const specie&>(bq));
}


inline Boussinesq operator==
(
    const Boussinesq& bq1,
    const Boussinesq& bq2
)
{
    return bq2 - bq1;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
