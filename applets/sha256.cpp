#include <iostream>

#include <unistd.h>

#include "applets.h"
#include "util.h"
#include "common/codec.h"
#include "common/fd.h"
#include "common/hex.h"
#include "common/sha.h"

const char sha256_usage[] =
	"  sha256 [<hex>]\n"
	"    Hash stdin or argument using SHA-256.";

int sha256_main(int argc, char *argv[]) {
	FileDescriptor stdin(STDIN_FILENO), stdout(STDOUT_FILENO);
	if (argc == 1 || argc == 2) {
		SHA256 sha;
		if (argc == 1) {
			(sha << stdin).flush_fully();
		}
		else { // (argc == 2)
			transcode<HexDecoder>(sha, argv[1], std::strlen(argv[1])).flush_fully();
		}
		transcode<HexEncoder>(stdout, sha.digest().data(), SHA256::digest_size).flush_fully();
		std::cout << std::endl;
		return 0;
	}
	std::clog << "usage:\n" << sha256_usage << std::endl;
	return -1;
}
