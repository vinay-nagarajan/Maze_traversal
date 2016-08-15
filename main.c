#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer03.h"

// declaration of a function defined answer03.c
int Find_path_from_top_entrance_to_bottom_exit(char *mazefile, char *directionfile, char *visitedfile);

int main(int argc, char* argv[]) 
{
   // the first argument is the option "-s" or "-m"
   // -s means to perform simulation 
   // in that case, the second argument is the maze file and 
   // the third argument is the direction file, both files should be read
   // the fourth argument is the result of the simulation, with the visited 
   // locations marked as '.'  You should show the movement until the first 
   // failed location
   // the fifth argument through eighth argument specify the row and column
   // coordinates of the source and the row and column coordinates of the
   // destination
   //
   // -m means to determine the moves
   // in that case, the second argument is the maze file, which is read and
   // the third argument is the directions file, which is to be written
   // the fourth argument through seventh argument specify the row and column
   // coordinates of the source and the row and column coordinates of the
   // destination
   // 
   // -t means to find a path from top-most, left-most entrance to 
   // the bottom-most, right-most exit
   // in that case, the second argument is the maze file, which is read and
   // the third argument is the direction file, which is to be written
   // the fourth argument stores the explored locations in the maze while
   // looking for this path
   //
   Coord source;
   Coord destination;
   if (argc < 2) 
   {
       return EXIT_FAILURE;
   }
  /*
   if (strcmp("-t", argv[1]) == 0) {
      if (argc != 5) { 
         return EXIT_FAILURE; 
      }
      int step_count = Find_path_from_top_entrance_to_bottom_exit(argv[2], argv[3], argv[4]);
      printf("%d\n", step_count); 
   }
  */
   if(strcmp("-m", argv[1]) == 0) { 
      if(argc != 8) {
        return EXIT_FAILURE;
     }
    char *ptr, *ptr1, *ptr2, *ptr3;
   ptr=ptr1=ptr2=ptr3 = NULL;
   source.row = strtol(argv[4],&ptr,10);
   source.col = strtol(argv[5],&ptr1,10);
   destination.row = strtol(argv[6],&ptr2,10);
   destination.col = strtol(argv[7],&ptr3,10);  
  // if(ptr != '\0' || ptr1 != '\0' || ptr2 != '\0' || ptr3 != '\0')
  // return EXIT_FAILURE; 
 int step_count = DFS_shortest_path_directions(argv[2], argv[3], source, destination); 
   printf("%d\n", step_count); 
   }

  else if(strcmp("-s", argv[1]) == 0) { 
      if(argc != 9) {
        return EXIT_FAILURE;
     }
    char *ptr, *ptr1, *ptr2, *ptr3;
      ptr=ptr1=ptr2=ptr3 = NULL;
   source.row = strtol(argv[5],&ptr,10);
   source.col = strtol(argv[6],&ptr1,10);
   destination.row = strtol(argv[7],&ptr2,10);
   destination.col = strtol(argv[8],&ptr3,10);  
 //  if(ptr != '\0' || ptr1 != '\0'|| ptr2 != '\0' || ptr3 != '\0')
  // return EXIT_FAILURE; 
   int returnval = Simulate_movement(argv[2], argv[3], argv[4], source, destination); 
   printf("%d\n", returnval); 
   }


   return EXIT_SUCCESS;
}
