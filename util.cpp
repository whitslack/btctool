#include "util.h"

#include <iostream>
#include <system_error>

#include <unistd.h>

#include "common/codec.h"
#include "common/ecp.h"
#include "common/fd.h"
#include "common/fp.h"
#include "common/hex.h"
#include "common/narrow.h"
#include "common/ripemd.h"
#include "common/sha.h"
#include "common/termios.h"
#include "libsatoshi/base58check.h"


const mp_limb_t P[MP_NLIMBS(32)] = {
	MP_LIMB_C(0xFFFFFF43, 0xFFFFFFFF), MP_LIMB_C(0xFFFFFFFF, 0xFFFFFFFF),
	MP_LIMB_C(0xFFFFFFFF, 0xFFFFFFFF), MP_LIMB_C(0xFFFFFFFF, 0xFFFFFFFF)
};


ssize_t ChompSource::read(void *buf, size_t n) {
	if (n == 0) {
		return 0;
	}
	uint8_t *p = static_cast<uint8_t *>(buf);
	if (full) {
		*p++ = slot;
		if (--n == 0) {
			return source.read(&slot, sizeof slot);
		}
	}
	ssize_t s = source.read(p, n);
	if (s > 0) {
		if (p[s - 1] == '\n') {
			if (!full) {
				full = true, --s;
			}
			slot = '\n';
		}
		else if (full) {
			full = false, ++s;
		}
	}
	return s;
}


Sink & operator << (Sink &sink, Source &source) {
	uint8_t buf[4096];
	for (ssize_t r; (r = source.read(buf, sizeof buf)) >= 0;) {
		sink.write_fully(buf, r);
	}
	return sink;
}

ulong parse_ulong(const char in[], int base) {
	char *endptr;
	ulong r = std::strtoul(in, &endptr, base);
	if (endptr == in || *endptr != '\0') {
		throw std::ios_base::failure("expected integer");
	}
	return r;
}

static void set_local_echo(bool echo) {
	try {
		struct termios t;
		posix::tcgetattr(STDIN_FILENO, t);
		t.c_lflag = echo ? t.c_lflag | ECHO : t.c_lflag & ~ECHO;
		posix::tcsetattr(STDIN_FILENO, TCSANOW, t);
	}
	catch (const std::system_error &) {
	}
}

std::string read_passphrase() {
	std::string passphrase;
	if (posix::isatty(STDIN_FILENO)) {
		std::cerr << "Enter passphrase (not echoed): " << std::flush;
		set_local_echo(false);
		std::getline(std::cin, passphrase);
		set_local_echo(true);
		std::cerr << std::endl;
	}
	else {
		std::getline(std::cin, passphrase);
	}
	return passphrase;
}
