#include <iostream>

#include <unistd.h>

#include "applets.h"
#include "common/fd.h"
#include "libsatoshi/base58check.h"

const char decode58_usage[] =
	"  decode58 [<base58>]\n"
	"    Decode Base58Check from stdin or argument.";

int decode58_main(int argc, char *argv[]) {
	FileDescriptor stdout(STDOUT_FILENO);
	if (argc == 1) {
		std::string buffer;
		std::getline(std::cin, buffer);
		uint8_t bytes[buffer.size()];
		stdout.write_fully(bytes, base58check_decode(bytes, sizeof bytes, buffer.data(), buffer.size()));
		stdout.flush_fully();
		return 0;
	}
	if (argc == 2) {
		uint8_t bytes[std::strlen(argv[1])];
		stdout.write_fully(bytes, base58check_decode(bytes, sizeof bytes, argv[1], sizeof bytes));
		stdout.flush_fully();
		return 0;
	}
	std::clog << "usage:\n" << decode58_usage << std::endl;
	return -1;
}
