/*
 * Thomas Heckman
 * Lab_3
 * 67349
 * T-2:15
 *
 * File: /generic/table.c
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "assert.h"
#include "set.h"

#define EMPTY 0
#define FILLED 1
#define DELETED 2

/* [typedef struct]
 * Worst Case: O(1) 
 * Best Case: O(1)
 */	
typedef struct set{
	int count;
	int length;
	void **data;
	int *flags;
	int(*compare)();
	unsigned (*hash)();
} SET;

/* [strhash]
 * Worst Case: O(1) 
 * Best Case: O(1)
 */	
unsigned strhash(char *s){
	unsigned hash = 0;
while(*s != '\0')
	hash = 31*hash+*s++;
return(hash);
}

/* [searchElement]
 * Worst Case: O(n) 
 * Best Case: O(1)
 */	
int searchElement(SET *sp, void *elt, bool *found){	
	assert(sp!=NULL);						//Assertion Start
	assert(sp->data!=NULL);					//...
	assert(sp->flags!=NULL);				//...
	assert(elt!=NULL);						//Assertion End
	int i,hk,deleted=-1;
	for(i=0; i<sp->length-1; i++){
		hk = ((*sp->hash)(elt)+i)%(sp->length);	 //re-evaluates hk
		if (sp->flags[hk]==0){				//if flag is EMPTY
			*found=false;					//elt not found
			if(deleted==-1)	  				//if deleted unchanged
				return(hk);					//return empty space
			else							//otherwise
				return(deleted);			//return deleted location
		}
		else if(sp->flags[hk]==1){			//if flag is FILED
			if((*sp->compare)(sp->data[hk],elt)==0){
				*found=true;				//elt found
				return(hk);					//return location where found
			}
		}
		else if(sp->flags[hk]==2){			//if flag is DELETED
			*found=false;					//elt not found
			if(deleted==-1)					//if deleted was unchanged
				deleted=hk;					//update deleted to first deleted spot
		}
	}
	*found=false;
	return(deleted);
}

/* [createSet]
 * Worst Case: O(m) 
 * Best Case: O(m)
 */	
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()){
	SET *sp;
	sp = malloc(sizeof(SET));
	assert(sp!=NULL);						//Alllocate and Assert sp
	sp->count = 0;							//Initial conditions
	sp->length = maxElts;					//Initial conditions
	sp->data = malloc(sizeof(void*)*maxElts);
	assert(sp->data!=NULL);					//Allocate and Assert data
	sp->flags = malloc(sizeof(int)*maxElts);
	assert(sp->flags!=NULL);				//Allocate and Assert flags
	int i;
	for(i=0;i<sp->length-1;i++){			//flags initially set to EMPTY
		sp->flags[i] = 0;
	}
	sp->hash=hash;
	sp->compare=compare;
	return(sp);
}

/* [destroySet]
 * Worst Case: O()
 * Best Case: O()
 */	
void destroySet(SET *sp){
	assert(sp!=NULL);						//Assertion Start
	assert(sp->flags!=NULL);				//...
	assert(sp->data!=NULL);					//Assertion End
	int i;
	for(i=0; i<sp->length-1; i++){			//free each string pointer
		free(sp->data[i]);
	}
	free(sp->data);							//free pointer to string pointer array
	free(sp->flags);						//free int array pointer
	free(sp);								//free set and non-pointer contents
	return;
}

/* [numElements]
 * Worst Case: O(1) 
 * Best Case: O(1)
 */	
int numElements(SET *sp){					
	assert(sp!=NULL);						//Assertion
	return(sp->count);						//return count
}

/* [addElement]
 * Worst Case: O(n) 
 * Best Case: O(1)
 */	
void addElement(SET *sp, void *elt){
	assert(sp!=NULL);						//Assertion Start
	assert(sp->flags!=NULL);				//...
	assert(sp->data!=NULL);					//...
	assert(elt!=NULL);						//Assertion End
	bool found;
	int idx = searchElement(sp,elt,&found);	//search for elt
	if(found==true){						//if elt found
		return;								//do nothing
	}
	else if(found==false){					//if elt not found 
		sp->data[idx]=malloc(sizeof(void));
		sp->data[idx]=elt;					//add elt to deleted or empty
		sp->flags[idx]=1;					//update flag to FILLED
		sp->count++;						//increment count
		return;								
	}
}

/* [removeElement]
 * Worst Case: O(n) 
 * Best Case: O(1)
 */	
void removeElement(SET *sp, void *elt){
	assert(sp!=NULL);						//Assertion Start
	assert(sp->data!=NULL);					//...
	assert(sp->flags!=NULL);				//...
	assert(elt!=NULL);						//Assertion End
	bool found;
	int idx = searchElement(sp,elt,&found);	//search for elt
	if(found==true){						//if elt found
		sp->data[idx]=NULL;					//free(sp->data) was NOT working!
		sp->flags[idx]=2;					//update flag to deleted
		sp->count--;						//decrement count
		return;
	}
	else{									//if not found
		return;								//do nothing
	}
}

/* [*findElement]
 * Worst Case: O(n) 
 * Best Case: O(1)
 */	
void *findElement(SET *sp, void *elt){
	assert(sp!=NULL);						//Assertion Start
	assert(sp->data!=NULL);					//...
	assert(sp->flags!=NULL);				//...
	assert(elt!=NULL);						//Assertion End
	bool found;
	int idx = searchElement(sp,elt,&found);	//search for elt
	if(found==true){						//if elt found
		return(sp->data[idx]);				//return pointer to location
	}
	else{									//otherwise
		return(NULL);						//return NULL
	}
}

/* [**getElements]
 * Worst Case: O(m) 
 * Best Case: O(m)
 */	
void *getElements(SET *sp){
	assert(sp!=NULL);						//Assertion Start
	assert(sp->data!=NULL);					//...
	assert(sp->flags!=NULL);				//Assertion End
	void **dupData = malloc(sizeof(void*)*sp->count);
	assert(dupData!=NULL);					//Allocation for new data
	int i,j=0;								//i~ m  j~ n
	for(i=0;i<sp->length-1;i++){			//step through data entirely
		if(sp->flags[i]==1){				//if flag is FILLED
			dupData[j]=sp->data[i];			//duplicate data at FILLED spot
			j++;							//increment j
		}
	}
	return(dupData);						//return duplicate array
}









