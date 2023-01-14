/*  gfash2-proj3.c

	Written [5/25/2022] by [George Fashho] for CS 211, Summer 2022

	This program maintains a linked list of integers,
    representing a set of vertices making up a polygon.
    each int represents the unique ID of a particular vertex.
    The program returns the list of vertices, the min & max vertices,
    and search for vertices entered by the user in the list.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

int debug = FALSE ;

// VertexNode properties:
typedef struct VertexNode {
    int ID;
    struct VertexNode *next;
}VertexNode;

// Polygon properties:
typedef struct Polygon {
    VertexNode *vertices;
    int nVertices;
    int min, max;
}Polygon;

/*Function definitions*/
int addVertexToPolygon( Polygon* polygon, int vertexID );
VertexNode* findVertexInPolygon( Polygon polygon, int vertexID, int * location );
int size( Polygon polygon );
int isEmpty( Polygon polygon );
int remove1stVertexFromPolygon( Polygon* polygon );
void printList(Polygon * head);
VertexNode* findVertexInPolygonBool( Polygon polygon, int vertexID, int *bool );


int main( int argc, char const *argv[ ] ){

    for( int i = 0; i < argc; i++ ) { //debugging information!!

        /* access the ith ARGument Value */
        if( strcmp(argv[i],"-d")==0 ) {
            printf("Debug mode on!\n");
            debug = TRUE;  //debug mode on since -d called
        }
    }

    Polygon listOne; //initializing a structure instance
    int userInput ;

    listOne.vertices = NULL; //setting head of linked list to NULL
    listOne.nVertices = 0; //counter to keep track of size of list

    /*Prompting user to enter a list of indices*/
    printf("Please enter a list of indices, followed by a negative integer\n");
    scanf("%d",&userInput);

    listOne.min = userInput; //initializing min and max values
    listOne.max = userInput;
    int bool = 0; //bool to check if integer is already present in list

    while(userInput >= 0){ //while loop to continue accepting user inputs until a -ve is inputted

        findVertexInPolygonBool(listOne,userInput,&bool);
        if(bool != 1) {
            int addedOrNot = addVertexToPolygon(&listOne, userInput);
            if(debug == 1){

                printf("[DEBUG] ");
                if(addedOrNot == 0){ printf("Successfully added Node\n");}
                else{ printf("Node not added to list\n");}
            }
        }
        else{
            printf("[WARNING] That value is already an indices, duplicates are not allowed.\nPlease enter another integer:\n");
        }
        scanf("%d",&userInput);
    }

    /*outputting the polygon properties and checking if the list of vertices is empty or not*/
    if(isEmpty(listOne)!= 1) {
        printf("The polygon has %d vertices, with indices ", size(listOne));
        printList(&listOne);
        printf(".\nThe min and max vertices found are %d and %d respectively.", listOne.min, listOne.max);
    }else{
        printf("The polygon contains no vertices!");
    }

    /*Prompting the user to input a set of integers to find in the list of vertices*/
    printf("\nPlease enter a set of integers to find possible corresponding vertices in the polygon.\n");

    scanf("%d",&userInput);
    int *location =NULL;

    while(userInput >= 0){//keep accepting user input until a -ve value is inputted
        findVertexInPolygon(listOne,userInput,location);

        scanf("%d",&userInput);
    }

    int vertexRemoved =0; //initializing integer of vertexID that was removed

    /*while loop to remove all integers from linked list and free allocated memory*/
    while( (vertexRemoved = remove1stVertexFromPolygon(&listOne))!=-1){
        if(debug ==1 ) { //debugging information about values removed and new list
            printf("[DEBUG] \nRemoved: %d\n[DEBUG] List:", vertexRemoved);
            printList(&listOne);
        }
    }
    return 0;
}// end main()


/*
 * This function traverses through a linked list and returns either a 0(FALSE) or 1(TRUE) if
 * the vertexID entered by the user is already present in the list
 *
 * @param polygon a pointer to structure Polygon
 * @param vertexID the inputted value by the user and value of the vertex
 * @param bool a reference integer that is either 0(FALSE) or 1(TRUE) that reports back to the user
 * if the VertexID was found in the list or not
 */
VertexNode* findVertexInPolygonBool( Polygon polygon, int vertexID, int *bool ){

    int index = 0 ; //initializing index of vertexID in the linked list
    *bool = 0;
    if(polygon.vertices == NULL){
        *bool =0;
    }

    VertexNode *current;
    /*for loop to traverse through linked list*/
    for( current = polygon.vertices; current != NULL; current = current->next ) {
        if (current->ID == vertexID) {
            *bool = 1;
        }
    }
    return 0;
}//end findVertexInPolygonBool()


/*
 * This function adds a vertex ID to the head of the linked list
 *
 * @param polygon a pointer to structure Polygon
 * @param vertexID the inputted value by the user and value of the vertex
 * ...
 * @return 0 if the vertex ID is successfully added to the polygon, or -1 otherwise.
 */
int addVertexToPolygon( Polygon* polygon, int vertexID ){

    /*finding the min and max integers in the list of vertices*/
    if(vertexID > polygon->max){
        polygon->max = vertexID;
    }
    else if(vertexID< polygon->min){
        polygon->min = vertexID;
    }

    /*Allocating memory to the node in the linked list*/
    VertexNode *tempNode = (VertexNode*) malloc(sizeof(VertexNode));

    tempNode->ID = vertexID;
    tempNode->next = polygon->vertices;

    polygon->vertices = tempNode;
    polygon->nVertices += 1;
    return 0; //returning 0 after successfully adding value
 } //end addVertexToPolygon()


/*
* This function traverses through a linked list and checks to see if the VertexID entered by a user is
* found in the list and returns various outputs depending on if the list is empty, vertexID is found
* or vertexID is not found
*
* @param polygon a pointer to structure Polygon
* @param vertexID the inputted value by the user and value of the vertex
* @param location a reference integer that returns the index of where the vertexID was found
* in the list
*/
VertexNode* findVertexInPolygon( Polygon polygon, int vertexID, int * location ){

    int index = 0 ; //initializing integer of vertexID index in list
    /*bool integer to check if list is empty,contains the integer or does not.*/
    int bool = 0;

    if(polygon.vertices == NULL){
        printf("The polygon contains no vertices, therefore Vertex %d, was not found!\n",vertexID);
        bool =1;
    }

    VertexNode *current;
    for( current = polygon.vertices; current != NULL; current = current->next ) {
        if (current->ID == vertexID) {
            printf("Vertex %d was found in VertexNode %d at address %p.\n", vertexID, index, current);
            bool = 1;
        }
        index++;
    }

    if(bool != 1){
        printf("Vertex %d was not found.\n",vertexID);
    }
    return 0;
}//end findVertexInPolygon()


/*
* This function returns the size of the linked list
*
* @param polygon a pointer to structure Polygon
* @return returns the size of the linked list
*/
int size( Polygon polygon ){
    return polygon.nVertices;
}


/*
* This function that traverses through a linked list and prints out the vertexID's
*
* @param head a pointer to the head of the linked list
*/
void printList( Polygon * head ) {
    VertexNode *current = NULL;

    for( current = head->vertices; current != NULL; current = current->next )
        printf( "%d ", current->ID );
} // end printList()


/*
* This function checks if the linked list is empty
*
* @param polygon a pointer to structure Polygon
* @return returns a 1 if the list is empty otherwise, returns a 0
*/
int isEmpty( Polygon polygon ){
    if(polygon.nVertices == 0){
        return 1;
    }
    return 0;
} //end isEmpty()


/*
* This function removes the vertexID at the head of the linked list and frees the allocated
* memory
*
* @param polygon a pointer to structure Polygon
* @return returns 01 if the list is empty otherwise, returns the vertexID that was removed from the list
*/
int remove1stVertexFromPolygon( Polygon* polygon ){

    int tempID = 0; //initializing temp integer to hold the value of vertexID

    if(polygon->vertices == NULL){
        return -1; //checking if list is empty
    }

    VertexNode * temp = NULL;
    tempID = polygon->vertices->ID;
    temp = polygon->vertices;
    polygon->vertices = temp->next ;
    temp->next = NULL;

    free(temp); //freeing up allocated memory!!
    return tempID;
} //end removeNodeFromHead()


