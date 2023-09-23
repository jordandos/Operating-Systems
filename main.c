#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main() {
  int x;
  printf("Write your Tests for your linked list implementation\n");
  list_t *mylist;
  mylist = list_alloc();
	//test list_print & list_length
  list_print(mylist);
	printf("Length of list %d\n", list_length(mylist));
  list_add_to_front(mylist, 1);
  list_add_to_front(mylist, 2);
  list_add_to_front(mylist, 3);
  list_print(mylist);
	printf("Length of list %d\n", list_length(mylist));
  list_add_to_front(mylist, 4);
  list_add_to_front(mylist, 5);
  list_add_to_front(mylist, 6);
  list_add_to_front(mylist, 7);
  list_add_to_front(mylist, 8);
  list_add_to_front(mylist, 9);
	//test add to front
	list_print(mylist);
	printf("Length of list %d\n", list_length(mylist));
	if(mylist->head->value != 100)
  {
        printf("FAILED: List head should be 100\n");
  }
	else{
		printf("SUCCESS\n");
	}
  list_add_to_front(mylist, 100);
	list_print(mylist);
	printf("Length of list %d\n", list_length(mylist));
  if(mylist->head->value != 100)
  {
        printf("FAILED: List head should be 100\n");
  }
	else{
		printf("SUCCESS\n");
	}
	
	//test for add back
	list_print(mylist);
	list_add_to_back(mylist, 12);
	list_print(mylist);
	list_add_to_back(mylist, 22);
	list_add_to_back(mylist, 82);
	list_add_to_back(mylist, 62);
	list_add_to_back(mylist, 62);
	list_add_to_back(mylist, 62);
	list_add_to_back(mylist, 62);
	list_print(mylist);

	//test for add at index
	list_add_at_index(mylist, 33, 5);
	list_print(mylist);
	list_add_at_index(mylist, 34, 1);
	list_print(mylist);

	//test for remove from list
	if(list_remove_from_back(mylist) == 62){
		printf("Removed last element\n");
	}
	list_print(mylist);
	printf("Removing value %d at index 12\n", list_remove_at_index(mylist, 12));
	list_print(mylist);
	printf("Removing head value: %d\n", list_remove_from_front(mylist));
	list_print(mylist);
	
	//test if element is in list
	int val = 36;
	if(list_is_in(mylist, val) == true){
		printf("%d is within the list\n", val);
	}
	else{
		printf("%d is not within the list\n", val);
	}
	val = 62;
	if(list_is_in(mylist, val) == true){
		printf("%d is within the list\n", val);
	}
	else{
		printf("%d is not within the list\n", val);
	}
	val = 4;
	if(list_is_in(mylist, val) == true){
		printf("%d is within the list\n", val);
	}
	else{
		printf("%d is not within the list\n", val);
	}
	//test for finding element from index
	int index = 1;
	printf("Value at index %d: %d\n", index, list_get_elem_at(mylist, index));
	index = 10;
	printf("Value at index %d: %d\n", index, list_get_elem_at(mylist, index));
	index = 100;
	printf("Value at index %d: %d\n", index, list_get_elem_at(mylist, index));
	//test getting index from value
	int num = 3;
	printf("Value %d is at index: %d\n", num, list_get_index_of(mylist,num));
	num = 2;
	printf("Value %d is at index: %d\n", num, list_get_index_of(mylist,num));
	num = 4;
	printf("Value %d is at index: %d\n", num, list_get_index_of(mylist,num));


  return 0;
}
