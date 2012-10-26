/*
 * egx.h
 *
 *  Created on: 25/10/2012
 *      Author: neonman
 */

#ifndef EGX_H_
#define EGX_H_

#include <istream>
#include <list>
#include <boost/regex.hpp>
#include <GL/glew.h> /* Needed for gl types */

namespace egx {

//Types & structs
struct point{
	GLfloat x;
	GLfloat y;
};

///A single track segment or a drill point
class Segment {
public:
	long xo, yo;
	long xd, yd;
public:
	Segment(long x_orig, long y_orig, long x_dest, long y_dest);
	Segment(long x, long y);
	virtual ~Segment();
};

///A set of contiguous track segments
class Track {
private:
	std::list<Segment> seg;
	double tool_diameter;
public:
	Track(std::list<Segment> segments, double diameter);
	Track();
	void addSegment(Segment s);
	std::list<Segment> getSegments();
	point* getPointArray();
	long size();
	virtual ~Track();
};

///A set of unique, zero-length segments
class DrillPlot : public Track{

};

class Layer {
private:
	std::list<Track> tracks;
public:
	Layer(std::list<Track> tracks);
	Layer();
	void addTrack(Track t);
	std::list<Track> getTracks();
	long size();
	virtual ~Layer();
};

//Helper functions
///Loads an egx file and returns a Layer object
Layer* loadLayer_egx(std::istream* f);

} /* namespace egx */
#endif /* EGX_H_ */
