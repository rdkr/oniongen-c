#include <stdio.h>
#include <inttypes.h>

#include "ed25519-supercop-ref10/crypto_sign.h"
#include "ed25519-supercop-ref10/crypto_hash_sha512.h"
#include "ed25519-supercop-ref10/ge.h"

#include "keccakf.h"

void hash(uint8_t *checksum)
{

  // H = SHA3-256 = SPONGE[f, pad, r](N, d) = Z, such that len(Z) = d
  //
  // f = KECCAK-f[b]
  // b = 1600
  // pad = pad10*1
  // r = 1088
  // N = M || 01
  // d = 256
  //
  // 1. Let P = N || pad(r, len(N))
  //        P = N || pad(pad10*1, len(N))
  //        P = M || 0b011[0...0]1 = checksum[0:136]
  // 2. Let n = len(P) / r
  //        n = 136*8 / 1088 = 1
  // 3. Let c = b - r
  //        c = 1600 - 1088 = 512
  // 4. Let P_0, ..., P_(n-1) be the unique sequence of strings
  //    of length r such that P = P_0 || ... || P_(n-1).
  //    P_(n-1) = P_(1-1) = P_0 = P_n = P
  // 5. Let S = 0^b
  // 6. For i from 0 to n-1, let S = f(S xor (P_i || 0^c))
  // 7. Let Z be the empty string
  // 8. Let Z = Z || Trunc_r(Z)
  // 9. If d <= |Z|, then return Trunc_d(Z)...
  //
  // as the message to be hashed is less than the size of P_i
  // we only need one string of bits, of length b, to perform
  // the hashing.
  // steps 1 - 4 all occur in the first r bits.
  // steps 5 - 6 are not needed as x xor 0 = x and there is only
  // steps 7 - 9 are simply reading the result of applying keccakf

  // pad checksum up r bits
  checksum[48] = 0x01; // pad10*1 start
  checksum[135] = 0x80; // pad10*1 end

  // cast checksum to S for keccakf implementation (blocks of 64 bits)
  uint64_t *S[25];
  for (size_t i = 0; i < 25; i++) {
    S[i] = (uint64_t *)&checksum[i * 8];
  }

  // run the keccakf transformation
  keccakf(*S);

}

void pubkey(uint8_t *seed, uint8_t *pk)
{

  uint8_t h[64];
  ge_p3 A;
  crypto_hash_sha512(h,seed,32);
  h[0] &= 248;
  h[31] &= 63;
  h[31] |= 64;
  ge_scalarmult_base(&A,h);
  ge_p3_tobytes(pk,&A);

}

int main(void)
{

  uint8_t pk[32];
  uint8_t seed[32];

  // generate public key from seed
  pubkey(seed, pk);

  // v3 checksum = H(".onion checksum" || pubkey || version)

  // create bit string of length b and build message in first 48 bytes
  uint8_t checksum[200] = ".onion checksum";
  for (int i = 0; i < 32; ++i) checksum[15 + i] = pk[i];
  checksum[47] = 0x03;

  // hash the message
  hash(checksum);

  // v3 onion_address = base32(pubkey || checksum || version)

  return 0;

}
