#include <numeric>
#include "lightextinction.h"
#include "windextinction.h"
#include "hydraulics.h"
#include "biophysicsutils.h"
#include "phenology.h"
#include "forestutils.h"
#include "photosynthesis.h"
#include "soil.h"
#include <Rcpp.h>
#include <meteoland.h>
using namespace Rcpp;

List profitMaximization1(List supplyFunction, List photosynthesisFunction) {
  NumericVector supplydEdp = supplyFunction["dEdP"];
  NumericVector Ag = photosynthesisFunction["Photosynthesis"];
  // NumericVector Gw = photosynthesisFunction["WaterVaporConductance"];
  int nsteps = supplydEdp.size();
  NumericVector profit(nsteps);
  NumericVector cost(nsteps);
  NumericVector gain(nsteps);
  double maxdEdp = 0.0, mindEdp = 99999999.0;
  double Agmax = 0.0;
  for(int i=0;i<nsteps;i++) {
    maxdEdp = std::max(maxdEdp, supplydEdp[i]);
    mindEdp =  std::min(mindEdp, supplydEdp[i]);
    Agmax = std::max(Agmax, Ag[i]);
  }
  for(int i=0;i<nsteps;i++) {
    cost[i] = (maxdEdp-supplydEdp[i])/(maxdEdp-mindEdp);
    gain[i] = Ag[i]/Agmax;
    profit[i] = gain[i]-cost[i];
  }
  
  int imaxprofit=0;
  double maxprofit=profit[0];
  //Searches the step corresponding to minimum (i.e. cuticular) conductance
  // for(int i=0;i<nsteps;i++){
  //   if(Gw[i]>Gwmin) {
  //     maxprofit = profit[i];
  //     imaxprofit = i;
  //     break;
  //   }
  // }
  //Searches for maximum profit (up to Gwmax)
  // if(imaxprofit>-1) {
  for(int i=(imaxprofit+1);i<nsteps;i++){
    if((profit[i]>maxprofit)) {
      maxprofit = profit[i];
      imaxprofit = i;
    }
  }
  // } else {
  // imaxprofit = 0;
  // }
  return(List::create(Named("Cost") = cost,
                      Named("Gain") = gain,
                      Named("Profit") = profit,
                      Named("iMaxProfit")=imaxprofit));
}

List profitMaximization2(List supplyFunction, List photosynthesisFunction, double kstemmax) {
  NumericVector supplyKterm = supplyFunction["kterm"];
  NumericVector Ag = photosynthesisFunction["Photosynthesis"];
  // NumericVector Gw = photosynthesisFunction["WaterVaporConductance"];
  int nsteps = supplyKterm.size();
  NumericVector profit(nsteps);
  NumericVector cost(nsteps);
  NumericVector gain(nsteps);
  double minKterm = 99999999.0;
  double Agmax = 0.0;
  for(int i=0;i<nsteps;i++) {
    minKterm =  std::min(minKterm, supplyKterm[i]);
    Agmax = std::max(Agmax, Ag[i]);
  }
  for(int i=0;i<nsteps;i++) {
    cost[i] = (kstemmax-supplyKterm[i])/(kstemmax-0.0);
    gain[i] = Ag[i]/Agmax;
    profit[i] = gain[i]-cost[i];
  }
  
  int imaxprofit=0;
  double maxprofit=profit[0];
  // Searches the step corresponding to minimum (i.e. cuticular) conductance
  // for(int i=0;i<nsteps;i++){
  //   if(Gw[i]>Gwmin) {
  //     maxprofit = profit[i];
  //     imaxprofit = i;
  //     break;
  //   }
  // }
  //Searches for maximum profit (up to Gwmax)
  // if(imaxprofit>-1) {
  for(int i=imaxprofit+1;i<nsteps;i++){
    if((profit[i]>maxprofit)) {
      // if((profit[i]>maxprofit) & (Gw[i]<Gwmax)) {
      maxprofit = profit[i];
      imaxprofit = i;
    }
  }
  // } else {
  //   imaxprofit = 0;
  // }
  return(List::create(Named("Cost") = cost,
                      Named("Gain") = gain,
                      Named("Profit") = profit,
                      Named("iMaxProfit")=imaxprofit));
}

List profitMaximization3(List supplyFunction, List photosynthesisFunction, double kstemmax) {
  NumericVector supplyKterm = supplyFunction["ktermcav"];
  NumericVector Ag = photosynthesisFunction["Photosynthesis"];
  // NumericVector Gw = photosynthesisFunction["WaterVaporConductance"];
  int nsteps = supplyKterm.size();
  NumericVector profit(nsteps);
  NumericVector cost(nsteps);
  NumericVector gain(nsteps);
  double minKterm = 99999999.0;
  double Agmax = 0.0;
  for(int i=0;i<nsteps;i++) {
    minKterm =  std::min(minKterm, supplyKterm[i]);
    Agmax = std::max(Agmax, Ag[i]);
  }
  for(int i=0;i<nsteps;i++) {
    cost[i] = (kstemmax-supplyKterm[i])/(kstemmax-0.0);
    gain[i] = Ag[i]/Agmax;
    profit[i] = gain[i]-cost[i];
  }
  
  int imaxprofit=0;
  double maxprofit=profit[0];
  //Searches the step corresponding to minimum (i.e. cuticular) conductance
  // for(int i=0;i<nsteps;i++){
  //   if(Gw[i]>Gwmin) {
  //     maxprofit = profit[i];
  //     imaxprofit = i;
  //     break;
  //   }
  // }
  //Searches for maximum profit (up to Gwmax)
  // if(imaxprofit>-1) {
  for(int i=(imaxprofit+1);i<nsteps;i++){
    // if((profit[i]>maxprofit) & (Gw[i]<Gwmax)) {
    if((profit[i]>maxprofit)) {
      maxprofit = profit[i];
      imaxprofit = i;
    }
  }
  // } else {
  //   imaxprofit = 0;
  // }
  return(List::create(Named("Cost") = cost,
                      Named("Gain") = gain,
                      Named("Profit") = profit,
                      Named("iMaxProfit")=imaxprofit));
}

// [[Rcpp::export("transp.profitMaximization")]]
List profitMaximization(List supplyFunction, List photosynthesisFunction, int type=1, double kstemmax = NA_REAL) {
  if(type==1) return(profitMaximization1(supplyFunction, photosynthesisFunction));
  else if(type==2) return(profitMaximization2(supplyFunction, photosynthesisFunction, kstemmax));
  return(profitMaximization3(supplyFunction, photosynthesisFunction, kstemmax));
}


// [[Rcpp::export("transp.dayCanopyTranspiration")]]
List dayCanopyTranspiration(List x, List soil, DataFrame meteo, int day,
                            double latitude, double elevation, double slope, double aspect) {
  
  //Extract meteo
  IntegerVector DOY = meteo["DOY"];
  NumericVector Precipitation = meteo["Precipitation"];
  NumericVector MeanTemperature = meteo["MeanTemperature"];
  NumericVector MinTemperature = meteo["MinTemperature"];
  NumericVector MaxTemperature = meteo["MaxTemperature"];
  NumericVector MinRelativeHumidity = meteo["MinRelativeHumidity"];
  NumericVector MaxRelativeHumidity = meteo["MaxRelativeHumidity"];
  NumericVector Radiation = meteo["Radiation"];
  NumericVector WindSpeed = meteo["WindSpeed"];
  CharacterVector dateStrings = meteo.attr("row.names");
  NumericVector GDD = gdd(DOY, MeanTemperature, 5.0);
  
  
  double rad = Radiation[day-1];
  double rain = Precipitation[day-1];
  double tmin = MinTemperature[day-1];
  double tmax = MaxTemperature[day-1];
  double rhmin = MinRelativeHumidity[day-1];
  double rhmax = MaxRelativeHumidity[day-1];
  double wind = WindSpeed[day-1];
    
  std::string c = as<std::string>(dateStrings[day-1]);
  int J = meteoland::radiation_julianDay(std::atoi(c.substr(0, 4).c_str()),std::atoi(c.substr(5,2).c_str()),std::atoi(c.substr(8,2).c_str()));
  double delta = meteoland::radiation_solarDeclination(J);
  double solarConstant = meteoland::radiation_solarConstant(J);
  
  //Control parameters
  List control = x["control"];
  List numericParams = control["numericParams"];
  double psiStep = numericParams["psiStep"];
  double psiMax = numericParams["psiMax"];
  int ntrial = numericParams["ntrial"];
  int maxNsteps  = numericParams["maxNsteps"];
  double psiTol = numericParams["psiTol"];
  double ETol = numericParams["ETol"];
  
  String canopyMode = Rcpp::as<Rcpp::String>(control["canopyMode"]);
  bool cavitationRefill = control["cavitationRefill"];
  int ntimesteps = control["ndailysteps"];
  int hydraulicCostFunction = control["hydraulicCostFunction"];
  
  
  DataFrame paramsBase = Rcpp::as<Rcpp::DataFrame>(x["paramsBase"]);
  NumericVector kPAR = Rcpp::as<Rcpp::NumericVector>(paramsBase["k"]);
  
  //Soil input
  NumericVector W = soil["W"];
  NumericVector psi = soil["psi"];
  NumericVector dVec = soil["dVec"];
  NumericVector Theta_FC = soil["Theta_FC"];
  NumericVector macro = soil["macro"];
  NumericVector rfc = soil["rfc"];
  NumericVector clay = soil["clay"];
  NumericVector sand = soil["sand"];
  NumericVector om = soil["om"];
  NumericVector Water_FC = waterFC(soil);
  int nlayers = W.size();
  
  //Vegetation input
  DataFrame above = Rcpp::as<Rcpp::DataFrame>(x["above"]);
  NumericVector LAIlive = Rcpp::as<Rcpp::NumericVector>(above["LAI_live"]);
  NumericVector LAIphe = Rcpp::as<Rcpp::NumericVector>(above["LAI_expanded"]);
  NumericVector LAIdead = Rcpp::as<Rcpp::NumericVector>(above["LAI_dead"]);
  NumericVector H = Rcpp::as<Rcpp::NumericVector>(above["H"]);
  NumericVector CR = Rcpp::as<Rcpp::NumericVector>(above["CR"]);
  int numCohorts = LAIlive.size();
  NumericVector pEmb = Rcpp::as<Rcpp::NumericVector>(x["ProportionCavitated"]);
  
  //Root distribution input
  List below = Rcpp::as<Rcpp::List>(x["below"]);
  NumericMatrix V =Rcpp::as<Rcpp::NumericMatrix>(below["V"]);
  NumericMatrix VCroot_kmax= Rcpp::as<Rcpp::NumericMatrix>(below["VCroot_kmax"]);
  NumericMatrix VGrhizo_kmax= Rcpp::as<Rcpp::NumericMatrix>(below["VGrhizo_kmax"]);
  NumericVector VG_n = Rcpp::as<Rcpp::NumericVector>(soil["VG_n"]);
  NumericVector VG_alpha = Rcpp::as<Rcpp::NumericVector>(soil["VG_alpha"]);
  
  //Transpiration params
  DataFrame paramsTransp = Rcpp::as<Rcpp::DataFrame>(x["paramsTransp"]);
  NumericVector Vmax298 = paramsTransp["Vmax298"];
  NumericVector Jmax298 = paramsTransp["Jmax298"];
  NumericVector VCstem_kmax = Rcpp::as<Rcpp::NumericVector>(paramsTransp["VCstem_kmax"]);
  NumericVector VCstem_c = Rcpp::as<Rcpp::NumericVector>(paramsTransp["VCstem_c"]);
  NumericVector VCstem_d = Rcpp::as<Rcpp::NumericVector>(paramsTransp["VCstem_d"]);
  NumericVector VCroot_c = paramsTransp["VCroot_c"];
  NumericVector VCroot_d = paramsTransp["VCroot_d"];
  NumericVector pRootDisc = Rcpp::as<Rcpp::NumericVector>(paramsTransp["pRootDisc"]);
  NumericVector Gwmin = Rcpp::as<Rcpp::NumericVector>(paramsTransp["Gwmin"]);
  NumericVector Gwmax = Rcpp::as<Rcpp::NumericVector>(paramsTransp["Gwmax"]);
  
  
  //Leaf phenology and the adjusted leaf area index
  double canopyHeight = 0.0, LAIcell= 0.0;
  for(int c=0;c<numCohorts;c++) {
    if(canopyHeight<H[c]) canopyHeight = H[c];
    LAIcell += (LAIphe[c]+LAIdead[c]);
  }
  int nz = ceil(canopyHeight/100.0); //Number of 100 cm layers
  
  double latrad = latitude * (PI/180.0);
  if(NumericVector::is_na(aspect)) aspect = 0.0;
  double asprad = aspect * (PI/180.0);
  if(NumericVector::is_na(slope)) slope = 0.0;
  double slorad = slope * (PI/180.0);
  //Day length (latitude in radians), atmospheric pressure, CO2 concentration
  double tauday = meteoland::radiation_daylengthseconds(latrad,  slorad,asprad, delta); 
  double Patm = meteoland::utils_atmosphericPressure(elevation);
  double Catm = 386.0;
  
  NumericVector z(nz+1,0.0);
  NumericVector zmid(nz);
  for(int i=1;i<=nz;i++) {
    z[i] = z[i-1] + 100.0;
    zmid[i-1] = 50.0 + 100.0*((double) (i-1));
  }
  NumericMatrix LAIme = LAIdistribution(z, LAIphe, H, CR);
  NumericMatrix LAImd = LAIdistribution(z, LAIdead, H, CR);
  
  
  //Light extinction and absortion by time steps
  List lightExtinctionAbsortion = instantaneousLightExtinctionAbsortion(LAIme, LAImd, kPAR,
                                                                        latitude, elevation, slope, aspect, solarConstant, delta, 
                                                                        rain,  rad,
                                                                        ntimesteps,  canopyMode);
  List abs_PAR_SL_list = lightExtinctionAbsortion["PAR_SL"];
  List abs_SWR_SL_list = lightExtinctionAbsortion["SWR_SL"];
  List abs_PAR_SH_list = lightExtinctionAbsortion["PAR_SH"];
  List abs_SWR_SH_list = lightExtinctionAbsortion["SWR_SH"];
  NumericVector SWR_direct = lightExtinctionAbsortion["SWR_direct"];
  NumericVector SWR_diffuse = lightExtinctionAbsortion["SWR_diffuse"];
  NumericVector fsunlit = lightExtinctionAbsortion["fsunlit"];

  double Tair;
  double tstep = 86400.0/((double) ntimesteps);
  double t = (-1.0*((86400.0-tauday)/2.0))+(tstep/2.0);
  //Daily average water vapor pressure
  double vpa = meteoland::utils_averageDailyVP(tmin, tmax, rhmin,rhmax);
  
  //Wind extinction profile
  NumericVector zWind;
  if(NumericVector::is_na(wind)) wind = 2.0; //Default wind speed when missing
  if(canopyMode=="multilayer") {
    zWind = windExtinctionProfile(zmid, wind, LAIcell, canopyHeight);
  } else if(canopyMode=="sunshade"){
    zWind = windExtinctionCohort(H,CR, wind,LAIcell, canopyHeight);
  }
  
  //Constant properties through time steps
  NumericVector Vmax298layer(nz), Jmax298layer(nz);
  double Vmax298SL= 0.0,Vmax298SH= 0.0,Jmax298SL= 0.0,Jmax298SH= 0.0;
  NumericVector SLarealayer(nz), SHarealayer(nz);
  double SLarea = 0.0, SHarea = 0.0;
  NumericVector QSH(nz), QSL(nz), absRadSL(nz), absRadSH(nz);
  double sn =0.0;
  
  NumericVector Vc, VCroot_kmaxc, VGrhizo_kmaxc, psic, VG_nc,VG_alphac;
  
  List cohort_list(numCohorts);
  cohort_list.attr("names") = above.attr("row.names");
  
  for(int c=0;c<numCohorts;c++) {
    //Determine to which layers is plant connected
    LogicalVector layerConnected(nlayers, false);
    int nlayersc = 0;
    for(int l=0;l<nlayers;l++) {
      if(V(c,l)>0.0) {
        double pRoot = xylemConductance(psi[l], 1.0, VCroot_c[c], VCroot_d[c]); //Relative conductance in the root
        layerConnected[l]= (pRoot>=pRootDisc[c]);
        if(layerConnected[l]) nlayersc++;
      }
    }
    // Rcout<<nlayersc;
    Vc = NumericVector(nlayersc);
    VCroot_kmaxc = NumericVector(nlayersc);
    VGrhizo_kmaxc = NumericVector(nlayersc);
    psic = NumericVector(nlayersc);
    VG_nc = NumericVector(nlayersc);
    VG_alphac= NumericVector(nlayersc);
    int cnt=0;
    for(int l=0;l<nlayers;l++) {
      if(layerConnected[l]) {
        Vc[cnt] = V(c,l);
        VCroot_kmaxc[cnt] = VCroot_kmax(c,l);
        VGrhizo_kmaxc[cnt] = VGrhizo_kmax(c,l);
        psic[cnt] = psi[l];
        VG_nc[cnt] = VG_n[l];
        VG_alphac[cnt] = VG_alpha[l];
        cnt++;
      }
    }
    
    double psiCav = 0.0;
    if(!cavitationRefill) {
      psiCav = xylemPsi(1.0-pEmb[c], 1.0, VCstem_c[c], VCstem_d[c]);//find water potential corresponding to this percentage of conductance loss
      // Rcout<< c <<" "<<psiCav<<"\n";
    }
    List supplyNetwork = supplyFunctionNetwork(psic,
                                          VGrhizo_kmaxc,VG_nc,VG_alphac,
                                          VCroot_kmaxc, VCroot_c[c],VCroot_d[c],
                                          VCstem_kmax[c], VCstem_c[c],VCstem_d[c], 
                                          psiCav, maxNsteps, psiStep, psiMax , ntrial, psiTol, ETol);
    
    for(int i=(nz-1);i>=0.0;i--) {
      //Effect of nitrogen concentration decay through the canopy
      double fn = exp(-0.713*(sn+LAIme(i,c)/2.0)/sum(LAIme(_,c)));
      sn+=LAIme(i,c);
      SLarealayer[i] = LAIme(i,c)*fsunlit[i];
      SHarealayer[i] = LAIme(i,c)*(1.0-fsunlit[i]);
      Vmax298layer[i] = Vmax298[c]*fn;
      Jmax298layer[i] = Jmax298[c]*fn;
    }
    if(canopyMode=="sunshade"){
      for(int i=0;i<nz;i++) {
        SLarea +=SLarealayer[i];
        SHarea +=SHarealayer[i];
        Vmax298SL +=Vmax298layer[i]*LAIme(i,c)*fsunlit[i];
        Jmax298SL +=Jmax298layer[i]*LAIme(i,c)*fsunlit[i];
        Vmax298SH +=Vmax298layer[i]*LAIme(i,c)*(1.0-fsunlit[i]);
        Jmax298SH +=Jmax298layer[i]*LAIme(i,c)*(1.0-fsunlit[i]);
      }
    }
    List photo(ntimesteps);
    List PM(ntimesteps);
    for(int n=0;n<ntimesteps;n++) {
      //Calculate instantaneous temperature and light conditions
      Tair = temperatureDiurnalPattern(t, tmin, tmax, tauday);
      
      if(canopyMode=="multilayer"){
        //Retrieve Light extinction
        NumericMatrix absPAR_SL = abs_PAR_SL_list[n];
        NumericMatrix absPAR_SH = abs_PAR_SH_list[n];
        NumericMatrix absSWR_SL = abs_SWR_SL_list[n];
        NumericMatrix absSWR_SH = abs_SWR_SH_list[n];
        for(int i=0;i<nz;i++) {
          QSL[i] = irradianceToPhotonFlux(absPAR_SL(i,c));
          QSH[i] = irradianceToPhotonFlux(absPAR_SL(i,c));
          absRadSL[i] = absSWR_SL(i,c);
          absRadSH[i] = absSWR_SH(i,c);
        }
        photo[n] = multilayerPhotosynthesisFunction(supplyNetwork, Catm, Patm,Tair, vpa, 
                                                 SLarealayer, SHarealayer, 
                                                 zWind, absRadSL, absRadSH, 
                                                 QSL, QSH,
                                                 Vmax298layer,Jmax298layer,
                                                 Gwmin[c], Gwmax[c]);
      } else if(canopyMode=="sunshade"){
        //Retrieve Light extinction
        NumericVector absPAR_SL = abs_PAR_SL_list[n];
        NumericVector absPAR_SH = abs_PAR_SH_list[n];
        NumericVector absSWR_SL = abs_SWR_SL_list[n];
        NumericVector absSWR_SH = abs_SWR_SH_list[n];
        
        photo[n] = sunshadePhotosynthesisFunction(supplyNetwork, Catm, Patm,Tair, vpa, 
                                               SLarea, SHarea, 
                                               zWind[c], absSWR_SL[c], absSWR_SH[c], 
                                                                                irradianceToPhotonFlux(absPAR_SL[c]), irradianceToPhotonFlux(absPAR_SH[c]),
                                                                                Vmax298SL, Vmax298SH,
                                                                                Jmax298SL, Jmax298SH,
                                                                                Gwmin[c], Gwmax[c]);
      }

      //Profit maximization
      PM[n] = profitMaximization(supplyNetwork, photo[n],  hydraulicCostFunction, VCstem_kmax[c]);
      

      t +=tstep;
    }
    cohort_list[c] = List::create(_["supply"]=supplyNetwork,
                                  _["photo"]=photo,
                                  _["PM"] = PM);
  }
  return(cohort_list);
} 