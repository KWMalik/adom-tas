/**
Provides general utility functions.
**/
#ifndef UTIL_H
#define UTIL_H

/**
Returns the bits of a byte.

@param byte The byte.
@return An argument list of the bits.
**/
#define BITS(byte) \
	byte&0b10000000 ? 1 : 0,\
	byte&0b01000000 ? 1 : 0,\
	byte&0b00100000 ? 1 : 0,\
	byte&0b00010000 ? 1 : 0,\
	byte&0b00001000 ? 1 : 0,\
	byte&0b00000100 ? 1 : 0,\
	byte&0b00000010 ? 1 : 0,\
	byte&0b00000001 ? 1 : 0

/**
Sets the print format of a byte.

Works in conjuction with the <code>BITS</code> macro:
<pre>
printf("0x%x = 0b"BYTE, 42, BITS(42));
</pre>
**/
#define BYTE "%d%d%d%d%d%d%d%d"

#endif
