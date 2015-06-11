#include <iostream>

#include "applets.h"
#include "util.h"
#include "common/ecp.h"
#include "common/fp.h"
#include "libsatoshi/types.h"

using namespace satoshi;

const char privkeys_usage[] =
	"  privkeys <k> [<privseed>]\n"
	"    Derive k private keys from private seed.";

int privkeys_main(int argc, char *argv[]) {
	if (argc == 2 || argc == 3) {
		size_t k = parse_ulong(argv[1]);
		PrivateKey privkey;
		if (argc == 2) {
			std::cin >> privkey;
		}
		else { // (argc == 3)
			privkey = decode_privkey(argv[2], std::strlen(argv[2]));
		}
		PrivateKey privkey_i = privkey;
		for (size_t i = 0; i < k; ++i) {
			fp_add(privkey_i.d, fp_dbl(privkey_i.d, privkey_i.d, secp256k1_n), privkey.d, secp256k1_n);
			std::cout << privkey_i << std::endl;
		}
		return 0;
	}
	std::clog << "usage:\n" << privkeys_usage << std::endl;
	return -1;
}
