#include <iostream>

#include "applets.h"
#include "libsatoshi/types.h"

using namespace satoshi;

const char pubkey_usage[] =
	"  pubkey [<privkey>]\n"
	"    Derive public key from private key.";

int pubkey_main(int argc, char *argv[]) {
	if (argc == 1 || argc == 2) {
		PrivateKey privkey;
		if (argc == 1) {
			std::cin >> privkey;
		}
		else { // (argc == 2)
			privkey = decode_privkey(argv[1], std::strlen(argv[1]));
		}
		std::cout << privkey_to_pubkey(privkey) << std::endl;
		return 0;
	}
	std::clog << "usage:\n" << pubkey_usage << std::endl;
	return -1;
}
