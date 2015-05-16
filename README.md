#pi

Utility for compressing and searching for numbers within the digits of pi

##Installation

```bash
make && sudo make install
```

##Usage

```bash
Usage: ./pi [-s,--search] <infile> <search>
            [-c,--compress] <infile> (<outfile>)
            [-h,--help]
```

Note: This utility will only search a compressed list of pi, so it must be compressed first.

##Example

Search for the first occurrence of the SSN [721-07-4426](https://upload.wikimedia.org/wikipedia/commons/thumb/0/03/PD_social_security_card.png/640px-PD_social_security_card.png):

```bash
make download
pi --compress digits.txt pi.txt
pi --search pi.txt 721-07-4426
```