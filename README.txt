/*
*** FILE NAME   : csl_clipping.c
*** DESCRIPTION : This program is an example program using OpenGL.
*** DATE        : Jan. 2015
*** WRITTEN By  : farheen
*/

/*
Program performs Clipping using Cohen Sutherland Algorithm

Uses Outcode to determine if line is Above, Below, Left, Right of a Clipping window.
Data structures used:

Methods used:

draw_rosette() --> draws the 7-rosette
draw_rectangle --> takes the cooridnates of lower and uppper boundary after user selects with mouse and draws a clipping boundary.

cohen_sutherland_clipping() --> takes the two end points of a line as parameters and determines if it is inside, outside, partially in or partially outside.
compute_outcode() --> takes x and y coordinates of a line and finds the outcode (above, below, left, right)
draw_line() --> after the clipping, draws the clipped line segment
display_text() --> displays the instructions on screen.

Mode of Execution:

1. Draw the rosette --> draw_rosette()
2. Prompt the user to select lower and upper boundary and then draw the clipping window. --> draw_rectangle()
3. Clip the lines against the clipping window --> cohen_sutherland_clipping(). This function internally calls compute_outcode().
4. Draw the clipped lines --> draw_line()

*/
