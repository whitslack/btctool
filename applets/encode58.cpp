#include <iostream>

#include <unistd.h>

#include "applets.h"
#include "util.h"
#include "common/codec.h"
#include "common/fd.h"
#include "common/hex.h"
#include "libsatoshi/base58check.h"

const char encode58_usage[] =
	"  encode58 [<hex>]\n"
	"    Encode stdin or argument to Base58Check.";

int encode58_main(int argc, char *argv[]) {
	FileDescriptor stdin(STDIN_FILENO);
	if (argc == 1 || argc == 2) {
		std::string buffer;
		if (argc == 1) {
			StringSink ss(buffer);
			(ss << stdin).flush_fully();
		}
		else { // (argc == 2)
			transcode<HexDecoder>(buffer, argv[1], std::strlen(argv[1]));
		}
		std::cout << base58check_encode(buffer.data(), buffer.size()) << std::endl;
		return 0;
	}
	std::clog << "usage:\n" << encode58_usage << std::endl;
	return -1;
}
