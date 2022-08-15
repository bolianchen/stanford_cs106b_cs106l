#include "Circle.h"
using namespace std;
#include "testing/SimpleTest.h"
#include "gmath.h"
#include "strlib.h"

/**
 * construct a new circle with the given radius
 * @r radius
 */
Circle::Circle(double radius) {
    this->radius = radius;
}


/**
 * compute the area of the circle
 */
double Circle::area() const {
    return PI * pow(radius, 2);
}

/**
 * compute the circumference of the circle
 */
double Circle::circumference() const{
    return 2 * PI * radius;
}

/**
 * get the radius of the circle
 */
double Circle::getRadius() const{
    return radius;
}

/**
 * returns a string representation for the circle such as
 * "Circle{radius=2.5}"
 */
string Circle::toString() const{
    return "Circle{radius=" + realToString(radius) + "}";
}

/* Provided Tests Below This Line */

PROVIDED_TEST("Simple Example"){
    Circle c(2.5);
    EXPECT_EQUAL(c.getRadius(), 2.5);
    EXPECT_EQUAL(c.circumference(), 2 * PI * 2.5);
    EXPECT_EQUAL(c.area(), PI * 2.5 * 2.5);
    EXPECT_EQUAL(c.toString(), "Circle{radius=2.5}");
}
