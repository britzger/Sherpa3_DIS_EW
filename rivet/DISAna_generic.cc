// -*- C++ -*-

#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/DISKinematics.hh"
#include "Rivet/Projections/DISFinalState.hh"
namespace Rivet {
  /// @brief Add a short analysis description here
  class DISAna_generic : public Analysis {
  public:
    /// Constructor
    DEFAULT_RIVET_ANALYSIS_CTOR(DISAna_generic);
    /// Book histograms and initialise projections before the run
    void init() {
    

      // Initialise and register projections. Note that the definition
      // of the scattered lepton can be influenced by sepcifying
      // options as declared in the .info file.
      DISLepton lepton(options());
      declare(lepton, "Lepton");
      declare(DISKinematics(lepton), "Kinematics");
      declare(FinalState(), "FS");
      declare(Beam(), "Beam");   // NEU !
     const DISFinalState& disfs = declare(DISFinalState(DISFinalState::BoostFrame::LAB), "DISFS"); //vorher auskommentiert
      
  //yoda plot und cc datei schicken!!!

      // Book histograms
      book(_hist_Higgs_pt_, "Higgs_pt", 20, -40, 500);
      book(_hist_Higgs_eta_, "Higgs_eta", 20, -9, 0.5);
      book(_hist_Higgs_theta_, "Higgs_theta", 20, 1.9, 3.4);
      book(_hist_Z_pt_, "Z_pt", 20, -40, 400);
      book(_hist_Z_eta_, "Z_eta", 20, -9, 0.5);
      book(_hist_Z_theta_, "Z_theta", 20, 0, 3.4);
      book(_hist_W_pt_, "W_pt", 20, -40, 300);
      book(_hist_W_eta_, "W_eta", 20, -9, 3);
      book(_hist_W_theta_, "W_theta", 20, 1.9, 3.4);
      book(_hist_sq_eta_, "sq_eta", 20, -9, 0.5);
      book(_hist_Q2_, "Q2",20, 0, 3.5);
      book(_hist_y_, "y", 20, -0.5,1.5);
      book(_hist_x_, "x", 20, -5,-0.5);
    
    }
      
      
       /// Perform the per-event analysis
      void analyze(const Event& event) {
         // Get the DIS kinematics
         using namespace std;

         // Momentum of the scattered lepton
         const DISLepton& dislep = apply<DISLepton>(event,"Lepton");
         if ( dislep.failed() ) return;
         const Particle _outLepton = dislep.out();
         const Particle _inLepton  = dislep.in();

         const FourMomentum pLepIn  = _inLepton.momentum();
         const FourMomentum pLepOut = _outLepton.momentum();
         const FourMomentum pGamma = pLepIn - pLepOut;
         const double SigE = _outLepton.E() + _outLepton.pz();
         const double E0     = _inLepton.E();
         //const double sqrts  = event.sqrtS();
         //double Ep     = sqrts*sqrts/4./E0;
         const double Ep     = 7000;             // Obacht! Dieser Wert muss richtig gesetzt sein!
         const double Q2 = -pGamma.mass2();
         const double y  = 1. - SigE/(2*_inLepton.E());
         const double x  = Q2/4./E0/y/Ep ;
         //cout<<" - Q2: "<<Q2<<"\ty="<<y<<"\tx: "<<x<<"\tE0: "<<E0<<"\tEp: "<<Ep<<endl;

         // phase space cuts (todo)
         if ( y>0.95 )  vetoEvent;
         if ( y<0.01 )  vetoEvent;

         // four-vector photon
         const FourMomentum q  = pGamma;

     

      // Extract the particles other than the lepton
      const FinalState& fs = apply<FinalState>(event, "FS");
      ConstGenParticlePtr dislepGP = dislep.out().genParticle();

      // select empty hemisphere events
      bool accepted = true;
      for(const Particle& p: fs.particles()) {
        ConstGenParticlePtr loopGP = p.genParticle();
        if (loopGP == dislepGP) continue;

        double pz2q2 =  -p.dot(q) ;
        if ( pz2q2 > 0 ) {
           accepted=false; // no particles in current hemisphere
           break;
        }
      }
         //DIS-Plots fillen
         _hist_Q2_   ->fill( log10(Q2) );
          //cout<<"Q2"<<endl;
         _hist_x_   ->fill( log10(x) );
         //cout<<"x"<<endl;
         _hist_y_   ->fill( y );
         //cout<<"y"<<endl;

         //Higgs Variablen     
        for(const Particle& particle : fs.particles()) {
         if ( particle.pid() == 25 ) {
            double H_pt    = particle.pt();
            double H_eta   = particle.eta();
            double H_theta = particle.theta();
            //cout<<"H_pt: "<<H_pt<<"\teta: "<<H_eta<<endl;
            // Hier werden dann die Histogramme mit den Higgs Variablen gefüllt
             _hist_Higgs_pt_  ->fill( H_pt );
             _hist_Higgs_eta_  ->fill( H_eta );
             _hist_Higgs_theta_  ->fill( H_theta );
            }
          //Z Variablen
          if ( particle.pid() == 23 ) {
            double Z_pt    = particle.pt();
            double Z_eta   = particle.eta();
            double Z_theta = particle.theta();
         
             _hist_Z_pt_  ->fill( Z_pt );
             _hist_Z_eta_  ->fill( Z_eta );
             _hist_Z_theta_  ->fill( Z_theta );
            }
           //W Variablen
           if ( particle.pid() == -24 ) {
            double W_pt    = particle.pt();
            double W_eta   = particle.eta();
            double W_theta = particle.theta();
         
             _hist_W_pt_  ->fill( W_pt );
             _hist_W_eta_  ->fill( W_eta );
             _hist_W_theta_  ->fill( W_theta );
            }
            //see Quark
           if ( particle.pid() < 6 ) {
            
            double sq_eta   = particle.eta();
         
             _hist_sq_eta_  ->fill( sq_eta );
            
            }
         }


     

      // break if there is radiation in the current hemisphere
      if ( !accepted ) vetoEvent;
      
    }


    /// Normalise histograms etc., after the run
    void finalize() {
              
       scale(_hist_Higgs_pt_, crossSection()/picobarn/sumW());
       scale(_hist_Higgs_eta_, crossSection()/picobarn/sumW());
       scale(_hist_Higgs_theta_, crossSection()/picobarn/sumW());
       scale(_hist_Z_pt_, crossSection()/picobarn/sumW());
       scale(_hist_Z_eta_, crossSection()/picobarn/sumW());
       scale(_hist_Z_theta_, crossSection()/picobarn/sumW());
       scale(_hist_W_pt_, crossSection()/picobarn/sumW());
       scale(_hist_W_eta_, crossSection()/picobarn/sumW());
       scale(_hist_W_theta_, crossSection()/picobarn/sumW());
       scale(_hist_Q2_, crossSection()/picobarn/sumW());
       scale(_hist_y_, crossSection()/picobarn/sumW());
       scale(_hist_x_, crossSection()/picobarn/sumW());
      
       scale(_hist_sq_eta_, crossSection()/picobarn/sumW());
     
    }

    //@}


    /// The histograms.
     //using Histo1DPtr = rivet_shared_ptr< Wrapper< YODA::Histo1D > >
     Histo1DPtr  _hist_Higgs_pt_;
     Histo1DPtr  _hist_Higgs_eta_;
     Histo1DPtr  _hist_Higgs_theta_;
     Histo1DPtr  _hist_Z_pt_;
     Histo1DPtr  _hist_Z_eta_;
     Histo1DPtr  _hist_Z_theta_;
     Histo1DPtr  _hist_W_pt_;
     Histo1DPtr  _hist_W_eta_;
     Histo1DPtr  _hist_W_theta_;
     Histo1DPtr  _hist_Q2_;
     Histo1DPtr  _hist_x_;
     Histo1DPtr  _hist_y_;
    
     Histo1DPtr  _hist_sq_eta_;
    
  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(DISAna_generic);


}
    

