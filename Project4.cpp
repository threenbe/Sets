/*
 * Project4.c 
 *
 * Raiyan Chowdhury
 * EE312 T/Th 3:30-5:00
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Set.h"

/*
 * Several of the required functions have been written already
 * You may use these functions as-is or you may modify them in any way. 
 * Please NOTE, you are responsible for ensuring that the functions work correctly in 
 * your project solution. Just because the function works correctly in my solution does 
 * NOT mean that the function will work correctly in yours. 
 */

/*
 * Design NOTES:
 *
 * The design provided in this starter kit assumes
 * (1) empty sets will be represented with length == 0 and elements == nullptr (i.e., address 0)
 * (2) amortized doubling is not used, and capacity is ignored/unused. Functions should assume that 
 * the amount of storage available in the elements[] array is equal to length
 */



/* done for you already */
void destroySet(Set* self) {
    free(self->elements);
}

/* done for you already */
void createEmptySet(Set* self) {
    self->len = 0;
    self->elements = 0;
}

/* done for you already */
void createSingletonSet(Set* self, int x) {
    self->elements = (int*) malloc(sizeof(int));
    self->elements[0] = x;
    self->len = 1;
}

/* done for you already */
void createCopySet(Set* self, const Set* other) {
    self->elements = (int*) malloc(other->len * sizeof(int));
    for (int k = 0; k < other->len; k += 1) {
        self->elements[k] = other->elements[k];
    }
    self->len = other->len; 
}

/* done for you already */
void assignSet(Set* self, const Set* other) {
    if (self == other) { return; }
    
    destroySet(self);
    createCopySet(self, other);
}

/***********************isMemberSet***********************
 * returns true if x is an element of self
 * [INPUTS] pointer to self array, variable being searched for
 * [OUTPUTS] boolean true or false
 */
bool isMemberSet(const Set* self, int x) {
    if (self->len == 0) { return false; }
    int mid = 0, start = 0;
    int end = self->len - 1;

    //does a binary search to find x
    while (start <= end) {
        mid = (start + end) / 2;

        if (x == self->elements[mid]) { return true; }

        if (x < self->elements[mid]) { end = mid - 1; }
        else { start = mid + 1; }
    }

    return false;
}

/***********************insertSet***********************
 * add x as a new member to this set. 
 * If x is already a member, then self should not be changed
 * Be sure to restore the design invariant property that elements[] remains sorted
 * (yes, you can assume it is sorted when the function is called, that's what an invariant is all about)
 * [INPUTS] pointer to self, x value to be added to self
 * [OUTPUTS] none
 */
void insertSet(Set* self, int x) {
    //don't bother doing anything if x is already there
    if(isMemberSet(self, x)) { return; }

    self->len++;
    self->elements = (int*) realloc(self->elements, self->len * sizeof(int));
    int i;

    //the loop below starts from the highest value and keeps moving
    //elements up until it finds the spot at which x can be placed
    for (i = (self->len - 1); i > 0; i--) {
        if (self->elements[i-1] < x) { 
            self->elements[i] = x; 
            return; 
        }
        else { 
            self->elements[i] = self->elements[i-1];
        }
    }
    if (i == 0) { self->elements[i] = x; }
}


/***********************removeSet***********************
 * removes an element from the set
 * don't forget: it is OK to try to remove an element
 * that is NOT in the set.  
 * If 'x' is not in the set 'self', then
 * removeSet should do nothing (it's not an error)
 * Otherwise, ('x' IS in the set), remove x. Be sure to update self->length
 * It is not necessary (nor recommended) to call malloc -- if removing an element means the 
 * array on the heap is "too big", that's almost certainly OK, and reallocating a smaller array 
 * is almost definitely NOT worth the trouble
 * [INPUTS] pointer to self, element to be removed
 * [OUTPUTS] none
 */
void removeSet(Set* self, int x) {
    if (self->len == 0) { return; }
    int mid = 0, start = 0;
    int end = self->len - 1;

    //the code bellow executes a binary search to find the element x
    //when found, it is removed and every element following it is moved back by one place
    //if it is not found then the function simply returns and nothing changes
    while (start <= end) {
        mid = (start + end) / 2;

        if (x == self->elements[mid]) {
            for(int i = mid; i < (self->len - 1); i++){
                self->elements[i] = self->elements[i+1];
            }
            self->len--;
            return;
        }

        if (x < self->elements[mid]) { end = mid - 1; }
        else { start = mid + 1; }
    }
}

/* done for you already */
void displaySet(const Set* self) {
    int k;
    
    printf("{");

    if (self->len == 0) { 
        printf("}"); 
    }
    else {
        for (k = 0; k < self->len; k += 1) {
            if (k < self->len - 1) {
                printf("%d,", self->elements[k]);
            } else {
                printf("%d}", self->elements[k]);
            }
        }
    }
}

/***********************isEqualToSet***********************
 * returns true if self and other have exactly the same elements
 * [INPUTS] pointers to self and other
 * [OUTPUTS] true or false bool value
 */
bool isEqualToSet(const Set* self, const Set* other) {
    //not equal if they're different sizes
    if (self->len != other->len) { return false; }

    //return false as soon as a discrepancy is found
    for (int i = 0; i < self->len; i++) {
        if (self->elements[i] != other->elements[i]) { return false; }
    }
    //if we reach here then the arrays are equal
    return true;
}

/***********************isSubsetOf***********************
 * returns true if every element of self is also an element of other 
 * [INPUTS] pointers to self and other
 * [OUTPUTS] true of false bool value
 */
bool isSubsetOf(const Set* self, const Set* other) {
    //if self is bigger than other then it can't possible be a subset of other
    if (self->len > other->len) { return false; }

    //if the index j ever reaches the size of other->len in the loop below,
    //then that means that whichever element in self that was being searched
    //for could not be found anywhere, hence the function returns false
    int i = 0, j = 0;
    for (; i < self->len; i++) {
        for (; j < other->len; j++) {
            if (self->elements[i] == other->elements[j]) { break; }
        }
        if (j == other->len) { return false; }
    }

    //if we get here then every element in self was located in other
    return true;
}

/* done for you */
bool isEmptySet(const Set* self) {
    return self->len == 0;
}

/***********************intersectFromSet***********************
 * removes all elements from self that are not also elements of other 
 * [INPUTS] pointers to self and other
 * [OUTPUTS] none
 */
void intersectFromSet(Set* self, const Set* other) {
    int i = 0, j = 0;
    int removed = 0;
    int len = self->len;

    while (i < self->len && j < other->len) {

        //increment both indices to continue checking the rest
        //also, if an element(s) had already been removed from self, the third line pushes the next element back
        //(it is done this way to avoid moving each element in self more than once, which is more efficient,)
        if (self->elements[i] == other->elements[j]) {
            i++;
            j++;
            //this if statement is in place to ensure that no invalid read/writes occur outside the malloc'd space
            if ((i+removed) < len) { self->elements[i] = self->elements[i+removed]; }
        }

        //if the current element in self > the current element in other, just increment the 
        //j index as a later element in other may match the current element in self
        else if (self->elements[i] > other->elements[j]) {
            j++;
        }

        //if the current element in other > the current element in self and this condition is triggered,
        //then the current element in other was not found in self, so the current element in self is removed
        else if (self->elements[i] < other->elements[j]) {
            removed++;
            self->len--;
            if ((i+removed) < len) { self->elements[i] = self->elements[i+removed]; }
        }
    }

    //if j hits the length of other->len first, then the rest of the elements in self are to be removed
    if (i < self->len) {
        self->len = i;
    }
}

/***********************subtractFromSet***********************
 * removes all elements from self that are also elements of other 
 * [INPUTS] pointers to self and other
 * [OUTPUTS] none
 */
void subtractFromSet(Set* self, const Set* other) {
    int i = 0, j = 0;
    int removed = 0;
    int len = self->len;

    while (i < self->len && j < other->len) {
        if (self->elements[i] == other->elements[j]) {
            removed++;
            self->len--;
            if ((i+removed) < len) { self->elements[i] = self->elements[i+removed]; }
            j++;
        }

        //increment j to continue looking for elements that match the current element in self
        else if (self->elements[i] > other->elements[j]) {
            j++;
        }

        //increment i, since at this point no matches for the current element in self will be found
        //also, if any elements have been removed already, move the next element back the appropriate
        //number of spaces so that it can be examined
        else if (self->elements[i] < other->elements[j]) {
            i++;
            if ((i+removed) < len) { self->elements[i] = self->elements[i+removed]; }
        }
    }

    if (i < self->len) {
        for (; i < self->len; i++) {
            if ((i+removed) < len) self->elements[i] = self->elements[i+removed];
        }
    }

}

/***********************unionInSet***********************
 * add all elements of other to self (obviously, without creating duplicate elements) 
 * [INPUTS] pointers to self and other
 * [OUTPUTS] none
 */
void unionInSet(Set* self, const Set* other) {
    //the malloc'd space declared here will hold the result, as this makes it easier
    //to sort the elements in an time-efficient manner
    int* union_self = (int*) malloc((self->len+other->len) * sizeof(int));
    int union_len = 0;
    int i = 0, j = 0;

    //the following loop adds all elements from self and other onto the malloc'd space
    //declared above in order
    while (i < self->len && j < other->len) {
        if (self->elements[i] == other->elements[j]) {
            union_self[union_len] = self->elements[i];
            union_len++;
            i++;
            j++;
        }
        else if (self->elements[i] < other->elements[j]) {
            union_self[union_len] = self->elements[i];
            union_len++;
            i++;
        }
        else if (self->elements[i] > other->elements[j]) {
            union_self[union_len] = other->elements[j];
            union_len++;
            j++;
        }
    }

    if (i < self->len) {
        for (; i < self->len; i++) {
            union_self[union_len] = self->elements[i];
            union_len++;
        }
    }
    else if (j < other->len) {
        for (; j < other->len; j++) {
            union_self[union_len] = other->elements[j];
            union_len++;
        }
    }

    //the original self set is freed and it now points to the new space malloc'd above
    destroySet(self);
    self->elements = union_self;
    self->len = union_len;
}
