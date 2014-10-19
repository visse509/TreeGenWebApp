
#pragma once
class TexCord
{
public:
	TexCord(float _u, float _v);
	float u,v;
	void indexToTexCord(int index,int n_edges);
	~TexCord(void);
};

