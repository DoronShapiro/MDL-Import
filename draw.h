#ifndef DRAW_H
#define DRAW_H

#include "matrix.h"
void draw_line(int x0, int y0, double z0,
	       int x1, int y1, double z1,
	       screen s, color c);
void add_point( struct matrix * points, 
		 double x, double y, double z);
void add_edge( struct matrix * points, 
	       double x0, double y0, double z0, 
	       double x1, double y1, double z1);
void draw_lines( struct matrix * points, screen s, color c);
//polygon draw routines
void add_polygon( struct matrix *points, 
		  double x0, double y0, double z0, 
		  double x1, double y1, double z1, 
		  double x2, double y2, double z2 );
void draw_polygons( struct matrix *points, screen s, color c , light_source l, double *ambient, double *camera);

//advanced shapes
void add_circle( struct matrix * points, 
		 double cx, double cy, 
		 double r, double step );
void add_curve( struct matrix *points, 
		double x0, double y0,
		double x1, double y1,
		double x2, double y2,
		double x3, double y3,
		double step, int type );
void add_sphere_mesh( struct matrix * points, 
		      double cx, double cy, double cz, double r, 
		      double step );
void generate_sphere( struct matrix * points, 
		      double cx, double cy, double cz, double r, 
		      double step );
void add_torus_mesh( struct matrix * points, 
		     double cx, double cy, double cz, 
		     double r1, double r2, 
		     double step );
void generate_torus( struct matrix * points, 
		     double cx, double cy, double cz,
		     double r1, double r2, 
		     double step );
void add_box_mesh( struct matrix * points,
		   double x, double y, double z,
		   double width, double height, double depth );

//polygons
void add_box( struct matrix * points,
	      double x, double y, double z,
	      double width, double height, double depth );
void add_sphere( struct matrix * points, 
		 double cx, double cy, double cz, double r, 
		 double step );
void add_torus( struct matrix * points, 
		double cx, double cy, double cz, double r1, double r2, 
		double step );

//mesh import
void import_mesh(struct matrix * points,
             char* filename);

#endif
