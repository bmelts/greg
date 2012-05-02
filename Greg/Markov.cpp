/*
 * Greg/Markov.cpp
 *
 * Copyright (c) 2011-2012 Bill Meltsner
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the author be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "Markov.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace Markov {
	
	MarkovNode::MarkovNode(std::string key) : id(key)
	{ 
		transitions = new std::vector<MarkovLink *>();
	}
	
	MarkovNode::~MarkovNode()
	{
		while (!transitions->empty()) {
			delete transitions->back();
			transitions->pop_back();
		}
		delete transitions;
	}
	
	MarkovLink::MarkovLink(MarkovNode * from, MarkovNode * to, double probability)
	: from(from), to(to), probability(probability) { }
	
	MarkovModel::MarkovModel(const char * file)
	{
		buildModel(file);
	}
	
	MarkovModel::~MarkovModel()
	{
		for (std::map<std::string, MarkovNode *>::iterator i = nodes.begin(); i != nodes.end(); ++i) {
			delete i->second;
		}
	}
	
	void MarkovModel::buildModel(const char * file)
	{
		using namespace std;
		ifstream in(file);
		if (!in) {
			cerr << "unable to load file: " << file << endl;
			return;
		}
		string str;
		string token;
		vector<string> tokens;
		
		while (getline(in, str)) {
			tokens.clear();
			istringstream iss(str);
			while (getline(iss, token, '\t')) {
				tokens.push_back(token);
			}
			if (tokens.size() != 3) continue;
			MarkovNode * from = addNode(tokens[0]);
			MarkovNode * to = addNode(tokens[2]);
			MarkovLink * link = new MarkovLink(from, to, atof(tokens[1].c_str()));
			from->transitions->push_back(link);
		}
	}
	
	MarkovNode * MarkovModel::addNode(std::string id)
	{
		if (!nodes.count(id)) nodes[id] = new MarkovNode(id);
		return nodes[id];
	}
	
	std::string MarkovModel::next(std::string key)
	{
		if (!nodes.count(key)) return "";
		MarkovNode * node = nodes[key];
		double prob = (double)rand() / (double)RAND_MAX;
		double totalprob = 0;
		for (std::vector<MarkovLink *>::iterator link_iter = node->transitions->begin(); link_iter != node->transitions->end(); ++link_iter) {
			totalprob += (*link_iter)->probability;
			if (prob < totalprob) return (*link_iter)->to->id;
		}
		return NULL;
	}
}