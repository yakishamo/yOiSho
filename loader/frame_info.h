#ifndef FRAME_INFO_H
#define FRAME_INFO_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
	RGB,
	BGR,
	BitMask,
	BltOnly,
	FormatMax
} PixelFormat;

typedef struct {
	unsigned int red;
	unsigned int green;
	unsigned int blue;
	unsigned int reserved;
} PixelBitmask;

typedef struct {
	unsigned char      *frame_base;
	unsigned long long  frame_size;
	PixelFormat         format;
	PixelBitmask        mask;
	unsigned int        horizontal_resolution;
	unsigned int        vertical_resolution;
	unsigned int        pixel_per_scanline;
} FrameInfo;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FRAME_INFO_H */
