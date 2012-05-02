/*
 * Greg/Markov.h
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

#ifndef Greg_Markov_h
#define Greg_Markov_h

#include <vector>
#include <map>
#include <string>

namespace Markov {
	
	struct MarkovNode;
	
	struct MarkovLink {
		double probability;
		MarkovNode * from;
		MarkovNode * to;
		MarkovLink(MarkovNode * from, MarkovNode * to, double probability);
	};
	
	struct MarkovNode {
		std::string id;
		void * data;
		std::vector<MarkovLink *> * transitions;
		MarkovNode(std::string id);
		~MarkovNode();
	};
	
	class MarkovModel {
	public:
		MarkovModel(const char * file);
		virtual ~MarkovModel();
		std::map<std::string, MarkovNode *> nodes;
		void buildModel(const char * file);
		MarkovNode * addNode(std::string id);
		std::string next(std::string key);
	};
	
};

#endif
