#include <iostream>

#include "applets.h"
#include "util.h"
#include "common/ecp.h"
#include "libsatoshi/types.h"

using namespace satoshi;

const char pubkeys_usage[] =
	"  pubkeys <k> [<pubseed>]\n"
	"    Derive k public keys from public seed.";

int pubkeys_main(int argc, char *argv[]) {
	if (argc == 2 || argc == 3) {
		size_t k = parse_ulong(argv[1]);
		PublicKey pubkey;
		if (argc == 2) {
			std::cin >> pubkey;
		}
		else { // (argc == 3)
			pubkey = decode_pubkey(argv[2], std::strlen(argv[2]));
		}
		decompress_pubkey(pubkey);
		PublicKey pubkey_i = pubkey;
		mp_limb_t R0[3][MP_NLIMBS(32)], R1[3][MP_NLIMBS(32)];
		for (size_t i = 0; i < k; ++i) {
			ecp_proj(pubkey_i.Q, ecp_add(R1, ecp_dbl(R0, pubkey_i.Q, secp256k1_a, secp256k1_p), pubkey.Q, secp256k1_a, secp256k1_p), secp256k1_p);
			std::cout << pubkey_i << std::endl;
		}
		return 0;
	}
	std::clog << "usage:\n" << pubkeys_usage << std::endl;
	return -1;
}
