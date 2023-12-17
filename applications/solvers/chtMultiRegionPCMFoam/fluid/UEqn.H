    // Solve the Momentum equation
    volScalarField gammaV ("gammaV", CuFluid[i] * pow( ( 1 - gamma ) , 2 ) / ( pow( gamma , 3 ) + bFluid[i])); //added
/*
Info << "thermoFluid[i].rho(): " << max(thermoFluid[i].rho()) << endl;
Info << "thermoFluid[i].Kappa(): " << max(thermoFluid[i].Cp()*thermoFluid[i].alpha()) << endl;//alpha=kappa/cp
Info << "thermoFluid[i].Cp(): " << max(thermoFluid[i].Cp()) << endl;
Info << "thermoFluid[i].alpha(): " << max(thermoFluid[i].alpha()) << endl;
Info << "thermoFluid[i].mu(): " << max(thermoFluid[i].mu()) << endl;
*/
    tmp<fvVectorMatrix> UEqn
    (
        fvm::ddt(rho, U)
      + fvm::div(phi, U)
      + turb.divDevRhoReff()
      + fvm::SuSp(gammaV, U) //added
    );

    UEqn().relax();

    if (momentumPredictor)
    {
         solve
         (
            UEqn()
         ==
            fvc::reconstruct
            (
                fvc::interpolate(rho)*(g & mesh.Sf())
              - fvc::snGrad(p)*mesh.magSf()
            )
        );
    }
