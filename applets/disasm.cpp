#include <iostream>

#include <unistd.h>

#include "applets.h"
#include "util.h"
#include "common/codec.h"
#include "common/fd.h"
#include "common/hex.h"
#include "libsatoshi/script.h"

using namespace satoshi;

const char disasm_usage[] =
	"  disasm [<script>]\n"
	"    Disassemble hex script from stdin or argument.";

int disasm_main(int argc, char *argv[]) {
	FileDescriptor stdin(STDIN_FILENO);
	if (argc == 1 || argc == 2) {
		std::vector<uint8_t> buffer;
		VectorSink vs(buffer);
		if (argc == 1) {
			ChompSource chomp(stdin);
			transcode<HexDecoder, 256, 512>(vs, chomp).flush_fully();
		}
		else { // (argc == 2)
			transcode<HexDecoder>(vs, argv[1], std::strlen(argv[1])).flush_fully();
		}
		Script script(std::move(buffer));
		std::cout << script << std::endl;
		return 0;
	}
	std::clog << "usage:\n" << disasm_usage << std::endl;
	return -1;
}
