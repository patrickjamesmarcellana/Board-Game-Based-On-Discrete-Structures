/******************************************************************
This is to certify that this project is our own work, based on our personal efforts in studying and applying the
concepts learned. We have constructed the functions and their respective algorithms
and corresponding code by ourselves. The program was run, tested, and debugged by our own efforts. We further
certify that we have not copied in part or whole or otherwise plagiarized the work of other students and/or
persons.


Patrick James T. Marcellana, DLSU ID# 12134457
******************************************************************/


/*
Description: This program allows two users to play a board game where each player is given 5 pieces
             on the board. Each piece can only move forward, and each piece can eat the other player's pieces.
             This program follows the Linux Kernel coding standard.
Programmed by: Patrick James T. Marcellana, S22
Last modified: May 15, 2022
Version: 1
*/


#include <stdio.h>

#define ROWSIZE 7
#define COLUMNSIZE 5


/* Structure for Board Coordinates */
typedef struct CoordinateTag {
        int x;
        int y;
} CoordinateType;


/* This function assigns the initial coordinate values of Set S.
*/
void
assignSetS (CoordinateType* S)
{
        int row, col, j;
        j = 0;


        for (row = 0; row < ROWSIZE; row++)
                for (col = 0; col < COLUMNSIZE; col++)
                        if ((row + 1) % 2 == (col + 1) % 2) {
                                (S + j)->x = row + 1;
                                (S + j)->y = col + 1;
                                j++;
                        }
}


/* This function assigns the initial coordinate values of Set E.
*/
void
assignSetE (CoordinateType* S, 
         CoordinateType* E)
{
        int i, j;
        j = 0;


        for (i = 0; i < 18; i++)
                if ((S + i)->x >= 6) {
                        (E + j)->x = (S + i)->x;
                        (E + j)->y = (S + i)->y;
                        j++;
                }
}


/* This function assigns the initial coordinate values of Set Y.
*/
void
assignSetY (CoordinateType* S, 
         CoordinateType* Y)
{
        int i, j;
        j = 0;


        for (i = 0; i < 18; i++)
                if ((S + i)->x <= 2) {
                        (Y + j)->x = (S + i)->x;
                        (Y + j)->y = (S + i)->y;
                        j++;
                }
}


/* This function assigns the initial coordinate values of Set Free.
*/
void
assignSetFree (CoordinateType* Free)
{
        int row, col, i, j;
        i = 0;


        /* Assign the First 25 Coordinates of Set Free */
        for (row = 0; row < ROWSIZE; row++)
                for (col = 0; col < COLUMNSIZE; col++)
                        if ((row >= 2 && row <= 4) || 
                            ((row + 1) % 2 != (col + 1) % 2)) {
                                (Free + i)->x = row + 1;
                                (Free + i)->y = col + 1;
                                i++;
                        }

        
        /* Assign the Remaining Elements of the Set to 0 */
        for (j = 25; j < 34; j++) {
                (Free + j)->x = 0;
                (Free + j)->y = 0;
        }
}


/* This function assigns the initial coordinate values of Set Alpha and Beta.
*/
void
assignAlphaAndBeta (CoordinateType* Alpha,
                 CoordinateType* Beta,
                 CoordinateType* E,
                 CoordinateType* Y)
{
        int i;


        for (i = 0; i < 5; i++) {
                (Alpha + i)->x = (E + i)->x;
                (Alpha + i)->y = (E + i)->y;
                (Beta + i)->x = (Y + i)->x;
                (Beta + i)->y = (Y + i)->y;
        }
}


/* This function assigns all the elements of the board in the 2D Array given the
   coordinates from each set.
*/
void
assignBoardElements (char Board[][COLUMNSIZE],
                     CoordinateType* S,
                     CoordinateType* Free,
                     CoordinateType* Alpha,
                     CoordinateType* Beta)
{
        int i;


        /* Assign Set Free to the Board */
        for (i = 0; i < 34; i++) {
                if ((Free + i)->x != 0 && 
                    (Free + i)->y != 0) {
                        Board[((Free + i)->x) - 1][((Free + i)->y) - 1] = '*';
                }
        }


        /* Assign Set S to the Board */

        for (i = 0; i < 18; i++) {
                Board[((S + i)->x - 1)][((S + i)->y) - 1] = '.';
        }
        

        /* Assign Alpha and Beta to the Board */
        for (i = 0; i < 5; i++) {
                if ((Alpha + i)->x != 0 &&
                    (Alpha + i)->y != 0)
                        Board[((Alpha + i)->x) - 1][((Alpha + i)->y) - 1] = 'A';

                if ((Beta + i)->x != 0 &&
                    (Beta + i)->y != 0)    
                        Board[((Beta + i)->x) - 1][((Beta + i)->y) - 1] = 'B';
        }
}


/* This function displays the current board to the players.
*/
void 
displayBoard (char Board[][COLUMNSIZE])
{
        int row, col;

        printf ("  1 2 3 4 5\n");

        for (row = 0; row < ROWSIZE; row++) {
                printf ("%d ", row + 1);
                for (col = 0; col < COLUMNSIZE; col++)  {
                        printf ("%c ", Board[row][col]);
                }
                printf ("\n");
        }
}


/* This function gets the coordinates of the piece that the player wants to move and
   the coordinates of the position where the player wants to move it.
*/
void
getUserInput (CoordinateType* prev,
              CoordinateType* next)
{
        printf ("Enter the x coordinate of the piece you want to move: ");
        scanf ("%d", &prev->x);
        printf ("Enter the y coordinate of the piece you want to move: ");
        scanf ("%d", &prev->y);


        printf ("Enter the x coordinate of the position where you want to move the piece: ");
        scanf ("%d", &next->x);
        printf ("Enter the y coordinate of the position where you want to move the piece: ");
        scanf ("%d", &next->y);
}


/* This function returns 1 if the coordinate is found in the set. 
   Otherwise, it returns 0.
*/
int
isCoordinateInSet (CoordinateType coordinate, 
                   CoordinateType* Set, 
                   int nSetSize)
{
        int i, nReturn;
        nReturn = 0;

        for (i = 0; i < nSetSize; i++)
                if (coordinate.x == (Set + i)->x &&
                    coordinate.y == (Set + i)->y)
                        nReturn = 1;
        
        return nReturn;
}


/* This function deletes a specific coordinate from a set.
*/
void
deleteCoordinateInSet (CoordinateType* Set,
                       CoordinateType coordinate,
                       int nSetSize)
{
        int i, j;

        for (i = 0; i < nSetSize; i++) {
                if ((Set + i)->x == coordinate.x &&
                    (Set + i)->y == coordinate.y) {
                        for (j = i; j < nSetSize - 1; j++) {
                                (Set + j)->x = (Set + j + 1)->x;
                                (Set + j)->y = (Set + j + 1)->y;
                        }
                        
                        (Set + nSetSize - 1)->x = 0;
                        (Set + nSetSize - 1)->y = 0;
                }
        }
}


/* This function adds a specific coordinate in a set.
*/
void
addCoordinateInSet (CoordinateType* Set,
                    CoordinateType coordinate,
                    int nSetSize)
{
        int i; 

        for (i = 0; i < nSetSize; i++)
                if ((Set + i)->x == 0 &&
                    (Set + i)->y == 0) {
                        (Set + i)->x = coordinate.x;
                        (Set + i)->y = coordinate.y;
                        i = nSetSize;
                }
}


/* This function subtracts Set B from Set A.
*/
void
subtractSets (CoordinateType* SetA,
              CoordinateType* SetB,
              int nSetSize)
{
        int i, j;

        for (i = 0; i < nSetSize; i++)
                for (j = 0; j < nSetSize; j++)
                        if ((SetB + i)->x == (SetA + j)->x &&
                            (SetB + i)->y == (SetA + j)->y) 
                                deleteCoordinateInSet (SetA, *(SetB + i), 5);
}


/* This function gets the cardinality of a set.
*/
int 
getCardinality (CoordinateType* Set,
                int nSetSize)
{
        int i, nCount;
        nCount = 0;

        for (i = 0; i < nSetSize; i++)
                if ((Set + i)->x != 0 &&
                    (Set + i)->y != 0)
                        nCount++;

        return nCount;    
}


/* This function gets the difference of two sets and gets the cardinality of the
   resulting set.
*/
int
getCardinalityOfDiff (CoordinateType* SetA,
                      CoordinateType* SetB,
                      int nSetSize)
{
        int nResult, i;
        CoordinateType SetC[5];

        for (i = 0; i < 5; i++) {
                (SetC + i)->x = (SetA + i)->x;
                (SetC + i)->y = (SetA + i)->y;
        }

        subtractSets (SetC, SetB, nSetSize);
        nResult = getCardinality (SetC, nSetSize);

        return nResult;
}


/* This function displays the results of the game if the game meets its
   conditions for it to end.
*/
void
displayResults (char Board[][COLUMNSIZE],
                CoordinateType* S,
                CoordinateType* Free,
                CoordinateType* Alpha,
                CoordinateType* Beta,
                int nResult)
{
        printf ("------------------------------------------------------------\n\n");
        assignBoardElements (Board, S, Free, Alpha, Beta);
        displayBoard (Board);

        if (nResult)
                printf ("\n*** Beta Wins! ***");
        else    
                printf ("\n*** Alpha Wins! ***");
}


int main ()
{
        /* Main Board */
        char Board[ROWSIZE][COLUMNSIZE];


        /* Important Sets */
        CoordinateType Alpha[5], Beta[5], Free[34];
        CoordinateType S[18], E[5], Y[5];
        

        /* Important Variables */
        CoordinateType prev, next;
        int over = 0, ok = 0, aTurn = 1, nResult;


        /* Set S */
        assignSetS (S);
        

        /* Set E */
        assignSetE (S, E);


        /* Set Y */
        assignSetY (S, Y);


        /* Set Free */
        assignSetFree (Free);


        /* Sets Alpha and Beta */
        assignAlphaAndBeta (Alpha, Beta, E, Y);


        /* Game Loop */
        while (!over) {
                /* Update Board Elements */
                printf ("------------------------------------------------------------\n\n");
                assignBoardElements (Board, S, Free, Alpha, Beta);
                displayBoard (Board);


                /* Check Whose Turn Is It */
                if (aTurn)
                        printf ("\nIt's Alpha's Turn!\n\n");
                else
                        printf ("\nIt's Beta's Turn!\n\n");


                /* Ask for the Player's Move */
                getUserInput (&prev, &next);


                /* Check if Input is Valid */
                if (aTurn &&
                    isCoordinateInSet (prev, Alpha, 5) &&
                    prev.x == next.x + 1 &&
                    (next.y == prev.y || next.y == prev.y + 1 || prev.y == next.y + 1)) {
                        ok = !ok;
                } else if (!aTurn &&
                           isCoordinateInSet (prev, Beta, 5) &&
                           next.x == prev.x + 1 &&
                           (next.y == prev.y || next.y == prev.y + 1 || prev.y == next.y + 1)) {
                        ok = !ok;
                } 


                /* Modify the Sets Based on the Player's Move */
                if (ok &
                    aTurn) { /* Alpha's Turn */
                        if (isCoordinateInSet (next, Free, 34)) { /* If Alpha moves a piece to a position where
                                                                     there is no piece on it */
                                /* Update Set Alpha */
                                deleteCoordinateInSet (Alpha, prev, 5);
                                addCoordinateInSet (Alpha, next, 5);


                                /* Update Set Free */
                                deleteCoordinateInSet (Free, next, 34);
                                addCoordinateInSet (Free, prev, 34);
                                

                                /* Pass the Turn to Beta and Reset Validity */
                                aTurn = !aTurn;
                                ok = !ok;
                        } else if (isCoordinateInSet (next, Beta, 5)) { /* If Alpha moves a piece to a position
                                                                           where there is a piece on it */
                                if (!(isCoordinateInSet (next, S, 18))) { /* If the desired position is not a
                                                                             valid position to eat pieces */
                                        /* Reset Validity */
                                        ok = !ok;
                                } else if (isCoordinateInSet (next, S, 18)) { /* If the desired position is a
                                                                                 valid position to eat pieces */
                                        /* Update Set Beta */
                                        deleteCoordinateInSet (Beta, next, 5);

                                        
                                        /* Update Set Alpha */
                                        deleteCoordinateInSet (Alpha, prev, 5);
                                        addCoordinateInSet (Alpha, next, 5);


                                        /* Update Set Free */
                                        addCoordinateInSet (Free, prev, 34);


                                        /* Pass the Turn to Beta and Reset Validity */
                                        aTurn = !aTurn;
                                        ok = !ok;
                                }
                                
                        }
                } else if (ok &&
                           !aTurn) { /* Beta's Turn */
                        if (isCoordinateInSet (next, Free, 34)) { /* If Beta moves a piece to a position where
                                                                     there is no piece on it */
                                /* Update Set Beta */
                                deleteCoordinateInSet (Beta, prev, 5);
                                addCoordinateInSet (Beta, next, 5);


                                /* Update Set Free */
                                deleteCoordinateInSet (Free, next, 34);
                                addCoordinateInSet (Free, prev, 34);


                                /* Pass the Turn to Alpha and Reset Validity */
                                aTurn = !aTurn;
                                ok = !ok;
                        } else if (isCoordinateInSet (next, Alpha, 5)) { /* If Beta moves a piece to a position
                                                                            where there is a piece on it */
                                if (!(isCoordinateInSet (next, S, 18))) { /* If the desired position is not a
                                                                             valid position to eat pieces */
                                        /* Reset Validity */
                                        ok = !ok;
                                } else if (isCoordinateInSet (next, S, 18)) { /* If the desired position is a
                                                                                 valid position to eat pieces */
                                        /* Update Set Alpha */
                                        deleteCoordinateInSet (Alpha, next, 5);


                                        /* Update Set Beta */
                                        deleteCoordinateInSet (Beta, prev, 5);
                                        addCoordinateInSet (Beta, next, 5);


                                        /* Update Set Free */
                                        addCoordinateInSet (Free, prev, 34);
                                        

                                        /* Pass the Turn to Alpha and Reset Validity */
                                        aTurn = !aTurn;
                                        ok = !ok;
                                } 
                        }
                }


                /* Check if Game is Over */
                if (getCardinality (Beta, 5) == 0 ||
                        (getCardinality (Alpha, 5) != 0 && getCardinalityOfDiff (Alpha, Y, 5) == 0)) {
                        /* Alpha Wins */
                        over = 1;
                        nResult = 0;

                        displayResults (Board, S, Free, Alpha, Beta, nResult);
                } else if (getCardinality (Alpha, 5) == 0 ||
                           (getCardinality (Beta, 5) != 0 && getCardinalityOfDiff (Beta, E, 5) == 0)) {
                        /* Beta Wins */
                        over = 1;
                        nResult = 1;

                        displayResults (Board, S, Free, Alpha, Beta, nResult);
                }
        }


        return 0;
}