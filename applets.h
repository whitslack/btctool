#define _(applet) \
	extern const char applet##_usage[]; \
	int applet##_main(int argc, char *argv[]);

_(encode16)
_(decode16)

_(encode64)
_(decode64)

_(encode58)
_(decode58)

_(disasm)

_(sha256)
_(rmd160)

_(privkey)
_(pubkey)
_(address)

_(encrypt)
_(decrypt)

_(privkeys)
_(pubkeys)
_(addresses)

#undef _
