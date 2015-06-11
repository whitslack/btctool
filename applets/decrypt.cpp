#include <iostream>

#include "applets.h"
#include "util.h"
#include "common/aes.h"
#include "common/scrypt.h"
#include "common/sha.h"
#include "libsatoshi/base58check.h"
#include "libsatoshi/types.h"

using namespace satoshi;

const char decrypt_usage[] =
	"  decrypt [<privkey>]\n"
	"    Decrypt private key per BIP38.";

int decrypt_main(int argc, char *argv[]) {
	if (argc == 1 || argc == 2) {
		uint8_t payload[39];
		size_t n;
		if (argc == 1) {
			std::string buffer;
			std::getline(std::cin, buffer);
			n = base58check_decode(payload, sizeof payload, buffer.data(), buffer.size());
		}
		else { // (argc == 2)
			n = base58check_decode(payload, sizeof payload, argv[1], std::strlen(argv[1]));
		}
		if (n != sizeof payload) {
			throw std::ios_base::failure("expected BIP38-encrypted private key, but input has wrong length");
		}
		if (payload[0] != 1 || payload[1] != 0x42) {
			throw std::ios_base::failure("expected BIP38-encrypted private key, but input has wrong version/application byte");
		}
		if ((payload[2] & 0xDF) != 0xC0) {
			throw std::ios_base::failure("BIP38-encrypted private key has unsupported flags");
		}
		bool compress = payload[2] & 0x20;
		std::string passphrase = read_passphrase();
		uint8_t derived_key[64];
		scrypt(derived_key, sizeof derived_key, passphrase.data(), passphrase.size(), payload + 3, 4, 8, 16384, 8);
		AES256Decrypter aes(reinterpret_cast<const uint8_t (*)[32]>(derived_key)[1]);
		uint8_t bytes[34];
		aes.process(reinterpret_cast<uint8_t (*)[16]>(bytes + 1)[0], payload + 7, 16);
		aes.process(reinterpret_cast<uint8_t (*)[16]>(bytes + 1)[1], payload + 23, 16);
		for (size_t i = 0; i < 32; ++i) {
			bytes[1 + i] ^= derived_key[i];
		}
		PrivateKey privkey;
		bytes_to_mpn(privkey.d, bytes + 1, 32);
		privkey.flags = compress ? PrivateKey::Flags::COMPRESS : PrivateKey::Flags::NONE;
		auto base58 = encode_address(pubkey_to_address(privkey_to_pubkey(privkey)));
		SHA256 isha;
		isha.write_fully(base58.data(), base58.size());
		SHA256 osha;
		osha << isha.digest();
		if (std::memcmp(payload + 3, osha.digest().data(), 4) != 0) {
			throw std::ios_base::failure("incorrect passphrase");
		}
		std::cout << privkey << std::endl;
		return 0;
	}
	std::clog << "usage:\n" << decrypt_usage << std::endl;
	return -1;
}
