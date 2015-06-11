#include <iostream>

#include "applets.h"
#include "util.h"
#include "common/ecp.h"
#include "common/ripemd.h"
#include "common/sha.h"
#include "libsatoshi/types.h"

using namespace satoshi;

const char addresses_usage[] =
	"  addresses <k> [<pubseed>]\n"
	"    Derive k addresses from public seed.\n"
	"  addresses <k> <m> [<pubseed>...]\n"
	"    Derive k m-of-n P2SH addresses from public seeds.";

int addresses_main(int argc, char *argv[]) {
	--argc, ++argv;
	if (argc > 0) {
		size_t k = parse_ulong(*argv++); --argc;
		size_t m = 1;
		if (argc > 0) {
			try {
				m = parse_ulong(*argv);
				++argv, --argc;
			}
			catch (...) {
				if (argc > 1) {
					throw;
				}
			}
		}
		std::vector<PublicKey> pubseeds;
		pubseeds.reserve(argc == 0 ? 256 : argc);
		for (;;) {
			PublicKey pubkey;
			if (argc == 0) {
				if (!std::cin) {
					break;
				}
				std::string buffer;
				std::getline(std::cin, buffer);
				if (buffer.empty() && !std::cin) {
					break;
				}
				pubkey = decode_pubkey(buffer.data(), buffer.size());
			}
			else if (argc <= static_cast<ssize_t>(pubseeds.size())) {
				break;
			}
			else {
				pubkey = decode_pubkey(argv[pubseeds.size()], std::strlen(argv[pubseeds.size()]));
			}
			decompress_pubkey(pubkey);
			pubseeds.push_back(std::move(pubkey));
		}
		auto pubkeys = pubseeds;
		mp_limb_t R0[3][MP_NLIMBS(32)], R1[3][MP_NLIMBS(32)];
		Address address;
		address.type = pubkeys.size() == 1 ? Address::Type::PUBKEY_HASH : Address::Type::SCRIPT_HASH;
		Script script;
		script.reserve(9 + pubkeys.size() * 66 + 9 + 1);
		std::vector<uint8_t> buffer;
		for (size_t i = 0; i < k; ++i) {
			for (size_t i = 0; i < pubkeys.size(); ++i) {
				ecp_proj(pubkeys[i].Q, ecp_add(R1, ecp_dbl(R0, pubkeys[i].Q, secp256k1_a, secp256k1_p), pubseeds[i].Q, secp256k1_a, secp256k1_p), secp256k1_p);
			}
			SHA256 sha;
			if (pubkeys.size() == 1) {
				sha << pubkeys[0];
			}
			else {
				script.clear();
				script.push_int(m);
				for (auto &pubkey : pubkeys) {
					buffer.clear();
					VectorSink vs(buffer);
					vs << pubkey;
					script.push_data(buffer.data(), buffer.size());
				}
				script.push_int(pubkeys.size());
				script.push_opcode(Script::OP_CHECKMULTISIG);
				sha.write_fully(script.data(), script.size());
			}
			RIPEMD160 rmd;
			rmd << sha.digest();
			address.hash = rmd.digest();
			std::cout << address << std::endl;
		}
		return 0;
	}
	std::clog << "usage:\n" << addresses_usage << std::endl;
	return -1;
}
