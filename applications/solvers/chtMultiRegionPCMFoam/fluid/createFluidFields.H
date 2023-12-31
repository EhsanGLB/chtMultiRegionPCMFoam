    // Initialise fluid field pointer lists
    PtrList<basicPsiThermo> thermoFluid(fluidRegions.size());
    PtrList<volScalarField> rhoFluid(fluidRegions.size());
    PtrList<volScalarField> KappaFluid(fluidRegions.size());
    PtrList<volVectorField> UFluid(fluidRegions.size());
    PtrList<surfaceScalarField> phiFluid(fluidRegions.size());
    PtrList<uniformDimensionedVectorField> gFluid(fluidRegions.size());
    PtrList<compressible::turbulenceModel> turbulence(fluidRegions.size());
    PtrList<volScalarField> DpDtFluid(fluidRegions.size());

    scalarList initialMassFluid(fluidRegions.size());

    // Populate fluid field pointer lists
    forAll(fluidRegions, i)
    {
        Info<< "*** Reading fluid mesh thermophysical properties for region "
            << fluidRegions[i].name() << nl << endl;

        Info<< "    Adding to thermoFluid\n" << endl;
        thermoFluid.set
        (
            i,
            basicPsiThermo::New(fluidRegions[i]).ptr()
        );

        Info<< "    Adding to rhoFluid\n" << endl;
        rhoFluid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "rho",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                thermoFluid[i].rho()
            )
        );

        Info<< "    Adding to KappaFluid\n" << endl;
        KappaFluid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "Kappa",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                thermoFluid[i].Cp()*thermoFluid[i].alpha()
            )
        );

        Info<< "    Adding to UFluid\n" << endl;
        UFluid.set
        (
            i,
            new volVectorField
            (
                IOobject
                (
                    "U",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                fluidRegions[i]
            )
        );

        Info<< "    Adding to phiFluid\n" << endl;
        phiFluid.set
        (
            i,
            new surfaceScalarField
            (
                IOobject
                (
                    "phi",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                linearInterpolate(rhoFluid[i]*UFluid[i])
                    & fluidRegions[i].Sf()
            )
        );

        Info<< "    Adding to gFluid\n" << endl;
        gFluid.set
        (
            i,
            new uniformDimensionedVectorField
            (
                IOobject
                (
                    "g",
                    runTime.constant(),
                    fluidRegions[i],
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            )
        );

        Info<< "    Adding to turbulence\n" << endl;
        turbulence.set
        (
            i,
            autoPtr<compressible::turbulenceModel>
            (
                compressible::turbulenceModel::New
                (
                    rhoFluid[i],
                    UFluid[i],
                    phiFluid[i],
                    thermoFluid[i]
                )
            ).ptr()
        );

        Info<< "    Adding to DpDtFluid\n" << endl;
        DpDtFluid.set
        (
            i,
            new volScalarField
            (
                "DpDt",
                fvc::DDt
                (
                    surfaceScalarField
                    (
                        "phiU",
                        phiFluid[i]/fvc::interpolate(rhoFluid[i])
                    ),
                    thermoFluid[i].p()
                )
            )
        );

        initialMassFluid[i] = fvc::domainIntegrate(rhoFluid[i]).value();
    }


// added
    PtrList<IOdictionary> phaseChangePropertiesFluid(fluidRegions.size());
    PtrList<dimensionedScalar> hmFluid(fluidRegions.size());
    PtrList<dimensionedScalar> TmFluid(fluidRegions.size());
    PtrList<dimensionedScalar> TmrFluid(fluidRegions.size());
    PtrList<dimensionedScalar> TRefFluid(fluidRegions.size());
    PtrList<dimensionedScalar> CuFluid(fluidRegions.size());
    PtrList<dimensionedScalar> bFluid(fluidRegions.size());

    PtrList<volScalarField> gammaFluid(fluidRegions.size());


    forAll(fluidRegions, i)
    {
        Info<< "    Adding to phaseChangePropertiesFluid\n" << endl;
        phaseChangePropertiesFluid.set
        (
            i,
            new IOdictionary
            (
                IOobject
                (
                    "phaseChangeProperties",
                    runTime.constant(),
                    fluidRegions[i],
                    IOobject::MUST_READ_IF_MODIFIED,
                    IOobject::NO_WRITE
                )
            )
        );

        Info<< "    Adding to gammaFluid\n" << endl;
        gammaFluid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "gamma",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                fluidRegions[i],
                dimensionedScalar("gamma", dimless, SMALL)
            )
        );

        hmFluid.set
        (
            i,
            new dimensionedScalar (phaseChangePropertiesFluid[i].lookup("hm"))
        );

        TmFluid.set
        (
            i,
            new dimensionedScalar (phaseChangePropertiesFluid[i].lookup("Tm"))
        );

        TmrFluid.set
        (
            i,
            new dimensionedScalar (phaseChangePropertiesFluid[i].lookup("Tmr"))
        );

        TRefFluid.set
        (
            i,
            new dimensionedScalar (phaseChangePropertiesFluid[i].lookup("TRef"))
        );

        CuFluid.set
        (
            i,
            new dimensionedScalar (phaseChangePropertiesFluid[i].lookup("Cu"))
        );

        bFluid.set
        (
            i,
            new dimensionedScalar (phaseChangePropertiesFluid[i].lookup("b"))
        );


    }



