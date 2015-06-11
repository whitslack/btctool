#include <iostream>

#include "applets.h"
#include "common/codec.h"
#include "common/ecp.h"
#include "common/hex.h"
#include "libsatoshi/types.h"

using namespace satoshi;

const char privkey_usage[] =
	"  privkey [<hex>]\n"
	"    Derive private key from stdin or argument.";

int privkey_main(int argc, char *argv[]) {
	if (argc == 1 || argc == 2) {
		uint8_t bytes[32];
		if ((argc == 1 ? std::cin.read(reinterpret_cast<char *>(bytes), sizeof bytes).gcount() : transcode<HexDecoder>(bytes, sizeof bytes, argv[1], std::strlen(argv[1]))) != sizeof bytes) {
			throw std::ios_base::failure("expected 32 bytes");
		}
		PrivateKey privkey;
		bytes_to_mpn(privkey.d, bytes, sizeof bytes);
		if (mpn_cmp(privkey.d, secp256k1_n, MP_NLIMBS(32)) >= 0) {
			mpn_sub_n(privkey.d, privkey.d, secp256k1_n, MP_NLIMBS(32));
			mpn_to_bytes(bytes, privkey.d, 32);
		}
		privkey.flags = PrivateKey::Flags::COMPRESS;
		std::cout << privkey << std::endl;
		return 0;
	}
	std::clog << "usage:\n" << privkey_usage << std::endl;
	return -1;
}
