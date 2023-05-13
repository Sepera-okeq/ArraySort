// //////////////////////////////////////////////////////////
// struct.h
// Copyright (c) 2023 Sergey Leshkevich.

// g++ -O3 sort_struct.cpp -o sort_struct -std=c++11


#pragma once

#include <algorithm>
#include <iterator>

#include "sort.h"

class TestTime 
{
	public:
		int h, m, s;
		
	TestTime(int h, int m, int s) 
	{
		this.h = h;
		this.m = m;
		this.s = s;
	}
}