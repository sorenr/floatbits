#include <cstdint> // int32_t
#include <cstring> // memcpy
#include <cstdio> // printf
#include <cmath> // pow

void print_bytes(const char* const label, uint32_t bytes)
{
	printf( "foo_i = 0x%I32x\n\n", bytes );

	for ( int i = sizeof( bytes ) - 1; i >= 0; i-- )
	{
		uint8_t b = 0xff & (bytes >> i * 8);
		printf( "%d = %02x\n", i, b );
	}
	printf("\n");
}

int main()
{
	float foo_f = static_cast<float>( 4.1 );
	printf( "foo_f = %0.16f\n\n", foo_f );

	// print the int, making sure our byte order is unsurprising
	uint32_t foo_i = 0xdeadbeef;
	print_bytes( "foo_i", foo_i );

	// check that sizes are consistent
	static_assert( sizeof( foo_f ) == sizeof( foo_i ) );

	// blindly copy the bytes from float32 to int32 without casting
	memcpy( &foo_i, &foo_f, sizeof( foo_f ) );

	// print the byte representation of 4.1
	print_bytes( "foo_i", foo_i );

	// extract the components of the IEEE-754 float
	// https://www.h-schmidt.net/FloatConverter/IEEE754.html
	const int sign = ( foo_i >> 31 ) & 0x1;
	const int exp = ( foo_i >> 23 ) & 0xff;
	const int frac = foo_i & ( ( 1 << 23 ) - 1 );
	printf( "sign = %d\n", sign );
	printf( "exp = %d\n", exp );
	printf( "frac = %d\n\n", frac );

	// reassemble the components into the original value
	const double sign_i = pow( -1, sign );
	const double exp_f = pow( 2, exp - 127 );
	const double frac_f = static_cast<float>(frac) / (1 << 23);
	printf( "sign_i = %0.16f\n", sign_i );
	printf( "exp_i = %0.16f\n", exp_f );
	printf( "frac_f = 1.0 + %0.16f\n\n", frac_f );

	printf( "result = %0.16f\n\n", sign_i * exp_f * (1.0 + frac_f) );

	return(0);
}
