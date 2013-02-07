/*
 * egx.h
 *
 *  Created on: 25/10/2012
 *      Author: neonman
 */

#ifndef EGX_H_
#define EGX_H_

#include <list>
#include <string>
#include <boost/regex.hpp>
#include <float.h>

namespace egx {

//Tool specs
class Tool{
public:
	//Static constants
	const static int TYPE_DRILL   = 1;
	const static int TYPE_MILL    = 2;

private:
	//Class attributes
	std::string description;
	int type;

public:
	Tool(); //Default tool
	Tool(int type, std::string description);
	virtual ~Tool();
};

//One point, coordinates in mm
class Point {
public:
	double x;
	double y;
	Point(double x, double y);
	virtual ~Point();
	Point operator+(Point p);
};

///A set of contiguous track segments
class Track {
private:
	std::list<Point> points;
	Tool tool;
public:
	Track(std::list<Point> segments, Tool tool);
	Track();
	void addPoint(Point s);
	std::list<Point> getPoints();
	long size();
	void translate(Point p);
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
	void translate(Point p);
	virtual ~Layer();
};

class Board {
private:
	std::list<Layer> layers;
public:
	Board(std::list<Layer> layers);
	Board();
	void addLayer(Layer l);
	std::list<Layer> getLayers();
	long size();
	void normalize();
	void translate(Point p);
	virtual ~Board();
};

} /* namespace egx */
#endif /* EGX_H_ */
