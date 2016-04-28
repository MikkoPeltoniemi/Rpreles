# Rpreles - R library for PRELES model

PRELES is a simple semi-empirical model of CO2 and H20 fluxes over boreal forests. It is intended for estimating daily fluxes of CO2 and H20 and soil moisture with minimal input data requirements. Daily sum of PPFD (mol m-2), mean temperature (deg C), mean VPD (kPA), and rainfall (mm), and [CO2] (ppm) are the required meteoroligical inputs. Additionally, the model requires lumped fAPAR to describe absorption of PPFD by forest vegetation, and for soil rooting depth, field capacity and wilting point.
Model has been described in detail in Peltoniemi et al. (2015), but has since been modified for CO2-modifiers. 

To use the model, call R function PRELES(), which further calls C functions. If parameter list is not modified, the model uses a set calibrated to 10 conifer sites in Scandinavia (re-parameterization based on Minunno et al., manuscript).

Citations: 
Peltoniemi M., Pulkkinen M., Aurela M., Pumpanen J., Kolari P. & Mäkelä A. 2015: A semi-empirical model of boreal-forest gross primary production, evapotranspiration, and soil water — calibration and sensitivity analysis. Boreal Env. Res. 20: 151–171.
