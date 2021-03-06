\encoding{UTF-8}
\name{exampleobs}
\Rdversion{1.1}
\alias{exampleobs}
\docType{data}
\title{Example observed data}
\description{
  Example (fake) data set of variables measured in a plot. 
}
\usage{data(exampleobs)}
\format{
 A data frame containing daily 'observed' values for year 2001.
   \describe{
\item{\code{SWC}}{Soil moisture content (in m3/m3).}
\item{\code{ETR}}{Total evapotranspiration (mm).}
\item{\code{E_T1_149}}{Transpiration of Pinus halepensis cohort 'T1_149' (L/m2 of leaf area).}
\item{\code{E_T2_169}}{Transpiration of Quercus ilex cohort 'T2_169' (L/m2 of leaf area).}
\item{\code{FMC_T1_149}}{Fuel moisture content of Pinus halepensis cohort 'T1_149'  (in percent).}
\item{\code{FMC_T2_169}}{Fuel moisture content of Quercus ilex cohort 'T2_169' (in percent).}
\item{\code{BAI_T1_149}}{Basal area increment for Pinus halepensis cohort 'T1_149'  (in cm2).}
\item{\code{BAI_T2_169}}{Basal area increment for Quercus ilex cohort 'T2_169' (in cm2).}
  }
}
\source{
This data set was actually created by running a simulation and adding some gaussian error to the outputs.
}
\seealso{ \code{\link{evaluation}}}
\examples{
data(exampleobs)
}
\keyword{datasets}
