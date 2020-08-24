//
// Created by kazem on 2020-05-09.
//

#include <unsupported/Eigen/SparseExtra>
#include <Util.h>
#include "nasoq_eigen.h"

using namespace nasoq;
int main(int argc, char *argv[]){

 std::map<std::string,std::string> qp_args;
 parse_args(argc, argv, qp_args);

 /// Reading input matrices.
 Eigen::SparseMatrix<double,Eigen::ColMajor,int> H, A, C;
 Eigen::VectorXd q, b, d;
 std::string message = "Could not load ";
 if( !Eigen::loadMarket( H, qp_args["H"] ) ){ std::cout<<message<<"H"; return 1; }
 if( !Eigen::loadMarketVector( q, qp_args["q"] ) ){ std::cout<<message<<"q"; return 1; }
 if( !Eigen::loadMarket( A, qp_args["A"] ) ){ std::cout<<message<<"A"; return 1; }
 if( !Eigen::loadMarketVector( b, qp_args["b"] ) ){ std::cout<<message<<"b"; return 1; }
 if( !Eigen::loadMarket( C, qp_args["C"] ) ){ std::cout<<message<<"C"; return 1; }
 if( !Eigen::loadMarketVector( d, qp_args["d"] ) ){ std::cout<<message<<"d"; return 1; }

 /// New settings if provided
 int mode, iter;
 std::string nasoq_mode;
 double  pert, eps, tol;
 auto *qs = new QPSettings();
 if(qp_args.find("mode") != qp_args.end())
  mode = std::stoi(qp_args["mode"]);
 if(qp_args.find("nasoq") != qp_args.end()){
  nasoq_mode = qp_args["nasoq"];
  qs->nasoq_variant=nasoq_mode;
 }
 if(qp_args.find("perturbation") != qp_args.end()){
  pert = pow(10, std::stoi(qp_args["perturbation"]) );
  qs->diag_perturb=pert;
 }
 if(qp_args.find("iterations") != qp_args.end()){
  iter = std::stoi(qp_args["iterations"]);
  qs->inner_iter_ref=qs->outer_iter_ref=iter;
 }
 if(qp_args.find("epsilon") != qp_args.end()){
  eps = pow(10, std::stoi(qp_args["epsilon"]) );
  qs->eps=eps;
 }
 if(qp_args.find("tolerance") != qp_args.end()){
  tol = pow(10, std::stoi(qp_args["tolerance"]) );
  qs->stop_tol=tol;
 }


 /// output vectors
 Eigen::VectorXd x, y, z;

 /// call the wrapper.
 int ret = nasoq::quadprog(H,q,A,b,C,d,x,y,z,qs);

 //std::cout<<" -->"<< ret<<"\n";
 delete qs;
 return 0;
}