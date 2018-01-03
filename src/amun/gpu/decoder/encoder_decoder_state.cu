#include <sstream>
#include "gpu/decoder/encoder_decoder_state.h"
#include "gpu/dl4mt/cellstate.h"

using namespace std;

namespace amunmt {
namespace GPU {

////////////////////////////////////////////
std::string EncoderDecoderState::Debug(unsigned verbosity) const
{
  stringstream strm;
  strm << "states_=" << states_.Debug(verbosity);
  strm << " embeddings_=" << embeddings_.Debug(verbosity);
  return strm.str();
}

CellState& EncoderDecoderState::GetStates() {
  return states_;
}

mblas::Matrix& EncoderDecoderState::GetEmbeddings() {
  return embeddings_;
}

const CellState& EncoderDecoderState::GetStates() const {
  return states_;
}

const mblas::Matrix& EncoderDecoderState::GetEmbeddings() const {
  return embeddings_;
}

}
}

