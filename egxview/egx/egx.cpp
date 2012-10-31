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

//egx file loader
Layer* loadLayer_egx(std::istream* f){
	std::string tmp_s("");
	Layer* l = new Layer();
	Track* t = new Track();
	long last_x = 0;
	long last_y = 0;

	//Read char-by-char until end-of-stream
	while(!f->eof()){
		//Append into tmp_s until ';' is found
		char c[2] = {'\0', '\0'};
		f->read(c, 1);
		if (c[0] != ';'){
			const boost::regex charset("^[A-Z0-9\\!\\,]$");
			if(boost::regex_match(c, charset))
				tmp_s.push_back(c[0]);
		}
		else{ //';' found, check if it matches any command regex
			const boost::regex pu_param ("^PU\\d+\\,\\d+$");
			const boost::regex pu("^PU$");
			const boost::regex pd_param ("^PD\\d+\\,\\d+$");

            if (boost::regex_match(tmp_s, pu)){ /* Put-UP, generally */
				if (t->size()>0){
					l->addTrack(*t);
					delete(t);
					t = new Track();
				}
            }
			if (boost::regex_match(tmp_s, pu_param)){ /* Put-UP, with parameters */
				if (t->size()>0){
					l->addTrack(*t);
					delete(t);
					t = new Track();
				}
				tmp_s = tmp_s.substr(2);
				std::string s_x = tmp_s.substr(0,tmp_s.find(','));
				std::string s_y = tmp_s.substr(tmp_s.find(',')+1);
				last_x = atol(s_x.c_str());
				last_y = atol(s_y.c_str());
			}
			if (boost::regex_match(tmp_s, pd_param)){ /* Put-Down (add segment to track)*/
				tmp_s = tmp_s.substr(2);
				std::string s_x = tmp_s.substr(0,tmp_s.find(','));
				std::string s_y = tmp_s.substr(tmp_s.find(',')+1);
				long cx = atol(s_x.c_str());
				long cy = atol(s_y.c_str());
				if (t->size() == 0){
					Point p0(last_x * 0.01, last_y * 0.01);
				    t->addPoint(p0);
				}
				Point p(cx * 0.01, cy * 0.01);
				t->addPoint(p);
				last_x = cx;
				last_y = cy;
			}
			tmp_s = "";
		}
	}
	if (t->size() > 0)
		l->addTrack(*t);
	delete(t);
	return l;
}

} /* namespace egx */

