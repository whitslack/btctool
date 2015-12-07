#include "common/io.h"
#include "common/mpn.h"
#include "common/serial.h"


template <typename T, size_t N>
static constexpr size_t countof(T (&)[N]) { return N; }


extern const mp_limb_t P[MP_NLIMBS(32)];


class ChompSource : public Source {

private:
	Source &source;
	uint8_t slot;
	bool full;

public:
	ChompSource(Source &source) : source(source), full() { }

public:
	ssize_t read(void *buf, size_t n) override;

};


Sink & operator << (Sink &sink, Source &source);

unsigned long parse_ulong(const char in[], int base = 10);

std::string read_passphrase();
