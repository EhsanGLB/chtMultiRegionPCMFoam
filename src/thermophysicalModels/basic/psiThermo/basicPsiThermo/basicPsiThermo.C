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

#include "basicPsiThermo.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(basicPsiThermo, 0);
    defineRunTimeSelectionTable(basicPsiThermo, fvMesh);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::basicPsiThermo::basicPsiThermo
(
    const fvMesh& mesh,
    const objectRegistry& obj
)
:
    basicThermo(mesh, obj)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::basicPsiThermo::~basicPsiThermo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::volScalarField& Foam::basicPsiThermo::drhodh() const
{
    notImplemented("basicPsiThermo::drhodh()");
    return const_cast<volScalarField&>(volScalarField::null());
}

const Foam::volScalarField& Foam::basicPsiThermo::drhode() const
{
    notImplemented("basicPsiThermo::drhode()");
    return const_cast<volScalarField&>(volScalarField::null());
}


// ************************************************************************* //
