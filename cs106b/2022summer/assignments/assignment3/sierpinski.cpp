/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * Draw Sierpinski Triangle and return the total black triangles drawn for the specified order
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    if (order == 0) {
        fillBlackTriangle(window, one, two, three);
        return 1;
    } else {
        int countDrawn = 0;
        GPoint p12 = GPoint((one.x + two.x) / 2, (one.y + two.y) / 2);
        GPoint p13 = GPoint((one.x + three.x) / 2, (one.y + three.y) / 2);
        GPoint p23 = GPoint((two.x + three.x) / 2, (two.y + three.y) / 2);
        countDrawn += drawSierpinskiTriangle(window, one, p12, p13, order - 1);
        countDrawn += drawSierpinskiTriangle(window, p12, two, p23, order - 1);
        countDrawn += drawSierpinskiTriangle(window, p13, p23, three, order - 1);
        return countDrawn;
    }
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

