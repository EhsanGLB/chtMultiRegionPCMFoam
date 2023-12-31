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

InClass
    Foam::basicRhoThermo

Description
    Macros for creating 'basic' density-based thermo packages

\*---------------------------------------------------------------------------*/

#ifndef makeBasicRhoThermo_H
#define makeBasicRhoThermo_H

#include "basicRhoThermo.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeBasicRhoThermo(Cthermo,Mixture,Transport,Thermo,EqnOfState)       \
                                                                              \
typedef Cthermo<Mixture<Transport<specieThermo<Thermo<EqnOfState> > > > >     \
    Cthermo##Mixture##Transport##Thermo##EqnOfState;                          \
                                                                              \
defineTemplateTypeNameAndDebugWithName                                        \
(                                                                             \
    Cthermo##Mixture##Transport##Thermo##EqnOfState,                          \
    #Cthermo                                                                  \
        "<"#Mixture"<"#Transport"<specieThermo<"#Thermo"<"#EqnOfState">>>>>", \
    0                                                                         \
);                                                                            \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    basicRhoThermo,                                                           \
    Cthermo##Mixture##Transport##Thermo##EqnOfState,                          \
    fvMesh                                                                    \
)


#define makeBasicRhoThermoPhys(CThermo,Mixture,ThermoPhys)                    \
                                                                              \
typedef CThermo<Mixture<ThermoPhys> >                                         \
    CThermo##Mixture##ThermoPhys;                                             \
                                                                              \
defineTemplateTypeNameAndDebugWithName                                        \
(                                                                             \
    CThermo##Mixture##ThermoPhys,                                             \
    #CThermo"<"#Mixture"<"#ThermoPhys">>",                                    \
    0                                                                         \
);                                                                            \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    basicRhoThermo,                                                           \
    CThermo##Mixture##ThermoPhys,                                             \
    fvMesh                                                                    \
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
