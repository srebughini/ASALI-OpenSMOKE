/*##############################################################################################
#                                                                                              #
#     #############       #############       #############       ####                ####     #
#    #             #     #             #     #             #     #    #              #    #    #
#    #    #####    #     #    #########      #    #####    #     #    #              #    #    #
#    #    #   #    #     #    #              #    #   #    #     #    #              #    #    #
#    #    #####    #     #    #              #    #####    #     #    #              #    #    #
#    #             #     #    #########      #             #     #    #              #    #    #
#    #             #     #             #     #             #     #    #              #    #    #
#    #    #####    #      #########    #     #    #####    #     #    #              #    #    #
#    #    #   #    #              #    #     #    #   #    #     #    #              #    #    #
#    #    #   #    #      #########    #     #    #   #    #     #    #########      #    #    #
#    #    #   #    #     #             #     #    #   #    #     #             #     #    #    #
#     ####     ####       #############       ####     ####       #############       ####     #
#                                                                                              #
#   Department of Energy                                                                       #
#   Politecnico di Milano                                                                      #
#   Author: Stefano Rebughini <stefano.rebughini@polimi.it>                                    #
#                                                                                              #
################################################################################################
#                                                                                              #
#   License                                                                                    #
#                                                                                              #
#   This file is part of ASALI.                                                                #
#                                                                                              #
#   ASALI is free software: you can redistribute it and/or modify                              #
#   it under the terms of the GNU General Public License as published by                       #
#   the Free Software Foundation, either version 3 of the License, or                          #
#   (at your option) any later version.                                                        #
#                                                                                              #
#   ASALI is distributed in the hope that it will be useful,                                   #
#   but WITHOUT ANY WARRANTY; without even the implied warranty of                             #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                              #
#   GNU General Public License for more details.                                               #
#                                                                                              #
#   You should have received a copy of the GNU General Public License                          #
#   along with ASALI. If not, see <http://www.gnu.org/licenses/>.                              #
#                                                                                              #
##############################################################################################*/

namespace ASALI
{
class BVPSystem
#if ASALI_USE_BZZ == 1
 : public BzzDaeSystemObject
#endif
{

public:

	BVPSystem(OpenSMOKE::ThermodynamicsMap_CHEMKIN<double>&          thermodynamicsMap, 
			  OpenSMOKE::KineticsMap_CHEMKIN<double>&                kineticsMap,
			  OpenSMOKE::ThermodynamicsMap_Surface_CHEMKIN<double>&  thermodynamicsSurfaceMap, 
			  OpenSMOKE::KineticsMap_Surface_CHEMKIN_Lumped<double>& kineticsSurfaceMap,
			  OpenSMOKE::TransportPropertiesMap_CHEMKIN<double>&     transportMap);

	#include "vector.h"

	void setSolid(const double rhoSolid, const double condSolid, const double cpSolid);

	void setHomogeneusReactions(const bool flag)   { homogeneusReactions_ = flag; }

	void setHeterogeneusReactions(const bool flag) { heterogeneusReactions_ = flag; }

	void setEnergyEquation(const bool flag)        { energy_ = flag; }

	void setReactorGeometry( const double alfa,      const double epsi, 
							 const double AsymptoticSh, const double Lcat, const double Linert,
							 const double Dh,           const double G);

	void setFeedValue(const double p, const double T0,
					  const OpenSMOKE::OpenSMOKEVectorDouble x0bulk);

	void setGrid(const OpenSMOKE::OpenSMOKEVectorDouble z);

	void setInert(const std::string inert);

	void resize(const unsigned int NP);

	unsigned int BlockDimension()		const {return NB_;};

	unsigned int NumberOfEquations()	const {return NE_;};

	void start();
	void end();

	unsigned int Equations(const double t, const OpenSMOKE::OpenSMOKEVectorDouble& y, OpenSMOKE::OpenSMOKEVectorDouble& dy);

	unsigned int Print(const double t, const OpenSMOKE::OpenSMOKEVectorDouble& y);

	void AlgebraicEquations(OpenSMOKE::OpenSMOKEVectorBool& algebraic);

	OpenSMOKE::OpenSMOKEVectorDouble getVelocity()	const {return v_;};

	#if ASALI_USE_BZZ == 1
	void AlgebraicEquations(BzzVectorInt& algebraic);
	virtual void GetSystemFunctions(BzzVector &y, double t, BzzVector &dy);
	virtual void ObjectBzzPrint(void);
	virtual ~BVPSystem(){};
	#endif

private:

	double MWbulk_;
	double MWwall_;
	double cTotBulk_;
	double cTotWall_;
	double rhoBulk_;
	double cp_;
	double condBulk_;
	double condWall_;
	double rhoWall_;
	double etaMix_;
	double p_;
	double T0_;
	double epsi_;
	double Dh_;
	double Lcat_;
	double Linert_;
	double AsymptoticSh_;
	double alfa_;
	double alfaTemp_;
	double G_;
	double SD_;
	double av_;
	double h_;
	double t_;

	double rhoSolid_;
	double cpSolid_;
	double condSolid_;
	double newCondSolid_;

	double QfromGas_;
	double QfromSurface_;

	unsigned int NC_;
	unsigned int SURF_NP_;
	unsigned int SURF_NC_;
	unsigned int NE_;
	unsigned int NB_;
	unsigned int NP_;

	unsigned int iteration;

	std::string inert_;

	bool homogeneusReactions_ ;
	bool heterogeneusReactions_;
	bool energy_;

	OpenSMOKE::ThermodynamicsMap_CHEMKIN<double>&			thermodynamicsMap_;				//!< thermodynamic map
	OpenSMOKE::KineticsMap_CHEMKIN<double>&					kineticsMap_;					//!< kinetic map
	OpenSMOKE::ThermodynamicsMap_Surface_CHEMKIN<double>&	thermodynamicsSurfaceMap_;		//!< thermodynamic map
	OpenSMOKE::KineticsMap_Surface_CHEMKIN<double>&			kineticsSurfaceMap_;			//!< kinetic map
	OpenSMOKE::TransportPropertiesMap_CHEMKIN<double>& 		transportMap_;					//!< transport map

	OpenSMOKE::OpenSMOKEVectorDouble *omegaBulk_;
	OpenSMOKE::OpenSMOKEVectorDouble *omegaWall_;

	OpenSMOKE::OpenSMOKEVectorDouble *jGas_;
	OpenSMOKE::OpenSMOKEVectorDouble *jSolid_;

	OpenSMOKE::OpenSMOKEVectorDouble *teta_;

	OpenSMOKE::OpenSMOKEVectorDouble  Tbulk_;
	OpenSMOKE::OpenSMOKEVectorDouble  Twall_;
	OpenSMOKE::OpenSMOKEVectorDouble  jbulk_;
	OpenSMOKE::OpenSMOKEVectorDouble  jwall_;

	OpenSMOKE::OpenSMOKEVectorDouble  x0bulk_;

	OpenSMOKE::OpenSMOKEVectorDouble  z_;
	OpenSMOKE::OpenSMOKEVectorDouble  Dz_;

	OpenSMOKE::OpenSMOKEVectorDouble RfromGas_;
	OpenSMOKE::OpenSMOKEVectorDouble RfromSurface_;
	OpenSMOKE::OpenSMOKEVectorDouble Rsurface_;

	OpenSMOKE::OpenSMOKEVectorDouble diffG_;
	OpenSMOKE::OpenSMOKEVectorDouble Kmat_;
	OpenSMOKE::OpenSMOKEVectorDouble v_;

	OpenSMOKE::OpenSMOKEVectorDouble xBulk_;
	OpenSMOKE::OpenSMOKEVectorDouble xWall_;
	OpenSMOKE::OpenSMOKEVectorDouble cBulk_;
	OpenSMOKE::OpenSMOKEVectorDouble cWall_;
	OpenSMOKE::OpenSMOKEVectorDouble RsurfacePhases_;

	OpenSMOKE::OpenSMOKEVectorDouble dyOS_;
	OpenSMOKE::OpenSMOKEVectorDouble  yOS_;

	void HeatTransferCoefficient(const double z);
	void MassTransferCoefficient(const double z);

};


BVPSystem::BVPSystem(	OpenSMOKE::ThermodynamicsMap_CHEMKIN<double>&          thermodynamicsMap, 
						OpenSMOKE::KineticsMap_CHEMKIN<double>&                kineticsMap,
						OpenSMOKE::ThermodynamicsMap_Surface_CHEMKIN<double>&  thermodynamicsSurfaceMap, 
						OpenSMOKE::KineticsMap_Surface_CHEMKIN_Lumped<double>& kineticsSurfaceMap,
						OpenSMOKE::TransportPropertiesMap_CHEMKIN<double>&     transportMap):
	thermodynamicsMap_(thermodynamicsMap), 
	kineticsMap_(kineticsMap),
	thermodynamicsSurfaceMap_(thermodynamicsSurfaceMap), 
	kineticsSurfaceMap_(kineticsSurfaceMap),
	transportMap_(transportMap)
	{
	}

void BVPSystem::resize(const unsigned int NP)
{
	NP_ = NP;
	NC_ = thermodynamicsMap_.NumberOfSpecies();
	SURF_NC_ = thermodynamicsSurfaceMap_.number_of_site_species();
	SURF_NP_ = thermodynamicsSurfaceMap_.number_of_site_phases(0);
	NB_ = NC_ + NC_ + SURF_NC_ + 1 + 1;
	NE_ = (NC_ + NC_ + SURF_NC_ + 1 + 1)*NP_;
	
	omegaBulk_     = new OpenSMOKE::OpenSMOKEVectorDouble[NP_];
	jGas_          = new OpenSMOKE::OpenSMOKEVectorDouble[NP_];
	jSolid_        = new OpenSMOKE::OpenSMOKEVectorDouble[NP_];
	omegaWall_     = new OpenSMOKE::OpenSMOKEVectorDouble[NP_];
	teta_          = new OpenSMOKE::OpenSMOKEVectorDouble[NP_];

	for (unsigned int i=0;i<NP_;i++)
	{
		ChangeDimensions(NC_, &omegaBulk_[i], true);
		ChangeDimensions(NC_, &omegaWall_[i], true);
		ChangeDimensions(NC_, &jGas_[i],      true);
		ChangeDimensions(NC_, &jSolid_[i],    true);
		ChangeDimensions(SURF_NC_, &teta_[i], true);
	}

	ChangeDimensions(NC_, &RfromGas_, true);
	ChangeDimensions(NC_, &RfromSurface_, true);
	ChangeDimensions(SURF_NC_, &Rsurface_, true);

	ChangeDimensions(NP_, &Tbulk_, true);
	ChangeDimensions(NP_, &Twall_, true);
	ChangeDimensions(NP_, &jbulk_, true);
	ChangeDimensions(NP_, &jwall_, true);
	ChangeDimensions(NP_, &v_,     true);

	ChangeDimensions(NC_, &xBulk_, true);
	ChangeDimensions(NC_, &xWall_, true);
	ChangeDimensions(NC_, &cBulk_, true);
	ChangeDimensions(NC_, &cWall_, true);
	ChangeDimensions(NC_, &diffG_, true);
	ChangeDimensions(NC_, &Kmat_, true);

	ChangeDimensions(NE_, &dyOS_, true);
	ChangeDimensions(NE_, &yOS_, true);
}

void BVPSystem::setSolid(const double rhoSolid, const double condSolid, const double cpSolid)
{
	rhoSolid_	= rhoSolid;
	condSolid_	= condSolid;
	cpSolid_	= cpSolid;
}

void BVPSystem::setInert(const std::string inert)
{
	inert_		= inert;
}

void BVPSystem::setReactorGeometry( const double alfa,         const double epsi, 
									const double AsymptoticSh, const double Lcat, const double Linert,
									const double Dh,           const double G)
{
	alfaTemp_		= alfa;
	Dh_				= Dh;
	epsi_			= epsi;
	Lcat_			= Lcat;
	Linert_			= Linert;
	AsymptoticSh_	= AsymptoticSh;
	G_				= G;
	av_				= 4.*epsi/Dh;
}

void BVPSystem::setFeedValue(const double p, const double T0,
							 const OpenSMOKE::OpenSMOKEVectorDouble x0bulk)
{
	p_				= p;
	T0_				= T0;
	ChangeDimensions(x0bulk.Size(), &x0bulk_, true);
	for (unsigned int j=1;j<=x0bulk.Size();j++)
		x0bulk_[j] = x0bulk[j];
}

void BVPSystem::setGrid(const OpenSMOKE::OpenSMOKEVectorDouble z)
{
	ChangeDimensions(z.Size(), &z_, true);
	for (unsigned int k=1;k<=z_.Size();k++)
		z_[k] = z[k];
	
	ChangeDimensions((NP_- 1), &Dz_, true);
	for (unsigned int k=1;k<=Dz_.Size();k++)
		Dz_[k] = z_[k+1] - z_[k];
}

void BVPSystem::HeatTransferCoefficient(const double z)
{
	double Re = G_*Dh_/(etaMix_*epsi_);
	double Pr = cp_*etaMix_/condBulk_;

	double Nu;
	double zNew;
	double zStar;
	if ( z > Linert_)
	{
		zNew  = std::max( z - Linert_, 1e-06);
		zStar = zNew/(Dh_*Re*Pr);
		zStar = fabs(zStar);
		Nu    = AsymptoticSh_ + 8.827*pow((1000.*zStar),-0.545)*exp(-48.2*zStar);
	}
	else
	{
		Nu = AsymptoticSh_;
	}

	h_ = Nu*condBulk_/Dh_;
}

void BVPSystem::MassTransferCoefficient(const double z)
{
	double Re = G_*Dh_/(etaMix_*epsi_);
	OpenSMOKE::OpenSMOKEVectorDouble Sc(NC_);
	OpenSMOKE::OpenSMOKEVectorDouble Sh(NC_);
	double zNew;
	double zStar;
	for (unsigned int i=1;i<=NC_;i++)
	{
		Sc[i] = etaMix_/(rhoBulk_*diffG_[i]);
		if ( z > Linert_)
		{
			zNew  = std::max( z - Linert_, 1e-06);
			zStar = zNew/(Dh_*Re*Sc[i]);
			zStar = fabs(zStar);
			Sh[i] = AsymptoticSh_ + 6.874*pow((1000.*zStar),-0.488)*exp(-57.2*zStar);
		}
		else
		{
			Sh[i] = AsymptoticSh_;
		}
		Kmat_[i] = Sh[i]*diffG_[i]/Dh_;
	}
}

#if ASALI_USE_BZZ == 1
void BVPSystem::GetSystemFunctions(BzzVector &y, double t, BzzVector &dy)
{
	FromBzzToOS(y,yOS_);
	#include "BVPequations.H"
	ObjectBzzPrint();
	FromOSToBzz(dyOS_,dy);
}

void BVPSystem::ObjectBzzPrint(void)
{
	#include "printIntegration.H"
}

void BVPSystem::AlgebraicEquations(BzzVectorInt& algebraic)
{
	ChangeDimensions(NE_, &algebraic, true);
	unsigned int counter = 1;
	for (unsigned int i=0;i<NP_;i++)
	{
		if ( i == 0)
		{
			for (unsigned int j=1;j<=NC_;j++)
				algebraic[counter++] = 0;
			for (unsigned int j=1;j<=NC_;j++)
				algebraic[counter++] = 0;
			for (unsigned int j=1;j<=SURF_NC_;j++)
			{
				if ( thermodynamicsSurfaceMap_.NamesOfSpecies()[j-1+thermodynamicsSurfaceMap_.number_of_gas_species()] != "Rh(s)" )
				{
					algebraic[counter++] = 1;
				}
				else
				{
					algebraic[counter++] = 0;
				}
			}
			algebraic[counter++] = 0;
			algebraic[counter++] = 0;
		}
		else if ( i == (NP_ - 1) )
		{
			for (unsigned int j=1;j<=NC_;j++)
				algebraic[counter++] = 0;
			for (unsigned int j=1;j<=NC_;j++)
				algebraic[counter++] = 0;
			for (unsigned int j=1;j<=SURF_NC_;j++)
			{
				if ( thermodynamicsSurfaceMap_.NamesOfSpecies()[j-1+thermodynamicsSurfaceMap_.number_of_gas_species()] != "Rh(s)" )
				{
					algebraic[counter++] = 1;
				}
				else
				{
					algebraic[counter++] = 0;
				}
			}
			algebraic[counter++] = 0;
			algebraic[counter++] = 0;
		}
		else
		{
			for (unsigned int j=1;j<=NC_;j++)
			{
				if ( thermodynamicsMapXML->NamesOfSpecies()[j-1] != inert_ )
				{
					algebraic[counter++] = 1;
				}
				else
				{
					algebraic[counter++] = 0;
				}
			}
			for (unsigned int j=1;j<=NC_;j++)
				algebraic[counter++] = 0;
			for (unsigned int j=1;j<=SURF_NC_;j++)
			{
				if ( thermodynamicsSurfaceMap_.NamesOfSpecies()[j-1+thermodynamicsSurfaceMap_.number_of_gas_species()] != "Rh(s)" )
				{
					algebraic[counter++] = 1;
				}
				else
				{
					algebraic[counter++] = 0;
				}
			}
			algebraic[counter++] = 1;
			algebraic[counter++] = 1;
		}
	}
}
#endif

unsigned int BVPSystem::Equations(const double t, const OpenSMOKE::OpenSMOKEVectorDouble& y, OpenSMOKE::OpenSMOKEVectorDouble& dy)
{
	ChangeDimensions(NE_, &yOS_,  true);
	ChangeDimensions(NE_, &dy,    true);
	ChangeDimensions(NE_, &dyOS_, true);

	for (unsigned int i=1;i<=NE_;i++)
		yOS_[i] = y[i];

	#include "BVPequations.H"

	for (unsigned int i=1;i<=NE_;i++)
		dy[i] = dyOS_[i];
	
	return 0;
}



unsigned int BVPSystem::Print(const double t, const OpenSMOKE::OpenSMOKEVectorDouble& y)
{
	t_ = t;
	#include "printIntegration.H"
	return 0;
}

void BVPSystem::AlgebraicEquations(OpenSMOKE::OpenSMOKEVectorBool& algebraic)
{
	ChangeDimensions(NE_, &algebraic, true);
	unsigned int counter = 1;
	for (unsigned int i=0;i<NP_;i++)
	{
		if ( i == 0)
		{
			for (unsigned int j=1;j<=NC_;j++)
				algebraic[counter++] = true;
			for (unsigned int j=1;j<=NC_;j++)
				algebraic[counter++] = true;
			for (unsigned int j=1;j<=SURF_NC_;j++)
			{
				if ( thermodynamicsSurfaceMap_.NamesOfSpecies()[j-1+thermodynamicsSurfaceMap_.number_of_gas_species()] != "Rh(s)" )
				{
					algebraic[counter++] = false;
				}
				else
				{
					algebraic[counter++] = true;
				}
			}
			algebraic[counter++] = true;
			algebraic[counter++] = true;
		}
		else if ( i == (NP_ - 1) )
		{
			for (unsigned int j=1;j<=NC_;j++)
				algebraic[counter++] = true;
			for (unsigned int j=1;j<=NC_;j++)
				algebraic[counter++] = true;
			for (unsigned int j=1;j<=SURF_NC_;j++)
			{
				if ( thermodynamicsSurfaceMap_.NamesOfSpecies()[j-1+thermodynamicsSurfaceMap_.number_of_gas_species()] != "Rh(s)" )
				{
					algebraic[counter++] = false;
				}
				else
				{
					algebraic[counter++] = true;
				}
			}
			algebraic[counter++] = true;
			algebraic[counter++] = true;
		}
		else
		{
			for (unsigned int j=1;j<=NC_;j++)
			{
				if ( thermodynamicsMapXML->NamesOfSpecies()[j-1] != inert_ )
				{
					algebraic[counter++] = false;
				}
				else
				{
					algebraic[counter++] = true;
				}
			}
			for (unsigned int j=1;j<=NC_;j++)
				algebraic[counter++] = true;
			for (unsigned int j=1;j<=SURF_NC_;j++)
			{
				if ( thermodynamicsSurfaceMap_.NamesOfSpecies()[j-1+thermodynamicsSurfaceMap_.number_of_gas_species()] != "Rh(s)" )
				{
					algebraic[counter++] = false;
				}
				else
				{
					algebraic[counter++] = true;
				}
			}
			algebraic[counter++] = false;
			algebraic[counter++] = false;
		}
	}
}

void BVPSystem::start()
{
	std::cout << "\n######################################" << std::endl;
	std::cout << "# DAE system:                 START  #" << std::endl;
	std::cout << "######################################\n" << std::endl;
}

void BVPSystem::end()
{
	delete [] omegaBulk_;
	delete [] omegaWall_;
	delete [] jGas_;
	delete [] jSolid_;
	delete [] teta_;
	std::cout << "\n######################################" << std::endl;
	std::cout << "# DAE system:                 END    #" << std::endl;
	std::cout << "######################################\n" << std::endl;
}

}
