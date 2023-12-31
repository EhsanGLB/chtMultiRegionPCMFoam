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

#include "ePsiThermo.H"
#include "fvMesh.H"
#include "fixedValueFvPatchFields.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class MixtureType>
void Foam::ePsiThermo<MixtureType>::calculate()
{
    const scalarField& eCells = e_.internalField();
    const scalarField& pCells = this->p_.internalField();

    scalarField& TCells = this->T_.internalField();
    scalarField& psiCells = this->psi_.internalField();
    scalarField& muCells = this->mu_.internalField();
    scalarField& alphaCells = this->alpha_.internalField();

    forAll(TCells, celli)
    {
        const typename MixtureType::thermoType& mixture_ =
            this->cellMixture(celli);

        TCells[celli] = mixture_.TE(eCells[celli], TCells[celli]);
        psiCells[celli] = mixture_.psi(pCells[celli], TCells[celli]);

        muCells[celli] = mixture_.mu(TCells[celli]);
        alphaCells[celli] = mixture_.alpha(TCells[celli]);
    }

    forAll(this->T_.boundaryField(), patchi)
    {
        fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
        fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
        fvPatchScalarField& ppsi = this->psi_.boundaryField()[patchi];

        fvPatchScalarField& pe = e_.boundaryField()[patchi];

        fvPatchScalarField& pmu = this->mu_.boundaryField()[patchi];
        fvPatchScalarField& palpha = this->alpha_.boundaryField()[patchi];

        if (pT.fixesValue())
        {
            forAll(pT, facei)
            {
                const typename MixtureType::thermoType& mixture_ =
                    this->patchFaceMixture(patchi, facei);

                pe[facei] = mixture_.E(pT[facei]);

                ppsi[facei] = mixture_.psi(pp[facei], pT[facei]);
                pmu[facei] = mixture_.mu(pT[facei]);
                palpha[facei] = mixture_.alpha(pT[facei]);
            }
        }
        else
        {
            forAll(pT, facei)
            {
                const typename MixtureType::thermoType& mixture_ =
                    this->patchFaceMixture(patchi, facei);

                pT[facei] = mixture_.TE(pe[facei], pT[facei]);

                ppsi[facei] = mixture_.psi(pp[facei], pT[facei]);
                pmu[facei] = mixture_.mu(pT[facei]);
                palpha[facei] = mixture_.alpha(pT[facei]);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class MixtureType>
Foam::ePsiThermo<MixtureType>::ePsiThermo
(
    const fvMesh& mesh,
    const objectRegistry& obj
)
:
    basicPsiThermo(mesh, obj),
    MixtureType(*this, mesh, obj),

    e_
    (
        IOobject
        (
            "e",
            mesh.time().timeName(),
            obj,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionSet(0, 2, -2, 0, 0),
        this->eBoundaryTypes()
    )
{
    scalarField& eCells = e_.internalField();
    const scalarField& TCells = this->T_.internalField();

    forAll(eCells, celli)
    {
        eCells[celli] = this->cellMixture(celli).E(TCells[celli]);
    }

    forAll(e_.boundaryField(), patchi)
    {
        e_.boundaryField()[patchi] ==
            e(this->T_.boundaryField()[patchi], patchi);
    }

    this->eBoundaryCorrection(e_);

    calculate();

    // Switch on saving old time
    this->psi_.oldTime();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class MixtureType>
Foam::ePsiThermo<MixtureType>::~ePsiThermo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class MixtureType>
void Foam::ePsiThermo<MixtureType>::correct()
{
    if (debug)
    {
        Info<< "entering ePsiThermo<MixtureType>::correct()" << endl;
    }

    // Force the saving of the old-time values
    this->psi_.oldTime();

    calculate();

    if (debug)
    {
        Info<< "exiting ePsiThermo<MixtureType>::correct()" << endl;
    }
}


template<class MixtureType>
Foam::tmp<Foam::scalarField> Foam::ePsiThermo<MixtureType>::e
(
    const scalarField& T,
    const labelList& cells
) const
{
    tmp<scalarField> te(new scalarField(T.size()));
    scalarField& h = te();

    forAll(T, celli)
    {
        h[celli] = this->cellMixture(cells[celli]).E(T[celli]);
    }

    return te;
}


template<class MixtureType>
Foam::tmp<Foam::scalarField> Foam::ePsiThermo<MixtureType>::e
(
    const scalarField& T,
    const label patchi
) const
{
    tmp<scalarField> te(new scalarField(T.size()));
    scalarField& h = te();

    forAll(T, facei)
    {
        h[facei] = this->patchFaceMixture(patchi, facei).E(T[facei]);
    }

    return te;
}


template<class MixtureType>
Foam::tmp<Foam::scalarField> Foam::ePsiThermo<MixtureType>::Cp
(
    const scalarField& T,
    const label patchi
) const
{
    tmp<scalarField> tCp(new scalarField(T.size()));
    scalarField& cp = tCp();

    forAll(T, facei)
    {
        cp[facei] = this->patchFaceMixture(patchi, facei).Cp(T[facei]);
    }

    return tCp;
}


template<class MixtureType>
Foam::tmp<Foam::scalarField> Foam::ePsiThermo<MixtureType>::Cp
(
    const scalarField& T,
    const labelList& cells
) const
{
    tmp<scalarField> tCp(new scalarField(T.size()));
    scalarField& cp = tCp();

    forAll(T, celli)
    {
        cp[celli] = this->cellMixture(cells[celli]).Cp(T[celli]);
    }

    return tCp;
}


template<class MixtureType>
Foam::tmp<Foam::volScalarField> Foam::ePsiThermo<MixtureType>::Cp() const
{
    const fvMesh& mesh = this->T_.mesh();

    tmp<volScalarField> tCp
    (
        new volScalarField
        (
            IOobject
            (
                "Cp",
                mesh.time().timeName(),
                T_.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimensionSet(0, 2, -2, -1, 0)
        )
    );

    volScalarField& cp = tCp();

    forAll(this->T_, celli)
    {
        cp[celli] = this->cellMixture(celli).Cp(this->T_[celli]);
    }

    forAll(this->T_.boundaryField(), patchi)
    {
        const fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
        fvPatchScalarField& pCp = cp.boundaryField()[patchi];

        forAll(pT, facei)
        {
            pCp[facei] = this->patchFaceMixture(patchi, facei).Cp(pT[facei]);
        }
    }

    return tCp;
}


template<class MixtureType>
Foam::tmp<Foam::scalarField> Foam::ePsiThermo<MixtureType>::Cv
(
    const scalarField& T,
    const label patchi
) const
{
    tmp<scalarField> tCv(new scalarField(T.size()));
    scalarField& cv = tCv();

    forAll(T, facei)
    {
        cv[facei] = this->patchFaceMixture(patchi, facei).Cv(T[facei]);
    }

    return tCv;
}


template<class MixtureType>
Foam::tmp<Foam::volScalarField> Foam::ePsiThermo<MixtureType>::Cv() const
{
    const fvMesh& mesh = this->T_.mesh();

    tmp<volScalarField> tCv
    (
        new volScalarField
        (
            IOobject
            (
                "Cv",
                mesh.time().timeName(),
                T_.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimensionSet(0, 2, -2, -1, 0)
        )
    );

    volScalarField& cv = tCv();

    forAll(this->T_, celli)
    {
        cv[celli] = this->cellMixture(celli).Cv(this->T_[celli]);
    }

    forAll(this->T_.boundaryField(), patchi)
    {
        cv.boundaryField()[patchi] =
            Cv(this->T_.boundaryField()[patchi], patchi);
    }

    return tCv;
}


template<class MixtureType>
bool Foam::ePsiThermo<MixtureType>::read()
{
    if (basicPsiThermo::read())
    {
        MixtureType::read(*this);
        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
