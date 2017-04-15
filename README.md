# oniongen-c

v3 .onion vanity URL generator written in C

## Usage

Compile ed25519 objects:
```
cd ed25519-supercop-ref10 ; gcc -std=c99 -Wall -pedantic -c $(find . -name '*.c' -a ! -name 'keypair.c') ; cd ..
```

Compile and run:
```
gcc -std=c99 -Wall -pedantic -o main *.c ed25519-supercop-ref10/*.o && ./main
```

## TODO

### Short term

Replicate functionality of [oniongen-go](https://github.com/rdkr/oniongen-go):

- [ ] Makefile
- [ ] ed25519
- [x] sha3
- [ ] base32
- [ ] regex
- [ ] cli arguments
- [ ] parallelise

### Longer term

Parallelise using CUDA:

- [ ] locally (GeForce)
- [ ] AWS cluster (Tesla)

## References

### v3 onion addresses

* tor-dev mailing list discussion - https://lists.torproject.org/pipermail/tor-dev/2017-January/011883.html

### sha3

* NIST FIPS SHA3 specification - http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf
* A C implementaiton - https://github.com/brainhub/SHA3IUF
* Official keccak implementation collection - https://github.com/gvanas/KeccakCodePackage
* A Python implementaiton - https://github.com/moshekaplan/python-sha3/blob/master/python_sha3.py


### ed25519

* IETF RFC on EdDSA - https://tools.ietf.org/html/rfc8032
* Bernstein paper - http://ed25519.cr.yp.to/ed25519-20110926.pdf
* Warner blog on keys and implementations - https://blog.mozilla.org/warner/2011/11/29/ed25519-keys/
* supercop-ref10 implementation - https://github.com/warner/python-ed25519/tree/ref10

## Acknowledgements

* keccakf implementation - https://github.com/brainhub/SHA3IUF
* ed25519 implementation - https://github.com/warner/python-ed25519/tree/ref10
