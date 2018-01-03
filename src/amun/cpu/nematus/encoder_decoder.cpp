#include "cpu/nematus/encoder_decoder.h"

#include <vector>
#include <yaml-cpp/yaml.h>

#include "common/sentence.h"
#include "common/sentences.h"
#include "common/hypothesis.h"
#include "cpu/decoder/encoder_decoder_loader.h"
#include "cpu/mblas/matrix.h"

using namespace std;

namespace amunmt {
namespace CPU {
namespace Nematus {

using EDState = EncoderDecoderState;

EncoderDecoder::EncoderDecoder(const God &god,
							   const std::string& name,
                               const YAML::Node& config,
                               unsigned tab,
                               const Nematus::Weights& model,
                               const Search &search)
  : CPUEncoderDecoderBase(god, name, config, tab, search),
    model_(model),
    encoder_(new CPU::Nematus::Encoder(model_)),
    decoder_(new CPU::Nematus::Decoder(model_))
{}


void EncoderDecoder::Decode(EncOutPtr encOut,
                            const State& in,
                            State& out,
                            const std::vector<unsigned>&)
{
  const EDState& edIn = in.get<EDState>();
  EDState& edOut = out.get<EDState>();

  decoder_->Decode(edOut.GetStates(), edIn.GetStates(),
                   edIn.GetEmbeddings(), SourceContext_);
}


void EncoderDecoder::BeginSentenceState(EncOutPtr encOut, State& state, unsigned batchSize) {
  EDState& edState = state.get<EDState>();
  decoder_->EmptyState(edState.GetStates(), SourceContext_, batchSize);
  decoder_->EmptyEmbedding(edState.GetEmbeddings(), batchSize);
}


void EncoderDecoder::Encode(const SentencesPtr &sources) {
  encoder_->GetContext(sources->Get(0)->GetWords(tab_),
                        SourceContext_);
}


void EncoderDecoder::AssembleBeamState(const State& in,
                                       const Hypotheses& beam,
                                       State& out) const
{
  std::vector<unsigned> beamWords;
  std::vector<unsigned> beamStateIds;
  for(auto h : beam) {
      beamWords.push_back(h->GetWord());
      beamStateIds.push_back(h->GetPrevStateIndex());
  }

  const EDState& edIn = in.get<EDState>();
  EDState& edOut = out.get<EDState>();

  edOut.GetStates() = mblas::Assemble<mblas::byRow, mblas::Matrix>(edIn.GetStates(), beamStateIds);
  decoder_->Lookup(edOut.GetEmbeddings(), beamWords);
}


void EncoderDecoder::GetAttention(mblas::Matrix& Attention) {
  decoder_->GetAttention(Attention);
}


mblas::Matrix& EncoderDecoder::GetAttention() {
  return decoder_->GetAttention();
}


unsigned EncoderDecoder::GetVocabSize() const {
  return decoder_->GetVocabSize();
}


void EncoderDecoder::Filter(const std::vector<unsigned>& filterIds) {
  decoder_->Filter(filterIds);
}


BaseMatrix& EncoderDecoder::GetProbs() {
  return decoder_->GetProbs();
}

}
}
}
