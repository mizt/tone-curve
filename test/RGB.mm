#import <Foundation/Foundation.h>
#import <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
namespace stb_image {
	#import "stb_image.h"
	#import "stb_image_write.h"
}

#import "LUT.h"

int main(int argc, char *argv[]) {
	@autoreleasepool {
		
		int info[3];
		unsigned int *src = (unsigned int *)stb_image::stbi_load("./test.jpg",info+0,info+1,info+2,4);
		if(src) {
			
			int w = info[0];
			int h = info[1]; 
			
			std::vector<std::array<float,2>> curve = {{0,1},{0.39215686274509803,0.8156862745098039},{0.592156862745098,0.45098039215686275},{1,0}};
			unsigned char *lut = LUT(&curve);
			
			for(int i=0; i<h; i++) {
				for(int j=0; j<w; j++) {
					unsigned int tmp = src[i*w+j];
					unsigned char b = lut[(tmp>>16)&0xFF];
					unsigned char g = lut[(tmp>>8)&0xFF];
					unsigned char r = lut[(tmp)&0xFF];
					src[i*w+j] = 0xFF000000|b<<16|g<<8|r;
				}
			}
			
			stb_image::stbi_write_png("./dst.png",w,h,4,(void const*)src,w<<2);
		}
	}
}