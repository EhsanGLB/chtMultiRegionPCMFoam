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

#include "hPolynomialThermo.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class EquationOfState, int PolySize>
inline Foam::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
    const EquationOfState& pt,
    const scalar Hf,
    const scalar Sf,
    const Polynomial<PolySize>& cpPoly,
    const typename Polynomial<PolySize>::intPolyType& hPoly,
    const Polynomial<PolySize>& sPoly
)
:
    EquationOfState(pt),
    Hf_(Hf),
    Sf_(Sf),
    cpPolynomial_(cpPoly),
    hPolynomial_(hPoly),
    sPolynomial_(sPoly)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class EquationOfState, int PolySize>
inline Foam::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
    const hPolynomialThermo& pt
)
:
    EquationOfState(pt),
    Hf_(pt.Hf_),
    Sf_(pt.Sf_),
    cpPolynomial_(pt.cpPolynomial_),
    hPolynomial_(pt.hPolynomial_),
    sPolynomial_(pt.sPolynomial_)
{}


template<class EquationOfState, int PolySize>
inline Foam::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
    const word& name,
    const hPolynomialThermo& pt
)
:
    EquationOfState(name, pt),
    Hf_(pt.Hf_),
    Sf_(pt.Sf_),
    cpPolynomial_(pt.cpPolynomial_),
    hPolynomial_(pt.hPolynomial_),
    sPolynomial_(pt.sPolynomial_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EquationOfState, int PolySize>
inline Foam::scalar Foam::hPolynomialThermo<EquationOfState, PolySize>::cp
(
    const scalar T
) const
{
    return cpPolynomial_.value(T);
}


template<class EquationOfState, int PolySize>
inline Foam::scalar Foam::hPolynomialThermo<EquationOfState, PolySize>::h
(
    const scalar T
) const
{
    return hPolynomial_.value(T);
}


template<class EquationOfState, int PolySize>
inline Foam::scalar Foam::hPolynomialThermo<EquationOfState, PolySize>::hs
(
    const scalar T
) const
{
    return h(T) - hc();
}


template<class EquationOfState, int PolySize>
inline Foam::scalar Foam::hPolynomialThermo<EquationOfState, PolySize>::hc()
const
{
    return Hf_;
}


template<class EquationOfState, int PolySize>
inline Foam::scalar Foam::hPolynomialThermo<EquationOfState, PolySize>::s
(
    const scalar T
) const
{
    return sPolynomial_.value(T);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class EquationOfState, int PolySize>
inline Foam::hPolynomialThermo<EquationOfState, PolySize>&
Foam::hPolynomialThermo<EquationOfState, PolySize>::operator=
(
    const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
    EquationOfState::operator=(pt);

    Hf_ = pt.Hf_;
    Sf_ = pt.Sf_;
    cpPolynomial_ = pt.cpPolynomial_;
    hPolynomial_ = pt.hPolynomial_;
    sPolynomial_ = pt.sPolynomial_;

    return *this;
}


template<class EquationOfState, int PolySize>
inline void Foam::hPolynomialThermo<EquationOfState, PolySize>::operator+=
(
    const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
    scalar molr1 = this->nMoles();

    EquationOfState::operator+=(pt);

    molr1 /= this->nMoles();
    scalar molr2 = pt.nMoles()/this->nMoles();

    Hf_ = molr1*Hf_ + molr2*pt.Hf_;
    Sf_ = molr1*Sf_ + molr2*pt.Sf_;
    cpPolynomial_ = molr1*cpPolynomial_ + molr2*pt.cpPolynomial_;
    hPolynomial_ = molr1*hPolynomial_ + molr2*pt.hPolynomial_;
    sPolynomial_ = molr1*sPolynomial_ + molr2*pt.sPolynomial_;
}


template<class EquationOfState, int PolySize>
inline void Foam::hPolynomialThermo<EquationOfState, PolySize>::operator-=
(
    const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
    scalar molr1 = this->nMoles();

    EquationOfState::operator-=(pt);

    molr1 /= this->nMoles();
    scalar molr2 = pt.nMoles()/this->nMoles();

    Hf_ = molr1*Hf_ - molr2*pt.Hf_;
    Sf_ = molr1*Sf_ - molr2*pt.Sf_;
    cpPolynomial_ = molr1*cpPolynomial_ - molr2*pt.cpPolynomial_;
    hPolynomial_ = molr1*hPolynomial_ - molr2*pt.hPolynomial_;
    sPolynomial_ = molr1*sPolynomial_ - molr2*pt.sPolynomial_;
}


template<class EquationOfState, int PolySize>
inline void Foam::hPolynomialThermo<EquationOfState, PolySize>::operator*=
(
    const scalar s
)
{
    EquationOfState::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class EquationOfState, int PolySize>
inline Foam::hPolynomialThermo<EquationOfState, PolySize> Foam::operator+
(
    const hPolynomialThermo<EquationOfState, PolySize>& pt1,
    const hPolynomialThermo<EquationOfState, PolySize>& pt2
)
{
    EquationOfState eofs = pt1;
    eofs += pt2;

    scalar molr1 = pt1.nMoles()/eofs.nMoles();
    scalar molr2 = pt2.nMoles()/eofs.nMoles();

    return hPolynomialThermo<EquationOfState, PolySize>
    (
        eofs,
        molr1*pt1.Hf_ + molr2*pt2.Hf_,
        molr1*pt1.Sf_ + molr2*pt2.Sf_,
        molr1*pt1.cpPolynomial_ + molr2*pt2.cpPolynomial_,
        molr1*pt1.hPolynomial_ + molr2*pt2.hPolynomial_,
        molr1*pt1.sPolynomial_ + molr2*pt2.sPolynomial_
    );
}


template<class EquationOfState, int PolySize>
inline Foam::hPolynomialThermo<EquationOfState, PolySize> Foam::operator-
(
    const hPolynomialThermo<EquationOfState, PolySize>& pt1,
    const hPolynomialThermo<EquationOfState, PolySize>& pt2
)
{
    EquationOfState eofs = pt1;
    eofs -= pt2;

    scalar molr1 = pt1.nMoles()/eofs.nMoles();
    scalar molr2 = pt2.nMoles()/eofs.nMoles();

    return hPolynomialThermo<EquationOfState, PolySize>
    (
        eofs,
        molr1*pt1.Hf_ - molr2*pt2.Hf_,
        molr1*pt1.Sf_ - molr2*pt2.Sf_,
        molr1*pt1.cpPolynomial_ - molr2*pt2.cpPolynomial_,
        molr1*pt1.hPolynomial_ - molr2*pt2.hPolynomial_,
        molr1*pt1.sPolynomial_ - molr2*pt2.sPolynomial_
    );
}


template<class EquationOfState, int PolySize>
inline Foam::hPolynomialThermo<EquationOfState, PolySize> Foam::operator*
(
    const scalar s,
    const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
    return hPolynomialThermo<EquationOfState, PolySize>
    (
        s*static_cast<const EquationOfState&>(pt),
        pt.Hf_,
        pt.Sf_,
        pt.cpPolynomial_,
        pt.hPolynomial_,
        pt.sPolynomial_
    );
}


template<class EquationOfState, int PolySize>
inline Foam::hPolynomialThermo<EquationOfState, PolySize> Foam::operator==
(
    const hPolynomialThermo<EquationOfState, PolySize>& pt1,
    const hPolynomialThermo<EquationOfState, PolySize>& pt2
)
{
    return pt2 - pt1;
}


// ************************************************************************* //
