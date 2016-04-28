#include "prelesglobals.h"

/* Seasonality model of Mäkelä et al 2004 */
double fS_model(double *S, double T, p2 GPP_par) {
  double fS; 
  
  *S = *S + (T-*S)/GPP_par.tau;
  if (0 > *S-GPP_par.S0) fS=0; else fS= *S-GPP_par.S0;
  if (1 < fS/GPP_par.Smax) fS=1; else fS=fS/GPP_par.Smax;
  
  return(fS);
};


double fPheno_model(p2 GPP_par, double T, double *PhenoS, 
		    int DOY, double fS) {
  double m; 
  double fPheno=0;

  if (GPP_par.t0 > -998) { // ie not -999 
    /* Budbreak must occur between specified min. date and end of July */
    if ( (DOY > (GPP_par.t0 - 0.5)) & (DOY < 213) )  {
      m = (T - GPP_par.tcrit);
      if (m < 0) m = 0;
      *PhenoS = *PhenoS + m ;
    } else {
      *PhenoS = 0;
    }
    
    if (*PhenoS > GPP_par.tsumcrit - 0.005) fPheno = 1; else fPheno = 0;
    /* Quick solution to leaf out: 
     * After end of July we just apply season prediction based on conifer fS 
     *  for gradual leaf out. Assume leaves drop much faster that fS.
     *  ...essentially this should be light driven process...i think. */
    if (DOY > 212) {
	fPheno = fS * fS;
      if (fPheno < 0.5) {
	fPheno = 0;
      } 
    }
    
    /* If there is no t0 parameter, it is an evergreen */
  } else {
    fPheno = 1;
  }
  
  return(fPheno);
};

/* *****************************************************************/
/* f-modifiers for increasing CO2 prepared by P. Kolari, pers. comm.*/
/*double fCO2_model_mean(double CO2, p2 GPP_par ) {
  return(1 + (CO2-380)/(CO2-380+GPP_par.bCO2));
}
double fCO2_VPD_exponent(double CO2, p2 GPP_par ) {
  return(pow(CO2/380, GPP_par.xCO2));
}
*/
/*
double fCO2_model_mean(double CO2, double b ) {
  return(1 + (CO2-380)/(CO2-380+b));
}
double fCO2_VPD_exponent(double CO2, double xCO2 ) {
  return(pow(380/CO2, xCO2));
}
*/

/* Note: 'ET_par.bC02' is the same as GPP_par.bCO2 */
/*
double fCO2_ET_model_mean(double CO2, p2 GPP_par ) {
  return(1 - 1.95*(CO2-380)/(CO2-380+(GPP_par.bCO2)));
}
*/
/* *****************************************************************/
/* New CO2 modifiers based on APES simulator (Launiainen et al.)
   which account for the energy balance of the forest. Fitted responses
   to model predicted
   bCO2 = 0.5; xCO2 = -0.364
*/
double fCO2_model_mean(double CO2, p2 GPP_par ) {
  return(1 + GPP_par.bCO2 * log(CO2/380) );
}
double fCO2_ET_model_mean(double CO2, p2 GPP_par ) {
  return(1 + GPP_par.xCO2 * log(CO2/380) );
}



/* GPP model, modified from Mäkelä et al 2008 */
void GPPfun(double *gpp, double *gpp380, 
	      double ppfd,  double D, double CO2, double theta, 
	      double fAPAR, double fSsub, 
              p2 GPP_par, p1 Site_par, double *fD, double *fW,
	      double *fE, FILE *flog, int LOGFLAG) {

    extern double fCO2_model_mean(double CO2, p2 b ) ;
    //    extern double fCO2_VPD_exponent(double CO2, double xCO2 ) ;
    double thetavol = theta/Site_par.soildepth;
    //  double GPPsub, GPP380sub;
    double fCO2;
    double REW=(thetavol-Site_par.ThetaPWP)/
        (Site_par.ThetaFC-Site_par.ThetaPWP);
    double fEsub, fWsub, fLsub, fDsub;
    // double fECO2sub, fDCO2sub, fWCO2sub;

    /* Calculate first the reference condition (ca=380 ppm) effect */
    fDsub = exp(GPP_par.kappa * D);
    fDsub = fDsub > 1 ? 1 : fDsub;

    if (GPP_par.soilthres < -998) { 
      fWsub = 1.0;      /* e.g. -999 means no water control of GPP*/
    } else {
      if (REW < GPP_par.soilthres) {
        if (REW > 0.01) fWsub = REW/GPP_par.soilthres; else fWsub = 0.0;
      } else {
        fWsub = 1.0;
      }
    }

    fLsub = 1 / (GPP_par.gamma * ppfd + 1);

    if (fDsub > fWsub) fEsub=fWsub; else fEsub = fDsub;
    *fW = fWsub;
    *fD = fEsub;

    *gpp380 = GPP_par.beta *  ppfd *  fAPAR * fSsub * fLsub * fEsub;
    fCO2 = fCO2_model_mean(CO2, GPP_par);
    *gpp = *gpp380 * fCO2;

    
    if (LOGFLAG > 1.5) 
      fprintf(flog, 
	      "   gpp(): Modifiers: fAPAR %lf\tfSsub %lf\t fLsub %lf\t fDsub %lf\t fWsub %lf\tfEsub %lf\t fCO2 %lf\n                    gpp380 %lf\t gpp %lf\n",
	      fAPAR, fSsub, fLsub, fDsub, fWsub, fEsub, fCO2, *gpp380, *gpp);



    /* This has been removed, and simpler multiplicative CO2 modifier to gpp380 is used.
    * CO2 effect not only influences fD but also fW, due to stomatal action
    
    fDCO2sub = fDsub * pow(exp(-0.4 * D),
			   fCO2_VPD_exponent(CO2, GPP_par.xCO2)) / exp(-0.4 * D) ;
    fWCO2sub = fWsub * pow(fWsub, fCO2_VPD_exponent(CO2, GPP_par.xCO2));

    if (LOGFLAG > 1.5) 
      fprintf(flog, 
	      "   gpp(): Modifier values for GPP at %lf\n      fD=%lf\tfW=%lf\tfCO2mean=%lf\n", 
	      CO2, fDCO2sub, fWCO2sub, fCO2_model_mean(CO2, GPP_par.bCO2));
    
    if (fDCO2sub > fWCO2sub) fECO2sub=fWCO2sub; else fECO2sub = fDCO2sub;
    
    *fECO2 = fECO2sub;
    
    *gpp = GPP_par.beta *  ppfd *  fAPAR * fSsub * fLsub  * fECO2sub * 
      fCO2_model_mean(CO2, GPP_par.bCO2);
*/

}
