\encoding{UTF-8}
\name{transp_stomatalregulation}
\alias{transp_profitMaximization}
\alias{transp_stomatalRegulationPlot}
\title{
Stomatal regulation
}
\description{
Set of high-level functions used in the calculation of stomatal conductance and transpiration. Function \code{transp_profitMaximization} calculates gain and cost functions, as well as profit maximization from supply and photosynthesis input functions. Function \code{transp_stomatalRegulationPlot} produces a plot with the cohort supply functions against water potential and a plot with the cohort photosynthesis functions against water potential, both with the maximum profit values indicated.
}
\usage{
transp_profitMaximization(supplyFunction, photosynthesisFunction, Gswmin, Gswmax, 
                          gainModifier = 1.0, costModifier = 1.0, costWater = "dEdP")
transp_stomatalRegulationPlot(x, meteo, day, timestep, 
                              latitude, elevation, slope = NA, aspect = NA,
                              type="E")
}
\arguments{
  \item{supplyFunction}{Water supply function (see \code{\link{hydraulics_supplyFunctionNetwork}}).}
  \item{photosynthesisFunction}{Function returned by \code{photo_photosynthesisFunction()}.}
  \item{Gswmin, Gswmax}{Minimum and maximum stomatal conductance to water vapour (mol·m-2·s-1).}
  \item{gainModifier, costModifier}{Modifiers (exponents) of the gain and cost functions defined in Sperry et al. (2017).}
  \item{costWater}{Cost water function, either "dEdP" (default) or "kterm" (experimental).}
  \item{x}{An object of class \code{\link{spwbInput}} built using the 'Sperry' transpiration mode.}
  \item{meteo}{A data frame with daily meteorological data series:
   \itemize{
     \item{\code{DOY}: Day of the year (Julian day).}
     \item{\code{Precipitation}: Precipitation (in mm).}
     \item{\code{MeanTemperature}: Mean temperature (in degrees Celsius).}
     \item{\code{MinTemperature}: Minimum temperature (in degrees Celsius).}
     \item{\code{MaxTemperature}: Maximum temperature (in degrees Celsius).}
     \item{\code{MinRelativeHumidity}: Minimum relative humidity (in percent).}
     \item{\code{MaxRelativeHumidity}: Maximum relative humidity (in percent).}
     \item{\code{Radiation}: Solar radiation (in MJ/m2/day).}
     \item{\code{WindSpeed}: Wind speed (in m/s). If not available, this column can be left with \code{NA} values.}
    }
  }  
  \item{day}{An integer to identify a day within \code{meteo}.}
  \item{timestep}{An integer between 1 and \code{ndailysteps} specified in \code{x} (see \code{\link{defaultControl}}).}
  \item{latitude}{Latitude (in degrees).}
  \item{elevation, slope, aspect}{Elevation above sea level (in m), slope (in degrees) and aspect (in degrees from North).} 
  \item{type}{A string with plot type, either \code{"E"} (transpiration flow), \code{"Ag"} (gross photosynthesis), \code{"An"} (net photosynthesis), \code{"Gsw"} (stomatal conductance to water vapour), \code{"T"} (temperature) or \code{"VPD"} (leaf vapour pressure deficit).}
}

\value{
Function \code{transp_profitMaximization} returns a list with the following elements:
\itemize{
  \item{\code{Cost}: Cost function [0-1].}
  \item{\code{Gain}: Gain function [0-1].}
  \item{\code{Profit}: Profit function [0-1].}
  \item{\code{iMaxProfit}: Index corresponding to maximum profit (starting from 0).}
}

}
\references{
Sperry, J. S., M. D. Venturas, W. R. L. Anderegg, M. Mencuccini, D. S. Mackay, Y. Wang, and D. M. Love. 2017. Predicting stomatal responses to the environment from the optimization of photosynthetic gain and hydraulic cost. Plant Cell and Environment 40, 816-830 (doi: 10.1111/pce.12852).
}
\author{
Miquel De \enc{Cáceres}{Caceres} Ainsa, CREAF
}
\seealso{
\code{\link{transp_transpirationSperry}}, \code{\link{hydraulics_supplyFunctionNetwork}}, \code{\link{biophysics_leafTemperature}}, \code{\link{photo_photosynthesis}}, \code{\link{spwb_day}}, \code{\link{plot.spwb_day}}
}
\examples{
#Load example daily meteorological data
data(examplemeteo)

#Load example plot plant data
data(exampleforestMED)

#Default species parameterization
data(SpParamsMED)


#Initialize soil with default soil params (2 layers)
examplesoil = soil(defaultSoilParams(2))

#Initialize control parameters
control = defaultControl(transpirationMode="Sperry")

#Initialize soil with default soil params (2 layers)
examplesoil2 = soil(defaultSoilParams(2))

#Initialize input
x2 = forest2spwbInput(exampleforestMED,examplesoil2, SpParamsMED, control)

# Stomatal VPD curve and chosen value for the 12th time step at day 100
transp_stomatalRegulationPlot(x2, examplemeteo, day=100, timestep = 12,
                              latitude = 41.82592, elevation = 100, type="VPD")
}
