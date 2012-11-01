/*
 * egx.cpp
 *
 *  Created on: 25/10/2012
 *      Author: neonman
 */

#include "egx.h"

namespace egx {

//Class: Tool
Tool::Tool(){
	this->type = this->TYPE_MILL;
	this->description = "---- DEFAULT TOOL ----";
}

Tool::Tool(int type, std::string description){
	this->type = type;
	this->description = description;
}

Tool::~Tool(){

}

//Class: Point
Point::Point(double x, double y){
	this->x = x;
	this->y = y;
}

Point::~Point(){

}

//Class: Track
Track::Track(std::list<Point> points, Tool tool){
	this->points = points;
	this->tool = tool;
}

Track::Track(){

}

void Track::addPoint(Point p){
	this->points.push_back(p);
}

std::list<Point> Track::getPoints(){
	return this->points;
}

long Track::size(){
	return this->points.size();
}

Track::~Track(){

}

//Class: Layer
Layer::Layer(std::list<Track> tracks){
	this->tracks = tracks;
}

Layer::Layer(){

}

void Layer::addTrack(Track t){
	this->tracks.push_back(t);
}

std::list<Track> Layer::getTracks(){
	return this->tracks;
}

long Layer::size(){
	return this->tracks.size();
}

Layer::~Layer(){

}

} /* namespace egx */

