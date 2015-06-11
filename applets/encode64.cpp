#include <iostream>

#include <unistd.h>

#include "applets.h"
#include "common/base64.h"
#include "common/codec.h"
#include "common/fd.h"
#include "common/hex.h"

const char encode64_usage[] =
	"  encode64 [<hex>]\n"
	"    Encode stdin or argument to Base64.";

int encode64_main(int argc, char *argv[]) {
	FileDescriptor stdin(STDIN_FILENO), stdout(STDOUT_FILENO);
	if (argc == 1) {
		transcode<Base64Encoder, 512, 384>(stdout, stdin).flush_fully();
		std::cout << std::endl;
		return 0;
	}
	if (argc == 2) {
		CodecSink<Base64Encoder> cs(stdout);
		transcode<HexDecoder>(cs, argv[1], std::strlen(argv[1])).flush_fully();
		std::cout << std::endl;
		return 0;
	}
	std::clog << "usage:\n" << encode64_usage << std::endl;
	return -1;
}
