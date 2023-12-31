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
    Foam::hsRhoThermo

Description
    Sensible enthalpy for a mixture based on density

SourceFiles
    hsRhoThermo.C

\*---------------------------------------------------------------------------*/

#ifndef hsRhoThermo_H
#define hsRhoThermo_H

#include "basicRhoThermo.H"
#include "basicMixture.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class hsRhoThermo Declaration
\*---------------------------------------------------------------------------*/

template<class MixtureType>
class hsRhoThermo
:
    public basicRhoThermo,
    public MixtureType
{
    // Private data

        //- Sensible enthalpy field [J/kg]
        volScalarField hs_;


    // Private member functions

        //- Calculate the thermo variables
        void calculate();

        //- Construct as copy (not implemented)
        hsRhoThermo(const hsRhoThermo<MixtureType>&);


public:

    //- Runtime type information
    TypeName("hsRhoThermo");


    // Constructors

        //- Construct from mesh and object registry
        hsRhoThermo(const fvMesh&, const objectRegistry&);


    //- Destructor
    virtual ~hsRhoThermo();


    // Member functions

        //- Return the compostion of the combustion mixture
        virtual basicMixture& composition()
        {
            return *this;
        }

        //- Return the compostion of the combustion mixture
        virtual const basicMixture& composition() const
        {
            return *this;
        }

        //- Update properties
        virtual void correct();


        // Access to thermodynamic state variables

            //- Sensible enthalpy [J/kg]
            //  Non-const access allowed for transport equations
            virtual volScalarField& hs()
            {
                return hs_;
            }

            //- Sensible enthalpy [J/kg]
            virtual const volScalarField& hs() const
            {
                return hs_;
            }


        // Fields derived from thermodynamic state variables

            //- Sensible enthalpy for cell-set [J/kg]
            virtual tmp<scalarField> hs
            (
                const scalarField& T,
                const labelList& cells
            ) const;

            //- Sensible enthalpy for patch [J/kg]
            virtual tmp<scalarField> hs
            (
                const scalarField& T,
                const label patchi
            ) const;

            //- Heat capacity at constant pressure for patch [J/kg/K]
            virtual tmp<scalarField> Cp
            (
                const scalarField& T,
                const label patchi
            ) const;

            //- Heat capacity at constant pressure for cell-set [J/kg/K]
            virtual tmp<scalarField> Cp
            (
                const scalarField& T,
                const labelList& cells
            ) const;

            //- Heat capacity at constant pressure [J/kg/K]
            virtual tmp<volScalarField> Cp() const;

            //- Heat capacity at constant volume for patch [J/kg/K]
            virtual tmp<scalarField> Cv
            (
                const scalarField& T,
                const label patchi
            ) const;

            //- Heat capacity at constant volume [J/kg/K]
            virtual tmp<volScalarField> Cv() const;


        //- Read thermophysicalProperties dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "hsRhoThermo.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
