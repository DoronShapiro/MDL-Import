#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "gmath.h"
#include "importers/import_obj.h"

/*======== void add_box() ==========
Inputs:   struct matrix * points
double x
double y
double z
double width
double height
double depth  
Returns: 

04/04/12 12:25:47
jdyrlandweaver
====================*/
void add_box( struct matrix * points, double x, double y, double z, 
        double width, double height, double depth ) {

    double x1, y1, z1;

    x1 = x + width;
    y1 = y - height;
    z1 = z - depth;

    //front
    add_polygon( points, 
            x, y, z,
            x, y1, z,
            x1, y1, z);
    add_polygon( points, 
            x1, y1, z, 
            x1, y, z,
            x, y, z);
    //back
    add_polygon( points, 
            x1, y, z1,
            x1, y1, z1,
            x, y1, z1);
    add_polygon( points, 
            x, y1, z1, 
            x, y, z1,
            x1, y, z1);
    //top
    add_polygon( points, 
            x, y, z1,
            x, y, z,
            x1, y, z);
    add_polygon( points, 
            x1, y, z, 
            x1, y, z1,
            x, y, z1);
    //bottom
    add_polygon( points, 
            x, y1, z,
            x, y1, z1,
            x1, y1, z1);
    add_polygon( points, 
            x1, y1, z1, 
            x1, y1, z,
            x, y1, z);
    //left
    add_polygon( points, 
            x, y, z1,
            x, y1, z1,
            x, y1, z);
    add_polygon( points, 
            x, y1, z, 
            x, y, z,
            x, y, z1);
    //right
    add_polygon( points, 
            x1, y, z,
            x1, y1, z,
            x1, y1, z1);
    add_polygon( points, 
            x1, y1, z1, 
            x1, y, z1,
            x1, y, z);
}

void add_sphere( struct matrix * points, 
        double cx, double cy, double cz, double r, 
        double step ) {

    struct matrix * temp;
    int lat, longt;
    int index;
    double ns;
    int num_steps;

    ns = 1.0 / step;
    num_steps = (int)ns;

    temp = new_matrix( 4, num_steps * num_steps );
    //generate the points on the sphere
    generate_sphere( temp, cx, cy, cz, r, step );

    int latStop, longtStop, latStart, longStart;
    latStart = 0;
    latStop = num_steps;
    longStart = 0;
    longtStop = num_steps;

    for ( lat = latStart; lat < latStop; lat++ ) {
        for ( longt = longStart; longt < longtStop; longt++ ) {      

            index = lat * num_steps + longt;  

            if ( lat != num_steps - 1 && longt != num_steps - 1 ) {

                add_polygon( points, 
                        temp->m[0][ index ], //first vertex
                        temp->m[1][ index ],
                        temp->m[2][ index ],
                        temp->m[0][ index + 1 ], //third vertex
                        temp->m[1][ index + 1 ],
                        temp->m[2][ index + 1 ],
                        temp->m[0][ index + num_steps ], //second vertex
                        temp->m[1][ index + num_steps ],
                        temp->m[2][ index + num_steps ] );
                add_polygon( points, 
                        temp->m[0][ index + 1 ], //first vertex
                        temp->m[1][ index + 1 ],
                        temp->m[2][ index + 1 ],
                        temp->m[0][ index + 1 + num_steps ], //third vertex
                        temp->m[1][ index + 1 + num_steps ],
                        temp->m[2][ index + 1 + num_steps ],
                        temp->m[0][ index + num_steps ], //second vertex
                        temp->m[1][ index + num_steps ],
                        temp->m[2][ index + num_steps ] );	
            } //end of non edge cases

            else if ( lat == num_steps - 1 ) {
                if  ( longt != num_steps -1 ) {
                    add_polygon( points, 
                            temp->m[0][ index ], //first vertex
                            temp->m[1][ index ],
                            temp->m[2][ index ],
                            temp->m[0][ index + 1 ], //second vertex
                            temp->m[1][ index + 1 ],
                            temp->m[2][ index + 1 ],
                            temp->m[0][ index % num_steps ], //third vertex
                            temp->m[1][ index % num_steps ],
                            temp->m[2][ index % num_steps ] );
                    add_polygon( points, 
                            temp->m[0][ index + 1 ], //first vertex
                            temp->m[1][ index + 1 ],
                            temp->m[2][ index + 1 ],
                            temp->m[0][ (index + 1) % num_steps ], //second vertex
                            temp->m[1][ (index + 1) % num_steps ],
                            temp->m[2][ (index + 1) % num_steps ],
                            temp->m[0][ index % num_steps ], //third vertex
                            temp->m[1][ index % num_steps ],
                            temp->m[2][ index % num_steps ] );
                }
                else {
                    add_polygon( points, 
                            temp->m[0][ index ], //first vertex
                            temp->m[1][ index ],
                            temp->m[2][ index ],
                            (temp->m[0][ 0 ]) - ( 2 * r ), //second vertex
                            temp->m[1][ 0 ],
                            temp->m[2][ 0 ],
                            temp->m[0][ index % num_steps ], //third vertex
                            temp->m[1][ index % num_steps ],
                            temp->m[2][ index % num_steps ] );	
                }
            } //end latitude edge
            else {
                add_polygon( points, 
                        temp->m[0][ index ], //first vertex
                        temp->m[1][ index ],
                        temp->m[2][ index ],
                        (temp->m[0][ index + 1 ]) - ( 2 * r ), //second vertex
                        temp->m[1][ index + 1 ],
                        temp->m[2][ index + 1 ],
                        temp->m[0][ index + num_steps ], //third vertex
                        temp->m[1][ index + num_steps ],
                        temp->m[2][ index + num_steps ] );	
            } //end longitude edge (south pole)

        } //end for long
    }// end for lat
}

void add_torus( struct matrix * points, 
        double cx, double cy, double cz, double r1, double r2, 
        double step ) {

    struct matrix * temp;
    int lat, longt;
    int index;
    double ns;
    int num_steps;

    ns = 1.0 / step;
    num_steps = (int)ns;

    temp = new_matrix( 4, num_steps * num_steps );
    //generate the points on the torus
    generate_torus( temp, cx, cy, cz, r1, r2, step );

    int latStop, longtStop, latStart, longStart;
    latStart = 0;
    longStart = 0;
    latStop = num_steps;
    longtStop = num_steps;

    //latitudinal lines  
    for ( lat = latStart; lat < latStop; lat++ ) {
        for ( longt = longStart; longt < longtStop; longt++ ) {

            index = lat * num_steps + longt;


            if ( lat != num_steps - 1 && longt != num_steps - 1 ) {

                add_polygon( points, 
                        temp->m[0][ index ], //first vertex
                        temp->m[1][ index ],
                        temp->m[2][ index ],
                        temp->m[0][ index + 1 ], //third vertex
                        temp->m[1][ index + 1 ],
                        temp->m[2][ index + 1 ],
                        temp->m[0][ index + num_steps ], //second vertex
                        temp->m[1][ index + num_steps ],
                        temp->m[2][ index + num_steps ] );
                add_polygon( points, 
                        temp->m[0][ index + 1 ], //first vertex
                        temp->m[1][ index + 1 ],
                        temp->m[2][ index + 1 ],
                        temp->m[0][ index + 1 + num_steps ], //third vertex
                        temp->m[1][ index + 1 + num_steps ],
                        temp->m[2][ index + 1 + num_steps ],
                        temp->m[0][ index + num_steps ], //second vertex
                        temp->m[1][ index + num_steps ],
                        temp->m[2][ index + num_steps ] );	

            } //end of non edge cases

            else if ( lat == num_steps - 1 ) {
                if  ( longt != num_steps -1 ) {
                    add_polygon( points, 
                            temp->m[0][ index ], //first vertex
                            temp->m[1][ index ],
                            temp->m[2][ index ],
                            temp->m[0][ index + 1 ], //second vertex
                            temp->m[1][ index + 1 ],
                            temp->m[2][ index + 1 ],
                            temp->m[0][ index % num_steps ], //third vertex
                            temp->m[1][ index % num_steps ],
                            temp->m[2][ index % num_steps ] );
                    add_polygon( points, 
                            temp->m[0][ index + 1 ], //first vertex
                            temp->m[1][ index + 1 ],
                            temp->m[2][ index + 1 ],
                            temp->m[0][ (index + 1) % num_steps ], //second vertex
                            temp->m[1][ (index + 1) % num_steps ],
                            temp->m[2][ (index + 1) % num_steps ],
                            temp->m[0][ index % num_steps ], //third vertex
                            temp->m[1][ index % num_steps ],
                            temp->m[2][ index % num_steps ] );
                }
                else { 
                    add_polygon( points, 
                            temp->m[0][ index ], //first vertex
                            temp->m[1][ index ],
                            temp->m[2][ index ],
                            temp->m[0][ index + 1 - num_steps ], //second vertex
                            temp->m[1][ index + 1 - num_steps ],
                            temp->m[2][ index + 1 - num_steps ],
                            temp->m[0][ index % num_steps ], //third vertex
                            temp->m[1][ index % num_steps ],
                            temp->m[2][ index % num_steps ] );	
                    add_polygon( points, 
                            temp->m[0][ index + 1 - num_steps ], //first vertex
                            temp->m[1][ index + 1 - num_steps ],
                            temp->m[2][ index + 1 - num_steps],
                            temp->m[0][ 0 ], //second vertex
                            temp->m[1][ 0 ],
                            temp->m[2][ 0 ],
                            temp->m[0][ index % num_steps ], //third vertex
                            temp->m[1][ index % num_steps ],
                            temp->m[2][ index % num_steps ] );	
                }
            } //end latitude edge
            else {
                add_polygon( points, 
                        temp->m[0][ index ], //first vertex
                        temp->m[1][ index ],
                        temp->m[2][ index ],
                        temp->m[0][ index + 1 - num_steps ], //second vertex
                        temp->m[1][ index + 1 - num_steps ],
                        temp->m[2][ index + 1 - num_steps ],
                        temp->m[0][ index + num_steps ], //third vertex
                        temp->m[1][ index + num_steps ],
                        temp->m[2][ index + num_steps ] );	
                add_polygon( points, 
                        temp->m[0][ index + 1 - num_steps ], //first vertex
                        temp->m[1][ index + 1 - num_steps ],
                        temp->m[2][ index + 1 - num_steps ],
                        temp->m[0][ index + 1 ], //second vertex
                        temp->m[1][ index + 1 ],
                        temp->m[2][ index + 1 ],
                        temp->m[0][ index ], //third vertex
                        temp->m[1][ index ],
                        temp->m[2][ index ] );	
            } //end longitude edge (south pole)


        } //end for longt
    } //end for lat
}

/*======== void draw_polygons() ==========
Inputs:   struct matrix *points
screen s
color c  
Returns: 

Go through the point matrix as if it were a polygon matrix
Call drawline in batches of 3s to create triangles.

04/04/12 13:39:09
jdyrlandweaver
====================*/
void draw_polygons( struct matrix *points, screen s, color c ,light_source l, double *ambient) {

    int i, n, b;
    double x1, y1, z1, x2, y2, z2, x3, y3, z3;
    double x_t, x_m, x_b, y_t, y_m, y_b, z_t, z_m, z_b;
    double k;
    n = 0;
    k = 1;
    printf("light data:%f\t%f\t%f\t%f\t%f\t%f\n", l.r, l.g, l.b,
            l.x, l.y, l.z);

    if ( points->lastcol < 3 ) {
        printf("Need at least 3 points to draw a polygon\n");
        return;
    }

    for(i=0; i < points->lastcol - 2; i+=3) {

        if ( calculate_dot( points, i ) >= 0 ) {
            if (ambient[0] >=0) {
                c.red = k * ambient[0] * 255;
                c.green = k * ambient[1] * 255;
                c.blue = k * ambient[2] * 255;
           }
           else {
                c = change_color( n++ );
           }
           printf("calculate_dot says %f\n", calculate_dot(points, i));

            x1 = points->m[0][i];
            y1 = points->m[1][i];
            z1 = points->m[2][i];
            x2 = points->m[0][i+1];
            y2 = points->m[1][i+1];
            z2 = points->m[2][i+1];
            x3 = points->m[0][i+2];
            y3 = points->m[1][i+2];
            z3 = points->m[2][i+2];
            y_b = y1 > y2 ? (y1 > y3 ? y1 : y3) : (y2 > y3 ? y2 : y3);
            y_t = y1 > y2 ? (y2 > y3 ? y3 : y2) : (y1 > y3 ? y3 : y1);
            y_m = y1 > y2 ? (y2 > y3 ? y2 : (y1 > y3 ? y3:y1)):(y1 > y3 ? y1 : (y2 > y3 ? y3 : y2));
            printf("YT %f\tYB%f\tYM%f\n", y_t, y_b, y_m);

            x_b = y1 > y2 ? (y1 > y3 ? x1 : x3) : (y2 > y3 ? x2 : x3);
            x_t = y1 > y2 ? (y2 > y3 ? x3 : x2) : (y1 > y3 ? x3 : x1);
            x_m = y1 > y2 ? (y2 > y3 ? x2 : (y1 > y3 ? x3:x1)):(y1 > y3 ? x1 : (y2 > y3 ? x3 : x2));

            z_b = y1 > y2 ? (y1 > y3 ? z1 : z3) : (y2 > y3 ? z2 : z3);
            z_t = y1 > y2 ? (y2 > y3 ? z3 : z2) : (y1 > y3 ? z3 : z1);
            z_m = y1 > y2 ? (y2 > y3 ? z2 : (y1 > y3 ? z3:z1)):(y1 > y3 ? z1 : (y2 > y3 ? z3 : z2));

            double m_topToMid, m_topToBottom, m_midToBottom, dzdx_TtM, dzdx_TtB, dzdx_MtB;
            double xleft, xright, zleft, zright;
            int  yy;
            xleft = xright = x_t;
            zleft = zright = z_t;

            m_topToMid = (x_t - x_m) / (y_t - y_m);
            m_topToBottom = (x_t - x_b) / (y_t - y_b);
            m_midToBottom = (x_m - x_b) / (y_m - y_b);
            dzdx_TtM = (x_t - x_m) / (z_t - z_m);
            dzdx_TtB = (x_t - x_b) / (z_t - z_b);
            dzdx_MtB = (x_m - x_b) / (z_m - z_b);

            yy = y_t;

            while (yy < y_m) {
                draw_line((int)xleft, (int)yy, zleft, (int)xright, (int)yy, zright, s, c);
                xleft += m_topToMid;
                xright += m_topToBottom;
                zleft += dzdx_TtM;
                zright += dzdx_TtB;
                yy++;
            }
            xleft = x_m;
            while (yy < y_b) {
                draw_line((int)xleft, (int)yy, zleft, (int)xright, (int)yy, zright, s, c);
                xleft += m_midToBottom;
                xright += m_topToBottom;
                zleft += dzdx_MtB;
                zright += dzdx_TtB;
                yy++;
            }
        }
    }
}

/*======== void add_polygon() ==========
Inputs:   struct matrix *points
double x0
double y0
double z0
double x1
double y1
double z1
double x2
double y2
double z2  
Returns: 

Adds points (x0, y0, z0), (x1, y1, z1) and (x2, y2, z2) to the
polygon matrix

04/04/12 12:43:57
jdyrlandweaver
====================*/
void add_polygon( struct matrix *points, 
        double x0, double y0, double z0, 
        double x1, double y1, double z1, 
        double x2, double y2, double z2 ) {

    add_point( points, x0, y0, z0 );
    add_point( points, x1, y1, z1 );
    add_point( points, x2, y2, z2 );
}

/*======== void add_sphere_mesh() ==========
Inputs:   struct matrix * points
double cx
double cy
double r
double step  
Returns: 

adds all the edges required to make a wire frame mesh
for a sphere with center (cx, cy) and radius r.

should call generate_sphere_mesh to create the
necessary points

03/22/12 13:34:03
jdyrlandweaver
====================*/
void add_sphere_mesh( struct matrix * points, 
        double cx, double cy, double cz, double r, 
        double step ) {

    struct matrix * temp;
    int lat, longt;
    int index;
    double ns, x, y, z;
    int num_steps;

    ns = 1.0 / step;
    num_steps = (int)ns;

    temp = new_matrix( 4, num_steps * num_steps );
    //generate the points on the sphere
    generate_sphere( temp, cx, cy, cz, r, step );

    int latStop, longtStop, latStart, longStart;
    latStart = 0;
    latStop = num_steps;
    longStart = 0;
    longtStop = num_steps;

    //latitudinal lines  
    for ( longt = longStart; longt < longtStop; longt++ ) {
        for ( lat = latStart; lat < latStop; lat++ ) {

            index = lat * num_steps + longt;

            if ( lat == num_steps - 1) {
                add_edge( points, temp->m[0][index],
                        temp->m[1][index],
                        temp->m[2][index],
                        temp->m[0][index % num_steps ],
                        temp->m[1][index % num_steps ],
                        temp->m[2][index % num_steps ] );
            }

            else {
                add_edge( points, temp->m[0][index],
                        temp->m[1][index],
                        temp->m[2][index],
                        temp->m[0][index + num_steps],
                        temp->m[1][index + num_steps],
                        temp->m[2][index + num_steps] );
            }
        }
    } //end latitudinal lines


    //longitudinal lines  
    for ( lat = latStart; lat < latStop; lat++ ) {
        for ( longt = longStart; longt < longtStop; longt++ ) {

            index = lat * num_steps + longt;

            //add the connecting circle line for the last index
            if ( longt == num_steps - 1 ) {
                if ( lat == num_steps - 1 ) {
                    x = temp->m[0][ 0 ] - ( 2 * r );
                    y = temp->m[1][ 0 ];
                    z = temp->m[2][ 0 ];
                }
                else {
                    x = temp->m[0][ index + 1 ] - ( 2 * r );
                    y = temp->m[1][ index + 1 ];
                    z = temp->m[2][ index + 1 ];
                }
                add_edge( points, temp->m[0][index],
                        temp->m[1][index],
                        temp->m[2][index],
                        x, y, z);
            }
            else
                add_edge( points, temp->m[0][index],
                        temp->m[1][index],
                        temp->m[2][index],
                        temp->m[0][index + 1],
                        temp->m[1][index + 1],
                        temp->m[2][index + 1] );
        }
    } //end longitudinal lines


    /*
    //points only
    for ( lat = 0; lat < num_steps; lat++ )
    for ( longt = 0; longt < num_steps; longt++ ) {

    index = lat * num_steps + longt;

    add_edge( points, temp->m[0][index],
    temp->m[1][index],
    temp->m[2][index],
    temp->m[0][index],
    temp->m[1][index],
    temp->m[2][index] );
    }//end points only
    */
}

/*======== void generate_sphere_mesh() ==========
Inputs:   struct matrix * points
double cx
double cy
double r
double step  
Returns: 

Generates all the points along the surface of a 
sphere with center (cx, cy) and radius r

Adds these points to the matrix parameter

03/22/12 11:30:26
jdyrlandweaver
====================*/
void generate_sphere( struct matrix * points, 
        double cx, double cy, double cz, double r, 
        double step ) {

    double x, y, z, circle, rotation;

    double rotStart = step * 0;
    double rotStop = 1;


    for ( rotation = rotStart; rotation <= rotStop; rotation += step ) {
        for ( circle = 0; circle <= 1; circle+= step ) {

            x = r * cos( M_PI * circle ) + cx;
            y = r * sin( M_PI * circle ) *
                cos( 2 * M_PI * rotation ) + cy;
            z = r * sin( M_PI * circle ) *
                sin( 2 * M_PI * rotation ) + cz;

            add_point( points, x, y, z );
        }
    }
}    

/*======== void add_torus_mesh() ==========
Inputs:   struct matrix * points
double cx
double cy
double r1
double r2
double step  
Returns: 

adds all the edges required to make a wire frame mesh
for a torus with center (cx, cy) and radii r1 and r2.

should call generate_torus_mesh to create the
necessary points

03/22/12 13:34:03
jdyrlandweaver
====================*/
void add_torus_mesh( struct matrix * points, 
        double cx, double cy, double cz, 
        double r1, double r2, 
        double step ) {

    struct matrix * temp;
    int lat, longt;
    int index;
    double ns;
    int num_steps;

    ns = 1.0 / step;
    num_steps = (int)ns;

    temp = new_matrix( 4, num_steps * num_steps );
    //generate the points on the torus
    generate_torus( temp, cx, cy, cz, r1, r2, step );

    int latStop, longtStop, latStart, longStart;
    latStart = 0;
    longStart = 0;
    latStop = num_steps;
    longtStop = num_steps;

    //latitudinal lines  
    for ( longt = longStart; longt < longtStop; longt++ ) {
        for ( lat = latStart; lat < latStop; lat++ ) {

            index = lat * num_steps + longt;

            if ( lat == num_steps - 1) {
                add_edge( points, temp->m[0][index],
                        temp->m[1][index],
                        temp->m[2][index],
                        temp->m[0][index % num_steps ],
                        temp->m[1][index % num_steps ],
                        temp->m[2][index % num_steps ] );
            }

            else {
                add_edge( points, temp->m[0][index],
                        temp->m[1][index],
                        temp->m[2][index],
                        temp->m[0][index + num_steps],
                        temp->m[1][index + num_steps],
                        temp->m[2][index + num_steps] );
            }
        }
    } //end latitudinal lines

    //longitudinal lines  
    for ( lat = latStart; lat < latStop; lat++ ) {
        for ( longt = longStart; longt < longtStop; longt++ ) {

            index = lat * num_steps + longt;

            //add the connecting circle line for the last index
            if ( longt == num_steps - 1 ) {
                add_edge( points, temp->m[0][index],
                        temp->m[1][index],
                        temp->m[2][index],
                        temp->m[0][index + 1 - num_steps],
                        temp->m[1][index + 1 - num_steps],
                        temp->m[2][index + 1 - num_steps] );
            }
            else
                add_edge( points, temp->m[0][index],
                        temp->m[1][index],
                        temp->m[2][index],
                        temp->m[0][index + 1],
                        temp->m[1][index + 1],
                        temp->m[2][index + 1] );
        }
    } //end longitudinal lines
}

/*======== void generate_torus() ==========
Inputs:   struct matrix * points
double cx
double cy
double r
double step  
Returns: 

Generates all the points along the surface of a 
tarus with center (cx, cy) and radii r1 and r2

Adds these points to the matrix parameter

03/22/12 11:30:26
jdyrlandweaver
====================*/
void generate_torus( struct matrix * points, 
        double cx, double cy, double cz,
        double r1, double r2, 
        double step ) {
    double x, y, z, circle, rotation;

    double rotStart = step * 0;
    double rotStop = 1;


    for ( rotation = rotStart; rotation <= rotStop; rotation += step ) {
        for ( circle = 0; circle <= 1; circle+= step ) {

            x = cos( 2 * M_PI * rotation ) *
                ( r1 * cos( 2 * M_PI * circle ) + r2 ) + cx;
            y = r1 * sin( 2 * M_PI * circle ) + cy;
            z = sin( 2 * M_PI * rotation ) *
                ( r1 * cos( 2 * M_PI * circle ) + r2 ) + cz;

            add_point( points, x, y, z );
        }
    }

}

/*======== void add_box_mesh() ==========
Inputs:   struct matrix * points
double x
double y
double z
double width
double height
double depth
Returns: 

adds all the edges required to make a wire frame mesh
for a rectagular prism whose upper-left corner is
(x, y, z) with width, height and depth dimensions.

03/22/12 13:34:03
jdyrlandweaver
====================*/
void add_box_mesh( struct matrix * points,
        double x, double y, double z,
        double width, double height, double depth ) {

    double x2, y2, z2;
    x2 = x + width;
    y2 = y - height;
    z2 = z - depth;

    add_edge( points, 
            x, y, z, x2, y, z );
    add_edge( points, 
            x2, y, z, x2, y2, z );
    add_edge( points, 
            x2, y2, z, x, y2, z );
    add_edge( points, 
            x, y2, z, x, y, z );

    add_edge( points, 
            x, y, z2, x2, y, z2 );
    add_edge( points, 
            x2, y, z2, x2, y2, z2 );
    add_edge( points, 
            x2, y2, z2, x, y2, z2 );
    add_edge( points, 
            x, y2, z2, x, y, z2 );

    add_edge( points, 
            x, y, z, x, y, z2 );
    add_edge( points, 
            x2, y, z, x2, y, z2 );
    add_edge( points, 
            x2, y2, z, x2, y2, z2 );
    add_edge( points, 
            x, y2, z, x, y2, z2 );
}

/*======== void add_circle() ==========
Inputs:   struct matrix * points
double cx
double cy
double y
double step  
Returns: 


03/16/12 19:53:52
jdyrlandweaver
====================*/
void add_circle( struct matrix * points, 
        double cx, double cy, 
        double r, double step ) {

    double x0, y0, x, y, t;

    x0 = cx + r;
    y0 = cy;

    for ( t = step; t <= 1; t+= step ) {

        x = r * cos( 2 * M_PI * t ) + cx;
        y = r * sin( 2 * M_PI * t ) + cy;

        add_edge( points, x0, y0, 0, x, y, 0 );
        x0 = x;
        y0 = y;
    }

    add_edge( points, x0, y0, 0, cx + r, cy, 0 );
}

/*======== void add_curve() ==========
Inputs:   struct matrix *points
double x0
double y0
double x1
double y1
double x2
double y2
double x3
double y3
double step
int type  
Returns: 

Adds the curve bounded by the 4 points passsed as parameters
of type specified in type (see matrix.h for curve type constants)
to the matrix points

03/16/12 15:24:25
jdyrlandweaver
====================*/
void add_curve( struct matrix *points, 
        double x0, double y0, 
        double x1, double y1, 
        double x2, double y2, 
        double x3, double y3, 
        double step, int type ) {

    double x, y, t;
    struct matrix * xcoefs;
    struct matrix * ycoefs;

    //generate the coeficients
    if ( type == BEZIER_MODE ) {
        ycoefs = generate_curve_coefs(y0, y1, y2, y3, BEZIER_MODE);
        xcoefs = generate_curve_coefs(x0, x1, x2, x3, BEZIER_MODE);
    }

    else {
        xcoefs = generate_curve_coefs(x0, x1, x2, x3, HERMITE_MODE);
        ycoefs = generate_curve_coefs(y0, y1, y2, y3, HERMITE_MODE);
    }

    /*
       printf("a = %lf b = %lf c = %lf d = %lf\n", xcoefs->m[0][0],
       xcoefs->m[1][0], xcoefs->m[2][0], xcoefs->m[3][0]);
       */

    for (t=step; t <= 1; t+= step) {

        x = xcoefs->m[0][0] * t * t * t + xcoefs->m[1][0] * t * t
            + xcoefs->m[2][0] * t + xcoefs->m[3][0];

        y = ycoefs->m[0][0] * t * t * t + ycoefs->m[1][0] * t * t
            + ycoefs->m[2][0] * t + ycoefs->m[3][0];

        add_edge(points, x0, y0, 0, x, y, 0);
        x0 = x;
        y0 = y;
    }

    free_matrix(xcoefs);
    free_matrix(ycoefs);
}

/*======== void add_point() ==========
Inputs:   struct matrix * points
int x
int y
int z 
Returns: 
adds point (x, y, z) to points and increment points.lastcol
if points is full, should call grow on points
====================*/
void add_point( struct matrix * points, double x, double y, double z) {

    if ( points->lastcol == points->cols )
        grow_matrix( points, points->lastcol + 100 );

    points->m[0][points->lastcol] = x;
    points->m[1][points->lastcol] = y;
    points->m[2][points->lastcol] = z;
    points->m[3][points->lastcol] = 1;

    points->lastcol++;
}

/*======== void add_edge() ==========
Inputs:   struct matrix * points
int x0, int y0, int z0, int x1, int y1, int z1
Returns: 
add the line connecting (x0, y0, z0) to (x1, y1, z1) to points
should use add_point
====================*/
void add_edge( struct matrix * points, 
        double x0, double y0, double z0, 
        double x1, double y1, double z1) {
    add_point( points, x0, y0, z0 );
    add_point( points, x1, y1, z1 );
}

/*======== void draw_lines() ==========
Inputs:   struct matrix * points
screen s
color c 
Returns: 
Go through points 2 at a time and call draw_line to add that line
to the screen
====================*/
void draw_lines( struct matrix * points, screen s, color c) {

    int i;

    if ( points->lastcol < 2 ) {

        printf("Need at least 2 points to draw a line!\n");
        return;
    }

    for ( i = 0; i < points->lastcol - 1; i+=2 ) {

        draw_line( points->m[0][i], points->m[1][i], 0,
                points->m[0][i+1], points->m[1][i+1], 0, s, c);
    } 	       
}


void draw_line(int x0, int y0, double z0, int x1, int y1, double z1, screen s, color c) {

    int x, y, z, d, dx, dy, dzdx;

    x = x0;
    y = y0;

    //swap points so we're always draing left to right
    if ( x0 > x1 ) {
        x = x1;
        y = y1;
        x1 = x0;
        y1 = y0;
    }

    //need to know dx and dy for this version
    dx = (x1 - x) * 2;
    dy = (y1 - y) * 2;

    //positive slope: Octants 1, 2 (5 and 6)
    if ( dy > 0 ) {

        //slope < 1: Octant 1 (5)
        if ( dx > dy ) {
            d = dy - ( dx / 2 );

            while ( x <= x1 ) {
                plot(s, c, x, y);

                if ( d < 0 ) {
                    x = x + 1;
                    d = d + dy;
                }
                else {
                    x = x + 1;
                    y = y + 1;
                    d = d + dy - dx;
                }
            }
        }

        //slope > 1: Octant 2 (6)
        else {
            d = ( dy / 2 ) - dx;
            while ( y <= y1 ) {

                plot(s, c, x, y );
                if ( d > 0 ) {
                    y = y + 1;
                    d = d - dx;
                }
                else {
                    y = y + 1;
                    x = x + 1;
                    d = d + dy - dx;
                }
            }
        }
    }

    //negative slope: Octants 7, 8 (3 and 4)
    else { 

        //slope > -1: Octant 8 (4)
        //NOTE: z-interpolation happens here
        if ( dx > abs(dy) ) {

            z = z0;
            d = dy + ( dx / 2 );
            dzdx = (z1 - z0) / (x1 - x0);
            
            while ( x <= x1 ) {

                if(z > get_current_z(s, x, y)){
                    c.z = z;
                    plot(s, c, x, y);
                }

                if ( d > 0 ) {
                    x = x + 1;
                    d = d + dy;
                }
                else {
                    x = x + 1;
                    y = y - 1;
                    d = d + dy + dx;
                }
            }
        }

        //slope < -1: Octant 7 (3)
        else {

            d =  (dy / 2) + dx;

            while ( y >= y1 ) {

                plot(s, c, x, y );
                if ( d < 0 ) {
                    y = y - 1;
                    d = d + dx;
                }
                else {
                    y = y - 1;
                    x = x + 1;
                    d = d + dy + dx;
                }
            }
        }
    }
}

void import_mesh(struct matrix * points, char* filename){
    FILE * f;
    
    if(!strcmp(&(filename[strlen(filename)-4]), ".obj") || !strcmp(&(filename[strlen(filename)-4]), ".OBJ")){
        printf("its an obj\n");
        f = fopen(filename, "r");
        if(f == NULL)
            printf("Error opening %s: %s\n",filename, strerror(errno));
        else{
            import_mesh_obj(points, f);
            fclose(f);
        }
    }
    else
        printf("Error opening %s: unrecognized filetype\n",filename);
}
