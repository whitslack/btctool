#include <iostream>

#include "address.h"
#include "applets.h"
#include "util.h"
#include "common/cli.h"
#include "common/ecp.h"
#include "common/ripemd.h"
#include "common/sha.h"

using namespace satoshi;

const char addresses_usage[] =
	"  addresses [<option>...] <k> [<pubseed>]\n"
	"    Derive k addresses from public seed.\n"
	"     --chain=  mainnet (default), testnet\n"
	"     --format=\n"
	"        legacy - (default) P2PKH format.\n"
	"        transitional - P2WPKH-in-P2SH format defined by BIP141.\n"
	"  addresses [<option>...] <k> <m> [<pubseed>...]\n"
	"    Derive k m-of-n multi-signature addresses from public seeds.\n"
	"     --chain=  mainnet (default), testnet\n"
	"     --format=\n"
	"        legacy - (default) P2SH format defined by BIP16.\n"
	"        transitional - P2WSH-in-P2SH format defined by BIP141.";

int addresses_main(int argc, char *argv[]) {
	cli::Option<const char *> chain_option("chain"), format_option("format");
	argc = cli::parse(--argc, ++argv, { &chain_option, &format_option });
	Chain chain = chain_option ? parse_chain(chain_option.value()) : MAINNET;
	AddressFormat format = format_option ? parse_address_format(format_option.value()) : LEGACY;
	if (argc > 0) {
		size_t k = parse_ulong(*argv++); --argc;
		std::experimental::optional<size_t> m;
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
		if (!m) { // single-sig mode
			PublicKey pubseed;
			if (argc == 0) {
				std::string buffer;
				std::getline(std::cin, buffer);
				if (buffer.empty() && !std::cin) {
					throw std::invalid_argument("no pubseed provided");
				}
				pubseed = decode_pubkey(buffer.data(), buffer.size());
			}
			else {
				pubseed = decode_pubkey(argv[0], std::strlen(argv[0]));
			}
			decompress_pubkey(pubseed);
			auto pubkey = pubseed;
			mp_limb_t R0[3][MP_NLIMBS(32)], R1[3][MP_NLIMBS(32)];
			for (size_t i = 0; i < k; ++i) {
				ecp_proj(pubkey.Q, ecp_add(R1, ecp_dbl(R0, pubkey.Q, secp256k1_a, secp256k1_p), pubseed.Q, secp256k1_a, secp256k1_p), secp256k1_p);
				std::cout << pubkey_to_address(pubkey, chain, format) << '\n';
			}
		}
		else { // multi-sig mode
			std::vector<PublicKey> pubseeds;
			pubseeds.reserve(argc == 0 ? 256 : argc);
			for (;;) {
				if (argc == 0) {
					if (!std::cin) {
						break;
					}
					std::string buffer;
					std::getline(std::cin, buffer);
					if (buffer.empty() && !std::cin) {
						break;
					}
					pubseeds.emplace_back(decode_pubkey(buffer.data(), buffer.size()));
				}
				else if (argc <= static_cast<ssize_t>(pubseeds.size())) {
					break;
				}
				else {
					pubseeds.emplace_back(decode_pubkey(argv[pubseeds.size()], std::strlen(argv[pubseeds.size()])));
				}
				decompress_pubkey(pubseeds.back());
			}
			if (pubseeds.empty()) {
				throw std::invalid_argument("no pubseeds provided");
			}
			auto pubkeys = pubseeds;
			mp_limb_t R0[3][MP_NLIMBS(32)], R1[3][MP_NLIMBS(32)];
			for (size_t i = 0; i < k; ++i) {
				for (size_t i = 0; i < pubkeys.size(); ++i) {
					ecp_proj(pubkeys[i].Q, ecp_add(R1, ecp_dbl(R0, pubkeys[i].Q, secp256k1_a, secp256k1_p), pubseeds[i].Q, secp256k1_a, secp256k1_p), secp256k1_p);
				}
				std::cout << pubkeys_to_address(m.value(), pubkeys, chain, format) << '\n';
			}
		}
		std::cout << std::flush;
		return 0;
	}
	std::clog << "usage:\n" << addresses_usage << std::endl;
	return -1;
}
