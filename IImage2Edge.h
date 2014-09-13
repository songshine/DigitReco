#ifndef _IIMAGE2EDGE_H_
#define _IIMAGE2EDGE_H_




class IImage2Edge
{
public:
	IImage2Edge(){}
	virtual IplImage* Convert2Edge(const IplImage* img) = 0;
};
#endif