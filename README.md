# Rpreles - R library for PRELES model

PRELES is a simple semi-empirical model of CO2 and H20 fluxes over boreal forests. It is intended for estimating daily fluxes of CO2 and H20 and soil moisture with minimal input data requirements. Daily sum of PPFD (mol m-2), mean temperature (deg C), mean VPD (kPA), and rainfall (mm), and CO2 concentration (ppm) are the required meteoroligical inputs. Additionally, the model requires lumped fAPAR to describe absorption of PPFD by forest vegetation, and for soil rooting depth, field capacity and wilting point.
Model has been described in detail in Peltoniemi et al. (2015a), but has since been modified for CO2-modifiers. 

The model can be installed directly from R using devtools::install_github('MikkoPeltoniemi/Rpreles').

To use the model, call R function PRELES(), which further calls C functions. If parameter list is not modified, the model uses a set calibrated to 10 conifer sites in Scandinavia (re-parameterization based on Minunno et al., manuscript). See also the branch MikkoPeltoniemi/Rpreles/ecolmod-version used in Minunno et al [devtools::install_github('MikkoPeltoniemi/Rpreles', ref="ecolmod-version")].

Citations: 
Peltoniemi M., Pulkkinen M., Aurela M., Pumpanen J., Kolari P. & Mäkelä A. 2015a: A semi-empirical model of boreal-forest gross primary production, evapotranspiration, and soil water — calibration and sensitivity analysis. Boreal Env. Res. 20: 151–171. [http://www.borenv.net/BER/pdfs/ber20/ber20-151.pdf]

Peltoniemi M., Markkanen T., Härkönen S., Muukkonen P., Thum T., Aalto T. & Mäkelä A. 2015b: Consistent estimates of gross primary production of Finnish forests — comparison of estimates of two process models. Boreal Env. Res. 20: 196–212. [http://www.borenv.net/BER/pdfs/ber20/ber20-196.pdf]

Minunno F., Peltoniemi M., Launiainen S., Aurela M., Lindroth A., Lohila A., Mammarella I., Minkkinen K., Mäkelä A. (Manuscript), Calibration and validation of a simple flux ecosystem model for coniferous forests in the Boreal region.


