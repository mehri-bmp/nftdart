/*
 * Copyright (C) 2012-2023 Rodney A. Sparapani
 * Copyright (C) 2023 Mehri BagheriMohmadiPour (ONLY FOR THE LINES MARKED AS "Mehri B.M.P.")
 * This file is part of nftbart.
 * cnft2.h
 *
 * nftbart is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * nftbart is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors contact information
 * Rodney A. Sparapani: rsparapa@mcw.edu
 * Mehri BagheriMohamadiPour: mehri@uwm.edu
 */

/*
#define ARNG             //creates a macro named "ARNG," but without a specific value
#include "rn.h"          //Including additional custom or external libraries and headers: These are custom or external headers or libraries

#include <iostream>      //Including necessary C++ standard libraries and headers: Used for input and output operations
#include <string>        //Including necessary C++ standard libraries and headers: Provides string manipulation capabilities.
#include <ctime>         //Including necessary C++ standard libraries and headers: Includes date and time-related functions.
#include <sstream>       //Including necessary C++ standard libraries and headers: Used for string stream operations.

#include <fstream>       //Including file-related headers:Used for file input and output operations.
#include <vector>        //Including the standard vector library: This library is used for creating and manipulating dynamic arrays.
#include <limits>

#include "ambrt.h"       //Including additional custom or external libraries and headers: These are custom or external headers or libraries
#include "psbrt.h"       //Including additional custom or external libraries and headers: These are custom or external headers or libraries
#include "brt.h"         //Including additional custom or external libraries and headers: These are custom or external headers or libraries
#include "brtfuns.h"     //Including additional custom or external libraries and headers: These are custom or external headers or libraries
#include "dinfo.h"       //Including additional custom or external libraries and headers: These are custom or external headers or libraries
#include "mbrt.h"        //Including additional custom or external libraries and headers: These are custom or external headers or libraries
#include "treefuns.h"    //Including additional custom or external libraries and headers: These are custom or external headers or libraries
#include "tree.h"        //Including additional custom or external libraries and headers: These are custom or external headers or libraries
*/

//#include "rrn.h"
//#include "rtnorm.h"

#include "nftdart.h" //Mehri B.M.P.

RcppExport SEXP cnft2(   //function definition in the R programming language using Rcpp
		     SEXP _ixftrain,
		     SEXP _ixstrain,
		     SEXP _iy, 		//train y
		     SEXP _idelta,         //delta
		     //  SEXP _ievents,        //events
		     SEXP _ixftest,
		     SEXP _ixstest,
		     SEXP _im, 		//num trees mean and var
		     SEXP _ind,		//number of draws kept  (adapt, then burn, then draw)
		     SEXP _iburn, 	        //number of burn draws
		     SEXP _inadapt, 	//number of adapt draws 
		     SEXP _iadaptevery,	//adapt every adaptevery draws
		     SEXP _itau,		//mu_i ~N(0,\tau^2)
		     SEXP _ioveralllambda, //sigma_i ~ prod (nu'*lambda')/chisquared^_nu', nu',lambda' from overallnu, overalllambda
		     SEXP _ioverallnu,
		     SEXP _ibase,		//tree prior for mean and var
		     SEXP _ipower,	        //tree prior for mean and var
		     SEXP _itc,		//thread count
		     //SEXP _isigmav,	//intial values for sigma_i 
		     SEXP _ichvf,
		     SEXP _ichvs,
		     SEXP _ipbd,		//prob of birth/death step, mean and var trees
		     SEXP _ipb,		//pb prob of birth given birth/death
		     SEXP _istepwpert,	//(in (0,1)) shrinkage factor for range of acceptable cutpoints for MH, mean and var, (adapts)
		     SEXP _iprobchv,	//prob of trying change variable move, mean and var, (prob of perturb is 1 minus this)
		     SEXP _iminnumbot,	//minimum number of observations in a bottom node, mean and var
		     SEXP _iprintevery,    //how often you print progress
		     SEXP _ixifcuts,   
		     SEXP _ixiscuts,   
		     //SEXP _isummarystats,  //boolean, do you want summary stats
//		     SEXP _ialphao, 
//		     SEXP _ibetao,
		     //  SEXP _mstart, //  SEXP _sstart,
		     SEXP _hyper, 
		     SEXP _C,
		     SEXP _states, 
		     SEXP _phi,
		     SEXP _prior,
		     //SEXP _idraws,
		     SEXP _idrawMuTau,
		     //SEXP _iedraws2,
		     //SEXP _izdraws2
		     //SEXP _impute_bin, 
		     //SEXP _impute_prior,
            //Mehri B.M.P. addiding dart extension, line 97~105.
             SEXP _idart,         //Mehri B.M.P., dart prior: true(1)=yes, false(0)=no
             SEXP _itheta,        //Mehri B.M.P.
             SEXP _iomega,        //Mehri B.M.P.
             SEXP _igrp,          //Mehri B.M.P.
             SEXP _ia,            //Mehri B.M.P., param a for sparsity prior
             SEXP _ib,            //Mehri B.M.P., param b for sparsity prior
             SEXP _irho,          //Mehri B.M.P., param rho for sparsity prior (default to p)
             SEXP _iaug,          //Mehri B.M.P., categorical strategy: true(1)=data augment false(0)=degenerate trees
             SEXP _varprob
		     )
//  //Mehri B.M.P.,matrix to return dart posteriors (counts and probs)
//std::vector< std::vector<size_t> > varcnt; //Mehri B.M.P.
//std::vector< std::vector<double> > varprb; //Mehri B.M.P.
{
  //random number generation

#if defined(RRNG)   // These lines suggest conditional compilation based on whether RRNG is defined
  GetRNGstate();    //If RRNG is defined, the code will interact with R's random number generator. GetRNGstate() is a function in R that is used to set the random number generator's state to a known state. This is often done to ensure reproducibility in random number generation.
#endif

  rrn gen; //Declare an object gen of a custom class rrn. A custom random number generator.

  //--------------------------------------------------
  //process args. Initializes numeric matrices Xt_ftrain and Xt_strain using certain indices. And it calculates the dimensions and pointers for these matrices.
  Rcpp::NumericMatrix Xt_ftrain(_ixftrain), Xt_strain(_ixstrain);
  size_t pf = Xt_ftrain.nrow(),  ps = Xt_strain.nrow(), n = Xt_ftrain.ncol();
  double *xftrain = &Xt_ftrain[0], *xstrain = &Xt_strain[0];

  //x out-of-sample. Handles out-of-sample data with matrices Xt_ftest and Xt_stest.
  Rcpp::NumericMatrix Xt_ftest(_ixftest), Xt_stest(_ixstest);
  size_t np = Xt_ftest.ncol();
  double *xftest = nullptr, *xstest = nullptr;
  if(np) { //It checks whether there is out-of-sample data (indicated by np) and, if so, initializes pointers to this data.
    xftest = &Xt_ftest[0];
    xstest = &Xt_stest[0];
  }

/*
  int impute_bin = Rcpp::as<int>(_impute_bin), impute_flag=(impute_bin>=0);
  Rcpp::NumericVector impute_prior(_impute_prior); 
  double impute_post0, impute_post1, *impute_Xrow_ptr = 0;
*/
  // Using the Rcpp library, to integrate R and C++ code.
  //y
  Rcpp::NumericVector yv(_iy); //yv is Rcpp numeric vectors.
  double *y = &yv[0];   //y is pointer to the data, ymax=Rcpp::max(yv);

  //delta
  Rcpp::IntegerVector delta(_idelta), censor(n); //delta and censor are Rcpp numeric vectors.
/*
  Rcpp::NumericVector deltav(_idelta);
  double *delta = &deltav[0];
*/

  //events
  //  Rcpp::NumericVector events(_ievents);
  //size_t K=events.size();

  //z and w and e
  Rcpp::NumericVector zv(n), wv(n); //zv, and wv are Rcpp numeric vectors, e(n);
  double *z = &zv[0], *w = &wv[0];  //z, and w are pointers to the data in the respective vectors.

  //number of trees, im is an Rcpp integer vector containing two values, representing the number of trees (m) and a second value (mh).
  Rcpp::IntegerVector im(_im);
  size_t m = im[0], mh = im[1];

  //mu prior (tau, ambrt) and sigma prior (lambda,nu, psbrt),tau, overalllambda, and overallnu are doubles used as parameters for prior distributions.opm is calculated as 1.0 divided by mh. lambda are computed based on the values of overalllambda.
  double tau = Rcpp::as<double>(_itau);
  double overalllambda = Rcpp::as<double>(_ioveralllambda);
  double overallnu = Rcpp::as<double>(_ioverallnu);

  double opm=1.0/((double)mh);
  //double nu=2.0*pow(overallnu,opm)/(pow(overallnu,opm)-pow(overallnu-2.0,opm));
  double nu=2./(1.-pow(1.-2/overallnu, opm));
  double lambda=pow(overalllambda,opm);

  //nd and burn, nd, burn, nadapt, and adaptevery are variables used for control and configuration.
  size_t nd = Rcpp::as<int>(_ind);
  size_t burn = Rcpp::as<int>(_iburn);
  size_t nadapt = Rcpp::as<int>(_inadapt);
  size_t adaptevery = Rcpp::as<int>(_iadaptevery);

/*
  size_t R;
  Rcpp::NumericMatrix edraws2(_iedraws2), zdraws2(_izdraws2);
  bool drawrho=(edraws2.nrow()>0);
  if(drawrho) { 
    //nd=edraws2.nrow()-burn;
    R=100;
  } else R=0;
  Rcpp::NumericVector rhodraws(drawrho ? nd : 0), rhovec(R);
  double rho=0.;
*/

  //tree strings
  /*
    std::stringstream mtrees, strees;  
    mtrees.precision(10);
    mtrees << nd << " " << m << " " << p << endl;
    strees.precision(10);
    strees << nd << " " << mh << " " << p << endl;
  */

  int drawMuTau = Rcpp::as<int>(_idrawMuTau);
    //draws = Rcpp::as<int>(_idraws), 
    //drawsd=0; //(1-draws)*(1-drawMuTau);

  Rcpp::NumericMatrix mdraws(nd, n), //sdraws(nd, pow(n, draws)),
    sdraws(nd, n), mpred(nd, np), spred(nd, np), zdraws(nd, n); 
    //zdraws(nd+burn, n), edraws(nd+burn, n);
  //Rcpp::NumericVector sddraws((nd+burn)*drawsd);

  // for varcounts
  Rcpp::IntegerMatrix fvc(nd, pf), svc(nd, ps);
    Rcpp::IntegerVector fvarcount(pf), svarcount(ps);
    unsigned int tmaxd=0;
    unsigned int tmind=0;
    double tavgd=0.0;

  //tree prior
  Rcpp::NumericVector ibase(_ibase), ipower(_ipower);
  double falpha=ibase[0], salpha=ibase[1], 
    mybeta=ipower[0], mybetah=ipower[1];

  //thread count
  int tc = Rcpp::as<int>(_itc);

  //sigma vector, sigmav is an Rcpp numeric vector, and sig is a pointer to its data.
  Rcpp::NumericVector sigmav(n); //sigmav(_isigmav);
  double *sig = &sigmav[0];
  dinfo disig;
  disig.n=n; disig.p=ps; disig.x = xstrain; disig.y = sig; disig.tc=tc;

  //f(x) function to be used like sig vector
  double *fun = new double[n]; 

  //change variable
  Rcpp::NumericMatrix chvf(_ichvf), chvs(_ichvs);
  COUT << "row, cols chvf: " << chvf.nrow() << ", " << chvf.ncol() << endl;
  COUT << "row, cols chvs: " << chvs.nrow() << ", " << chvs.ncol() << endl;
  std::vector<std::vector<double> > chgvf(chvf.nrow());
  for(int i=0;i<chvf.nrow();i++) {
    chgvf[i].resize(chvf.ncol());
    for(int j=0;j<chvf.ncol();j++) chgvf[i][j]=chvf(i,j);
  }
  std::vector<std::vector<double> > chgvs(chvs.nrow());
  for(int i=0;i<chvs.nrow();i++) {
    chgvs[i].resize(chvs.ncol());
    for(int j=0;j<chvs.ncol();j++) chgvs[i][j]=chvs(i,j);
  }

  //control, Various control parameters such as printevery, pbd, pb, stepwpert, stepwperth, probchv, and minnumbot are set.
  size_t printevery = Rcpp::as<int>(_iprintevery);
  Rcpp::NumericVector ipbd(_ipbd), ipb(_ipb), istepwpert(_istepwpert),
    iprobchv(_iprobchv);
  double pbd=ipbd[0], pbdh=ipbd[1], pb=ipb[0], pbh=ipb[1],
    stepwpert = istepwpert[0], stepwperth = istepwpert[1],
    probchv = iprobchv[0], probchvh = iprobchv[1];
  Rcpp::IntegerVector iminnumbot(_iminnumbot);
  size_t minnumbot = iminnumbot[0], minnumboth = iminnumbot[1];
  bool dopert=true, doperth=true;
  if(probchv<0) dopert=false;
  if(probchvh<0) doperth=false;

  //summary statistics yes/no, summarystats is a boolean indicating whether to calculate summary statistics.
  bool summarystats = true; //Rcpp::as<bool>(_isummarystats);

  //error variance prior
/*double alphao = Rcpp::as<double>(_ialphao);
  double betao = Rcpp::as<double>(_ibetao);
  double lambdao=betao/alphao, nuo=2.*alphao;*/

  // DPM LIO, Variables like C, states, phi, prior, hyper, and constrain are part of a Dirichlet Process Mixture model.
  Rcpp::IntegerVector C(_C), states(_states);
  Rcpp::NumericMatrix phi(_phi);
  Rcpp::List prior(_prior), hyper(_hyper);
  //const int eal_m=Rcpp::as<int>(prior["m"]), 
  const int constrain=Rcpp::as<int>(prior["constrain"]);

  //double alpha=Rcpp::as<double>(hyper["alpha"]); // inital value
  Rcpp::NumericMatrix Y(n, 1);
  
  //Mehri B.M.P. addiding dart extension, line 277~292.
  bool dart;  //Mehri B.M.P.
  if(Rcpp::as<int>(_idart)==1) dart=true; //Mehri B.M.P.
  else dart=false; //Mehri B.M.P.
  double a = Rcpp::as<double>(_ia); //Mehri B.M.P.
  double b = Rcpp::as<double>(_ib); //Mehri B.M.P.
  //   Rcpp::NumericVector irho(_irho); //Mehri B.M.P.
  //   double *rho = &irho[0]; //Mehri B.M.P.
  double rho = Rcpp::as<double>(_irho); //Mehri B.M.P.
  bool aug; //Mehri B.M.P.
  if(Rcpp::as<int>(_iaug)==1) aug=true; //Mehri B.M.P.
  else aug=false; //Mehri B.M.P.
  Rcpp::NumericVector varprob(_varprob); //Mehri B.M.P.
  double theta = Rcpp::as<double>(_itheta); //Mehri B.M.P.
  double omega = Rcpp::as<double>(_iomega); //Mehri B.M.P.
  Rcpp::IntegerVector _grp(_igrp); //Mehri B.M.P.
  int *grp = &_grp[0]; //Mehri B.M.P.
    
  // return data structures, Variables for storing the results, such as dalpha, dnpart, dmu, dsig, dpC, dpMU, and dpSD, are defined.
  int ndMT=0, ndbMT=0, nMT=0;
  if(drawMuTau>0) {
    ndMT=nd;
    ndbMT=(nd+burn);
    nMT=n;
  }
  Rcpp::NumericVector dalpha(ndMT); 
  for(int i=0;i<ndMT;i++) dalpha[i]=0.;
  Rcpp::IntegerVector dnpart(ndbMT);
  Rcpp::NumericMatrix dmu(ndMT, nMT);
  Rcpp::NumericMatrix dsig(ndMT, nMT);
  Rcpp::IntegerMatrix dpC(ndMT, nMT);
  Rcpp::NumericMatrix dpMU(ndMT, nMT);
  Rcpp::NumericMatrix dpSD(ndMT, nMT);
  //Rcpp::NumericMatrix dpWT(ndMT, nMT);

  //--------------------------------------------------
  //print args, //Using Rprintf and COUT (from R or C++ libraries) to print various information to the console. It prints the values of n, pf, ps, xftrain, xstrain, y, np, xftest, xstest, information related to tree models and probabilities, information about alpha values and draw constraintsand more. Format specifiers %ld used for long integers, %lf for double-precision floating-point numbers, and %d for integers.
  //  Rprintf("**********************\n");
  Rprintf("n: %ld\n",n);
  Rprintf("pf, ps: %ld, %ld\n",pf, ps);
  Rprintf("xftrain first row: %lf, %lf\n",xftrain[0],xftrain[pf-1]);
//  Rprintf("xftrain second row: %lf, %lf\n",xftrain[pf],xftrain[2*pf-1]);
  Rprintf("xftrain last row: %lf, %lf\n",xftrain[(n-1)*pf],xftrain[n*pf-1]);
  Rprintf("xstrain first row: %lf, %lf\n",xstrain[0],xstrain[ps-1]);
//  Rprintf("xstrain second row: %lf, %lf\n",xstrain[ps],xstrain[2*ps-1]);
  Rprintf("xstrain last row: %lf, %lf\n",xstrain[(n-1)*ps],xstrain[n*ps-1]);
  Rprintf("first and last y: %lf, %lf\n",y[0],y[n-1]);
  if(np) {
    Rprintf("np: %d\n",np);
    Rprintf("xftest first row xftest: %lf, %lf\n",xftest[0],xftest[pf-1]);
//    Rprintf("xftest second row xftest: %lf, %lf\n",xftest[pf],xftest[2*pf-1]);
    Rprintf("xftest last row xftest : %lf, %lf\n",xftest[(np-1)*pf],xftest[np*pf-1]);
    Rprintf("xstest first row xstest: %lf, %lf\n",xstest[0],xstest[ps-1]);
//    Rprintf("xstest second row xstest: %lf, %lf\n",xstest[ps],xstest[2*ps-1]);
    Rprintf("xstest last row xstest : %lf, %lf\n",xstest[(np-1)*ps],xstest[np*ps-1]);
  } else {
    Rprintf("no test observations\n");
  }
  Rprintf("number of trees mean: %ld\n",m);
  Rprintf("number of trees stan dev: %ld\n",mh);
  Rprintf("tau: %lf\n",tau);
  Rprintf("total.lambda, lambda: %lf, %lf\n",overalllambda, lambda);
  Rprintf("total.nu, nu: %lf, %lf\n",overallnu, nu);
  Rprintf("burn (nskip): %ld\n",burn);
  Rprintf("nd (ndpost): %ld\n",nd);
  Rprintf("nadapt: %ld\n",nadapt);
  Rprintf("adaptevery: %ld\n",adaptevery);
  Rprintf("mean tree prior base: %lf\n",falpha);
  Rprintf("mean tree prior power: %lf\n",mybeta);
  Rprintf("variance tree prior base: %lf\n",salpha);
  Rprintf("variance tree prior power: %lf\n",mybetah);
  Rprintf("thread count: %ld\n",tc);
//Rprintf("first and last sigmav: %lf, %lf\n",sigmav[0],sigmav[n-1]);
  Rprintf("chgvf first row: %lf, %lf\n",chgvf[0][0],chgvf[0][pf-1]);
  Rprintf("chgvf last row: %lf, %lf\n",chgvf[pf-1][0],chgvf[pf-1][pf-1]);
  Rprintf("chgvs first row: %lf, %lf\n",chgvs[0][0],chgvs[0][ps-1]);
  Rprintf("chgvs last row: %lf, %lf\n",chgvs[ps-1][0],chgvs[ps-1][ps-1]);
  Rprintf("mean trees prob birth/death: %lf\n",pbd);
  Rprintf("mean trees prob birth: %lf\n",pb);
  Rprintf("variance trees prob birth/death: %lf\n",pbdh);
  Rprintf("variance trees prob birth: %lf\n",pbh);
  Rprintf("mean trees initial step width pert move: %lf\n",stepwpert);
  Rprintf("variance trees initial step width pert move: %lf\n",stepwperth);
  Rprintf("mean trees prob of a change var move : %lf\n",probchv);
  Rprintf("variance trees prob of a change var move : %lf\n",probchvh);
  Rprintf("mean trees min num obs in bottom node: %ld\n",minnumbot);
  Rprintf("variance trees min num obs in bottom node: %ld\n",minnumboth);
//  Rprintf("sigma prior : alphao=%lf, betao=%lf\n", alphao, betao);
  //Rprintf("base prior : muo=%lf, ko=%lf, alphao=%lf, betao=%lf\n",
  // muo, ko, alphao, betao);
  //Rprintf("init values : mu=%lf, tau=%lf, sd=%lf, alpha=%lf\n",
  // mstart, tauinit, sstart, alpha);
  COUT << "m:" << Rcpp::as<int>(prior["m"]) << '\n';
  COUT << "states:\n"; 
  for(size_t u=0; u<(size_t)R::imin2(5, states.size()); ++u) 
			 COUT << states[u] << ' ';
  COUT << '\n';
  COUT << "C:\n"; for(size_t u=0; u<(size_t)R::imin2(5, C.size()); ++u) 
		    COUT << C[u] << ' ';
  COUT << '\n';
  COUT << "phi:\n"; for(size_t u=0; u<(size_t)R::imin2(5, phi.rows()); ++u) {
    for(size_t v=0; v<(size_t)phi.cols(); ++v)
      COUT << phi(u, v) << ' ';
    COUT << '\n'; }
  Rprintf("alpha draw: alpha_a=%lf, alpha_b=%lf\n", 
	  Rcpp::as<double>(prior["alpha.a"]),
	  Rcpp::as<double>(prior["alpha.b"]));
  Rprintf("draw: DPM=%ld, constrain=%ld\n", drawMuTau, constrain);
  //Rprintf("draw: s=%ld, MuTau=%ld\n", draws, drawMuTau);
/*
  if(impute_flag) {
    cout << "Missing imputation column index:\n" << impute_bin << endl;
    cout << "Missing imputation probabilities:\n index 0=" << impute_prior[0]
	 << ',' << "index n-1=" << impute_prior[n-1] << endl;
  }
*/
  Rprintf("printevery: %d\n",printevery);
    
  //Mehri B.M.P. addiding dart extension, line 398~400.
  cout << "*****Dirichlet:sparse,theta,omega,rho,a,b,augment,grp[0],grp[p-1]:\n"
         << dart << ',' << theta << ',' << omega << ',' << rho << ',' << a << ',' //Mehri B.M.P.
         << b << ',' << aug << ',' << grp[0] << ',' << grp[pf-1] << endl; //Mehri B.M.P.
    

  //--------------------------------------------------
  //make xinfo. Initializing xinfo and Rcpp::List objects ixif and ixis using the inputs _ixifcuts and _ixiscuts.
  xinfo xif, xis;
  Rcpp::List ixif(_ixifcuts), ixis(_ixiscuts);
  if(ixif.length()!= (int)pf || ixis.length()!= (int)ps) { //Data Validation: It checks if the lengths of ixif and ixis match the expected values pf and ps. If not, it prints an error message using Rprintf and returns 0.
    Rprintf("Cutpoint definition does not match number of predictor variables!\n");
    return 0;
  }
  xif.resize(pf); //Data Extraction: It extracts data from ixif into xif and also prints some information about the data using COUT.
  for(size_t i=0;i<pf;i++) xif[i]=Rcpp::as< std::vector<double> >(ixif[i]);
  for(size_t i=0;i<pf;i++) {
      COUT << "f variable " << i << " has numcuts=" << xif[i].size() << " : ";
      COUT << xif[i][0] << " ... " << xif[i][xif[i].size()-1] << endl;
    }
  xis.resize(ps); //Data Extraction: It extracts data from ixis into xis and also prints some information about the data using COUT.
  for(size_t i=0;i<ps;i++) xis[i]=Rcpp::as< std::vector<double> >(ixis[i]);
  for(size_t i=0;i<ps;i++) {
      COUT << "s variable " << i << " has numcuts=" << xis[i].size() << " : ";
      COUT << xis[i][0] << " ... " << xis[i][xis[i].size()-1] << endl;
    }

  for(size_t i=0;i<n;i++) { //Initialization of Variables: It initializes various variables like z, w, and censor.
    z[i]=y[i]; // initialize z
    w[i]=1.;   // initialize w
    censor[i] = 1-delta[i]; // -1 left, 0 event, 1 right
    Y(i, 0)=y[i];
/*
    if(impute_flag) 
      Xt(impute_bin, i)=gen.bin(1, impute_prior[i]);
*/
  }

  //--------------------------------------------------
  //dinfo, dinfo Object Creation: A dinfo object di is created and initialized with relevant data.
  dinfo di;
  di.n=n; di.p=pf; di.x = xftrain; di.y = z; di.tc=tc;
  //--------------------------------------------------
  // set up ambrt object, ambrt Object Setup: An ambrt object ambm is set up. This object is related to BART modeling. It's configured with cutpoints, data, thread count, tree prior parameters, and MCMC information.
  ambrt ambm(m);

  //cutpoints
  ambm.setxi(&xif);    //set the cutpoints for this model object
  //data objects
  ambm.setdata(&di);  //set the data
  //thread count
  ambm.settc(tc);      //set the number of threads when using OpenMP, etc.
  //tree prior
  ambm.settp(falpha, //the alpha parameter in the tree depth penalty prior
	     mybeta     //the beta parameter in the tree depth penalty prior
	     );
  //MCMC info
  ambm.setmi(
	     pbd,  //probability of birth/death
	     pb,  //probability of birth
	     minnumbot,    //minimum number of observations in a bottom node
	     dopert, //do perturb/change variable proposal?
	     stepwpert,  //initialize stepwidth for perturb proposal.  If no adaptation it is always this.
	     probchv,  //probability of doing a change of variable proposal.  perturb prob=1-this.
	     &chgvf  //initialize the change of variable correlation matrix.
	     );
  ambm.setci(tau,sig);
 
  //Mehri B.M.P. addiding dart extension, line 466~478.
  nftdart Insnftdart; //Mehri B.M.P., Create an instance of the nftdart class
  Insnftdart.setdart(a,b,grp,aug,dart,rho); //Mehri B.M.P
  //bm.setdart(a,b,rho,aug,dart); //Mehri B.M.P
  Insnftdart.setpv(&varprob[0]); //Mehri B.M.P

  // dart iterations, //Mehri B.M.P
  std::vector<double> ivarprb (pf,0.); //Mehri B.M.P
  std::vector<size_t> ivarcnt (pf,0); //Mehri B.M.P
  ivarprb=Insnftdart.getpv(); //Mehri B.M.P
//       if(verbose==1) { //Mehri B.M.P
//     cout << "*****Variable selection probability pv[0],pv[p-1]:\n" //Mehri B.M.P
//         << ivarprb[0] << ',' << ivarprb[pf-1] << endl; //Mehri B.M.P
//     printf("\nMCMC\n"); //Mehri B.M.P
//       } //Mehri B.M.P
  //--------------------------------------------------
  //setup psbrt object, psbrt Object Setup: A psbrt object psbm is set up. It is related to posterior sampling for BART. It's configured with cutpoints, data, thread count, tree prior parameters, and MCMC information.
  psbrt psbm(mh,overalllambda);

  //make di for psbrt object
  dinfo dips;
  double *r = new double[n]; 
  for(size_t i=0;i<n;i++) { sigmav[i]=1.; r[i]=sigmav[i]; }
  dips.x = xstrain; dips.y=r; dips.p=ps; dips.n=n; dips.tc=tc;


  //cutpoints
  psbm.setxi(&xis);    //set the cutpoints for this model object
  //data objects
  psbm.setdata(&dips);  //set the data
  //thread count
  psbm.settc(tc); 
  //tree prior
  psbm.settp(salpha, //the alpha parameter in the tree depth penalty prior
	     mybetah     //the beta parameter in the tree depth penalty prior
	     );
  psbm.setmi(
	     pbdh,  //probability of birth/death
	     pbh,  //probability of birth
	     minnumboth,    //minimum number of observations in a bottom node
	     doperth, //do perturb/change variable proposal?
	     stepwperth,  //initialize stepwidth for perturb proposal.  If no adaptation it is always this.
	     probchvh,  //probability of doing a change of variable proposal.  perturb prob=1-this.
	     &chgvs  //initialize the change of variable correlation matrix.
	     );
  psbm.setci(nu,lambda);

  // x.test predictions, x.test Predictions: Predictions are made on test data and stored in _f and _s
  double *_f = new double[np], *_s = new double[np];
  dinfo f, s;
  f.x = xftest; f.y=_f; f.p = pf; f.n=np; f.tc=tc;
  s.x = xstest; s.y=_s; s.p = ps; s.n=np; s.tc=tc;

  // DPM LIO, DPM LIO: It is some operation involving a Dirichlet process mixture model (DPM). Parameters are calculated for mvec and svec based on phi.
  double *mvec = new double[n];
  double *svec = new double[n];
  //Rcpp::NumericVector mvec(n), svec(n);
  for(size_t i=0;i<n;i++) {
    mvec[i]=phi(0, 0);
    svec[i]=pow(phi(0, 1), -0.5);
  }
  //--------------------------------------------------
  //run mcmc. Implements MCMC sampling
  //Variable Declarations: Declaring several std::vector objects to store various data, parameters, and intermediate results. These vectors have different dimensions and are used to hold information about the MCMC sampling process.
  std::vector<int> onn(nd*m,1);
  std::vector<std::vector<int> > oid(nd*m, std::vector<int>(1));
  std::vector<std::vector<int> > ovar(nd*m, std::vector<int>(1));
  std::vector<std::vector<int> > oc(nd*m, std::vector<int>(1));
  std::vector<std::vector<double> > otheta(nd*m, std::vector<double>(1));
  std::vector<int> snn(nd*mh,1);
  std::vector<std::vector<int> > sid(nd*mh, std::vector<int>(1));
  std::vector<std::vector<int> > svar(nd*mh, std::vector<int>(1));
  std::vector<std::vector<int> > sc(nd*mh, std::vector<int>(1));
  std::vector<std::vector<double> > stheta(nd*mh, std::vector<double>(1));
  //brtMethodWrapper fambm(&brt::f,ambm);
  //brtMethodWrapper fpsbm(&brt::f,psbm);

  // x predictions
/*
  double fhat0, fhat1, shat0=1., shat1=1.;
  dinfo f0, f1, s0, s1;
  f0.y=&fhat0; f0.p = pf; f0.n=1; f0.tc=tc;
  f1.y=&fhat1; f1.p = pf; f1.n=1; f1.tc=tc;
  s0.y=&shat0; s0.p = ps; s0.n=1; s0.tc=tc;
  s1.y=&shat1; s1.p = ps; s1.n=1; s1.tc=tc;
*/

  Rprintf("Starting MCMC...\n");
  //Initialization: Some variables like adapting, burning, keeping, and drawDP are initialized. These variables control different phases of the MCMC algorithm, such as adaptation, burning, and data imputation.
  bool adapting=true, burning=false, keeping=false, adapting_every, drawDP=false;
//, draw_s=true; 
#ifdef PROFILER 
  ProfilerStart(PROFILER);
#endif
  //MCMC Loop: The core of the MCMC algorithm is in the for loop, which runs for a specified number of iterations (M). Inside the loop, the code performs different operations depending on whether it's in the adaptation phase, burning phase, or keeping phase. The code also handles data imputation, including censoring and random draws. Updating Parameters: The code updates various parameters such as z[k], sig[k], mvec[k], and phi(C[k], 0) based on the current state and the MCMC sampling process. These parameters are essential for Bayesian inference. MCMC Sampling: The code uses random number generation functions (e.g., gen.normal) to sample from probability distributions. This is a fundamental aspect of MCMC, where random samples are drawn to estimate posterior distributions. Data Imputation: The code appears to be imputing data or estimating missing values. This is a common task in Bayesian analysis, where incomplete data is estimated using the observed data and the model.Constraining Parameters: There's a section that seems to constrain parameters, which is often done in Bayesian analysis to ensure certain properties or priors on parameters. Updating Count of Partitions: The code updates the count of partitions (dnpart[h]), which could be related to the number of clusters or groups in the data. Other Operations: The code includes some commented-out sections that might be part of the larger program but are currently inactive.
  for(size_t h, i=0, j, L=nadapt+burn, M=L+nd; i<M; i++) {
    adapting=(i<nadapt);
    h=i-nadapt;
    j=i-L;

    if(!adapting) {
      adapting_every=false;
      burning=(i<L); 
      if(!burning) keeping=true;
      if(i==nadapt) {
	drawDP=(drawMuTau>0);
      }

      if(burning && (h % printevery)==0) COUT << "draw burn " << h << endl;
      else if(keeping) {
	if ((j % printevery)==0) COUT << "draw keep " << j << endl;
	//if(summarystats && j==0) {
	if(j==0) {
	  ambm.setstats(true);
	  psbm.setstats(true);
	}
      }
        
    if(i==(burn/2)&&dart) Insnftdart.startdart(); //Mehri B.M.P. addiding dart extension
    } 
    else adapting_every=(i>0 && (i%adaptevery)==0);

    if(adapting_every) {
      COUT << "adapting  " << i << endl;
    }

    size_t K=Rcpp::max(C)+1;
    int g;
    for(size_t k=0;k<n;k++) {
	if(drawDP) {
	  Rcpp::NumericVector prob(K);
	  for(size_t l=0;l<K;l++) 
	    prob[l]=states[l]*
	      R::pnorm(y[k], phi(l, 0)*sig[k]+ambm.f(k), 
		       pow(phi(l, 1), -0.5)*sig[k], 
		       (int)(delta[k]==2), 0)/n; // left and right
		       //pow(phi(g, 1), -0.5)*sig[k], 0, 0)/n; right only
	  g=gen.rcat(prob);
	  if(g==-1) g=C[k];
	}
      if(delta[k]==1) {
	z[k] = y[k];
/*
	if(drawrho && !adapting) {
	  if(drawDP) z[k] -= rho*pow(phi(g, 1), -0.5)*sig[k]*edraws2(h, k);
	  else z[k] -= rho*sig[k]*edraws2(h, k);
	}
*/
      }
      else {
	if(drawDP) {
	  // right and left censoring
/*
	  if(drawrho && !adapting) z[k]=censor[k]*
	    gen.rtnorm(censor[k]*y[k], 
		       censor[k]*
		       (phi(g, 0)*sig[k]+ambm.f(k)+
			rho*pow(phi(g, 1), -0.5)*sig[k]*edraws2(h, k)), 
		       pow(1.-pow(rho, 2.), 0.5)*pow(phi(g, 1), -0.5)*sig[k]);
	  else */
	  z[k]=censor[k]*
	    gen.rtnorm(censor[k]*y[k], 
		       censor[k]*(phi(g, 0)*sig[k]+ambm.f(k)), 
		       pow(phi(g, 1), -0.5)*sig[k]);
	}
	else {
/*
	  if(drawrho && !adapting)
	    z[k]=censor[k]*
	      gen.rtnorm(censor[k]*y[k], 
			 censor[k]*(ambm.f(k)+rho*sig[k]*edraws2(h, k)), 
			 pow(1.-pow(rho, 2.), 0.5)*sig[k]);
	  else */
	    z[k]=censor[k]*gen.rtnorm(censor[k]*y[k], 
				       censor[k]*ambm.f(k), sig[k]);
	}

/*
	if(drawrho && !adapting) {
	  if(z[k]>zdraws2(h, k)) z[k]=zdraws2(h, k);
	}
*/
      }
/*
      e[k]=z[k];
      if(keeping || burning) {
	zdraws(h, k)=z[k];
	if(drawrho) sig[k] *= pow(1.-pow(rho, 2.), 0.5);
      }
*/
      if(keeping) {
	if(drawDP) zdraws(j, k)=gen.normal(phi(g, 0)*sig[k]+ambm.f(k), 
					   pow(phi(g, 1), -0.5)*sig[k]);
	else zdraws(j, k)=gen.normal(ambm.f(k), sig[k]);
      }
    }

    for(size_t k=0;k<n;k++) {
      if(drawDP) {
	z[k] = z[k]-mvec[k]*sig[k];
	sig[k] = sig[k]*svec[k];
	//if(draws) sig[k] = sig[k]*svec[k];
	//else sig[k] = svec[k];
      }
    }

    ambm.draw(gen);
    for(size_t k=0;k<n;k++) fun[k]=ambm.f(k);
    if(adapting_every) ambm.adapt();

    //if(draws) 
    {
      for(size_t k=0;k<n;k++) { 
	r[k]=z[k]-fun[k]; 
	//r[k]=z[k]-ambm.f(k); 
	if(drawDP) r[k]=r[k]/svec[k];
      }
      //dips = di;    
      //dips -= fambm;
      psbm.draw(gen);

      for(size_t k=0;k<n;k++) { 
	sig[k] = psbm.f(k);
	//if(drawDP) sig[k] *= svec[k]; 
      }
      //disig = fpsbm;
      if(adapting_every) psbm.adapt(); 
    }
/*
    else if(!drawDP) {
      double rss, sigma;
      rss=nuo*lambdao;
      for(size_t k=0;k<n;k++) {
	//rss += pow(z[k]-ambm.f(k), 2.);
	rss += pow(z[k]-fun[k], 2.);
      }
      sigma = sqrt(rss/gen.chi_square(n+nuo));
      for(size_t k=0;k<n;k++) sig[k]=sigma;
      sddraws[h] = sigma; 
    }
*/

    if(drawDP) {
      for(size_t k=0;k<n;k++) {
	//z[k] = mvec[k]+(z[k]-ambm.f(k))/sig[k];
	z[k] = mvec[k]+(z[k]-fun[k])/sig[k];
	Y(k, 0) = z[k];
	//Y[k] = z[k];
	//Y[k] = mvec[k]+(z[k]-ambm.f(k))/sig[k];
	//sig[k] /= svec[k];
	//Y[k] = (z[k] + mvec[k]-ambm.f(k))/sig[k];
	//w[k] = 1./sig[k];
      }

      //if(drawMuTau==1)
	DPMLIOneal8(Rcpp::wrap(Y), _phi, _C, _states, _prior, _hyper,
		  gen, &DPMLIOmutau_F, &DPMLIOmutau_G0tau, 
		  &DPMLIOmutau_G0mu, &DPMLIOmutau_P0);
      // else
      // 	DPMtauneal8(Rcpp::wrap(Y), _phi, _C, _states, _prior, _hyper,
      // 		  gen, &DPMtau_F, &DPMtau_G0, &DPMtau_P0);

      // testing mu.=0
      //mvec[k]=0.;
      double mu0, var0; 
      mu0=0.; var0=0.; 
      for(size_t k=0;k<n;k++) {
	mvec[k] = phi(C[k], 0);
	mu0 += mvec[k]/n;
	svec[k] = pow(phi(C[k], 1), -0.5);
	var0 += pow(svec[k], 2.)/n;
      }

      if(constrain) {
	double sd0=sqrt(var0); 
	for(size_t k=0;k<n;k++) {
	  mvec[k] = mvec[k]-mu0;
	  phi(C[k], 0)=mvec[k];
	  //if(draws) {
	    svec[k] = svec[k]/sd0;
	    phi(C[k], 1)=pow(svec[k], -2.);
	  //}
	}
      }

      dnpart[h]=Rcpp::max(C)+1;
    }

/*
    for(size_t k=0;k<n;k++) {
      if(drawDP) {
	e[k]=(e[k]-fun[k]-sig[k]*mvec[k])/(sig[k]*svec[k]);
      } else e[k]=(e[k]-fun[k])/sig[k];
    }

    if(drawrho && !adapting) {
      double reduc;
      for(int l=0;l<R;l++) {
	rho=l/R;
	reduc=1.-pow(rho, 2.);
	rhovec[l]=0.;
	for(int k=0;k<n;k++) 
	  rhovec[l] += 
	    pow(e[k], 2.)+pow(edraws2(h, k), 2.)-2.*rho*e[k]*edraws2(h, k);
	rhovec[l] = pow(reduc, -0.5*n)*exp(-0.5*rhovec[l]/reduc);
      }
      rho = ((double)gen.rcat(rhovec))/R;
      if(keeping) rhodraws[j]=rho;
    }
    if(keeping || burning) for(size_t k=0;k<n;k++) edraws(h, k)=e[k];
*/
    if(keeping) {
      //save tree to vec format

      ambm.savetree(j,m,onn,oid,ovar,oc,otheta);
      //if(draws) 
      psbm.savetree(j,mh,snn,sid,svar,sc,stheta);

      if(drawMuTau>0) dalpha[j]=hyper["alpha"];

      ambm.getstats(&fvarcount[0],&tavgd,&tmaxd,&tmind);
      for(size_t k=0;k<pf;k++) fvc(j, k)=fvarcount[k];

      //if(draws) 
      {
	psbm.getstats(&svarcount[0],&tavgd,&tmaxd,&tmind);
	for(size_t k=0;k<ps;k++) svc(j, k)=svarcount[k];
      }

      //double wt=1./n;
      for(size_t k=0;k<n;k++) {
	//mdraws(j, k) = ambm.f(k);
	mdraws(j, k) = fun[k];
	//if(draws) {
	  //sdraws(j, k) = psbm.f(k);
	  sdraws(j, k) = sig[k];
	//}
	if(drawMuTau>0) {
	  dmu(j, k) = mvec[k];
	  dsig(j, k) = svec[k];
	  dpMU(j, k)=phi(k, 0);
	  dpSD(j, k)=pow(phi(k, 1), -0.5);
	  dpC(j, k) = C[k]+1;
	  //dpWT(j, C[k])=dpWT(j, C[k])+wt;
	} 
      }

      if(np) {
	ambm.predict(&f);
	//if(draws) 
	psbm.predict(&s);

	for(size_t k=0;k<np;k++) {
	  mpred(j, k) = f.y[k];
	  //if(draws) 
	  spred(j, k) = s.y[k];
	}
      }
    } 
  }

#ifdef PROFILER  // Some conditional preprocessor directives, such as checking for the presence of the PROFILER macro and using ProfilerStop() if it's defined.
  ProfilerStop();
#endif

  //Two std::stringstream objects, mtrees and strees, are declared.
  std::stringstream mtrees, strees;  
  mtrees=ambm.gettrees(nd,m,onn,oid,ovar,oc,otheta,0.); //The ambm.gettrees function is called with various parameters, and the result is assigned to mtrees.
  //if(draws) 
  strees=psbm.gettrees(nd,mh,snn,sid,svar,sc,stheta,-1.); //psbm.gettrees is called and assigned to strees.

  /*
    Flatten posterior trees to a few XXL vectors so we can just pass pointers
    to these vectors back to R (which is much faster than copying all the data back)
  */
  //The code then flattens posterior trees into several vectors, such as e_ots, e_oid, e_ovar, e_oc, and e_otheta. These vectors are filled with data from the input vectors oid, ovar, oc, and otheta.
  std::vector<int>* e_ots=new std::vector<int>(nd*m);
  std::vector<int>* e_oid=new std::vector<int>;
  std::vector<int>* e_ovar=new std::vector<int>;
  std::vector<int>* e_oc=new std::vector<int>;
  std::vector<double>* e_otheta=new std::vector<double>;
  for(size_t i=0;i<nd;i++)
    for(size_t j=0;j<m;j++) {
      e_ots->at(i*m+j)=static_cast<int>(oid[i*m+j].size());
      e_oid->insert(e_oid->end(),oid[i*m+j].begin(),oid[i*m+j].end());
      e_ovar->insert(e_ovar->end(),ovar[i*m+j].begin(),ovar[i*m+j].end());
      e_oc->insert(e_oc->end(),oc[i*m+j].begin(),oc[i*m+j].end());
      e_otheta->insert(e_otheta->end(),otheta[i*m+j].begin(),otheta[i*m+j].end());
    }
  //The code creates Rcpp::XPtr objects for the vectors created in the previous step. These are used for interfacing with R.
  Rcpp::XPtr< std::vector<int> > extern_ots(e_ots,true);
  Rcpp::XPtr< std::vector<int> > extern_oid(e_oid,true);
  Rcpp::XPtr< std::vector<int> > extern_ovar(e_ovar,true);
  Rcpp::XPtr< std::vector<int> > extern_oc(e_oc,true);
  Rcpp::XPtr< std::vector<double> > extern_otheta(e_otheta,true);

  Rcpp::List ret = Rcpp::List::create( //A Rcpp::List object named ret is created. It contains several named elements, including "ots," "oid," "ovar," "oc," and "otheta," which correspond to the vectors and XPtrs created earlier. There are also other elements like "f.train" and "f.trees."
				      Rcpp::Named("ots")=extern_ots,
				      Rcpp::Named("oid")=extern_oid,
				      Rcpp::Named("ovar")=extern_ovar,
				      Rcpp::Named("oc")=extern_oc,
				      Rcpp::Named("otheta")=extern_otheta,
				      /*
					Rcpp::Named("sts")=extern_sts,
					Rcpp::Named("sid")=extern_sid,
					Rcpp::Named("svar")=extern_svar,
					Rcpp::Named("sc")=extern_sc,
					Rcpp::Named("stheta")=extern_stheta,
				      */
				      Rcpp::Named("f.train")=mdraws,
				      Rcpp::Named("f.trees")=Rcpp::CharacterVector(mtrees.str()));

  //if(draws) , A conditional block that deals with more data and vectors if certain conditions are met. If draws is true, it creates additional vectors and adds them to the ret list.
  {
    std::vector<int>* e_sts=new std::vector<int>(nd*mh);
    std::vector<int>* e_sid=new std::vector<int>;
    std::vector<int>* e_svar=new std::vector<int>;
    std::vector<int>* e_sc=new std::vector<int>;
    std::vector<double>* e_stheta=new std::vector<double>;
    for(size_t i=0;i<nd;i++)
      for(size_t j=0;j<mh;j++) {
	e_sts->at(i*mh+j)=static_cast<int>(sid[i*mh+j].size());
	e_sid->insert(e_sid->end(),sid[i*mh+j].begin(),sid[i*mh+j].end());
	e_svar->insert(e_svar->end(),svar[i*mh+j].begin(),svar[i*mh+j].end());
	e_sc->insert(e_sc->end(),sc[i*mh+j].begin(),sc[i*mh+j].end());
	e_stheta->insert(e_stheta->end(),stheta[i*mh+j].begin(),stheta[i*mh+j].end());
      }

    Rcpp::XPtr< std::vector<int> > extern_sts(e_sts,true);
    Rcpp::XPtr< std::vector<int> > extern_sid(e_sid,true);
    Rcpp::XPtr< std::vector<int> > extern_svar(e_svar,true);
    Rcpp::XPtr< std::vector<int> > extern_sc(e_sc,true);
    Rcpp::XPtr< std::vector<double> > extern_stheta(e_stheta,true);

    ret["sts"]=extern_sts;
    ret["sid"]=extern_sid;
    ret["svar"]=extern_svar;
    ret["sc"]=extern_sc;
    ret["stheta"]=extern_stheta;
    ret["s.train"]=sdraws;
    ret["s.trees"]=Rcpp::CharacterVector(strees.str());
  }
//  else if(drawMuTau==0) ret["sigma"]=sddraws;

  if(drawMuTau>0) { //Another conditional block is used to populate the ret list with more elements if drawMuTau is greater than 0.
    ret["dpalpha"]=dalpha;
    ret["dpn"]=dnpart;
    ret["dpmu"]=dmu;
    ret["dpsd"]=dsig;
    ret["dpC"]=dpC;
    ret["dpmu."]=dpMU;
    ret["dpsd."]=dpSD;
    //ret["dpwt."]=dpWT;
  }

  if(np) { //If np is true, it adds elements related to "f.test" and "s.test" to the ret list.
    ret["f.test"]=mpred;
    //if(draws) 
    ret["s.test"]=spred;
  }

  ret["z.train"]=zdraws; //The z.train element is added to the ret list.
  //ret["e.train"]=edraws;
  //if(drawrho) ret["rho"]=rhodraws;

  // summary statistics
  if(summarystats) {
    //unsigned int varcount[p];
    //    std::vector<unsigned int> varcount(p);
/*
    Rcpp::IntegerVector varcount(p);
    for(size_t i=0;i<p;i++) varcount[i]=0;
    unsigned int tmaxd=0;
    unsigned int tmind=0;
    double tavgd=0.0;
*/

    ambm.getstats(&fvarcount[0],&tavgd,&tmaxd,&tmind); //A section related to summary statistics calculates various values and adds them to the ret list. The exact calculations involve statistics and vectors like fvarcount, tavgd, tmaxd, and tmind.
    tavgd/=(double)(nd*m);
    ret["f.tavgd"]=tavgd;
    ret["f.tmaxd"]=tmaxd;
    ret["f.tmind"]=tmind;
    ret["f.varcount"]=fvc;
    //ret["f.varcount"]=Rcpp::clone(varcount);
    //Rcpp::NumericVector vc(p);
    //Rcpp::IntegerVector vc(p);
    //for(size_t i=0;i<p;i++) vc[i]=(int)varcount[i];
    //ret["f.varcount"]=vc;

    //if(draws) 
    {
      //for(size_t i=0;i<p;i++) varcount[i]=0;
      //tmaxd=0; tmind=0; tavgd=0.0;
      psbm.getstats(&svarcount[0],&tavgd,&tmaxd,&tmind);
      tavgd/=(double)(nd*mh);
      ret["s.tavgd"]=tavgd;
      ret["s.tmaxd"]=tmaxd;
      ret["s.tmind"]=tmind;
      ret["s.varcount"]=svc;
      //ret["s.varcount"]=varcount;
      //Rcpp::NumericVector sdvc(p);
      //Rcpp::IntegerVector sdvc(p);
      //for(size_t i=0;i<p;i++) sdvc[i]=(int)varcount[i];
      //ret["s.varcount"]=sdvc;
    }
  }

#if defined(RRNG) //The code checks if RRNG is defined and includes some related operations.
  PutRNGstate();
#endif
 
  // The code deallocates memory for several dynamically allocated arrays using delete.
  if(_f) delete [] _f;
  if(_s) delete [] _s;
  if(r) delete [] r;
  if(fun) delete [] fun;
  if(mvec) delete [] mvec;
  if(svec) delete [] svec;

  return ret; // It returns the ret list.
}


