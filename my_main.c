/*========== my_main.c ==========

  This is the only file you need to modify in order
  to get a working mdl project (for now).

  my_main.c will serve as the interpreter for mdl.
  When an mdl script goes through a lexer and parser, 
  the resulting operations will be in the array op[].

  Your job is to go through each entry in op and perform
  the required action from the list below:

  frames: set num_frames (in misc_headers.h) for animation

  basename: set name (in misc_headers.h) for animation

  vary: manipluate knob values between two given frames
        over a specified interval

  set: set a knob to a given value
  
  setknobs: set all knobs to a given value

  push: push a new origin matrix onto the origin stack

  pop: remove the top matrix on the origin stack

  move/scale/rotate: create a transformation matrix 
                     based on the provided values, then 
                     multiply the current top of the
                     origins stack by it.

  box/sphere/torus: create a solid object based on the
                    provided values. Store that in a 
                    temporary matrix, multiply it by the
                    current top of the origins stack, then
                    call draw_polygons.

  line: create a line based on the provided values. Store 
        that in a temporary matrix, multiply it by the
        current top of the origins stack, then call draw_lines.

  save: call save_extension with the provided filename

  display: view the image live
  
  jdyrlandweaver
  =========================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "parser.h"
#include "symtab.h"
#include "y.tab.h"

#include "misc_headers.h"
#include "matrix.h"
#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "stack.h"

/*======== void first_pass()) ==========
  Inputs:   
  Returns: 

  Checks the op array for any animation commands
  (frames, basename, vary)
  
  Should set num_frames and basename if the frames 
  or basename commands are present

  If vary is found, but frames is not, the entire
  program should exit.

  If frames is found, but basename is not, set name
  to some default value, and print out a message
  with the name being used.
  
  05/17/12 09:27:02
  jdyrlandweaver
  ====================*/
void first_pass() {
    int i, will_vary;
    will_vary = 0;
    *name = '\0';
    num_frames = 0;
    for (i = 0; i < lastop; i++) {
        switch (op[i].opcode) {
            case BASENAME:
                strncpy(name, op[i].op.basename.p->name, 127);
                break;
            case FRAMES:
                num_frames = op[i].op.frames.num_frames;
                break;
            case VARY:
                will_vary = 1;
                break;
                if ((num_frames == 0) && will_vary) {
                    printf("Error: vary but no frames, exiting.\n");
                    exit(-1);
                }
                if (num_frames != 0 && (&name == '\0')) {
                    printf("Warning: no basename provided, using \"anim\".\n");
                    strcpy(name, "anim");
                }
        }
    }
    if (num_frames == 0)
        num_frames = 1;
    //printf("first_pass results: num_frames=%d, basename is \"%s\".\n", num_frames, name);
}

void print_vary_knobs(struct vary_node **knobs) {
    int i;
    struct vary_node *current;
    for (i = 0; i < num_frames; i++) {
        printf("Frame #%d:\n", i);
        current = knobs[i];
        while (current != NULL) {
            printf("\t%s is %lf\n", current->name, current->value);
            current = current->next;
        }
    }
    printf("all done...\n");
}

/*======== struct vary_node ** second_pass()) ==========
  Inputs:   
  Returns: An array of vary_node linked lists

  In order to set the knobs for animation, we need to keep
  a separate value for each knob for each frame. We can do
  this by using an array of linked lists. Each array index
  will correspond to a frame (eg. knobs[0] would be the first
  frame, knobs[2] would be the 3rd frame and so on).

  Each index should contain a linked list of vary_nodes, each
  node contains a knob name, a value, and a pointer to the
  next node.

  Go through the opcode array, and when you find vary, go 
  from knobs[0] to knobs[frames-1] and add (or modify) the
  vary_node corresponding to the given knob with the
  appropirate value. 

  05/17/12 09:29:31
  jdyrlandweaver
  ====================*/
struct vary_node ** second_pass() {
    int i, j;
    struct vary_node **knobs, *current;
    knobs = (struct vary_node**) malloc(num_frames * sizeof (struct vary_node*));

    //Initialize all knob value lists to NULL for checking against later
    for (i = 0; i < num_frames; i++)
        knobs[i] = NULL;

    //printf("starting second pass...\n");
    for (i = 0; i < lastop; i++) { //for each op in the mdl file
        if (op[i].opcode == VARY) {
            //printf("\tbeginning vary %s\n", op[i].op.vary.p->name);
            for (j = 0; j < num_frames; j++) { //for each frame
                //printf("\t\tapplying %s to frame %d\n", op[i].op.vary.p->name, j);
                if (knobs[j] == NULL) { //Add the first element in the linked list, if necessary
                    knobs[j] = (struct vary_node*) malloc(sizeof (struct vary_node));
                    strncpy(knobs[j]->name, op[i].op.vary.p->name, 127);
                    knobs[j]->next = NULL;
                    //printf("\t\t\tadding the first element (%s) for frame%d!\n", op[i].op.vary.p->name, j);
                }
                current = knobs[j];
                while (strcmp(current->name, op[i].op.vary.p->name)) { //otherwise parse the LL looking for our knob
                    //printf("\t\t\tlooking for %s and the first element is %s (next is %p)\n", op[i].op.vary.p->name, current->name, (void*) (current->next));
                    if (current->next != NULL) { //go forward in the LL if possible
                        current = current->next;
                        //printf("\t\t\t\t\tmoving down linked list looking for %s\n", current->name);
                    } else { //Otherwise add a new node and set its value if it is outside the range of the knob
                        current->next = (struct vary_node*) malloc(sizeof (struct vary_node));
                        current = current->next;
                        strncpy(current->name, op[i].op.vary.p->name, 127);
                        current->next = NULL;
                        if (j < op[i].op.vary.start_frame)
                            current->value = op[i].op.vary.start_val;
                        else if (j > op[i].op.vary.end_frame)
                            current->value = op[i].op.vary.end_val;
                        //printf("\t\t\t\t\tadding %s to linked list\n", current->name);
                    }
                }
                if (j >= op[i].op.vary.start_frame && j <= op[i].op.vary.end_frame)//set the value of all knobs within range
                    current->value = op[i].op.vary.start_val + (j - op[i].op.vary.start_frame) * (op[i].op.vary.end_val - op[i].op.vary.start_val) / abs(op[i].op.vary.end_frame - op[i].op.vary.start_frame);
            }
        }
    }
    //printf("finished second pass.\n\n\n");
    //print_vary_knobs(knobs);
    return knobs;
}

void print_knobs() {

    int i;

    printf("ID\tNAME\t\tTYPE\t\tVALUE\n");
    for (i = 0; i < lastsym; i++) {

        if (symtab[i].type == SYM_VALUE) {
            printf("%d\t%s\t\t", i, symtab[i].name);

            printf("SYM_VALUE\t");
            printf("%6.2f\n", symtab[i].s.value);
        }
    }
}

void process_knobs() {

    int i;
    double v;

    if (lastsym == 0)
        return;

    do {
        printf("Knob List:\n");
        print_knobs();
        printf("Enter knob ID to set (-1 to stop): ");
        scanf("%d", &i);

        if (i >= lastsym || i < -1)
            printf("Invalid entry, please try again.\n");

        else if (i != -1) {

            printf("Enter new value for %s: ", symtab[i].name);
            scanf("%lf", &v);
            symtab[i].s.value = v;
        }
        printf("\n");

    } while (i != -1);
}

void modify_knobs(struct vary_node *vary) {
    int i;
    struct vary_node *current;

    for (i = 0; i < lastsym; i++) {
        if (symtab[i].type == SYM_VALUE) {

            current = vary;
            while ((current != NULL) && strcmp(symtab[i].name, current->name))
                current = current->next;

            symtab[i].s.value = current->value;
        }
    }
}

void free_knob_array(struct vary_node **knobs) {
    int i, j;
    struct vary_node *current, *next;
    for (i = 0; i < num_frames; i++) {
        //printf("freeing i=%d\n", i);
        current = knobs[i];
        j = 0;
        while (current != NULL) {
            // printf("j is %d\n", j);
            next = current->next;
            free(current);
            current = next;
            j++;
        }
    }
    free(knobs);
}

/*======== void my_main() ==========
  Inputs:   int polygons  
  Returns: 

  This is the main engine of the interpreter, it should
  handle most of the commands in mdl.

  If frames is not present in the source (and therefore 
  num_frames is 1, then process_knobs should be called.

  If frames is present, the enitre op array must be
  applied frames time. At the end of each frame iteration
  save the current screen to a file named the
  provided basename plus a numeric string such that the
  files will be listed in order, then clear the screen and
  reset any other data structures that need it.

  Important note: you cannot just name your files in 
  regular sequence, like pic0, pic1, pic2, pic3... if that
  is done, then pic1, pic10, pic11... will come before pic2
  and so on. In order to keep things clear, add leading 0s
  to the numeric portion of the name. If you use sprintf, 
  you can use "%0xd" for this purpose. It will add at most
  x 0s in front of a number, if needed, so if used correctly,
  and x = 4, you would get numbers like 0001, 0002, 0011,
  0487

  05/17/12 09:41:35
  jdyrlandweaver
  ====================*/
void my_main(int polygons) {

    int i, f, j;
    double step;
    double xval, yval, zval, knob_value;
    struct matrix *transform;
    struct matrix *tmp;
    struct stack *s;
    screen t;
    color g;
    light_source l;
    char q;
    struct vary_node ** knobs;
    struct vary_node *vn;
    char frame_name[128], final_command[150];

    num_frames = 1;
    step = 0.05;

    g.red = 0;
    g.green = 255;
    g.blue = 255;

    first_pass();

    if (num_frames == 1)
        process_knobs();
    else
        knobs = second_pass();

    if (num_frames > 1)
        printf("Animating...");

    for (f = 0; f < num_frames; f++) {
        //printf("starting %dth frame...\n", f);
        if (f > 0) {
            printf(".");
            fflush(stdout);
        }

        s = new_stack();
        tmp = new_matrix(4, 1000);
        clear_screen(t);

        if (f > 0)
            modify_knobs(knobs[f]);

        for (i = 0; i < lastop; i++) {

            switch (op[i].opcode) {

                case IMPORT:
                    import_mesh(tmp, op[i].op.import.filename);
                    matrix_mult(s->data[ s->top ], tmp);
                    draw_polygons(tmp, t, g, l);
                    tmp->lastcol = 0;
                    break;
                case SET:
                    set_value(lookup_symbol(op[i].op.set.p->name),
                            op[i].op.set.p->s.value);
                    break;

                case SETKNOBS:
                    for (j = 0; j < lastsym; j++)
                        if (symtab[j].type == SYM_VALUE)
                            symtab[j].s.value = op[i].op.setknobs.value;
                    break;
                case LIGHT:
                    l.r =symtab[j].s.l->c[0]; 
                    l.g =symtab[j].s.l->c[1];
                    l.b =symtab[j].s.l->c[2];
                    l.x =symtab[j].s.l->l[0];
                    l.y =symtab[j].s.l->l[1];
                    l.z =symtab[j].s.l->l[2];
                    printf("LIGHT: %f\t%f\t%f\t%f\t%f\t%f\n",
                            l.r, l.g, l.b, l.x, l.y, l.z);
                    break;
                case AMBIENT:
                    break;

                case SPHERE:
                    add_sphere(tmp, op[i].op.sphere.d[0], //cx
                            op[i].op.sphere.d[1], //cy
                            op[i].op.sphere.d[2], //cz
                            op[i].op.sphere.r,
                            step);
                    //apply the current top origin
                    matrix_mult(s->data[ s->top ], tmp);
                    draw_polygons(tmp, t, g, l);
                    tmp->lastcol = 0;
                    break;

                case TORUS:
                    add_torus(tmp, op[i].op.torus.d[0], //cx
                            op[i].op.torus.d[1], //cy
                            op[i].op.torus.d[2], //cz
                            op[i].op.torus.r0,
                            op[i].op.torus.r1,
                            step);
                    matrix_mult(s->data[ s->top ], tmp);
                    draw_polygons(tmp, t, g, l);
                    tmp->lastcol = 0;
                    break;

                case BOX:
                    add_box(tmp, op[i].op.box.d0[0],
                            op[i].op.box.d0[1],
                            op[i].op.box.d0[2],
                            op[i].op.box.d1[0],
                            op[i].op.box.d1[1],
                            op[i].op.box.d1[2]);
                    matrix_mult(s->data[ s->top ], tmp);
                    draw_polygons(tmp, t, g, l);
                    tmp->lastcol = 0;
                    break;

                case LINE:
                    add_edge(tmp, op[i].op.line.p0[0],
                            op[i].op.line.p0[1],
                            op[i].op.line.p0[1],
                            op[i].op.line.p1[0],
                            op[i].op.line.p1[1],
                            op[i].op.line.p1[1]);
                    draw_lines(tmp, t, g);
                    tmp->lastcol = 0;
                    break;

                case MOVE:
                    //get the factors
                    xval = op[i].op.move.d[0];
                    yval = op[i].op.move.d[1];
                    zval = op[i].op.move.d[2];

                    if (op[i].op.move.p != NULL) {

                        knob_value = lookup_symbol(op[i].op.move.p->name)->s.value;
                        xval = xval * knob_value;
                        yval = yval * knob_value;
                        zval = zval * knob_value;
                    }

                    transform = make_translate(xval, yval, zval);
                    //multiply by the existing origin
                    matrix_mult(s->data[ s->top ], transform);
                    //put the new matrix on the top
                    copy_matrix(transform, s->data[ s->top ]);
                    free_matrix(transform);
                    break;

                case SCALE:
                    xval = op[i].op.scale.d[0];
                    yval = op[i].op.scale.d[1];
                    zval = op[i].op.scale.d[2];

                    if (op[i].op.scale.p != NULL) {

                        knob_value = lookup_symbol(op[i].op.scale.p->name)->s.value;
                        //printf("bigenator is %f\n", knob_value);
                        xval = xval * knob_value;
                        yval = yval * knob_value;
                        zval = zval * knob_value;
                    }

                    transform = make_scale(xval, yval, zval);
                    matrix_mult(s->data[ s->top ], transform);
                    //put the new matrix on the top
                    copy_matrix(transform, s->data[ s->top ]);
                    free_matrix(transform);
                    break;

                case ROTATE:
                    xval = op[i].op.rotate.degrees * (M_PI / 180);

                    //if knob exists, scale value accordingly
                    if (op[i].op.rotate.p != NULL) {

                        knob_value = lookup_symbol(op[i].op.rotate.p->name)->s.value;
                        xval = xval * knob_value;
                    }

                    //get the axis
                    if (op[i].op.rotate.axis == 0)
                        transform = make_rotX(xval);
                    else if (op[i].op.rotate.axis == 1)
                        transform = make_rotY(xval);
                    else if (op[i].op.rotate.axis == 2)
                        transform = make_rotZ(xval);

                    matrix_mult(s->data[ s->top ], transform);
                    //put the new matrix on the top
                    copy_matrix(transform, s->data[ s->top ]);
                    free_matrix(transform);
                    break;

                case PUSH:
                    push(s);
                    break;
                case POP:
                    pop(s);
                    break;
                case SAVE:
                    save_extension(t, op[i].op.save.p->name);
                    break;
                case DISPLAY:
                    display(t);
                    break;
            }
        }

        free_stack(s);
        free_matrix(tmp);
        //free_matrix( transform );

        sprintf(frame_name, "animations/%s%03d.png", name, f);
        //printf("saving %s...\n",frame_name);
        save_extension(t, frame_name);
    }
    if (num_frames > 1) {
        printf("\n");
        free_knob_array(knobs);
        sprintf(final_command, "animate -delay 4 animations/%s*", name);
        system(final_command);
    }
}
