## Author: F. Minunno

library(Rpreles)
## Dummy run, three days
PRELES( PAR = c(30, 40, 60),  TAir = c(10, 10, 10),  DOY = 1:3,  VPD = c(1, 1, 1),
        Precip = c(1, 1, 1),  CO2 = rep(380, 3),  fAPAR = c(0.8, 0.8, 0.8),  control = 1)
## $GPP
## [1] 5.707933 6.514932 7.587695
## 
## $ET
## [1] 2.149772 2.501831 3.025874
## 
## $SW
## [1] 178.8502 177.3484 175.3225


## Run PRELES model for a Boreal sites, a fake boreal site
data('boreal_site.rdata')
## Minunno's posteriori parameter set:
data('parameters.rdata')

GPPsim <- ETsim <- matrix(NA, nrow(pSample) + 1, length(s1$PAR))
preles_MAP <-  PRELES(PAR = s1$PAR, TAir = s1$TAir, DOY = s1$DOY, VPD = s1$VPD,
                      Precip = s1$Precip, CO2 = s1$CO2, fAPAR = s1$fAPAR, p = psetMAP)
GPPsim[1, ] <- preles_MAP$GPP
ETsim[1, ] <- preles_MAP$ET

for (i in 1:1000) {
  preles_out <- PRELES(PAR = s1$PAR, TAir = s1$TAir, VPD = s1$VPD,  Precip = s1$Precip,
                       DOY = s1$DOY, CO2 = s1$CO2,  fAPAR = s1$fAPAR, p = pSample[i, ])
  GPPsim[(i + 1), ] <- preles_out$GPP
  ETsim[(i + 1), ] <- preles_out$ET
}

## Plot of GPP over time: model prediction incl. error
par(mfrow = c(2, 1))
plot(GPPsim[1, ], col = 2, xlab = '',  ylab = 'tC ha-1',  main = 'GPP',  type = "n",ylim=c(0,12))
for (i in 1:nrow(GPPsim)) {
  lines(GPPsim[i,] , col = rgb(0.8, 0.8, 0, 0.05))
}
lines(GPPsim[1,], col = rgb(0.8, 0.8, 0., 0.5))
plot( ETsim[1, ],  col = 2,  xlab = '',  ylab = 'mm ha-1',  main = 'ET',  type = "n",ylim=c(0,5))
for (i in 1:nrow(ETsim)) {
  lines(ETsim[i,] , col = rgb(0., 0.8, 0.5, 0.05))
}
lines(ETsim[1,], col = rgb(0., 0.8, 0.5, 0.5))
