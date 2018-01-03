#pragma once

#include <vector>
#include <yaml-cpp/yaml.h>

#include "cpu/decoder/encoder_decoder.h"
#include "cpu/nematus/encoder.h"
#include "cpu/nematus/decoder.h"
#include "cpu/nematus/model.h"

#include "cpu/mblas/matrix.h"

namespace amunmt {

class Sentence;

namespace CPU {
namespace Nematus {

class EncoderDecoder : public CPUEncoderDecoderBase {
  private:
    using EDState = EncoderDecoderState;

  public:
    EncoderDecoder(const God &god,
    			   const std::string& name,
                   const YAML::Node& config,
                   unsigned tab,
                   const Nematus::Weights& model,
                   const Search &search);

    virtual void Decode(EncOutPtr encOut,
                        const State& in,
                        State& out,
                        const std::vector<unsigned>& beamSizes);

    virtual void BeginSentenceState(EncOutPtr encOut, State& state, unsigned batchSize);

    virtual void Encode(const SentencesPtr &sources);

    virtual void AssembleBeamState(const State& in,
                                   const Hypotheses& beam,
                                   State& out) const;

    void GetAttention(mblas::Matrix& Attention);
    mblas::Matrix& GetAttention();

    unsigned GetVocabSize() const;

    BaseMatrix& GetProbs();

    void Filter(const std::vector<unsigned>& filterIds);

  protected:
    const Nematus::Weights& model_;
    std::unique_ptr<Nematus::Encoder> encoder_;
    std::unique_ptr<Nematus::Decoder> decoder_;
};


}
}
}
