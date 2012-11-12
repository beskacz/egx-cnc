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

Point Point::operator+(Point p){
	Point temp(this->x + p.x, this->y + p.y);
	return temp;
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

void Track::translate(Point p){
	//For each point in the track, add the parameter point
	std::list<Point>::iterator iter = this->points.begin();
	while(iter != this->points.end()){
		*iter = (*iter) + p;
		iter++;
	}


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

void Layer::translate(Point p){
	std::list<Track>::iterator iter = this->tracks.begin();
	while(iter != this->tracks.end()){
		iter->translate(p);
		iter++;
	}
}

Layer::~Layer(){

}

//Class: Board
Board::Board(std::list<Layer> layers){
	this->layers = layers;
}

Board::Board(){

}

void Board::addLayer(Layer l){
	this->layers.push_back(l);
}

std::list<Layer> Board::getLayers(){
	return this->layers;
}

long Board::size(){
	return this->layers.size();
}

void Board::normalize(){

}

void Board::Translate(Point p){
	std::list<Layer>::iterator iter = this->layers.begin();
	while(iter != this->layers.end()){
		iter->translate(p);
		iter++;
	}
}
virtual Board::~Board(){

}

} /* namespace egx */

