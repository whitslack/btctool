#include <iostream>

#include <unistd.h>

#include "applets.h"
#include "common/codec.h"
#include "common/fd.h"
#include "common/hex.h"

const char encode16_usage[] =
	"  encode16\n"
	"    Encode stdin to hex.";

int encode16_main(int argc, char *[]) {
	FileDescriptor stdin(STDIN_FILENO), stdout(STDOUT_FILENO);
	if (argc == 1) {
		transcode<HexEncoder, 512, 256>(stdout, stdin).flush_fully();
		std::cout << std::endl;
		return 0;
	}
	std::clog << "usage:\n" << encode16_usage << std::endl;
	return -1;
}
