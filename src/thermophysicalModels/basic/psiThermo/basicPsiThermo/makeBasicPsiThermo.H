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
    Foam::basicPsiThermo

Description
    Macros for creating 'basic' compresibility-based thermo packages

\*---------------------------------------------------------------------------*/

#ifndef makeBasicPsiThermo_H
#define makeBasicPsiThermo_H

#include "basicPsiThermo.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeBasicPsiThermo(Cthermo,Mixture,Transport,Thermo,EqnOfState)       \
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
    basicPsiThermo,                                                           \
    Cthermo##Mixture##Transport##Thermo##EqnOfState,                          \
    fvMesh                                                                    \
)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeBasicRealGasThermo(Cthermo,Mixture,Transport,SpecieThermo,Thermo,EqnOfState) \
                                                                              \
typedef Cthermo<Mixture<Transport<SpecieThermo<Thermo<EqnOfState> > > > >     \
    Cthermo##Mixture##Transport##SpecieThermo##Thermo##EqnOfState;            \
                                                                              \
defineTemplateTypeNameAndDebugWithName                                        \
(                                                                             \
    Cthermo##Mixture##Transport##SpecieThermo##Thermo##EqnOfState,            \
    #Cthermo                                                                  \
        "<"#Mixture"<"#Transport"<"#SpecieThermo"<"#Thermo"<"#EqnOfState">>>>>", \
    0                                                                         \
);                                                                            \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    basicPsiThermo,                                                           \
    Cthermo##Mixture##Transport##SpecieThermo##Thermo##EqnOfState,            \
    fvMesh                                                                    \
)

#define makeBasicExternalLibraryBasedThermo(Cthermo)                          \
                                                                              \
typedef Cthermo                                                               \
    Cthermo;                                                                  \
                                                                              \
defineTypeNameAndDebug                                                        \
(                                                                             \
    Cthermo,                                                                  \
    0                                                                         \
);                                                                            \
                                                                              \
addToRunTimeSelectionTable                                                    \
(                                                                             \
    basicPsiThermo,                                                           \
    Cthermo,                                                                  \
    fvMesh                                                                    \
)

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
