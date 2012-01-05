/*
 * testtest.cpp
 *
 *  Created on: Jan 5, 2012
 *      Author: joel
 */

#include <boost/test/unit_test.hpp>
#include "../prototype/openclexperiments.hpp"

BOOST_AUTO_TEST_CASE(test_that_I_can_test){
	BOOST_CHECK_EQUAL(3,3);
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_prototype){
	testopencl();
}
