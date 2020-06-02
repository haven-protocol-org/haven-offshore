// Copyright (c) 2019, Haven Protocol
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Portions of this code based upon code Copyright (c) 2019, The Monero Project

#include "pricing_record.h"

#include <cstring>

#include "serialization/keyvalue_serialization.h"
#include "storages/portable_storage.h"

namespace offshore
{

  namespace
  {
    struct pr_serialized
    {
      uint64_t xAG;
      uint64_t xAU;
      uint64_t xAUD;
      uint64_t xBTC;
      uint64_t xCAD;
      uint64_t xCHF;
      uint64_t xCNY;
      uint64_t xEUR;
      uint64_t xGBP;
      uint64_t xJPY;
      uint64_t xNOK;
      uint64_t xNZD;
      uint64_t xUSD;
      uint64_t unused1;
      uint64_t unused2;
      uint64_t unused3;
      std::string signature;

      BEGIN_KV_SERIALIZE_MAP()
        KV_SERIALIZE(xAG)
        KV_SERIALIZE(xAU)
        KV_SERIALIZE(xAUD)
        KV_SERIALIZE(xBTC)
        KV_SERIALIZE(xCAD)
        KV_SERIALIZE(xCHF)
        KV_SERIALIZE(xCNY)
        KV_SERIALIZE(xEUR)
        KV_SERIALIZE(xGBP)
        KV_SERIALIZE(xJPY)
        KV_SERIALIZE(xNOK)
        KV_SERIALIZE(xNZD)
        KV_SERIALIZE(xUSD)
        KV_SERIALIZE(unused1)
        KV_SERIALIZE(unused2)
        KV_SERIALIZE(unused3)
        KV_SERIALIZE(signature)
      END_KV_SERIALIZE_MAP()
    };
  }
  
  pricing_record::pricing_record() noexcept
    : xAG(0)
    , xAU(0)
    , xAUD(0)
    , xBTC(0)
    , xCAD(0)
    , xCHF(0)
    , xCNY(0)
    , xEUR(0)
    , xGBP(0)
    , xJPY(0)
    , xNOK(0)
    , xNZD(0)
    , xUSD(0)
    , unused1(0)
    , unused2(0)
    , unused3(0)
  {
    std::memset(signature, 0, sizeof(signature));
  }

  bool pricing_record::_load(epee::serialization::portable_storage& src, epee::serialization::section* hparent)
  {
    pr_serialized in{};
    if (in._load(src, hparent))
    {
      // Copy everything into the local instance
      xAG = in.xAG;
      xAU = in.xAU;
      xAUD = in.xAUD;
      xBTC = in.xBTC;
      xCAD = in.xCAD;
      xCHF = in.xCHF;
      xCNY = in.xCNY;
      xEUR = in.xEUR;
      xGBP = in.xGBP;
      xJPY = in.xJPY;
      xNOK = in.xNOK;
      xNZD = in.xNZD;
      xUSD = in.xUSD;
      unused1 = in.unused1;
      unused2 = in.unused2;
      unused3 = in.unused3;
      std::memcpy(signature, in.signature.data(), in.signature.size());
      return true;
    }
    // HERE BE DRAGONS!!!
    // Report error here?
    return false;
    // LAND AHOY!!!
  }

  bool pricing_record::store(epee::serialization::portable_storage& dest, epee::serialization::section* hparent) const
  {
    const pr_serialized out{xAG,xAU,xAUD,xBTC,xCAD,xCHF,xCNY,xEUR,xGBP,xJPY,xNOK,xNZD,xUSD,unused1,unused2,unused3,signature};
    return out.store(dest, hparent);
  }

  pricing_record::pricing_record(const pricing_record& orig) noexcept
    : xAG(orig.xAG)
    , xAU(orig.xAU)
    , xAUD(orig.xAUD)
    , xBTC(orig.xBTC)
    , xCAD(orig.xCAD)
    , xCHF(orig.xCHF)
    , xCNY(orig.xCNY)
    , xEUR(orig.xEUR)
    , xGBP(orig.xGBP)
    , xJPY(orig.xJPY)
    , xNOK(orig.xNOK)
    , xNZD(orig.xNZD)
    , xUSD(orig.xUSD)
    , unused1(orig.unused1)
    , unused2(orig.unused2)
    , unused3(orig.unused3)
  {
    std::memcpy(signature, orig.signature, sizeof(signature));
  }

  pricing_record& pricing_record::operator=(const pricing_record& orig) noexcept
  {
    xAG = orig.xAG;
    xAU = orig.xAU;
    xAUD = orig.xAUD;
    xBTC = orig.xBTC;
    xCAD = orig.xCAD;
    xCHF = orig.xCHF;
    xCNY = orig.xCNY;
    xEUR = orig.xEUR;
    xGBP = orig.xGBP;
    xJPY = orig.xJPY;
    xNOK = orig.xNOK;
    xNZD = orig.xNZD;
    xUSD = orig.xUSD;
    unused1 = orig.unused1;
    unused2 = orig.unused2;
    unused3 = orig.unused3;
    ::memcpy(signature, orig.signature, sizeof(signature));
    return *this;
  }

  bool pricing_record::equal(const pricing_record& other) const noexcept
  {
    return ((xAG == other.xAG) &&
	    (xAU == other.xAU) &&
	    (xAUD == other.xAUD) &&
	    (xBTC == other.xBTC) &&
	    (xCAD == other.xCAD) &&
	    (xCHF == other.xCHF) &&
	    (xCNY == other.xCNY) &&
	    (xEUR == other.xEUR) &&
	    (xGBP == other.xGBP) &&
	    (xJPY == other.xJPY) &&
	    (xNOK == other.xNOK) &&
	    (xNZD == other.xNZD) &&
	    (xUSD == other.xUSD) &&
	    (unused1 == other.unused1) &&
	    (unused2 == other.unused2) &&
	    (unused3 == other.unused3) &&
	    !::memcmp(signature, other.signature, sizeof(signature)));
  }

}
