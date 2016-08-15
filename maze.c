#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "maze.h"

Maze *Allocate_maze_space(int nrow, int ncol)
{ int i = 0;
  int j = 0;
 Maze * maze = (Maze *)malloc(sizeof(Maze));

 if(maze == NULL)
  {
   free(maze);
   return NULL;
  }

 maze->maze_array = (char **)malloc(sizeof(char *)*nrow);

if(maze->maze_array == NULL)
  {
    free(maze->maze_array);
    free(maze);
    return NULL;
  }


 for(i = 0;i<nrow;i++)
    {
      maze->maze_array[i] = (char *)malloc(sizeof(char)*ncol);
      if(maze->maze_array[i] == NULL)
   {
      free(maze->maze_array[i]);
    for(j = 0;j<i;j++)
       {
         free(maze->maze_array[j]);
       }   
    free(maze->maze_array);
    free(maze);
   return NULL;
        
    }

 }


 maze->nrow = nrow;
 maze->ncol = ncol;
 return maze;
}

// free the memory used for the maze
// you may assume that maze is not NULL, and all memory addresses are valid

void Deallocate_maze_space(Maze *maze)
{  int i;
   for(i = 0;i<maze->nrow;i++)
   { 
     free(maze->maze_array[i]);
   }
   free(maze->maze_array);
   free (maze);
   return;
}

/* Read maze in a multi-line file and store the necessary information */
/* into a 2D array of characters.  The address of the 2D array and the */
/* dimensions of the 2D array should be stored in a Maze structure allocated */
/* from the heap memory */
/* if the allocation fails, should return NULL */

Maze *Read_maze_from_2Dfile(FILE *fptr)
{ Maze *maze;
  int i,j;
  i = 0;
  j = 0;
  int ch;
  fseek(fptr,0,SEEK_SET);

  while((ch = fgetc(fptr)) != EOF)
     {
        if(ch != '\n')
          j = j +1;
        else if(ch == '\n')
          i = i + 1;
     }   
   j = j / i;

  maze =  Allocate_maze_space(i, j);

  if(maze == NULL)
  return NULL;
  fseek(fptr,0,SEEK_SET);
  i = 0; j = 0;
  while((ch = fgetc(fptr)) != EOF)
     {
        if(ch != '\n')
           {maze->maze_array[i][j] = ch; j++;}
        else if(ch == '\n')
          {i++; j = 0;}
     }  
  return maze;
}

/* Write the maze into a multi-line file */
/* the maze has to stay intact */

int Write_maze_to_2Dfile(char *filename, const Maze *maze)
{

  int ch = 0;
  int i,j;
  FILE *wptr;
  wptr = fopen(filename,"w");
  for(i = 0;i<maze->nrow;i++)
  { 
    for(j = 0;j<maze->ncol;j++)
     {
       ch = maze->maze_array[i][j];
       fputc(ch,wptr);
 
     }
       fputc('\n',wptr);
  }
   fclose(wptr);
   return 0;
}
/* given a maze, given a location cur, is that PATH */
/* if out of bound, return 0                        */
/* if within bound, but not PATH, return 0          */
/* otherwise return 1                               */

static int Is_path(Maze *maze, int row, int col)
{
   if ((row >= 0) && (row < maze->nrow)) {
      if ((col >= 0) && (col < maze->ncol)) {
         return maze->maze_array[row][col] == PATH;
      }
   }
   return 0;
}

static int minpathfinder(Maze *maze, int curr, int curc, int endr, int endc,
                            int count, FILE *dfptr)
{
  maze->maze_array[curr][curc] = VISITED;
 int i;
 if ((curr == endr) && (curc == endc)) { // reach the destination 
 for (i = 0; i < count; i++) { // create enough space in file
      fputc(VISITED, dfptr);
      }
    fputc('\n', dfptr);
}     
 int found;
   if (Is_path(maze, curr-1, curc)) {

      found = minpathfinder(maze, curr-1, curc, endr, endc, count+1, dfptr);
      if (found != -1) {
         fseek(dfptr, -1, SEEK_CUR); // go back one position
         fputc('N', dfptr);
         fseek(dfptr, -1, SEEK_CUR); // go back one position
         return found;
      }
   }
   if (Is_path(maze, curr+1, curc)) {

      found = minpathfinder(maze, curr+1, curc, endr, endc, count+1, dfptr);
      if (found != -1) {
         fseek(dfptr, -1, SEEK_CUR); // go back one position
         fputc('S', dfptr);
         fseek(dfptr, -1, SEEK_CUR); // go back one position
         return found;
      }
   }
   if (Is_path(maze, curr, curc+1)) {

       found = minpathfinder(maze, curr, curc+1, endr, endc, count+1, dfptr);
      if (found != -1) {
         fseek(dfptr, -1, SEEK_CUR); // go back one position
        fputc('E', dfptr);
         fseek(dfptr, -1, SEEK_CUR); // go back one position
         return found;
      }
   }
   if (Is_path(maze, curr, curc-1)) {

     found = minpathfinder(maze, curr, curc-1, endr, endc, count+1, dfptr);
      if (found != -1) {
        fseek(dfptr, -1, SEEK_CUR); // go back one position
        fputc('W', dfptr);
        fseek(dfptr, -1, SEEK_CUR); // go back one position
         return found;
      }
   }
   maze->maze_array[curr][curc] = ' ';
   return -1;
}

int helper3(FILE *dfptr)
{

    int ch;

 
//  fseek(dfptr, 2,SEEK_SET);
// ch =  ftell(dfptr);
 //  ch = fgetc(dfptr);
 //  printf("ch value %d\n", ch);
  int count2 = 0; int countmin = 0; int firstrow = 1;
 
  while((ch = fgetc(dfptr)) != EOF)
     {  

        count2++;
        if(firstrow == 1)
           {
             countmin = count2;
           }
         if(ch == '\n')
           { firstrow++;
             if(countmin > count2)
                countmin = count2;     
             count2 = 0;        
           } 
         

     }
 

  return countmin-1;
}

static int Pathfinder_helper(Maze *maze, int curr, int curc, int endr, int endc,
                            int count, FILE *dfptr, int countmin)
{
   // you may uncomment this and use the output here to trace the function
   // with sample5.pdf
   //printf("(%d, %d), %d\n", curr, curc, count);
   maze->maze_array[curr][curc] = VISITED;
   if ((curr == endr) && (curc == endc) && (count == countmin)) { // reach the destination 
    int i;
      for (i = 0; i < count-1; i++) { // create enough space in file
         fputc(VISITED, dfptr);
   }
    //  fputc('\n',dfptr);
    fseek(dfptr,0,SEEK_SET);
    return count;
   }

   int found;
   if (Is_path(maze, curr-1, curc)) {
      found = Pathfinder_helper(maze, curr-1, curc, endr, endc, count+1, dfptr, countmin);
     if (found != -1) {
	
        fseek(dfptr, count,SEEK_SET); // go back one position
        fputc('N', dfptr);
   //     fseek(dfptr, -1, SEEK_CUR); // go back one position
       return found;  }
   }
   if (Is_path(maze, curr+1, curc)) {
      found = Pathfinder_helper(maze, curr+1, curc, endr, endc, count+1, dfptr, countmin);
     if (found != -1) {
      fseek(dfptr, count,SEEK_SET);  // go back one position
        fputc('S', dfptr);
     //   fseek(dfptr, -1, SEEK_CUR); // go back one position
        return found;}
   }
   if (Is_path(maze, curr, curc+1)) {
      found = Pathfinder_helper(maze, curr, curc+1, endr, endc, count+1, dfptr, countmin);
     if (found != -1) {
     fseek(dfptr,count,SEEK_SET); //    fseek(dfptr, -1, SEEK_CUR); // go back one position
         fputc('E', dfptr);
      //  fseek(dfptr, -1, SEEK_CUR); // go back one position
       return found;}
   }
   if (Is_path(maze, curr, curc-1)) {
      found = Pathfinder_helper(maze, curr, curc-1, endr, endc, count+1, dfptr, countmin);
      if (found != -1) {
        fseek(dfptr, count , SEEK_SET); // go back one position
       fputc('W', dfptr);
     //  fseek(dfptr, -1, SEEK_CUR); // go back one position
       return found;}
   }
 // fputc('\n',dfptr);
  maze->maze_array[curr][curc] = ' ';
   return -1;
}

/* given a maze, current row (curr),                                     */
/* current column (curc), destination row (drow), destination col (dcol) */
/* current distance from source, determine the where to explore from the */
/* current location                                                      */
/* a recursive function: from the current location, try 'N', 'S', 'E',   */
/* 'W' locations one at a time if the attempt is successful, write the   */
/* direction in the file                                                 */



/* this is a function implemented by the instructor to illustrate           */
/* how to use a recursive function to go from the left-most top entrance to */
/* the right-most bottom exit of a maze.  The function will print the       */
/* directions taken to reach the bottom exit, without visiting a location   */
/* twice.  Note that the recursion may visit a location a few times         */
/* Given three filenames, first one is a given maze, the second one is to   */
/* be written with the directions, the third is the explored maze           */
/* If the maze file does not exist, the function immediately return -1 and  */
/* do nothing else                                                          */
/* the directions are in the form of a series of characters                 */
/* 'N', 'S', 'E', 'W', without spacing (and without the quotes).            */
/* There should be just a single line, without a terminating newline        */
/* character. In other words, to the vi editor, it is an incomplete file    */
/* you are to assume that you start from the top-most, left-most entry      */
/* of the maze.  Therefore, the first direction is always 'S'               */
/* similarly, the last direction is always 'S'                              */
/* the returned value should be the number of steps taken to reach          */
/* the right-most bottom exit.  It is typically not the shortest distance   */
/* Note that the maze can be generated by amaze, or some combinations of    */
/* the maze expansion functions that you have written in PE08               */
/* you may assume that the maze file, if it exists, is in the correct format*/
/*
int Find_path_from_top_entrance_to_bottom_exit(char *mazefile, char *directionfile)
{
   FILE *mfptr = fopen(mazefile, "r");
   if (mfptr == NULL) {
      return -1;
   }
*/   
   /* read the maze from mfptr */
/*
   Maze *maze = Read_maze_from_2Dfile(mfptr);
   Maze *maze2 = Read_maze_from_2Dfile(mfptr);
   fclose(mfptr);
   if (maze == NULL) {
      return -1;
   }
   if(maze2 == NULL)
   {
     return -1;
   }
  */ 
   /* find the starting location, the top-most, left-most opening */
  /* int startr, startc;
   startr = 0;
   for (startc = 0; startc < maze->ncol; startc++) {
      if (maze->maze_array[startr][startc] == PATH) {
         break;
      }
   }
*/
   /* find the ending location, the bottom-most, right most opening */
  /* int endr, endc;
   endr = maze->nrow-1;
   for (endc = maze->ncol-1; endc >= 0; endc--) {
      if (maze->maze_array[endr][endc] == PATH) {
         break;
      }
   } 
*/
   // no starting location, deallocate and return 
  /* if (startc == maze->ncol) {
      Deallocate_maze_space(maze);
      Deallocate_maze_space(maze2);
      return -1;
   }*/
   // no ending location, deallocate and return 
  /* if (endc == -1) {
      Deallocate_maze_space(maze);
      Deallocate_maze_space(maze2);
      return -1;
   }

   FILE *dfptr = fopen(directionfile, "w");
   if (dfptr == NULL) {
      Deallocate_maze_space(maze);
      Deallocate_maze_space(maze2); 
      return -1;
   }
   minpathfinder(maze, startr, startc, endr, endc, 0, dfptr);
   fclose(dfptr);
  
   FILE *dfptr1 = fopen(directionfile, "r");
   if (dfptr1 == NULL) {
      Deallocate_maze_space(maze);
      Deallocate_maze_space(maze2); 
      return -1;
   }   

  int countmin = helper3(dfptr1);
  // printf(" Value of countmin %d\n",countmin);
   fclose(dfptr1);

   FILE *dfptr2 = fopen(directionfile, "w");
   if (dfptr2 == NULL) {
      Deallocate_maze_space(maze);
      Deallocate_maze_space(maze2);
      return -1;
 }

   int steps = Pathfinder_helper(maze, startr, startc, endr, endc, 0, dfptr2, countmin);
   fclose(dfptr2);
   Deallocate_maze_space(maze);
   Deallocate_maze_space(maze2);  
   return steps;
}
*/

/* Details of the function can be found in README                        */
/* this function expects four parameters: two filenames, and coordinates */
/* of two locations                                                      */
/* two filenames: first one is a given maze, the second one is to        */
/* be written with the directions                                        */
/* coordinates of two locations: source and destination locations        */
/*                                                                       */
/* If the maze file does not exist, you should immediately return -1 and */
/* do nothing else, i.e., the direction file should not be created       */
/* If the maze file cannot be read into a Maze structure, you should     */
/* immediately return -1 and do nothing else                             */
/* If the coordinates are not valid, do nothing and return -1            */
/* If the directions file cannot be opened for writing, return -1        */
/* The function should write the directions of a shortest path from the  */
/* source location to the destination location into the directions file  */
/* the directions are in the form of a series of characters              */
/* 'N', 'S', 'E', 'W', without spacing (and without the quotes).         */
/* There should be just a single line, without a terminating newline     */
/* character. In other words, to the vi editor, it is an incomplete file */
/* The directions should allow an agent to travel in the maze from the   */
/* source coordinates to the destination coordinates using the fewest    */
/* number of moves                                                       */
/* the number of moves taken by the agent in a shortest path should be   */
/* returned                                                              */
/* Note that the maze can be generated by amaze, or some combinations of */
/* the maze expansion functions that you have written in PE08            */
/* you may assume that the maze file, if it exists, is in the correct    */
/* format                                                                */
int DFS_shortest_path_directions(char *mazefile, char *directionfile,
                        Coord source, Coord destination)
{
  FILE *mfptr = fopen(mazefile, "r");
   if (mfptr == NULL) {
     fclose(mfptr); 
     return -1;
   }
   
   /* read the maze from mfptr */


   Maze *maze = Read_maze_from_2Dfile(mfptr);
   Maze *maze2 = Read_maze_from_2Dfile(mfptr);
   fclose(mfptr);
   if (maze == NULL) {
     
      Deallocate_maze_space(maze);
      Deallocate_maze_space(maze2);    
      return -1;
   }
   if(maze2 == NULL)
   {     Deallocate_maze_space(maze);
     Deallocate_maze_space(maze2);
     return -1;
   }
   
   /* find the starting location, the top-most, left-most opening */
   if(source.row > maze->nrow || source.col > maze->ncol || source.row < 0 || source.col < 0 || destination.row > maze->nrow || destination.col > maze->ncol || destination.col <0 || destination.row < 0)
   {
     Deallocate_maze_space(maze);
     Deallocate_maze_space(maze2);
    // fclose(mfptr);
     return -1;
 
    }
int check = 0;

   if((Is_path(maze, source.row, source.col)))
     {
    // fclose(mfptr);
     check = -1;
     }

  if((Is_path(maze, destination.row, destination.col)))
     {
     // fclose(mfptr);
     check =  -1;
     }
   // no starting location, deallocate and return 
   
   // no ending location, deallocate and return 
   if(check != -1)
  {
    Deallocate_maze_space(maze);
       Deallocate_maze_space(maze2);
    return -1;

}

   FILE *dfptr = fopen(directionfile, "w");
   if (dfptr == NULL) {
      Deallocate_maze_space(maze);
      Deallocate_maze_space(maze2); 
      return -1;
   }
   minpathfinder(maze, source.row, source.col, destination.row, destination.col, 0, dfptr);
   fclose(dfptr);
  
   FILE *dfptr1 = fopen(directionfile, "r");
   if (dfptr1 == NULL) {
      Deallocate_maze_space(maze);
      Deallocate_maze_space(maze2); 
      return -1;
   }   

  int countmin = helper3(dfptr1);
  // printf(" Value of countmin %d\n",countmin);
   fclose(dfptr1);

   FILE *dfptr2 = fopen(directionfile, "w");
   if (dfptr2 == NULL) {
      Deallocate_maze_space(maze);
      Deallocate_maze_space(maze2);
      return -1;
 }

   int steps = Pathfinder_helper(maze, source.row, source.col, destination.row, destination.col, 0, dfptr2, countmin);
//   Write_maze_to_2Dfile(visitedfile, maze);
   fclose(dfptr2);
   Deallocate_maze_space(maze);
   Deallocate_maze_space(maze2);  
   return steps;

}

/* Details of the function can be found in README                        */
/* this function expects 5 parameters: 3 filenames, and coordinates of   */
/* the source and destination locations                                  */
/* three filenames: first one is a given maze, the second one is a       */
/* directions file (a series of characters 'N', 'S', 'E', 'W'), you are  */
/* to simulate the movement of agent according to the directions         */
/* given in the direction files. The maze showing the locations that the */
/* agent has visited will be stored in the third file.                   */
/* the source and destination coordinates are the starting and end points*/
/* of the agent's path in the maze                                       */
/*                                                                       */
/* If the maze file does not exist, you should immediately return -1 and */
/* do nothing else                                                       */
/* If the maze file cannot be read into a Maze structure, you should     */
/* immediately return -1 and do nothing else                             */
/* If the direction file does not exist, you should immediately return   */
/* -1 and do nothing else                                                */
/* if the coordinates are not valid, you should immediately return -1    */
/* and do nothing else                                                   */
/* Otherwise, it means that the agent has been airdropped to the starting*/
/* and the starting location has been visited                            */
/* You should simulate until you reach the end of file of the direction  */
/* file or encounter an invalid direction                                */
/* you should print the visited maze into the third given filename, the  */
/* visitedfile, with visited locations represented by '.'                */
/* if you encounter an invalid direction (wrong character, try to visit  */
/* a location with WALL, try to visit a location again, try to move the  */
/* agent out of bound), you should return -1                             */
/* do not change a location with WALL to a VISITED location              */
/* If the agent successfully reach the final destination with valid      */
/* directions in the direction file, and the visited maze can be output  */
/* properly, you should return the number of visited locations,          */
/* otherwise, return -1                                                  */
/* you may assume that the maze file, if it exists, is in correct format */

void gotolocation(FILE *fptr, int row, int col)
{
int ch = 0; int nrow, ncol;
      nrow = ncol = 0;
      fseek(fptr,0,SEEK_SET);
      while((ch = fgetc(fptr)) != EOF)
       {
        if(ch != '\n')
          ncol = ncol +1;
        else if(ch == '\n')
          nrow = nrow + 1;
       }   
      ncol = (ncol) / (nrow);
  
     int location = 0;
     fseek(fptr,0,SEEK_SET);

     location = (ncol+1)*row + (col+1);
     fseek(fptr,location-1,SEEK_SET);

} 


int Simulate_movement(char *mazefile, char *directionfile, char *visitedfile,
                      Coord source, Coord destination)
{
  int ch; 
  FILE *fptr, *fptr1, *fptr2;
  fptr = fopen(mazefile,"r");
  if(fptr == NULL)
   { fclose(fptr);
    return -1;
}

  FILE *dptr;
  dptr = fopen(directionfile,"r");
  if(dptr == NULL)
{    fclose(fptr); fclose(dptr);
      return -1;
}
  fptr1 = fopen(visitedfile, "w");
  if(fptr1 == NULL)
    {
     fclose(fptr); fclose(dptr); fclose(fptr1);
 return -1;
}

ch = 0; 

while((ch = fgetc(fptr)) != EOF)
{
if(ch == 'X')
 {
 fputc('X',fptr1);
 }
else if(ch == ' ')
 {
 fputc(' ',fptr1);
 }
 else 
 {fputc('\n',fptr1);}
}

fclose(fptr1);



  fptr2 = fopen(visitedfile, "r+");
  if(fptr2 == NULL)
{  fclose(fptr); fclose(dptr); fclose(fptr2);
  
    return -1;
}
 gotolocation(fptr2, source.row, source.col) ;


 int crow = 0;
 int ccol = 0; 
int count = 0;
 if((ch = fgetc(fptr2)) == ' ')
 {   fseek(fptr2, -1, SEEK_CUR);
     fputc('.', fptr2);
     count++;
     crow = source.row;
     ccol = source.col;
    }
 else 
 {// fclose(fptr2);
//   fclose(fptr1);
  // fclose(fptr);
  // fclose(dptr);
fcloseall;
  return -1;
}  
int ch2 = ' ';
fseek(dptr, 0, SEEK_SET);

// while((ch2 == fgetc(dptr)) == 'N' || (ch2 == fgetc(dptr)) == 'S' || (ch2 == fgetc(dptr)) == 'E' || (ch2 == fgetc(dptr)) != 'W')
//{
//printf("%c\n",ch2);
//}


//fseek(dptr, 0, SEEK_SET);

 while((ch2 = fgetc(dptr)) != EOF)
     { 
       if(ch2 == 'N')
         {   
             gotolocation(fptr2, crow-1, ccol) ;
             fputc('.', fptr2);
             crow = crow - 1;
            // printf("Value of row and column is (%d %d)\n",crow,ccol);
             count++;
         }
       else if(ch2 == 'S')
         {  
             gotolocation(fptr2, crow+1, ccol) ;
             fputc('.', fptr2);
             crow = crow+1;
             //          printf("Value of row and column is (%d %d)\n",crow,ccol);
             count++;
         }

       else if(ch2 == 'E')
         {   
             gotolocation(fptr2, crow, ccol+1) ;
             fputc('.', fptr2);
             ccol = ccol +1;
           //  printf("Value of row and column is (%d %d)\n",crow,ccol);
             count++;
         } 

       else if(ch2 == 'W')
         {   
             gotolocation(fptr2, crow, ccol-1) ;
             fputc('.', fptr2);
             ccol = ccol - 1;
         //    printf("Value of row and column is (%d %d)\n",crow,ccol);
             count++;         
           }
       else
        { 
fclose(fptr2);
//   fclose(fptr1);
   fclose(fptr);
   fclose(dptr);


        return -1; }
//       ch2 = fgetc(dptr);
     } 

   fclose(fptr2);
//   fclose(fptr1);
   fclose(fptr);
   fclose(dptr);
   return count;
}

