#include <rapidcheck.h>
#include <graph.hpp>
#include <timsort.hpp>
#include <vector>
#include <set>

int main() {
	// graph-test.cpp
	
	rc::check("graph, multiple vertex addition", [](const std::set<int>& v) {
		graph<int,int> *tg1;
		tg1 = new graph<int,int>();
		
		for(auto i : v) {
			tg1->addVertex(i);
		}
		
		RC_ASSERT(tg1->getNumVertexes() == v.size());
		
		delete tg1;
	});
	
	rc::check("graph, multiple vertex addition and removal", [](const std::set<int>& x, const std::set<int>& y) {
		std::set<int> alladds = x;
		for(auto i : y) {
			alladds.insert(i);
		}
		
		graph<int,int> *tg1;
		tg1 = new graph<int,int>();
		
		for(auto i : alladds) {
			tg1->addVertex(i);
		}
		for(auto i : y) {
			tg1->removeVertex(tg1->getVertexForValue(i));
		}
		
		RC_ASSERT(tg1->getNumVertexes() == (alladds.size() - y.size()));
		
		delete tg1;
	});
	
	// timsort-test.cpp
	
	rc::check("timsort, sort high to low", [](const std::vector<double>& v0) {
		size_t len = v0.size();
		std::pair<double, size_t>* c = new std::pair<double, size_t>[len];
		// Generate a list of random doubles with increasing size_t labels.
		for(size_t i = 0; i < len; ++i) {
			c[i] = std::pair<double, size_t>{v0[i],i};
		}

		madlib::sortHighToLow(c,len);
		if(len>1) {
			for(size_t i = 0; i+1 < len; i++) {
				RC_ASSERT(c[i].first >= c[i+1].first);
			}
		}
		delete[] c;
	});

	rc::check("timsort, sort low to high", [](const std::vector<double>& v0) {
		size_t len = v0.size();
		std::pair<double, size_t>* c = new std::pair<double, size_t>[len];
		// Generate a list of random doubles with increasing size_t labels.
		for(size_t i = 0; i < len; ++i) {
			c[i] = std::pair<double, size_t>{v0[i],i};
		}
		
		madlib::sortLowToHigh(c,len);
		if(len>1) {
			for(size_t i = 0; i+1 < len; i++) {
				RC_ASSERT(c[i].first <= c[i+1].first);
			}
		}
		delete[] c;
	});

	return 0;
}
