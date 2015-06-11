# btctool

A multi-tool for Bitcoin keys and addresses, written in C++ with minimal dependencies.

**Functionality:**

* Encode and decode arbitrary data in Base16, Base64, and Base58Check.
* Hash using SHA-256 and RIPEMD-160.
* Disassemble Bitcoin Script.
* Construct private key from provided bits.
* Encrypt and decrypt private key per [BIP38][].
* Derive public key from private key.
* Derive P2PKH address from public key.
* Derive P2SH multi-sig address from public keys.
* Derive private keys from private seed.
* Derive public keys from public seed.
* Derive P2PKH addresses from public seed.
* Derive P2SH multi-sig addresses from public seeds.

Applets can accept their inputs either as command-line arguments or via standard input.

Specifying private keys via the command line poses a security risk on multi-user hosts.
Private keys should be specified via standard input for greatest security.

## Building

1. Install dependencies:

	* [GMP — The GNU Multiple Precision Arithmetic Library][GMP]

	**Gentoo:** `# emerge -n dev-libs/gmp`  
	**Ubuntu:** `$ sudo apt-get install build-essential libgmp-dev`

1. Check out sources:

		$ git clone --recursive https://github.com/whitslack/btctool.git
		$ cd btctool

1. Build:

		$ make
		$ cd out/"$(gcc -dumpmachine)"

## Usage

### encode16

	usage:
	  encode16
	    Encode stdin to hex.
<!-- -->
	$ echo 'Hello world!' | ./btctool encode16
	48656c6c6f20776f726c64210a

### decode16

	usage:
	  decode16 [<hex>]
	    Decode hex from stdin or argument.
<!-- -->
	$ ./btctool decode16 48656c6c6f20776f726c64210a
	Hello world!
<!-- -->
	$ echo 48656c6c6f20776f726c64210a | ./btctool decode16
	Hello world!

### encode64

	usage:
	  encode64 [<hex>]
	    Encode stdin or argument to Base64.
<!-- -->
	$ echo 'Hello world!' | ./btctool encode64
	SGVsbG8gd29ybGQhCg==
<!-- -->
	$ ./btctool encode64 48656c6c6f20776f726c64210a
	SGVsbG8gd29ybGQhCg==

### decode64

	usage:
	  decode64 [<base64>]
	    Decode Base64 from stdin or argument.
<!-- -->
	$ ./btctool decode64 SGVsbG8gd29ybGQhCg==
	Hello world!
<!-- -->
	$ echo SGVsbG8gd29ybGQhCg== | ./btctool decode64
	Hello world!

### encode58

	usage:
	  encode58 [<hex>]
	    Encode stdin or argument to Base58Check.
<!-- -->
	$ echo 'Hello world!' | ./btctool encode58
	gTazoqFi2U9CKLR6zpJ8CEV
<!-- -->
	$ ./btctool encode58 48656c6c6f20776f726c64210a
	gTazoqFi2U9CKLR6zpJ8CEV

### decode58

	usage:
	  decode58 [<base58>]
	    Decode Base58Check from stdin or argument.
<!-- -->
	$ ./btctool decode58 gTazoqFi2U9CKLR6zpJ8CEV
	Hello world!
<!-- -->
	$ echo gTazoqFi2U9CKLR6zpJ8CEV | ./btctool decode58
	Hello world!

### disasm

	usage:
	  disasm [<script>]
	    Disassemble hex script from stdin or argument.
<!-- -->
	$ ./btctool disasm 76a914c4c5d791fcb4654a1ef5e03fe0ad3d9c598f982788ac
	OP_DUP OP_HASH160 0xc4c5d791fcb4654a1ef5e03fe0ad3d9c598f9827 OP_EQUALVERIFY OP_CHECKSIG
<!-- -->
	$ echo 76a914c4c5d791fcb4654a1ef5e03fe0ad3d9c598f982788ac | ./btctool disasm
	OP_DUP OP_HASH160 0xc4c5d791fcb4654a1ef5e03fe0ad3d9c598f9827 OP_EQUALVERIFY OP_CHECKSIG

### sha256

	usage:
	  sha256 [<hex>]
	    Hash stdin or argument using SHA-256.
<!-- -->
	$ echo 'Hello world!' | ./btctool sha256
	0ba904eae8773b70c75333db4de2f3ac45a8ad4ddba1b242f0b3cfc199391dd8
<!-- -->
	$ ./btctool sha256 48656c6c6f20776f726c64210a
	0ba904eae8773b70c75333db4de2f3ac45a8ad4ddba1b242f0b3cfc199391dd8

### rmd160

	usage:
	  rmd160 [<hex>]
	    Hash stdin or argument using RIPEMD-160.
<!-- -->
	$ echo 'Hello world!' | ./btctool rmd160
	31bc8e633097159a704659964eadf48dd776e2c0
<!-- -->
	$ ./btctool rmd160 48656c6c6f20776f726c64210a
	31bc8e633097159a704659964eadf48dd776e2c0

### privkey

	usage:
	  privkey [<hex>]
	    Derive private key from stdin or argument.
<!-- -->
	$ ./btctool privkey < /dev/random
	L3p8oAcQTtuokSCRHQ7i4MhjWc9zornvpJLfmg62sYpLRJF9woSu
<!-- -->
	$ ./btctool privkey c4bbcb1fbec99d65bf59d85c8cb62ee2db963f0fe106f483d9afa73bd4e39a8a
	L3p8oAcQTtuokSCRHQ7i4MhjWc9zornvpJLfmg62sYpLRJF9woSu
<!-- -->
	$ ./btctool privkey "$(echo -n 'correct horse battery staple' | ./btctool sha256)"
	L3p8oAcQTtuokSCRHQ7i4MhjWc9zornvpJLfmg62sYpLRJF9woSu

### pubkey

	usage:
	  pubkey [<privkey>]
	    Derive public key from private key.
<!-- -->
	$ ./btctool pubkey L3p8oAcQTtuokSCRHQ7i4MhjWc9zornvpJLfmg62sYpLRJF9woSu
	0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71
<!-- -->
	$ echo L3p8oAcQTtuokSCRHQ7i4MhjWc9zornvpJLfmg62sYpLRJF9woSu | ./btctool pubkey
	0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71

### address

**P2PKH:**

	usage:
	  address [<pubkey>]
	    Derive address from public key.
<!-- -->
	$ ./btctool address 0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71
	1C7zdTfnkzmr13HfA2vNm5SJYRK6nEKyq8
<!-- -->
	$ echo 0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71 | ./btctool address
	1C7zdTfnkzmr13HfA2vNm5SJYRK6nEKyq8

You can verify the address produced by this command by manually hashing the public key and encoding its digest:

	$ ./btctool encode58 "00$(./btctool rmd160 "$(./btctool sha256 0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71)")"
	1C7zdTfnkzmr13HfA2vNm5SJYRK6nEKyq8

**P2SH multi-sig:**

	usage:
	  address <m> [<pubkey>...]
	    Derive m-of-n P2SH address from public keys.
<!-- -->
	$ ./btctool address 1 \
		0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71 \
		02ea7e5f909d33216c744c02173f322ef4d87a3b7cedb4d407f8a258dde24502d6
	3GYVbmYjeoHhHb4svsJeoERHMseupd8ReA
<!-- -->
	$ { echo 0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71
		echo 02ea7e5f909d33216c744c02173f322ef4d87a3b7cedb4d407f8a258dde24502d6 ; } |
		./btctool address 1
	3GYVbmYjeoHhHb4svsJeoERHMseupd8ReA
<!-- -->
	$ ./btctool address 1 \
		"$(./btctool pubkey "$(./btctool privkey "$(echo -n 'correct horse battery staple' | ./btctool sha256)")")" \
		"$(./btctool pubkey "$(./btctool privkey "$(echo -n 'opensesame' | ./btctool sha256)")")"
	3GYVbmYjeoHhHb4svsJeoERHMseupd8ReA

### encrypt

	usage:
	  encrypt [<privkey>]
	    Encrypt private key per BIP38.
<!-- -->
	$ ./btctool encrypt L3p8oAcQTtuokSCRHQ7i4MhjWc9zornvpJLfmg62sYpLRJF9woSu
	Enter passphrase (not echoed):
	6PYLr7QZWi5wipwCNgQKfJwqQeBQ6JnLVbz3sZceYRhf1yWZeTDy17CEGD
<!-- -->
	$ echo 'opensesame' | ./btctool encrypt L3p8oAcQTtuokSCRHQ7i4MhjWc9zornvpJLfmg62sYpLRJF9woSu
	6PYLr7QZWi5wipwCNgQKfJwqQeBQ6JnLVbz3sZceYRhf1yWZeTDy17CEGD
<!-- -->
	$ { ./btctool privkey c4bbcb1fbec99d65bf59d85c8cb62ee2db963f0fe106f483d9afa73bd4e39a8a
		echo 'opensesame' ; } | ./btctool encrypt
	6PYLr7QZWi5wipwCNgQKfJwqQeBQ6JnLVbz3sZceYRhf1yWZeTDy17CEGD

### decrypt

	usage:
	  decrypt [<privkey>]
	    Decrypt private key per BIP38.
<!-- -->
	$ ./btctool decrypt 6PYLr7QZWi5wipwCNgQKfJwqQeBQ6JnLVbz3sZceYRhf1yWZeTDy17CEGD
	Enter passphrase (not echoed):
	L3p8oAcQTtuokSCRHQ7i4MhjWc9zornvpJLfmg62sYpLRJF9woSu
<!-- -->
	$ echo 'opensesame' | ./btctool decrypt 6PYLr7QZWi5wipwCNgQKfJwqQeBQ6JnLVbz3sZceYRhf1yWZeTDy17CEGD
	L3p8oAcQTtuokSCRHQ7i4MhjWc9zornvpJLfmg62sYpLRJF9woSu
<!-- -->
	$ { echo 6PYLr7QZWi5wipwCNgQKfJwqQeBQ6JnLVbz3sZceYRhf1yWZeTDy17CEGD
		echo 'opensesame' ; } | ./btctool decrypt
	L3p8oAcQTtuokSCRHQ7i4MhjWc9zornvpJLfmg62sYpLRJF9woSu

### privkeys

	usage:
	  privkeys <k> [<privseed>]
	    Derive k private keys from private seed.
<!-- -->
	$ ./btctool privkeys 3 L3p8oAcQTtuokSCRHQ7i4MhjWc9zornvpJLfmg62sYpLRJF9woSu
	Kyqisrs8E2MwqoB5AeWinAiELDCEoTVjkiWgNyiSxtHWkDdtUx89
	KzUXcQjTvSBN1399VDhe1xpNDcC6k9dq96NgSTtz8BqWcXQ3G2oL
	L1k95WU9KFpCKX5J8N5UVZ2dzQBpdXv1ur6gZSG4SnwWM8sm45yc
<!-- -->
	$ echo L3p8oAcQTtuokSCRHQ7i4MhjWc9zornvpJLfmg62sYpLRJF9woSu | ./btctool privkeys 3
	Kyqisrs8E2MwqoB5AeWinAiELDCEoTVjkiWgNyiSxtHWkDdtUx89
	KzUXcQjTvSBN1399VDhe1xpNDcC6k9dq96NgSTtz8BqWcXQ3G2oL
	L1k95WU9KFpCKX5J8N5UVZ2dzQBpdXv1ur6gZSG4SnwWM8sm45yc

### pubkeys

	usage:
	  pubkeys <k> [<pubseed>]
	    Derive k public keys from public seed.
<!-- -->
	$ ./btctool pubkeys 3 0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71
	03d2cd68564c61d49e86b76b16bb86b605e3ffae37ad04992540b3a91b1404caf8
	02ca341396eb0e90cd99b6b396308c8acfedca8382e58898e4cae58a1c814aa6ba
	03d49806dc2c3fa8a27d3451ccfdbd1144d2c09d9c72bd0ba9e4d5f68eddb9f80f
<!-- -->
	$ echo 0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71 | ./btctool pubkeys 3
	03d2cd68564c61d49e86b76b16bb86b605e3ffae37ad04992540b3a91b1404caf8
	02ca341396eb0e90cd99b6b396308c8acfedca8382e58898e4cae58a1c814aa6ba
	03d49806dc2c3fa8a27d3451ccfdbd1144d2c09d9c72bd0ba9e4d5f68eddb9f80f

You can verify that the public keys produced by this command indeed correspond one-to-one with the private keys produced by the `privkeys` command:

	$ for privkey in $(./btctool privkeys 3 L3p8oAcQTtuokSCRHQ7i4MhjWc9zornvpJLfmg62sYpLRJF9woSu) ; do
		./btctool pubkey "${privkey}" ; done
	03d2cd68564c61d49e86b76b16bb86b605e3ffae37ad04992540b3a91b1404caf8
	02ca341396eb0e90cd99b6b396308c8acfedca8382e58898e4cae58a1c814aa6ba
	03d49806dc2c3fa8a27d3451ccfdbd1144d2c09d9c72bd0ba9e4d5f68eddb9f80f

### addresses

**P2PKH:**

	usage:
	  addresses <k> [<pubseed>]
	    Derive k addresses from public seed.
<!-- -->
	$ ./btctool addresses 3 0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71
	16MqnKxF7WNsGG7Aksn2d982uLe6NjpCq6
	13SUk8ySvnUoWiHyG3bydEtGrfyaJNJ5wT
	19r3Mjh1krhFybuV9RjmR2nuM9WJvbH75F
<!-- -->
	$ echo 0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71 | ./btctool addresses 3
	16MqnKxF7WNsGG7Aksn2d982uLe6NjpCq6
	13SUk8ySvnUoWiHyG3bydEtGrfyaJNJ5wT
	19r3Mjh1krhFybuV9RjmR2nuM9WJvbH75F

You can verify that the addresses produced by this command indeed correspond one-to-one with the public keys produced by the `pubkeys` command:

	$ for pubkey in $(./btctool pubkeys 3 0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71) ; do
		./btctool address "${pubkey}" ; done
	16MqnKxF7WNsGG7Aksn2d982uLe6NjpCq6
	13SUk8ySvnUoWiHyG3bydEtGrfyaJNJ5wT
	19r3Mjh1krhFybuV9RjmR2nuM9WJvbH75F

**P2SH multi-sig:**

	usage:
	  addresses <k> <m> [<pubseed>...]
	    Derive k m-of-n P2SH addresses from public seeds.
<!-- -->
	$ ./btctool addresses 3 1 \
		0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71 \
		02ea7e5f909d33216c744c02173f322ef4d87a3b7cedb4d407f8a258dde24502d6
	3Moyic1GdKfbp8ThFkzg1p84QZWtrVRE8k
	3LSCy5vN9qZ9QPLLMn7xoQxCoD1zix2uMJ
	3LTe2vn5m6ydshPywAfedEqekTgtgYAmvo
<!-- -->
	$ { echo 0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71
		echo 02ea7e5f909d33216c744c02173f322ef4d87a3b7cedb4d407f8a258dde24502d6 ; } |
		./btctool addresses 3 1
	3Moyic1GdKfbp8ThFkzg1p84QZWtrVRE8k
	3LSCy5vN9qZ9QPLLMn7xoQxCoD1zix2uMJ
	3LTe2vn5m6ydshPywAfedEqekTgtgYAmvo

You can verify that the addresses produced by this command indeed correspond one-to-one with the public keys produced by the `pubkeys` command:

	$ while read -r pubkey1 <&3 && read -r pubkey2 <&4 ; do
		./btctool address 1 "${pubkey1}" "${pubkey2}" ; done \
		3< <(./btctool pubkeys 3 0378d430274f8c5ec1321338151e9f27f4c676a008bdf8638d07c0b6be9ab35c71) \
		4< <(./btctool pubkeys 3 02ea7e5f909d33216c744c02173f322ef4d87a3b7cedb4d407f8a258dde24502d6)
	3Moyic1GdKfbp8ThFkzg1p84QZWtrVRE8k
	3LSCy5vN9qZ9QPLLMn7xoQxCoD1zix2uMJ
	3LTe2vn5m6ydshPywAfedEqekTgtgYAmvo


[BIP38]: https://github.com/bitcoin/bips/blob/master/bip-0038.mediawiki
[GMP]: https://gmplib.org/
