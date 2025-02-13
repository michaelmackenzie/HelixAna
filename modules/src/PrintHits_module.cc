// Print tracker hit information
// Original author: Michael MacKenzie (2025)
//

// art
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/OptionalAtom.h"
#include "fhiclcpp/types/OptionalSequence.h"
#include "canvas/Utilities/InputTag.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"

// mu2e
#include "Offline/RecoDataProducts/inc/ComboHit.hh"

// std
#include <vector>
#include <iostream>
#include <string>

namespace mu2e {
  class PrintHits : public art::EDAnalyzer {
  public:
    using Name=fhicl::Name;
    using Comment=fhicl::Comment;
    struct Config {
      fhicl::Atom<art::InputTag> hitCollTag{Name("hitCollTag"), Comment("Hit collection tag"), "makePH"};
    };
    using Parameters = art::EDAnalyzer::Table<Config>;
    explicit PrintHits(const Parameters& conf);
    void analyze(const art::Event& evt) override;
  private:
    art::InputTag _hitCollTag;
  };

  PrintHits::PrintHits(const Parameters& config )  :
    art::EDAnalyzer{config},
    _hitCollTag(config().hitCollTag())
  {
  }

  void PrintHits::analyze(const art::Event& event) {
    // retrieve the input collections
    auto hitColl = event.getValidHandle<ComboHitCollection>(_hitCollTag);
    auto const& hits = *hitColl;
    printf("[PrintHits::%s] Printing combo hit collection %s:\n", __func__, _hitCollTag.encode().c_str());
    for(auto hit : hits) {
      printf(" %4i: flag = %s, index = %4i, energy = %.4f, x = %.3f, y = %.3f, z = %.3f\n",
             (int) hit.strawId().asUint16(), hit.flag().hex().c_str(), (int) hit.index(), hit.energyDep(),
             hit.pos().x(), hit.pos().y(), hit.pos().z()
             );
    }
  }
}
DEFINE_ART_MODULE(mu2e::PrintHits)
