#include "Rivet/Projections/InitialQuarks.hh"
#include "Rivet/Analysis.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/UnstableParticles.hh"
namespace Rivet {
   class Hcc_generic : public Analysis {
   public:
      /// Constructor                                           
      DEFAULT_RIVET_ANALYSIS_CTOR(Hcc_generic);
      
      
      void init() {
         
         FinalState fs;
         declare(fs, "FS");
         
         declare(UnstableParticles(), "UPS");
         
         declare(FastJets(fs, FastJets::ANTIKT, 1.0), "Jets");
         
       
         book(_hist_pdg423_pT, "pdg423_pT",  20, -100, 200);
         book(_hist_pdg433_pT, "pdg433_pT",  20, -100, 200);
         book(_hist_cHadrons_pT, "cHadrons_pT",  20, -100, 200); 
         book(_hist_pdg411_pT, "pdg411_pT",  20, -100, 200);
         book(_hist_pdg421_pT, "pdg421_pT",  20, -100, 200);
         book(_hist_pdg431_pT, "pdg431_pT",  20, -100, 200);
         book(_hist_pdg413_pT, "pdg413_pT",  20, -100, 200);
         book(_hist_pdg4122_pT, "pdg4122_pT",  20, -100, 200);
         
         book(_hist_num_cTags, "num_cTags", 20, 0, 6);
         book(_hist_cTagged_jet_pT, "cTagged_jet_pT",  20, -100, 200);
         book(_hist_cTagged_jet_theta, "cTagged_jet_theta", 20, 0, 4);
         book(_hist_cTagged_jet_eta, "cTagged_jet_eta", 20, -5, 3);
         
         book(_hist_c_theta, "c_theta", 20, 0, 4);
         book(_hist_c_eta, "c_eta", 20, -6, 3);
         book(_hist_c_pt, "c_pt", 20, -100, 200);
      }






      void analyze(const Event & e){
         using namespace std;
         
         const FinalState& fs = apply<FinalState>(e, "FS");
         cout<<"........START"<<endl;
         

         Particles allpfs = fs.particlesByPt();
         if (allpfs.empty()) vetoEvent;
         
         /**charm quarks from Higgs decay*/
         const Particles& particles = e.allParticles();
         for ( const Particle& par : particles ) {
            if (par.pid()==25){
               for ( const Particle& pc : par.children() ){
                  cout<<"this is the child of H: "<<pc.pid()<<endl; //will print charm and anti-charm
                  if(pc.pid()==4){
                     _hist_c_theta->fill(pc.theta());
                     _hist_c_eta->fill(pc.eta());
                     _hist_c_pt->fill(pc.pt());
                  }                  
               }
            }
         }

         
         /**Ctagged jets*/
         const Jets jets = apply<FastJets>(e, "Jets").jetsByPt();
         for (const Jet& j : jets) {
            if (j.cTags().size()>= 1) {
               _hist_num_cTags->fill(j.cTags().size());     //number of ctagged particles in a jet
            }
            if (j.cTagged()){
               _hist_cTagged_jet_pT->fill(j.pt());
               _hist_cTagged_jet_eta->fill(j.eta());
               _hist_cTagged_jet_theta->fill(j.theta());
            }
         }

         
         
         
      
         /**Charmed Hadrons from Higgs decay*/
         const UnstableParticles& ufs = apply<UnstableParticles>(e, "UPS");
         for (const Particle& p : ufs.particles()){
            if (p.hasCharm()){
               //does not have an ancestor -> first produced in the interaction
               if(p.ancestors().empty()) {
                  cout<<"Charmed Hadrons with no ancestor: "<<p.pid()<<endl;
                  _hist_cHadrons_pT->fill(p.pt());
                  
                  switch (abs(p.abspid())){
                  case 433:
                     _hist_pdg433_pT->fill(p.pt());
                     break;
                  case 423:
                     _hist_pdg423_pT->fill(p.pt());
                     break;
                  case 411:                                                   
                     _hist_pdg411_pT->fill(p.pt());                                
                     break; 
                  case 421:
                     _hist_pdg421_pT->fill(p.pt());
                     break; 
                  case 431:
                     _hist_pdg431_pT->fill(p.pt());
                     break; 
                  case 413:
                     _hist_pdg413_pT->fill(p.pt());
                     break; 
                  case 4122:
                     _hist_pdg4122_pT->fill(p.pt());
                  }
               }
            }
         }
         cout<<"..........END EVENT"<<endl;
      }
      void finalize(){
         scale(_hist_c_eta, crossSection()/picobarn/sumW());
         scale(_hist_c_theta, crossSection()/picobarn/sumW());
         scale(_hist_c_pt, crossSection()/picobarn/sumW());
         scale(_hist_cTagged_jet_theta, crossSection()/picobarn/sumW());
         scale(_hist_cTagged_jet_eta, crossSection()/picobarn/sumW());
         scale(_hist_cTagged_jet_pT, crossSection()/picobarn/sumW());
         scale(_hist_num_cTags, crossSection()/picobarn/sumW());
         scale(_hist_cHadrons_pT, crossSection()/picobarn/sumW());
         scale(_hist_pdg433_pT,crossSection()/picobarn/sumW());
         scale(_hist_pdg423_pT,crossSection()/picobarn/sumW());
         scale(_hist_pdg411_pT, crossSection()/picobarn/sumW());
         scale(_hist_pdg421_pT, crossSection()/picobarn/sumW());
         scale(_hist_pdg431_pT, crossSection()/picobarn/sumW());
         scale(_hist_pdg413_pT, crossSection()/picobarn/sumW());
         scale(_hist_pdg4122_pT, crossSection()/picobarn/sumW());
          }
   
      Histo1DPtr _hist_cTagged_jet_theta;
      Histo1DPtr _hist_cTagged_jet_eta;
      Histo1DPtr _hist_cTagged_jet_pT;
      Histo1DPtr _hist_num_cTags;
      Histo1DPtr _hist_pdg411_pT;
      Histo1DPtr _hist_pdg421_pT;
      Histo1DPtr _hist_pdg431_pT;
      Histo1DPtr _hist_pdg413_pT;
      Histo1DPtr _hist_pdg4122_pT;
      Histo1DPtr _hist_pdg433_pT;
      Histo1DPtr _hist_pdg423_pT;
      Histo1DPtr _hist_cHadrons_pT;
      Histo1DPtr _hist_c_eta;
      Histo1DPtr _hist_c_theta;
      Histo1DPtr _hist_c_pt;

   };
   DECLARE_RIVET_PLUGIN(Hcc_generic);
}
