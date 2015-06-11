#include <iostream>

#include <unistd.h>

#include "applets.h"
#include "util.h"
#include "common/codec.h"
#include "common/fd.h"
#include "common/hex.h"
#include "common/ripemd.h"

const char rmd160_usage[] =
	"  rmd160 [<hex>]\n"
	"    Hash stdin or argument using RIPEMD-160.";

int rmd160_main(int argc, char *argv[]) {
	FileDescriptor stdin(STDIN_FILENO), stdout(STDOUT_FILENO);
	if (argc == 1 || argc == 2) {
		RIPEMD160 rmd;
		if (argc == 1) {
			(rmd << stdin).flush_fully();
		}
		else { // (argc == 2)
			transcode<HexDecoder>(rmd, argv[1], std::strlen(argv[1])).flush_fully();
		}
		transcode<HexEncoder>(stdout, rmd.digest().data(), RIPEMD160::digest_size).flush_fully();
		std::cout << std::endl;
		return 0;
	}
	std::clog << "usage:\n" << rmd160_usage << std::endl;
	return -1;
}
