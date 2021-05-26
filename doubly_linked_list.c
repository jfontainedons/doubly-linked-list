/* 
 * Implement a sorted linked list of strings with operations Insert
 * in alphabetical order, Print, Member, Delete, Free_list.
 * The list nodes are doubly linked.
 *
 * Input:    Single character lower case letters to indicate operations,
 *           possibly followed by value needed by operation -- e.g. 'i'
 *           followed by "hello" to insert the string "hello" -- no
 *           double or single quotes.
 *
 * Output:   Results of operations.
 *
 * Compile:  gcc -g -Wall -o doubly_linked_list doubly_linked_list.c
 *
 * Run:      ./doubly_linked_list
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Max 99 chars + '\0' */
#define STRING_MAX 100

struct list_node_s {
   char*  data;
   struct list_node_s* prev_p;
   struct list_node_s* next_p;
};

/* Pointers to the head and tail of the list */
struct list_s {
   struct list_node_s* h_p;
   struct list_node_s* t_p;
};

void Insert(struct list_s* list_p, char string[]);
void Print(struct list_s* list_p);
int  Member(struct list_s* list_p, char string[]);
void Delete(struct list_s* list_p, char string[]);
void Free_list(struct list_s* list_p);
char Get_command(void);
void Get_string(char string[]);
void Free_node(struct list_node_s* node_p);
void ReverseMyList(struct list_s* list_p);
struct list_node_s* Allocate_node(int size);
void Print_node(char title[], struct list_node_s* node_p);


/*-----------------------------------------------------------------*/
int main(void) {
   char          command;
   char          string[STRING_MAX];
   struct list_s list;

   /* Empty list has NULL h_p, t_p pointers */
   list.h_p = list.t_p = NULL;

   command = Get_command();
   while (command != 'q' && command != 'Q') {
      switch (command) {
         case 'i':
         case 'I':
            Get_string(string);
            Insert(&list, string);
            break;
         case 'p':
         case 'P':
            Print(&list);
            break;
         case 'm':
         case 'M':
            Get_string(string);
            if (Member(&list, string))
               printf("%s is in the list\n", string);
            else
               printf("%s is not in the list\n", string);
            break;
         case 'd':
         case 'D':
            Get_string(string);
            Delete(&list, string);
            break;
         case 'f':
         case 'F':
            Free_list(&list);
            break;
         case 'r':
         case 'R':
            ReverseMyList(&list);
            break;
         default:
            printf("There is no %c command\n", command);
            printf("Please try again\n");
      }
      command = Get_command();
   }
   Free_list(&list);

   return 0;
}  /* main */


/*-----------------------------------------------------------------*/
/* Function:   Allocate_node
 * Purpose:    Allocate storage for a list node
 * Input arg:  size = number of chars needed in data member (including
 *                storage for the terminating null)
 * Return val: Pointer to the new node
 */
struct list_node_s* Allocate_node(int size) {
  struct list_node_s* temp_p;
  temp_p = (struct list_node_s*) malloc(sizeof(struct list_node_s));
  /* Allocate memory. Include room for the terminating null character. */
  temp_p->data = (char*) malloc((size + 1) * sizeof(char));
  return temp_p;
} 


/* Reverse the linked list. You can assume every list has at least 3 elements. */
void ReverseMyList(struct list_s* list_p) {
  struct list_node_s* head = list_p->h_p;
  struct list_node_s* tail = list_p->t_p;

  struct list_node_s* prev = NULL;
  struct list_node_s* current = head;
  struct list_node_s* next = NULL;
  while (current != NULL) {
      next = current->next_p;

      /* Reverse current node's pointer. */
      current->next_p = prev;

      /* Move pointers one position ahead. */
      prev = current;
      current = next;
  }
  list_p->h_p = prev;
}


/*-----------------------------------------------------------------*/
/* Function:   Insert
 * Purpose:    Insert new node in correct alphabetical location in list
 * Input arg:  string = new string to be added to list
 * In/out arg: list_p = pointer to struct storing head and tail ptrs
 * Note:       If the string is already in the list, print a message
 *                and return, leaving list unchanged
 */
void Insert(struct list_s* list_p, char string[]) {
  struct list_node_s* new_node = list_p->h_p;
  int len = (int) strlen(string) + 1;
  new_node = Allocate_node(len);
  strcpy(new_node->data, string);
  new_node->next_p = NULL;
  new_node->prev_p = NULL;

  /* Case 1: List is empty. */
  if (list_p->h_p == NULL) {
    list_p->h_p = new_node;
    list_p->t_p = new_node;
    return;
  } else {
    /* Case 2: List is not empty. */
    struct list_node_s* temp = list_p->h_p;
    struct list_node_s* curr = NULL;
    while (temp != NULL) {
      if (strcmp(temp->data, new_node->data) == 0) {
        free(new_node);
        return;
      }
      if (strcmp(temp->data, new_node->data) > 0) {
        break;
      }
      curr = temp;
      temp = temp->next_p;
    }

    if (curr == NULL) {
      new_node->next_p = temp;
      temp->prev_p = new_node;
      list_p->h_p = new_node;
    } else if (temp == NULL){
      curr->next_p = new_node;
      new_node->prev_p = curr;
      list_p->t_p = new_node;
    } else {
      curr->next_p = new_node;
      temp->prev_p = new_node;
      new_node->prev_p = curr;
      new_node->next_p = temp;
    }
  }
}   

/*-------------------// ----------------------------------------------*/
/* Function:   Print
 * Purpose:    Print the contents of the nodes in the list
 * Input arg:  list_p = pointers to first and last nodes in list
 */
void Print(struct list_s* list_p) {
   struct list_node_s* curr_p = list_p->h_p;

   printf("list = ");

   while (curr_p != NULL) {
      printf("%s ", curr_p->data);
      curr_p = curr_p->next_p;
   }
   printf("\n");
}  


/*-----------------------------------------------------------------*/
/* Function:   Member
 * Purpose:    Search list for string
 * Input args: string = string to search for
 *             list_p = pointers to first and last nodes in list
 * Return val: 1, if string is in the list, 0 otherwise
 */
int Member(struct list_s* list_p, char string[]) {
  struct list_node_s* current_p;
  current_p = list_p->h_p;                                            

  while(current_p != NULL) {
    if(strcmp(current_p->data, string) == 0) {
      return 1;
    }
    current_p = current_p->next_p;
  }
   return 0;
} 


/*-----------------------------------------------------------------*/
/* Function:   Free_node
 * Purpose:    Free storage used by a node of the list
 * In/out arg: node_p = pointer to node to be freed
 */
void Free_node(struct list_node_s* node_p) {
   free(node_p->data);
   free(node_p);
} 


/*-----------------------------------------------------------------*/
/* Function:   Delete
 * Purpose:    Delete node containing string.
 * Input arg:  string = string to be deleted
 * In/out arg  list_p = pointers to head and tail of list
 * Note:       If the string is in the list, it will be unique.  So
 *             at most one node will be deleted.  If the string isn't
 *             in the list, the function just prints a message and
 *             returns, leaving the list unchanged.
 */
void Delete(struct list_s* list_p, char string[]) {
  struct list_node_s* temp_p = list_p->h_p;
  struct list_node_s* curr_p = NULL;

  while (temp_p != NULL) {
    if(strcmp(string, temp_p->data) == 0) {
      break;
    }
    curr_p = temp_p;
    temp_p = temp_p->next_p;
  }

  /* Case 1: List is empty. */
  if (temp_p == NULL) {
    return;
  } else {
    /* Case 2: List is not empty. */
    if (list_p->h_p->next_p == NULL) {
      list_p->h_p = NULL;
      list_p->t_p = NULL;
    } else if (curr_p == NULL) {
      list_p->h_p = temp_p->next_p;
      list_p->h_p->prev_p = NULL;
    } else if (temp_p->next_p == NULL) {
      list_p->t_p = temp_p->prev_p;
      list_p->t_p->next_p = NULL;
    } else {
      curr_p->next_p = temp_p->next_p;
      temp_p->next_p->prev_p = curr_p;
    }

    Free_node(temp_p);
  }
}  

/*-----------------------------------------------------------------*/
/* Function:   Free_list
 * Purpose:    Free storage used by list
 * In/out arg: list_p = pointers to head and tail of list
 */
void Free_list(struct list_s* list_p) {
  struct list_node_s* current_p;
  current_p = list_p->h_p;                                            
  struct list_node_s* temp_p;                                         

  if (current_p == NULL) {
    return;
  } else {
    while(current_p != NULL) {
      temp_p = current_p->next_p;
      Free_node(current_p);
      current_p = temp_p;
    }
  }
}  /* Free_list */


/*-----------------------------------------------------------------*/
/* Function:   Get_command
 * Purpose:    Find and return the next non-whitespace character in
 *             the input stream
 * Return val: The next non-whitespace character in stdin
 */
char Get_command(void) {
   char c;

   printf("Please enter a command (i, d, m, p, f, q):  ");
   /* Put the space before the %c so scanf will skip white space */
   scanf(" %c", &c);
   return c;
}  

/*-----------------------------------------------------------------*/
/* Function:   Get_string
 * Purpose:    Read the next string in stdin (delimited by whitespace)
 * Out arg:    string = next string in stdin
 */
void Get_string(char string[]) {

   printf("Please enter a string:  ");
   scanf("%s", string);
}  


/*-----------------------------------------------------------------*/
/* Function:  Print_node
 * Purpose:   Print the data member in a node or NULL if the
 *            pointer is NULL
 * In args:   title:  name of the node
 *            node_p:  pointer to node
 */
void Print_node(char title[], struct list_node_s* node_p) {           
   printf("%s = ", title);
   if (node_p != NULL)
      printf("%s\n", node_p->data);
   else
      printf("NULL\n");
}  
