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
    Foam::basicPsiThermo

Description
    Basic thermodynamic properties based on compressibility

SourceFiles
    basicPsiThermo.C
    newBasicPsiThermo.C

\*---------------------------------------------------------------------------*/

#ifndef basicPsiThermo_H
#define basicPsiThermo_H

#include "basicThermo.H"
#include "runTimeSelectionTables.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class basicPsiThermo Declaration
\*---------------------------------------------------------------------------*/

class basicPsiThermo
:
    public basicThermo
{

protected:

    // Protected member functions

        //- Construct as copy (not implemented)
        basicPsiThermo(const basicPsiThermo&);


public:

    //- Runtime type information
    TypeName("basicPsiThermo");


    //- Declare run-time constructor selection table

    declareRunTimeSelectionTable
    (
        autoPtr,
        basicPsiThermo,
        fvMesh,
        (const fvMesh& mesh, const objectRegistry& obj),
        (mesh, obj)
    );


    // Constructors

        //- Construct from mesh and object registry
        basicPsiThermo(const fvMesh&, const objectRegistry&);


    //- Selector
    static autoPtr<basicPsiThermo> New(const fvMesh&, const objectRegistry&);

    static autoPtr<basicPsiThermo> New(const fvMesh& mesh)
    {
        return New(mesh, mesh);
    }


    //- Destructor
    virtual ~basicPsiThermo();


    // Member functions

        // Fields derived from thermodynamic state variables

            //- Density [kg/m^3] - uses current value of pressure
            virtual tmp<volScalarField> rho() const
            {
                return p_*psi();
            }

            //CL: drhodh needed for pressure equation of the real gas solver
            virtual const volScalarField& drhodh() const;

            //CL: drhode needed for pressure equation of the real gas solver
            virtual const volScalarField& drhode() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
