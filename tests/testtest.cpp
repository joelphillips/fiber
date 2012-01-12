/*
 * testtest.cpp
 *
 *  Created on: Jan 5, 2012
 *      Author: joel
 */

#include <boost/test/unit_test.hpp>
#include "../prototype/openclexperiments.hpp"
#include "../prototype/integrator.hpp"

#include <boost/assign/std/vector.hpp> // for 'operator+=()'
#include <iostream>

using namespace std;
using namespace boost::assign;


BOOST_AUTO_TEST_CASE(test_that_I_can_test){
	BOOST_CHECK_EQUAL(3,3);
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_prototype){
	testopencl();
}

BOOST_AUTO_TEST_CASE(test_polynomial){
	std::vector<double> c1,c2,c3;
	c1+=1.0,-1.0,-1.0;
	c2+=0.0, 1.0, 0.0;
	c3+=0.0, 0.0, 1.0;
	Polynomial f1(c1, 1); // 1 - x - y
	Polynomial f2(c2, 1); // x
	Polynomial f3(c3, 1); // y
	Point3 p1 = {0,0,1};
	Point3 p2 = {1,0,0};
	Point3 p3 = {0,1,0};
	Point3 pc = {1.0/3,1.0/3,1.0/3};
	BOOST_CHECK_EQUAL(f1.evaluate(p1),1);
	BOOST_CHECK_EQUAL(f1.evaluate(p2),0);
	BOOST_CHECK_EQUAL(f1.evaluate(p3),0);
	BOOST_CHECK_EQUAL(f2.evaluate(p1),0);
	BOOST_CHECK_EQUAL(f2.evaluate(p2),1);
	BOOST_CHECK_EQUAL(f2.evaluate(p3),0);
	BOOST_CHECK_EQUAL(f3.evaluate(p1),0);
	BOOST_CHECK_EQUAL(f3.evaluate(p2),0);
	BOOST_CHECK_EQUAL(f3.evaluate(p3),1);
	BOOST_CHECK_CLOSE(f1.evaluate(pc),1.0/3,1E-12);
	BOOST_CHECK_CLOSE(f2.evaluate(pc),1.0/3,1E-12);
	BOOST_CHECK_CLOSE(f3.evaluate(pc),1.0/3,1E-12);
}
