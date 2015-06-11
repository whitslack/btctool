#include <cmath>
#include <iostream>

#include <unistd.h>

#include "util.h"
#include "common/ecp.h"
#include "libsatoshi/types.h"

using namespace satoshi;

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::clog << "usage: " << (argc > 0 ? argv[0] : "rollkey") << " <sides>" << std::endl;
		return -1;
	}
	int n_sides = std::stoi(argv[1]);
	if (n_sides < 2 || n_sides > 256) {
		std::clog << "<sides> must be between 2 and 256" << std::endl;
		return -1;
	}
	uint8_t rolls[static_cast<size_t>(std::ceil(256 / std::log2(n_sides)))];
	union {
		PrivateKey privkey;
		mp_limb_t limbs[MP_NLIMBS(32) + 2];
	} buf;
restart:
	std::clog << "reading approximately " << sizeof rolls << " rolls" << std::endl;
	for (;;) {
		std::fill(rolls, rolls + sizeof rolls, 0);
		for (size_t i = 0; i < sizeof rolls; ++i) {
			uint roll;
			if (!(std::cin >> roll)) {
				return 1;
			}
			rolls[i] = static_cast<uint8_t>(roll % n_sides);
			std::memset(&buf, 0, sizeof buf);
			mpn_set_str(buf.limbs, rolls, sizeof rolls, n_sides);
			if (!mpn_zero_p(buf.limbs + MP_NLIMBS(32), 2) || mpn_cmp(buf.limbs, secp256k1_n, MP_NLIMBS(32)) >= 0) {
				goto restart;
			}
		}
		break;
	}
	buf.privkey.flags = PrivateKey::Flags::NONE;
	PublicKey pubkey = privkey_to_pubkey(buf.privkey);
	std::cout << "\nuncompressed:\n"
		"\tprivkey: " << buf.privkey << "\n"
		"\tpubkey: " << pubkey << "\n"
		"\taddress: " << pubkey_to_address(pubkey) << std::endl;
	buf.privkey.flags = PrivateKey::Flags::COMPRESS;
	pubkey.compress = true;
	std::cout << "\ncompressed:\n"
		"\tprivkey: " << buf.privkey << "\n"
		"\tpubkey: " << pubkey << "\n"
		"\taddress: " << pubkey_to_address(pubkey) << std::endl;
	return 0;
}
