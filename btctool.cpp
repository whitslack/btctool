#include <cstring>
#include <iostream>

#include "applets.h"


int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::clog << "usage: " << (argc > 0 ? argv[0] : "btctool") << " <function> [<args>]\n" <<
				encode16_usage << '\n' << decode16_usage << '\n' <<
				encode64_usage << '\n' << decode64_usage << '\n' <<
				encode58_usage << '\n' << decode58_usage << '\n' <<
				disasm_usage << '\n' <<
				sha256_usage << '\n' << rmd160_usage << '\n' <<
				privkey_usage << '\n' << pubkey_usage << '\n' << address_usage << '\n' <<
				encrypt_usage << '\n' << decrypt_usage << '\n' <<
				privkeys_usage << '\n' << pubkeys_usage << '\n' << addresses_usage << std::endl;
		return -1;
	}
	--argc, ++argv;
	const char *f = argv[0];
	switch (f[0] | 0x20) {
		case '-': // -
			if (std::strcmp(f, "--version") == 0) { // --version
				std::clog << VERSION << std::endl;
				return 0;
			}
			break;
		case 'a': // a
			if ((f[1] | 0x20) == 'd' && (f[2] | 0x20) == 'd' && (f[3] | 0x20) == 'r' && (f[4] | 0x20) == 'e' && (f[5] | 0x20) == 's' && (f[6] | 0x20) == 's') { // address
				if (f[7] == '\0') { // address
					return address_main(argc, argv);
				}
				if ((f[7] | 0x20) == 'e' && (f[8] | 0x20) == 's' && f[9] == '\0') { // addresses
					return addresses_main(argc, argv);
				}
			}
			break;
		case 'd': // d
			switch (f[1] | 0x20) {
				case 'e': // de
					if ((f[2] | 0x20) == 'c') { // dec
						switch (f[3] | 0x20) {
							case 'o': // deco
								if ((f[4] | 0x20) == 'd' && (f[5] | 0x20) == 'e') { // decode
									switch (f[6]) {
										case '1': // decode1
											if (f[7] == '6' && f[8] == '\0') { // decode16
												return decode16_main(argc, argv);
											}
											break;
										case '5': // decode5
											if (f[7] == '8' && f[8] == '\0') { // decode58
												return decode58_main(argc, argv);
											}
											break;
										case '6': // decode6
											if (f[7] == '4' && f[8] == '\0') { // decode64
												return decode64_main(argc, argv);
											}
											break;
									}
								}
								break;
							case 'r': // decr
								if ((f[4] | 0x20) == 'y' && (f[5] | 0x20) == 'p' && (f[6] | 0x20) == 't' && f[7] == '\0') { // decrypt
									return decrypt_main(argc, argv);
								}
								break;
						}
					}
					break;
				case 'i': // di
					if ((f[2] | 0x20) == 's' && (f[3] | 0x20) == 'a' && (f[4] | 0x20) == 's' && (f[5] | 0x20) == 'm' && f[6] == '\0') { // disasm
						return disasm_main(argc, argv);
					}
					break;
			}
			break;
		case 'e': // e
			if ((f[1] | 0x20) == 'n' && (f[2] | 0x20) == 'c') { // enc
				switch (f[3] | 0x20) {
					case 'o': // enco
						if ((f[4] | 0x20) == 'd' && (f[5] | 0x20) == 'e') { // encode
							switch (f[6]) {
								case '1': // encode1
									if (f[7] == '6' && f[8] == '\0') { // encode16
										return encode16_main(argc, argv);
									}
									break;
								case '5': // encode5
									if (f[7] == '8' && f[8] == '\0') { // encode58
										return encode58_main(argc, argv);
									}
									break;
								case '6': // encode6
									if (f[7] == '4' && f[8] == '\0') { // encode64
										return encode64_main(argc, argv);
									}
									break;
							}
						}
						break;
					case 'r': // encr
						if ((f[4] | 0x20) == 'y' && (f[5] | 0x20) == 'p' && (f[6] | 0x20) == 't' && f[7] == '\0') { // encrypt
							return encrypt_main(argc, argv);
						}
						break;
				}
			}
			break;
		case 'p': // p
			switch (f[1] | 0x20) {
				case 'r': // pr
					if ((f[2] | 0x20) == 'i' && (f[3] | 0x20) == 'v' && (f[4] | 0x20) == 'k' && (f[5] | 0x20) == 'e' && (f[6] | 0x20) == 'y') { // privkey
						if (f[7] == '\0') { // privkey
							return privkey_main(argc, argv);
						}
						if ((f[7] | 0x20) == 's' && f[8] == '\0') { // privkeys
							return privkeys_main(argc, argv);
						}
					}
					break;
				case 'u': // pu
					if ((f[2] | 0x20) == 'b' && (f[3] | 0x20) == 'k' && (f[4] | 0x20) == 'e' && (f[5] | 0x20) == 'y') { // pubkey
						if (f[6] == '\0') { // pubkey
							return pubkey_main(argc, argv);
						}
						if ((f[6] | 0x20) == 's' && f[7] == '\0') { // pubkeys
							return pubkeys_main(argc, argv);
						}
					}
					break;
			}
			break;
		case 'r': // r
			if ((f[1] | 0x20) == 'm' && (f[2] | 0x20) == 'd' && f[3] == '1' && f[4] == '6' && f[5] == '0' && f[6] == '\0') { // rmd160
				return rmd160_main(argc, argv);
			}
			break;
		case 's': // s
			if ((f[1] | 0x20) == 'h' && (f[2] | 0x20) == 'a' && f[3] == '2' && f[4] == '5' && f[5] == '6' && f[6] == '\0') { // sha256
				return sha256_main(argc, argv);
			}
			break;
	}
	std::clog << "unknown function: " << f << std::endl;
	return -1;
}
