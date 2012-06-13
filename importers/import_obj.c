#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "import_obj.h"

int getNumVerts(FILE * f){
    int ans;
    char line[256];
    
    ans = 0;
    while (fgets(line, 255, f) != NULL)
        if(line[0] == 'v')
            ans++;
    
    return ans;
}

void import_mesh_obj(struct matrix * points, FILE * f) {
    char c, *line, *savedLine;
    int numVerts, current, i, j;
    double **vertIndex, pts[3][3];
  
                    int k;
    
    line = (char*)malloc(256*sizeof(char));
    savedLine = line;
    numVerts = getNumVerts(f);
    printf("there are %d lines of verts\n",numVerts);
    vertIndex = (double**)malloc(numVerts * sizeof(double*));
    for(i = 0; i < numVerts; i++)
        vertIndex[i] = (double*)malloc(3 * sizeof(double));
    rewind(f);
    current = 0;
    
    while (fgets(line, 255, f) != NULL) {
        line[strlen(line) - 1] = '\0';
        c = line[0];
        
        printf("the line is %s\n",line);
        switch(c) {
            case 'v':
                strsep(&line," ");
                
                vertIndex[current][0] = atof(strsep(&line," "));
                vertIndex[current][1] = atof(strsep(&line," "));
                vertIndex[current][2] = atof(strsep(&line," "));
                printf("\tPut %lf in v[%d][0], %lf in v[c][1], and %lf in v[c][2]\n",vertIndex[current][0],current,vertIndex[current][1],vertIndex[current][2]);
                current++;
                break;
            case 'f':
/*
                printf("begin\n");

                for(j=0;j<numVerts;j++){
                    for(k=0;k<3;k++){
                        printf("\t%lf",vertIndex[j][k]);
                    }
                    printf("\n");
                }

                printf("done\n");
*/
                strsep(&line," ");
                for(j = 0; j < 3; j++){
                    i = atoi(strsep(&line," ")) - 1;
                    pts[j][0] = vertIndex[i][0];
                    pts[j][1] = vertIndex[i][1];
                    pts[j][2] = vertIndex[i][2];
                }
                add_polygon( points,
                        pts[0][0], pts[0][1], pts[0][2],
                        pts[1][0], pts[1][1], pts[1][2],
                        pts[2][0], pts[2][1], pts[2][2]);
                break;
        }
        line = savedLine;
    }
    
    for(i = 0; i < numVerts; i++)
        free(vertIndex[i]);
    free(vertIndex);
    free(line);
}