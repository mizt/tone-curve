// GPL-3.0
// https://github.com/RawTherapee/RawTherapee/blob/dev/rtengine/diagonalcurves.cc

enum {
	X = 0,
	Y
};

unsigned char *LUT(std::vector<std::array<float,2>> *curve) {
	
	unsigned char *lut = new unsigned char[256];
	for(int n=0; n<256; n++) lut[n] = n; 

	unsigned int N = curve->size();
	float *u = new float[N-1];
	float *y = new float[N];
	
	y[0] = u[0] = 0.0; 
	
	for(int i=1; i<N-1; i++) {
		float sig = ((*curve)[i][X]-(*curve)[i-1][X])/((*curve)[i+1][X]-(*curve)[i-1][X]);
		float p = sig*y[i-1]+2.0;
		y[i] = (sig-1.0)/p;
		u[i] = (((*curve)[i+1][Y]-(*curve)[i][Y])/((*curve)[i+1][X]-(*curve)[i][X])-((*curve)[i][Y]-(*curve)[i-1][Y])/((*curve)[i][X]-(*curve)[i-1][X]));
		u[i] = (6.0*u[i]/((*curve)[i+1][X]-(*curve)[i-1][X])-sig*u[i-1])/p;
	}
	
	y[N-1] = 0.0;
	
	for(int i=N-2; i>=0; i--) {
		y[i] = y[i]*y[i+1]+u[i];
	}
	
	
	for(int n=0; n<256; n++) {
		
		float t = n/255.0;
		
		int k_lo = 0;
		int k_hi = N-1;
		
		float r = 0;
		
		if (t>(*curve)[N-1][X]) {
			r = (*curve)[N-1][Y];
		} 
		else if (t<(*curve)[0][X]) {
			r = (*curve)[0][Y];
		}
		else {
			
			while(k_hi>1+k_lo) {
				int k = ((k_hi+k_lo)/2)>>0;
				if((*curve)[k][X]>t) {
					k_hi = k;
				} 
				else {
					k_lo = k;
				}
			}
			
			float h = (*curve)[k_hi][X]-(*curve)[k_lo][X];
			float a = ((*curve)[k_hi][X]-t)/h;
			float b = (t-(*curve)[k_lo][X])/h;
			r = a*(*curve)[k_lo][Y]+b*(*curve)[k_hi][Y]+((a*a*a-a)*y[k_lo]+(b*b*b-b)*y[k_hi])*(h*h)*0.1666666666666666666666666666666;
		}
		
		if(r<=0) r = 0;
		if(r>=1) r = 1;
		
		lut[n] = 255-(r*255);
		
	}
	
	return lut;
};