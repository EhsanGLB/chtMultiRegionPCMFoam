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

#include "janafThermo.H"
#include "specie.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class equationOfState>
inline Foam::janafThermo<equationOfState>::janafThermo
(
    const equationOfState& st,
    const scalar Tlow,
    const scalar Thigh,
    const scalar Tcommon,
    const typename janafThermo<equationOfState>::coeffArray& highCpCoeffs,
    const typename janafThermo<equationOfState>::coeffArray& lowCpCoeffs
)
:
    equationOfState(st),
    Tlow_(Tlow),
    Thigh_(Thigh),
    Tcommon_(Tcommon)
{
    for (label coefLabel = 0; coefLabel < nCoeffs_; coefLabel++)
    {
        highCpCoeffs_[coefLabel] = highCpCoeffs[coefLabel];
        lowCpCoeffs_[coefLabel] = lowCpCoeffs[coefLabel];
    }
}


template<class equationOfState>
inline void Foam::janafThermo<equationOfState>::checkT(scalar& T) const
{
    if (T <  Tlow_ || T > Thigh_)
    {
        // Improvements: graceful exit with recovery.  HJ, 11/Oct/2010
        InfoIn
        (
            "janafThermo<equationOfState>::checkT(scalar& T) const"
        )   << "attempt to use janafThermo<equationOfState>"
               " out of temperature range "
            << Tlow_ << " -> " << Thigh_ << ";  T = " << T
            << endl;

        // Bracket T to avoid out-of-range error
        T = Foam::min(Thigh_, Foam::max(T, Tlow_));
    }
}


template<class equationOfState>
inline const typename Foam::janafThermo<equationOfState>::coeffArray&
Foam::janafThermo<equationOfState>::coeffs
(
    scalar& T
) const
{
    // Note: T will be bounded by checkT in coeffs(T).  No longer const
    // HJ, 12/Oct/2010
    checkT(T);

    if (T < Tcommon_)
    {
        return lowCpCoeffs_;
    }
    else
    {
        return highCpCoeffs_;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class equationOfState>
inline Foam::janafThermo<equationOfState>::janafThermo
(
    const word& name,
    const janafThermo& jt
)
:
    equationOfState(name, jt),
    Tlow_(jt.Tlow_),
    Thigh_(jt.Thigh_),
    Tcommon_(jt.Tcommon_)
{
    for (label coefLabel = 0; coefLabel < nCoeffs_; coefLabel++)
    {
        highCpCoeffs_[coefLabel] = jt.highCpCoeffs_[coefLabel];
        lowCpCoeffs_[coefLabel] = jt.lowCpCoeffs_[coefLabel];
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class equationOfState>
inline Foam::scalar Foam::janafThermo<equationOfState>::cp
(
    scalar T
) const
{
    const coeffArray& a = coeffs(T);
    return this->RR()*((((a[4]*T + a[3])*T + a[2])*T + a[1])*T + a[0]);
}


template<class equationOfState>
inline Foam::scalar Foam::janafThermo<equationOfState>::h
(
    scalar T
) const
{
    const coeffArray& a = coeffs(T);
    return this->RR()*
    (
        ((((a[4]/5.0*T + a[3]/4.0)*T + a[2]/3.0)*T + a[1]/2.0)*T + a[0])*T
      + a[5]
    );
}


template<class equationOfState>
inline Foam::scalar Foam::janafThermo<equationOfState>::hs
(
    scalar T
) const
{
    return h(T) - hc();
}


template<class equationOfState>
inline Foam::scalar Foam::janafThermo<equationOfState>::hc() const
{
    const coeffArray& a = lowCpCoeffs_;
    const scalar& Tstd = specie::Tstd();
    return this->RR()*
    (
        (
            (((a[4]/5.0*Tstd + a[3]/4.0)*Tstd + a[2]/3.0)*Tstd + a[1]/2.0)*Tstd
          + a[0]
        )*Tstd + a[5]
    );
}


template<class equationOfState>
inline Foam::scalar Foam::janafThermo<equationOfState>::s
(
    scalar T
) const
{
    // Note: T will be bounded by checkT in coeffs(T).  No longer const
    // HJ, 12/Oct/2010

    const coeffArray& a = coeffs(T);
    return
    this->RR()*
    (
        (((a[4]/4.0*T + a[3]/3.0)*T + a[2]/2.0)*T + a[1])*T + a[0]*::log(T)
      + a[6]
    );
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class equationOfState>
inline void Foam::janafThermo<equationOfState>::operator+=
(
    const janafThermo<equationOfState>& jt
)
{
    scalar molr1 = this->nMoles();

    equationOfState::operator+=(jt);

    molr1 /= this->nMoles();
    scalar molr2 = jt.nMoles()/this->nMoles();

    Tlow_ = max(Tlow_, jt.Tlow_);
    Thigh_ = min(Thigh_, jt.Thigh_);
    Tcommon_ = molr1*Tcommon_ + molr2*jt.Tcommon_;

    for
    (
        label coefLabel = 0;
        coefLabel<janafThermo<equationOfState>::nCoeffs_;
        coefLabel++
    )
    {
        highCpCoeffs_[coefLabel] =
            molr1*highCpCoeffs_[coefLabel]
          + molr2*jt.highCpCoeffs_[coefLabel];

        lowCpCoeffs_[coefLabel] =
            molr1*lowCpCoeffs_[coefLabel]
          + molr2*jt.lowCpCoeffs_[coefLabel];
    }
}


template<class equationOfState>
inline void Foam::janafThermo<equationOfState>::operator-=
(
    const janafThermo<equationOfState>& jt
)
{
    scalar molr1 = this->nMoles();

    equationOfState::operator-=(jt);

    molr1 /= this->nMoles();
    scalar molr2 = jt.nMoles()/this->nMoles();

    Tlow_ = max(Tlow_, jt.Tlow_);
    Thigh_ = min(Thigh_, jt.Thigh_);
    Tcommon_ = molr1*Tcommon_ - molr2*jt.Tcommon_;

    for
    (
        label coefLabel = 0;
        coefLabel<janafThermo<equationOfState>::nCoeffs_;
        coefLabel++
    )
    {
        highCpCoeffs_[coefLabel] =
            molr1*highCpCoeffs_[coefLabel]
          - molr2*jt.highCpCoeffs_[coefLabel];

        lowCpCoeffs_[coefLabel] =
            molr1*lowCpCoeffs_[coefLabel]
          - molr2*jt.lowCpCoeffs_[coefLabel];
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class equationOfState>
inline Foam::janafThermo<equationOfState> Foam::operator+
(
    const janafThermo<equationOfState>& jt1,
    const janafThermo<equationOfState>& jt2
)
{
    equationOfState eofs = jt1;
    eofs += jt2;

    scalar molr1 = jt1.nMoles()/eofs.nMoles();
    scalar molr2 = jt2.nMoles()/eofs.nMoles();

    typename janafThermo<equationOfState>::coeffArray highCpCoeffs;
    typename janafThermo<equationOfState>::coeffArray lowCpCoeffs;

    for
    (
        label coefLabel = 0;
        coefLabel<janafThermo<equationOfState>::nCoeffs_;
        coefLabel++
    )
    {
        highCpCoeffs[coefLabel] =
            molr1*jt1.highCpCoeffs_[coefLabel]
          + molr2*jt2.highCpCoeffs_[coefLabel];

        lowCpCoeffs[coefLabel] =
            molr1*jt1.lowCpCoeffs_[coefLabel]
          + molr2*jt2.lowCpCoeffs_[coefLabel];
    }

    return janafThermo<equationOfState>
    (
        eofs,
        max(jt1.Tlow_, jt2.Tlow_),
        min(jt1.Thigh_, jt2.Thigh_),
        molr1*jt1.Tcommon_ + molr2*jt2.Tcommon_,
        highCpCoeffs,
        lowCpCoeffs
    );
}


template<class equationOfState>
inline Foam::janafThermo<equationOfState> Foam::operator-
(
    const janafThermo<equationOfState>& jt1,
    const janafThermo<equationOfState>& jt2
)
{
    equationOfState eofs = jt1;
    eofs -= jt2;

    scalar molr1 = jt1.nMoles()/eofs.nMoles();
    scalar molr2 = jt2.nMoles()/eofs.nMoles();

    typename janafThermo<equationOfState>::coeffArray highCpCoeffs;
    typename janafThermo<equationOfState>::coeffArray lowCpCoeffs;

    for
    (
        label coefLabel = 0;
        coefLabel<janafThermo<equationOfState>::nCoeffs_;
        coefLabel++
    )
    {
        highCpCoeffs[coefLabel] =
            molr1*jt1.highCpCoeffs_[coefLabel]
          - molr2*jt2.highCpCoeffs_[coefLabel];

        lowCpCoeffs[coefLabel] =
            molr1*jt1.lowCpCoeffs_[coefLabel]
          - molr2*jt2.lowCpCoeffs_[coefLabel];
    }

    return janafThermo<equationOfState>
    (
        eofs,
        max(jt1.Tlow_, jt2.Tlow_),
        min(jt1.Thigh_, jt2.Thigh_),
        molr1*jt1.Tcommon_ - molr2*jt2.Tcommon_,
        highCpCoeffs,
        lowCpCoeffs
    );
}


template<class equationOfState>
inline Foam::janafThermo<equationOfState> Foam::operator*
(
    const scalar s,
    const janafThermo<equationOfState>& jt
)
{
    return janafThermo<equationOfState>
    (
        s*static_cast<const equationOfState&>(jt),
        jt.Tlow_,
        jt.Thigh_,
        jt.Tcommon_,
        jt.highCpCoeffs_,
        jt.lowCpCoeffs_
    );
}


template<class equationOfState>
inline Foam::janafThermo<equationOfState> Foam::operator==
(
    const janafThermo<equationOfState>& jt1,
    const janafThermo<equationOfState>& jt2
)
{
    return jt2 - jt1;
}


// ************************************************************************* //
