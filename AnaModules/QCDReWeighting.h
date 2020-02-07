#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <utility>

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TChain.h"

#include "Math/QuantFuncMathCore.h"
#include "TMath.h"

struct QCDSampleInfo
{
  std::string QCDTag;
  double weight;
  double xsec_in, nevents_in, lumi_in, kf_in;
  TChain *chain;
};

class QCDSampleWeight
{
 public:
  std::vector<QCDSampleInfo> QCDSampleInfos;
  void QCDSampleInfo_push_back( std::string tag, double xsec, double nevents, double lumi, double kf, const TString &inputFileList, std::string TrainMode );
  void GenLatexTable();
  float xsecWeightCalcLHEJets(int bit, int lheNJets);
 private:
  bool FillChain(TChain *chain, const TString &inputFileList, std::string tag);
  void removeCharsFromString( std::string &str, char* charsToRemove );
  const std::map<std::string, std::pair <int, float> > mStat = 
  {
    //Map between dtag and total number of events per process (Npos - Nneg)
    //including Negative weight events

    { "MC13TeV_Zh_amass20", std::make_pair(487600, 0.8594*0.067316) },     
    { "MC13TeV_Zh_amass25", std::make_pair(493600, 0.8594*0.067316) }, //389360, 1.37*0.2132) }, 
    { "MC13TeV_Zh_amass30", std::make_pair(498500, 0.8594*0.067316) }, //389560, 1.37*0.2132) }, 
    { "MC13TeV_Zh_amass40", std::make_pair(500000, 0.8594*0.067316) }, //398000, 1.37*0.2132) },  
    { "MC13TeV_Zh_amass50", std::make_pair(498600, 0.8594*0.067316) }, //399000, 1.37*0.2132) },   
    { "MC13TeV_Zh_amass60", std::make_pair(500000, 0.8594*0.067316) }, //388720, 1.37*0.2132) },

    //h->aa->4b Signal
    { "MC13TeV_Wh_amass12", std::make_pair(377600, 1.37*0.2132) }, //3.62818e-06 },
    { "MC13TeV_Wh_amass15", std::make_pair(379120, 1.37*0.2132) }, //3.61363e-06 },
    { "MC13TeV_Wh_amass20", std::make_pair(383440, 1.37*0.2132) },
    { "MC13TeV_Wh_amass25", std::make_pair(389360, 1.37*0.2132) },
    { "MC13TeV_Wh_amass30", std::make_pair(389560, 1.37*0.2132) },
    { "MC13TeV_Wh_amass40", std::make_pair(398000, 1.37*0.2132) },
    { "MC13TeV_Wh_amass50", std::make_pair(399000, 1.37*0.2132) },
    { "MC13TeV_Wh_amass60", std::make_pair(388720, 1.37*0.2132) },

    //Single top
    { "MC13TeV_SingleT_s"  , std::make_pair(622990, 3.362) }, //0.193558 },
    { "MC13TeV_SingleT_atW", std::make_pair(6933094, 35.6) }, //0.184169 },
    { "MC13TeV_SingleT_tW" , std::make_pair(6952830, 35.6) }, //0.183646 },
    { "MC13TeV_SingleT_at" , std::make_pair(37904304, 80.95) }, //67240808 }, //0.0765989 },
    { "MC13TeV_SingleT_t"  , std::make_pair(67240808, 136.02) }, //37904304 }, //0.0725545 },
    //TTJets
    { "MC13TeV_TTJets", std::make_pair(77081156, 831.76) }, // Powheg sample
    { "MC13TeV_TTJets_slt"      , std::make_pair(61973977, 182.7) }, //0.105736*0.1929 },
    { "MC13TeV_TTJets_slt_ext1" , std::make_pair(61973977, 182.7) }, //0.105736*0.8071 },
    { "MC13TeV_TTJets_slat"     , std::make_pair(60210394, 182.7) }, // 0.108833*0.1984 },
    { "MC13TeV_TTJets_slat_ext1", std::make_pair(60210394, 182.7) }, //0.109933*0.8016 },
    { "MC13TeV_TTJets_dl"       , std::make_pair(30444678, 88.2877) }, //0.104012*0.2002 },
    { "MC13TeV_TTJets_dl_ext1"  , std::make_pair(30444678, 88.2877) }, // 0.104012*0.7998 },
    //WJets
    { "MC13TeV_WJets"     , std::make_pair(85772627, 50690) }, //280929804 }, //30.7868*0.3425 },
    { "MC13TeV_WJets_ext2", std::make_pair(85772627, 50690) }, //280929804 }, // 30.7868*0.6575 },
    { "MC13TeV_W1Jets"     , std::make_pair(44954933, 9493) },
    { "MC13TeV_W2Jets"     , std::make_pair(60039784, 3120) },
    { "MC13TeV_W2Jets_ext1", std::make_pair(60039784, 3120) },
    { "MC13TeV_W3Jets"     , std::make_pair(59067548, 942.3) },
    { "MC13TeV_W3Jets_ext1", std::make_pair(59067548, 942.3) },
    { "MC13TeV_W4Jets"     , std::make_pair(29995313, 524.2) },
    { "MC13TeV_W4Jets_ext1", std::make_pair(29995313, 524.2) },
    { "MC13TeV_W4Jets_ext2", std::make_pair(29995313, 524.2) },
    /*
    { "MC13TeV_WJets_ext3" , std::make_pair(280929804, 61526.7) },
    { "MC13TeV_WJets_ext4" , std::make_pair(280929804, 61526.7) },
    { "MC13TeV_WJets_ext5" , std::make_pair(280929804, 61526.7) },
    { "MC13TeV_WJets_ext6" , std::make_pair(280929804, 61526.7) },
    { "MC13TeV_WJets_ext7" , std::make_pair(280929804, 61526.7) },
    { "MC13TeV_WJets_ext8" , std::make_pair(280929804, 61526.7) },
    { "MC13TeV_WJets_ext9" , std::make_pair(280929804, 61526.7) },
    { "MC13TeV_WJets_ext10", std::make_pair(280929804, 61526.7) },
    */
    //DY
    { "MC13TeV_DYJetsToLL_10to50"      , std::make_pair(35291566, 18610) }, //12.9777*0.434 },
    { "MC13TeV_DYJetsToLL_10to50_ext1" , std::make_pair(35291566, 18610) }, //12.9777*0.566 },
    { "MC13TeV_DY1JetsToLL_10to50" , std::make_pair(39840774, 725) },
    { "MC13TeV_DY2JetsToLL_10to50" , std::make_pair(19442927, 394.5) },
    { "MC13TeV_DY3JetsToLL_10to50" , std::make_pair(4964197, 96.47) },
    { "MC13TeV_DY4JetsToLL_10to50" , std::make_pair(2087849, 34.84) },
    { "MC13TeV_DYJetsToLL_50toInf_ext1", std::make_pair(145803217, 4895) }, //81781064 }, //2.52854 },
    { "MC13TeV_DYJetsToLL_50toInf_ext2", std::make_pair(145803217, 4895) },
    { "MC13TeV_DY1JetsToLL_50toInf" , std::make_pair(61805710, 1016) },
    { "MC13TeV_DY2JetsToLL_50toInf" , std::make_pair(19970551, 331.4) },
    { "MC13TeV_DY3JetsToLL_50toInf" , std::make_pair(5856110, 96.36) },
    { "MC13TeV_DY4JetsToLL_50toInf" , std::make_pair(4197868, 51.4) },
    //{ "MC13TeV_DYJetsToLL_50toInf_ext3", std::make_pair(238454920, 5765.4) },
    //{ "MC13TeV_DYJetsToLL_50toInf_ext4", std::make_pair(238454920, 5765.4) },
    //{ "MC13TeV_DYJetsToLL_50toInf_ext5", std::make_pair(238454920, 5765.4) },
    //{ "MC13TeV_DYJetsToLL_50toInf_ext6", std::make_pair(238454920, 5765.4) },
    //TG, TTG, TTW, TTZ
    { "MC13TeV_TGJets"      , std::make_pair(368562, 2.967) }, //0.288736*0.25 },
    { "MC13TeV_TGJets_ext1" , std::make_pair(368562, 2.967) }, //0.288736*0.75 },
    { "MC13TeV_TTGJets"     , std::make_pair(1577833, 3.697) }, // 0.0840393 },
    { "MC13TeV_TTWJetslnu"  , std::make_pair(1603527, 0.2043) }, //0.00456969 },
    //    { "MC13TeV_TTZJets2l2nu", std::make_pair(927976, 0.2529) }, //0.00977476 },
    //Di-boson
    { "MC13TeV_WW2l2nu"     , std::make_pair(1999000, 12.178) }, // 0.218503 },
    { "MC13TeV_WWlnu2q"     , std::make_pair(8997800, 49.997) }, //0.199298*0.5 },
    { "MC13TeV_WWlnu2q_ext1", std::make_pair(8997800, 49.997) }, //0.199298*0.5 },
    { "MC13TeV_WZ"          , std::make_pair(3871065, 47.13) }, //0.436678*0.25 },
    { "MC13TeV_WZ_ext1"     , std::make_pair(3871065, 47.13) }, //0.436678*0.75 },
    { "MC13TeV_ZZ"          , std::make_pair(1988098, 16.523) }, //0.298089*0.5 },
    { "MC13TeV_ZZ_ext1"     , std::make_pair(1988098, 16.523) }, //0.298089*0.5 },
    //Tri-boson
    { "MC13TeV_ZZZ"   , std::make_pair(213197, 0.01398) }, //0.0355368 },
    { "MC13TeV_WZZ"   , std::make_pair(216366, 0.05565) }, //0.0267381 },
    { "MC13TeV_WWZ"   , std::make_pair(221468, 0.1651) }, //0.00922509 },
    { "MC13TeV_WWW_4F", std::make_pair(210538, 0.2086) }, //0.00235191 },
    //QCD
    //{ "MC13TeV_QCD_Pt50To80_EMEnr"  , std::make_pair(23474171 },
    { "MC13TeV_QCD_Pt80To120_EMEnr" , std::make_pair(35841783, 350000) }, // 350.246 },
    { "MC13TeV_QCD_Pt120To170_EMEnr", std::make_pair(35817281, 62964) }, //63.0513 },
    { "MC13TeV_QCD_Pt170To300_EMEnr", std::make_pair(11540163, 18810) }, //58.4617 },
    { "MC13TeV_QCD_Pt300ToInf_EMEnr", std::make_pair(7373633, 1350) }, //6.56669 },
    { "MC13TeV_QCD_Pt20ToInf_MuEnr" , std::make_pair(22094081, 302672) } //491.35 }
  };
};
