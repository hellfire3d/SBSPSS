/*
	Improved VLC coding functions

	NOTE: This VLC decode function requires the GTE, so InitGeom must be called prior to use
	Also this function uses a table held in scratchpad ram ( taking approx 512 bytes )
*/

#ifdef __cplusplus
extern "C" {
#endif

// Setup function, this builds the bit decode table in scratchpad ram
void DecDCTvlcBuild3();

// Break size function, set 0 for no partial decode
int DecDCTvlcSize3( int breaksize );

// VLC decode function, returns 0 if successful, 1 if partially complete
int DecDCTvlc3( unsigned long *bs, unsigned long *buf );

#ifdef __cplusplus
}
#endif
