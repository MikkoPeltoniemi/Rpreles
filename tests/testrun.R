library(Rpreles)
data(hydata)
hydata[is.na(hydata)] <- -999

daterange = 1:365
DOY <- daterange
n = length(daterange)

CO2 <- rep(376, len=n)
T=hydata[daterange, 'TAir']
Precip=hydata[daterange, 'Precip']
PAR=hydata[daterange, 'PPFD']
## Fake VPD
D=sin(pi*(1:n)/n) + 0.01
D = tapply(D, 1:n, function(x) runif(1, x/4, x*1.5))
fAPAR=rep(0.765, len=n)

## CONIFER EXAMPLE
## This is the default set for conifers, based on calibration to 10 sites
defaults = c(413, 0.45, 0.118, 3, 0.7457, 10.93, -3.063,
            17.72, -0.1027, 0.03673, 0.7779, 0.5, -0.364, 0.2715,
            0.8351, 0.07348, 0.9996, 0.4428, 1.2, 0.33, 4.970496,
            0, 0, 160, 0, 0, 0, -999, -999, -999)

o1 <- PRELES(PAR, T, D, Precip, CO2, fAPAR, DOY=DOY, p=defaults,
             returncols=c("GPP", "ET",  "fS", "fW", "SW",
                      "Canopywater", "SOG", "S"), LOGFLAG=0)

## DECIDUOUS EXAMPLE (birch)
decid <- defaults
decid[1:3] <- c(700, 0.45, 0.05) # Soil usually moister 
decid[5] <- 0.94 # LUE usually higher (estimated based on linear [N]-LUE relation)
decid[9] <- -0.4 # But kappa for VPD more negative, meaning more responsive to VPD
decid[24:27] <- c(decid[1]*(decid[2]-decid[3]), 0, 0, 0) #SW; CW; SOG; S
decid[28:30] <- c(57, 1.5, 134) # Phenol. mod. (Linkosalo et al. 2008) modifies fAPAR (0/1)

o2 <- PRELES(PAR, T,  D, Precip, CO2, fAPAR, DOY=DOY, p=decid,
               returncols=c("GPP", "ET",  "fS", "fW", "SW",
                      "Canopywater", "SOG", "S"), pft="notconifer",
             LOGFLAG=0)

measuredGPP = hydata$GPP[daterange]
measuredGPP[measuredGPP < -998] <- NA
plot(DOY, measuredGPP)
points(DOY, o1$GPP, col=2)
points(DOY, o2$GPP, col=3)
