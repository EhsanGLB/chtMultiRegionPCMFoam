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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct from components
inline Foam::thirdBodyEfficiencies::thirdBodyEfficiencies
(
    const speciesTable& species,
    const scalarList& efficiencies
)
:
    scalarList(efficiencies),
    species_(species)
{
    if (size() != species_.size())
    {
        FatalErrorIn
        (
            "thirdBodyEfficiencies::thirdBodyEfficiencies"
            "(const speciesTable& species, const scalarList& efficiencies)"
        )   << "number of efficiencies = " << size()
            << " is not equat to the number of species " << species_.size()
            << exit(FatalError);
    }
}


//- Construct from Istream
inline Foam::thirdBodyEfficiencies::thirdBodyEfficiencies
(
    const speciesTable& species,
    Istream& is
)
:
    scalarList(species.size()),
    species_(species)
{
    is.readBeginList
    (
        "thirdBodyEfficiencies::thirdBodyEfficiencies"
        "(const speciesTable& species, Istream& is)"
    );
    scalar defaultEff = readScalar(is);
    scalarList::operator=(defaultEff);

    token t;

    while ((is >> t) && !t.isPunctuation())
    {
        if (t.isWord())
        {
            operator[](species[t.wordToken()]) = readScalar(is);
        }
        else
        {
            FatalIOErrorIn
            (
                "thirdBodyEfficiencies::thirdBodyEfficiencies"
                "(const speciesTable& species, Istream& is)",
                is
            )   << "expected <word>, found " << t.info()
                << exit(FatalIOError);
        }
    }

    if (t.pToken() != token::END_LIST)
    {
        FatalIOErrorIn
        (
            "thirdBodyEfficiencies::thirdBodyEfficiencies"
            "(const speciesTable& species, Istream& is)",
            is
        )   << "expected ')', found " << t.info()
            << exit(FatalIOError);
    }

    if (size() != species_.size())
    {
        FatalIOErrorIn
        (
            "thirdBodyEfficiencies::thirdBodyEfficiencies"
            "(const speciesTable& species, Istream& is)",
            is
        )   << "number of efficiencies = " << size()
            << " is not equat to the number of species " << species_.size()
            << exit(FatalIOError);
    }
}


// * * * * * * * * * * * * * * * Member functions  * * * * * * * * * * * * * //

inline Foam::scalar Foam::thirdBodyEfficiencies::M(const scalarList& c) const
{
    scalar M = 0.0;
    forAll (*this, i)
    {
        M += operator[](i)*c[i];
    }

    return M;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

inline Foam::Ostream& Foam::operator<<
(
    Ostream& os,
    const thirdBodyEfficiencies& tbes
)
{
    scalarList orderedTbes = tbes;
    sort(orderedTbes);

    scalar val = orderedTbes[0];
    label count = 1;

    scalar valMaxCount = val;
    label maxCount = 1;

    for (label i=1; i<orderedTbes.size(); i++)
    {
        if (equal(orderedTbes[i], val))
        {
            count++;
        }
        else
        {
            if (count > maxCount)
            {
                maxCount = count;
                valMaxCount = val;
            }

            count = 1;
            val = orderedTbes[i];
        }
    }

    if (count > maxCount)
    {
        maxCount = count;
        valMaxCount = val;
    }

    os << token::BEGIN_LIST << valMaxCount;

    forAll (tbes, i)
    {
        if (notEqual(tbes[i], valMaxCount))
        {
            os  << token::SPACE << tbes.species_[i]
                << token::SPACE << tbes[i];
        }
    }

    os << token::END_LIST;

    return os;
}


// ************************************************************************* //
