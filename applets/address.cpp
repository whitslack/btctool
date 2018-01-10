#include "address.h"

#include <iostream>

#include <strings.h>

#include "applets.h"
#include "util.h"
#include "common/cli.h"
#include "common/ripemd.h"
#include "common/sha.h"

using namespace satoshi;

const char address_usage[] =
	"  address [<option>...] [<pubkey>]\n"
	"    Derive address from public key.\n"
	"     --chain=  mainnet (default), testnet\n"
	"     --format=\n"
	"        legacy - (default) P2PKH format.\n"
	"        transitional - P2WPKH-in-P2SH format defined by BIP141.\n"
	"  address [<option>...] <m> [<pubkey>...]\n"
	"    Derive m-of-n multi-signature address from public keys.\n"
	"     --chain=  mainnet (default), testnet\n"
	"     --format=\n"
	"        legacy - (default) P2SH format defined by BIP16.\n"
	"        transitional - P2WSH-in-P2SH format defined by BIP141.";

int address_main(int argc, char *argv[]) {
	cli::Option<const char *> chain_option("chain"), format_option("format");
	argc = cli::parse(--argc, ++argv, { &chain_option, &format_option });
	Chain chain = chain_option ? parse_chain(chain_option.value()) : MAINNET;
	AddressFormat format = format_option ? parse_address_format(format_option.value()) : LEGACY;
	std::experimental::optional<size_t> m;
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
	if (!m) { // single-sig mode
		PublicKey pubkey;
		if (argc == 0) {
			std::string buffer;
			std::getline(std::cin, buffer);
			if (buffer.empty() && !std::cin) {
				throw std::invalid_argument("no pubkey provided");
			}
			pubkey = decode_pubkey(buffer.data(), buffer.size());
		}
		else {
			pubkey = decode_pubkey(argv[0], std::strlen(argv[0]));
		}
		std::cout << pubkey_to_address(pubkey, chain, format) << std::endl;
	}
	else { // multi-sig mode
		std::vector<PublicKey> pubkeys;
		pubkeys.reserve(argc == 0 ? 256 : argc);
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
				pubkeys.emplace_back(decode_pubkey(buffer.data(), buffer.size()));
			}
			else if (argc <= static_cast<ssize_t>(pubkeys.size())) {
				break;
			}
			else {
				pubkeys.emplace_back(decode_pubkey(argv[pubkeys.size()], std::strlen(argv[pubkeys.size()])));
			}
		}
		std::cout << pubkeys_to_address(m.value(), pubkeys, chain, format) << std::endl;
	}
	return 0;
}

Chain parse_chain(const char str[]) {
	if (::strcasecmp(str, "mainnet") == 0) {
		return MAINNET;
	}
	if (::strcasecmp(str, "testnet") == 0) {
		return TESTNET;
	}
	throw std::invalid_argument(std::string("unrecognized chain: ") + str);
}

AddressFormat parse_address_format(const char str[]) {
	if (::strcasecmp(str, "legacy") == 0) {
		return LEGACY;
	}
	if (::strcasecmp(str, "transitional") == 0) {
		return TRANSITIONAL;
	}
	throw std::invalid_argument(std::string("unrecognized format: ") + str);
}

Address pubkey_to_address(const PublicKey &pubkey, Chain chain, AddressFormat format) {
	SHA256 sha;
	sha << pubkey;
	RIPEMD160 rmd;
	rmd << sha.digest();
	switch (format) {
		case LEGACY:
			return { chain == MAINNET ? Address::Type::PUBKEY_HASH : Address::Type::TESTNET_PUBKEY_HASH, rmd.digest() };
		case TRANSITIONAL: {
			Script script;
			script.reserve(1 + 1 + RIPEMD160::digest_size);
			script.push_int(0); // witness program version 0
			script.push_data(rmd.digest().data(), RIPEMD160::digest_size);
			sha = { };
			sha.write_fully(script.data(), script.size());
			rmd = { };
			rmd << sha.digest();
			return { chain == MAINNET ? Address::Type::SCRIPT_HASH : Address::Type::TESTNET_SCRIPT_HASH, rmd.digest() };
		}
		default:
			throw std::invalid_argument("format");
	}
}

Address pubkeys_to_address(size_t m, const std::vector<PublicKey> &pubkeys, Chain chain, AddressFormat format) {
	if (m == 0) {
		throw std::invalid_argument("<m> must not be zero");
	}
	if (pubkeys.empty()) {
		throw std::invalid_argument("no pubkeys provided");
	}
	if (m > pubkeys.size()) {
		throw std::invalid_argument("<m> exceeds number of pubkeys");
	}
	SHA256 sha;
	{
		Script script;
		script.reserve(9 + pubkeys.size() * 66 + 9 + 1);
		script.push_int(m);
		std::vector<uint8_t> buffer;
		VectorSink vs(buffer);
		for (auto &pubkey : pubkeys) {
			buffer.clear();
			vs << pubkey;
			script.push_data(buffer.data(), buffer.size());
		}
		script.push_int(pubkeys.size());
		script.push_opcode(Script::OP_CHECKMULTISIG);
		sha.write_fully(script.data(), script.size());
	}
	switch (format) {
		case LEGACY:
			break;
		case TRANSITIONAL: {
			Script script;
			script.reserve(1 + 1 + SHA256::digest_size);
			script.push_int(0); // witness program version 0
			script.push_data(sha.digest().data(), SHA256::digest_size);
			sha = { };
			sha.write_fully(script.data(), script.size());
			break;
		}
		default:
			throw std::invalid_argument("format");
	}
	RIPEMD160 rmd;
	rmd << sha.digest();
	return { chain == MAINNET ? Address::Type::SCRIPT_HASH : Address::Type::TESTNET_SCRIPT_HASH, rmd.digest() };
}
