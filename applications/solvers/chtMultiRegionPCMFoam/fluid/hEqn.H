//for (int i=0; i<10; i++ ) // added
{

    volScalarField gammaT ("gammaT", hmFluid[i]*4.0*exp(-pow(4.0*(thermoFluid[i].T()-TmFluid[i])/TmrFluid[i],2))/Foam::sqrt(mathematicalConstant::pi)/TmrFluid[i]); //added

    fvScalarMatrix hEqn
    (
        fvm::ddt(rho, h)
      + fvm::div(phi, h)
      - fvm::laplacian(turb.alphaEff(), h)
      + (1/thermoFluid[i].Cp())*gammaT*fvm::ddt(rho, h) //added
      + (rho/thermoFluid[i].Cp())*gammaT*(U & fvc::grad(h)) //added
     ==
        DpDt
    );
    if (oCorr == nOuterCorr-1)
    {
        hEqn.relax();
        hEqn.solve(mesh.solutionDict().solver("hFinal"));
    }
    else
    {
        hEqn.relax();
        hEqn.solve();
    }

    thermo.correct();
    gammaFluid[i] = 0.5 * erf( 4.0 * ( thermoFluid[i].T() - TmFluid[i] ) / TmrFluid[i] ) + scalar(0.5); //added

    Info<< "Min/max T:" << min(thermo.T()).value() << ' '
        << max(thermo.T()).value() << endl;
}
