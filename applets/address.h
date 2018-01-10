#include "common/compiler.h"
#include "libsatoshi/types.h"

enum Chain {
	MAINNET,
	TESTNET,
};

Chain parse_chain(const char str[]) _pure;

enum AddressFormat {
	LEGACY,
	TRANSITIONAL,
};

AddressFormat parse_address_format(const char str[]) _pure;

satoshi::Address pubkey_to_address(const satoshi::PublicKey &pubkey, Chain chain, AddressFormat format);
satoshi::Address pubkeys_to_address(size_t m, const std::vector<satoshi::PublicKey> &pubkeys, Chain chain, AddressFormat format);
