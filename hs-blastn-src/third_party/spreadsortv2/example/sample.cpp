// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#include <boost/algorithm/sorting/spread_sort.hpp>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace boost;

#define DATA_TYPE int


//Pass in an argument to test std::sort
int main(int argc, const char ** argv) {
	size_t uCount,uSize=sizeof(DATA_TYPE);
  //worst-case handling code check
  /*for(unsigned u = 0; u <= 64; ++u)
    printf("%d %d\n", u, detail::get_max_count<detail::LOG_MEAN_BIN_SIZE, 
    detail::LOG_MIN_SPLIT_COUNT, detail::LOG_FINISHING_COUNT>(u));
  printf("float_sort\n");
  for(unsigned u = 0; u <= 64; ++u)
    printf("%d %d\n", u, detail::get_max_count<detail::FLOAT_LOG_MEAN_BIN_SIZE, 
    detail::FLOAT_LOG_MIN_SPLIT_COUNT, detail::FLOAT_LOG_FINISHING_COUNT>(u));*/
	bool stdSort = false;
	unsigned loopCount = 1;
	for(int u = 1; u < argc; ++u) {
		if(std::string(argv[u]) == "-std")
			stdSort = true;
		else
			loopCount = atoi(argv[u]);
	}
	std::ifstream input("input.txt", std::ios_base::in | std::ios_base::binary);
	if(input.fail()) {
		printf("input.txt could not be opened\n");
		return 1;
	}
	double total = 0.0;
	std::vector<DATA_TYPE> array;
	input.seekg (0, std::ios_base::end);
    size_t length = input.tellg();
	uCount = length/uSize;
	//Run multiple loops, if requested
	for(unsigned u = 0; u < loopCount; ++u) {
		input.seekg (0, std::ios_base::beg);
		//Conversion to a vector
		array.resize(uCount);
		unsigned v = 0;
		while ( input.good() && v < uCount)
		 input.read( (char *) &(array[v++]), uSize );
		if(v < uCount)
			array.resize(v);
		clock_t start, end;
		double elapsed;
		start = clock();
		if(stdSort)
			//std::sort(&(array[0]), &(array[0]) + uCount);
			std::sort(array.begin(), array.end());
		else
			//spread_sort(&(array[0]), &(array[0]) + uCount);
			spread_sort(array.begin(), array.end());
		end = clock();
		elapsed = ((double) (end - start)) ;
		std::ofstream ofile;
		if(stdSort)
			ofile.open("standard_sort_out.txt", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
		else
			ofile.open("spread_sort_out.txt", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
		if(ofile.good()) {
			for(unsigned v = 0; v < array.size(); ++v) {
				ofile.write( (char *) &(array[v]), sizeof(array[v]) );
			}
			ofile.close();
		}
		total += elapsed;
		array.clear();
	}
	input.close();
	if(stdSort)
		printf("std::sort elapsed time %f\n", total / CLOCKS_PER_SEC);
	else
		printf("spreadsort elapsed time %f\n", total / CLOCKS_PER_SEC);
	return 0;
}
