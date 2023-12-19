# chtMultiRegionPCMFoam
This is a solver for solid-liquid phase change material written based on foam-extend-4.1.


## Mathematical Relationships
 $$ \nabla . U = 0 $$

$$ {dU \over dt}+ {(U . \nabla) U} = - {1 \over\rho} \nabla p + \nu \nabla . {\nabla U} + \beta g (T - T_ref) - {Cu \over \rho} {{(1 - \lambda)^2} \over {\lambda^3 + 10^{-3}}} U $$

$$ \rho \left({C_p + h{d\lambda \over dT} }\right) {dT \over dt} + \rho C_p {U . \nabla T} = k {\nabla}^2 T $$

$$ \lambda = 0.5 \ erf \left({4 { {T_s - T_m} \over {T_l - T_s} }}\right) + 0.5 $$

Which $U$, $p$, $T$, $t$, $g$, and $\lambda$ are velocity vector, pressure, temperature, time, gravitational acceleration, and melting fraction, respectively.
And $\rho$, $\nu$, $\beta$, $C_p$, $h$, $k$, $T_m$, $T_s$, and $T_l$  are density, kinematic viscosity, thermal expansion coefficient, specific heat capacity, melting enthalpy, thermal conductivity, melting temperature, solidification temperature, and liquidification temperature, respectively.


## Installation
It is working on foam-extend-4.1
```bash
git clone https://github.com/EhsanGLB/chtMultiRegionPCMFoam.git
cd chtMultiRegionPCMFoam
Allwmake
```


## Getting Started
```bash
./Allrun
```


## References
* [Golab, Ehsan, Behzad Vahedi, Ankur Jain, Robert A. Taylor, and Kambiz Vafai. "Laminar forced convection in a tube with a nano-encapsulated phase change materials: Minimizing exergy losses and maximizing the heat transfer rate." Journal of Energy Storage 65 (2023): 107233.](https://www.sciencedirect.com/science/article/abs/pii/S2352152X23006308)
* [Vahedi, Behzad, Ehsan Golab, Arsalan Nasiri Sadr, and Kambiz Vafai. "Thermal, thermodynamic and exergoeconomic investigation of a parabolic trough collector utilizing nanofluids." Applied Thermal Engineering 206 (2022): 118117.](https://www.sciencedirect.com/science/article/abs/pii/S1359431122000813)
* [Golab, Ehsan, Sahar Goudarzi, Hamed Kazemi-Varnamkhasti, Hossein Amigh, Ferial Ghaemi, Dumitru Baleanu, and Arash Karimipour. "Investigation of the effect of adding nano-encapsulated phase change material to water in natural convection inside a rectangular cavity." Journal of Energy Storage 40 (2021): 102699.](https://www.sciencedirect.com/science/article/abs/pii/S2352152X21004357)
* [Abbasi, Mohammad, Amin Nadimian Esfahani, Ehsan Golab, Omid Golestanian, Nima Ashouri, S. Mohammad Sajadi, Ferial Ghaemi, Dumitru Baleanu, and A. Karimipour. "Effects of Brownian motions and thermophoresis diffusions on the hematocrit and LDL concentration/diameter of pulsatile non-Newtonian blood in abdominal aortic aneurysm." Journal of Non-Newtonian Fluid Mechanics 294 (2021): 104576.](https://www.sciencedirect.com/science/article/abs/pii/S0377025721000859)
* [Jafarzadeh, Sina, Arsalan Nasiri Sadr, Ehsan Kaffash, Sahar Goudarzi, Ehsan Golab, and Arash Karimipour. "The effect of hematocrit and nanoparticles diameter on hemodynamic parameters and drug delivery in abdominal aortic aneurysm with consideration of blood pulsatile flow." Computer Methods and Programs in Biomedicine 195 (2020): 105545.](https://www.sciencedirect.com/science/article/abs/pii/S0169260720307914)
* [Goudarzi, Sahar, Masih Shekaramiz, Alireza Omidvar, Ehsan Golab, Arash Karimipour, and Aliakbar Karimipour. "Nanoparticles migration due to thermophoresis and Brownian motion and its impact on Ag-MgO/Water hybrid nanofluid natural convection." Powder Technology 375 (2020): 493-503.](https://www.sciencedirect.com/science/article/abs/pii/S0032591020307397)
* [Motlagh, Saber Yekani, Ehsan Golab, and Arsalan Nasiri Sadr. "Two-phase modeling of the free convection of nanofluid inside the inclined porous semi-annulus enclosure." International Journal of Mechanical Sciences 164 (2019): 105183.](https://www.sciencedirect.com/science/article/abs/pii/S0020740319315279)




