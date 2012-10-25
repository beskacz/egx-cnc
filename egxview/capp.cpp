/*
 * capp.cpp
 *
 *  Created on: 25/10/2012
 *      Author: neonman
 */

#include "capp.h"
#include <iostream>
#include <istream>
#include <fstream>
using namespace std;

#include "egx.h"


//Argument 1 -> input file
int main(int argc, char* argv[]){
	ifstream* egxfile;
	if (argc == 1){
		const char default_file[] =  "/home/neonman/egx-cnc/sample/egx/ASD.egx";
		cout<<"No file provided on cmd, loading <"<<default_file<<">"<<endl;
		egxfile = new ifstream(default_file);
		if(egxfile->fail()){
			cerr<<"Failed to load <"<<default_file<<">"<<endl;
			return(-1);
	    }
	}
	else if (argc == 2){
		//Load supplied file as egx
		cout<<"Loading <"<<argv[1]<<">"<<endl;
		egxfile = new ifstream(argv[1]);
		if(egxfile->fail()){
			cerr<<"Failed to load <"<<argv[1]<<">"<<endl;
			return(-1);
	    }
	}
	else
		return(-1);

	egx::Layer* l;
	l = egx::loadLayer_egx(egxfile);
	delete(egxfile);

	list<egx::Track> trk = l->getTracks();
	list<egx::Track>::iterator iter = trk.begin();
	cout << "Layer has "<<trk.size()<<" Tracks"<<endl;
	while(iter != trk.end()){
		cout<<"  Track has "<<(iter->getSegments().size())<<" segments"<<endl;
		iter++;
	}
	delete(l);
	cout<<"Program end"<<endl;
}
