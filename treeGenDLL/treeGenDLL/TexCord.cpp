#include "TexCord.h"
#pragma unmanaged

TexCord::TexCord(float _u,float _v)
{
	u = _u;
	v = _v;
}

void TexCord::indexToTexCord(int index,int n_edges){
	int nDiv = index / n_edges;
	if(((index % n_edges)  == 0 ) &&  ((nDiv % 2) == 0)){
		u = 0;
		v = 0;
	}
	else if(((index % n_edges)  == 1 ) &&  ((nDiv % 2) == 0)){
		u = 0;
		v = 1;
	}
	else if(((index % n_edges)  == 0 ) &&  ((nDiv % 2) == 1)){
		u = 1;
		v = 0;
	}
	else if(((index % n_edges)  == 1 ) &&  ((nDiv % 2) == 1)){
		u = 1;
		v = 1;
	}
}
TexCord::~TexCord(void)
{
}
