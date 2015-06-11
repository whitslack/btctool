#include <iostream>

#include <unistd.h>

#include "applets.h"
#include "util.h"
#include "common/base64.h"
#include "common/codec.h"
#include "common/fd.h"

const char decode64_usage[] =
	"  decode64 [<base64>]\n"
	"    Decode Base64 from stdin or argument.";

int decode64_main(int argc, char *argv[]) {
	FileDescriptor stdin(STDIN_FILENO), stdout(STDOUT_FILENO);
	if (argc == 1) {
		ChompSource chomp(stdin);
		transcode<Base64Decoder, 384, 512>(stdout, chomp).flush_fully();
		return 0;
	}
	if (argc == 2) {
		transcode<Base64Decoder>(stdout, argv[1], std::strlen(argv[1])).flush_fully();
		return 0;
	}
	std::clog << "usage:\n" << decode64_usage << std::endl;
	return -1;
}
