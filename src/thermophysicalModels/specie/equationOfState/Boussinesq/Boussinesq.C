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

Description
    Perfect gas equation of state.

\*---------------------------------------------------------------------------*/

#include "Boussinesq.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Boussinesq::Boussinesq(Istream& is)
:
    specie(is),
    rho0_(readScalar(is)),
    T0_(readScalar(is)),
    beta_(readScalar(is))
{
    is.check("Boussinesq::Boussinesq(Istream& is)");
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

Ostream& operator<<(Ostream& os, const Boussinesq& bq)
{
    //os  << static_cast<const specie&>(bq);
    os  << static_cast<const specie&>(bq) << tab
        << "rhoBoussinesq" << tab << bq.rho0_ << tab << bq.T0_ << tab << bq.beta_;
    os.check("Ostream& operator<<(Ostream& os, const Boussinesq& st)");
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
