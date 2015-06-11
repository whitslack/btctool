#include <iostream>

#include "applets.h"
#include "util.h"
#include "common/ripemd.h"
#include "common/sha.h"
#include "libsatoshi/types.h"

using namespace satoshi;

const char address_usage[] =
	"  address [<pubkey>]\n"
	"    Derive address from public key.\n"
	"  address <m> [<pubkey>...]\n"
	"    Derive m-of-n P2SH address from public keys.";

int address_main(int argc, char *argv[]) {
	--argc, ++argv;
	size_t m = 1;
	if (argc > 0) {
		try {
			m = parse_ulong(argv[0]);
			++argv, --argc;
		}
		catch (...) {
			if (argc > 1) {
				throw;
			}
		}
	}
	Script script;
	script.reserve(9 + (argc == 0 ? 256 : argc) * 66 + 9 + 1);
	script.push_int(m);
	size_t n = 0;
	std::vector<uint8_t> buffer;
	for (;; ++n) {
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
		else if (argc <= static_cast<ssize_t>(n)) {
			break;
		}
		else {
			pubkey = decode_pubkey(argv[n], std::strlen(argv[n]));
		}
		buffer.clear();
		VectorSink vs(buffer);
		vs << pubkey;
		script.push_data(buffer.data(), buffer.size());
	}
	SHA256 sha;
	if (n == 1) {
		sha.write_fully(buffer.data(), buffer.size());
	}
	else {
		script.push_int(n);
		script.push_opcode(Script::OP_CHECKMULTISIG);
		sha.write_fully(script.data(), script.size());
	}
	RIPEMD160 rmd;
	rmd << sha.digest();
	Address address = { n == 1 ? Address::Type::PUBKEY_HASH : Address::Type::SCRIPT_HASH, rmd.digest() };
	std::cout << address << std::endl;
	return 0;
}
