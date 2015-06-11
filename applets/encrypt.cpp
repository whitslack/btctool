#include <iostream>

#include "applets.h"
#include "util.h"
#include "common/aes.h"
#include "common/scrypt.h"
#include "common/sha.h"
#include "libsatoshi/base58check.h"
#include "libsatoshi/types.h"

using namespace satoshi;

const char encrypt_usage[] =
	"  encrypt [<privkey>]\n"
	"    Encrypt private key per BIP38.";

int encrypt_main(int argc, char *argv[]) {
	if (argc == 1 || argc == 2) {
		PrivateKey privkey;
		if (argc == 1) {
			std::cin >> privkey;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else { // (argc == 2)
			privkey = decode_privkey(argv[1], std::strlen(argv[1]));
		}
		std::string passphrase = read_passphrase();
		uint8_t payload[39];
		payload[0] = 1, payload[1] = 0x42, payload[2] = (privkey.flags & PrivateKey::Flags::COMPRESS) != PrivateKey::Flags::NONE ? 0xE0 : 0xC0;
		auto base58 = encode_address(pubkey_to_address(privkey_to_pubkey(privkey)));
		SHA256 isha;
		isha.write_fully(base58.data(), base58.size());
		SHA256 osha;
		osha << isha.digest();
		std::memcpy(payload + 3, osha.digest().data(), 4);
		uint8_t derived_key[64];
		scrypt(derived_key, sizeof derived_key, passphrase.data(), passphrase.size(), payload + 3, 4, 8, 16384, 8);
		AES256Encrypter aes(reinterpret_cast<const uint8_t (*)[32]>(derived_key)[1]);
		uint8_t bytes[32];
		mpn_to_bytes(bytes, privkey.d, sizeof bytes);
		for (size_t i = 0; i < sizeof privkey.d; ++i) {
			bytes[i] ^= derived_key[i];
		}
		aes.process(*reinterpret_cast<uint8_t (*)[16]>(payload + 7), bytes, 16);
		aes.process(*reinterpret_cast<uint8_t (*)[16]>(payload + 23), bytes + 16, 16);
		std::cout << base58check_encode(payload, sizeof payload) << std::endl;
		return 0;
	}
	std::clog << "usage:\n" << encrypt_usage << std::endl;
	return -1;
}
