#include <iostream>

#include <unistd.h>

#include "applets.h"
#include "util.h"
#include "common/codec.h"
#include "common/fd.h"
#include "common/hex.h"

const char decode16_usage[] =
	"  decode16 [<hex>]\n"
	"    Decode hex from stdin or argument.";

int decode16_main(int argc, char *argv[]) {
	FileDescriptor stdin(STDIN_FILENO), stdout(STDOUT_FILENO);
	if (argc == 1) {
		ChompSource chomp(stdin);
		transcode<HexDecoder, 256, 512>(stdout, chomp).flush_fully();
		return 0;
	}
	if (argc == 2) {
		transcode<HexDecoder>(stdout, argv[1], std::strlen(argv[1])).flush_fully();
		return 0;
	}
	std::clog << "usage:\n" << decode16_usage << std::endl;
	return -1;
}
