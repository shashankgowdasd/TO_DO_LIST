/*  Author: Shashank S D
*	Date: 12/02/2025
*	Description: The Below sample program demonstarates the use of linked list to create and delete the to do list for the daily use.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Task structure
typedef struct task
{
    int task_id;
    char *descript;
    char *task_status;  // Use an array instead of a pointer
    struct task *nxt;
} task_t;

// Function to create a new task
task_t *create_task(int id, char *des,char *tsk_status)
{
    task_t *new_task = malloc(sizeof(task_t));
    
    if (new_task == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    new_task->descript = malloc(strlen(des) + 1);  // Allocate memory for description
    if (new_task->descript == NULL)
    {
        printf("Memory allocation failed for description\n");
        free(new_task->descript);
        return NULL;
    }
    
    new_task->task_status = malloc(strlen(tsk_status)+1);
    if(new_task->task_status == NULL)
    {
        printf("Mem alloc failed\n");
        free(new_task->task_status);
        return NULL;
    }

    strcpy(new_task->descript, des);
    strcpy(new_task->task_status, tsk_status);
    new_task->task_id = id;  // Assign task ID
    new_task->nxt = NULL;    // Set next pointer to NULL

    return new_task;
}

bool check_task_presence(task_t *tsk)
{
    int existing_id;
    char line[200];
    
    FILE *fptr = fopen("Task_file.txt","r");

    if(fptr == NULL)
    {
        printf("ERROR: File not present\n");
        return false;
    }
   
    while(fgets(line,sizeof(line),fptr))             // scan every line and then put on to the line buff
    {
        if(sscanf(line,"ID: %d",&existing_id) == 1) // extracting the id from the line
        {
            if(existing_id == tsk->task_id)
            {
                printf("DEBUG: Task ID already present in the file\n");
                fclose(fptr);
                return true;
            }
        }
    }
    
    fclose(fptr);
    return false;
}


// Function to add a task to the list
void add_task(task_t **t, int id, char *des,char *tsk_status)
{
    int existing_id;
    printf("DEBUG: new_task_id : %d\n",id);
    
    task_t *new_task = create_task(id, des, tsk_status);
    
    if (*t == NULL)
    {
        *t = new_task;
    }
    else
    {
        task_t *temp = *t;
        while (temp->nxt != NULL)
        {
            temp = temp->nxt;
        }
        temp->nxt = new_task;
    }
    
    // 1. check the file already present or not
    // 2. if present, then scan the task id and for new_task task_id is already present
    // 3. if the task_id is already present just update the file or create the new task and add the entry to it.
    bool res = check_task_presence(new_task);
    
    if(res == true)
    {
        printf("DEBUG: File already present,So skipping the addition\n");   
    }
    
    FILE *file_check = fopen("Task_file.txt","r");
    
    if(file_check == NULL)
    {
        printf("ERROR: File not present,So creating one\n");
        
        FILE *fptr = fopen("Task_file.txt","w");
        
        if(fptr == NULL)
        {
            printf("ERROR: File not created\n");
            return;
        }
        
        fclose(fptr);
    }
    else
    {
        fclose(file_check);
    }
    
    FILE *append_ptr = fopen("Task_file.txt","a+");
    
    if(append_ptr == NULL)
    {
        printf("ERROR: File is not present\n");
        return;
    }
    
    if(new_task-> task_id == 1)
    {
        struct tm* local; 
        time_t t = time(NULL); 
  
        // Get the localtime 
        local = localtime(&t); 
        fprintf(append_ptr,"Updated Interval: %s\n",asctime(local));
    }
    fprintf(append_ptr,"ID: %d | DESCRIPTION: %s | STATUS: %s\n",new_task->task_id,new_task->descript,new_task->task_status);
    fclose(append_ptr);
}

void delete_tsk(task_t **t,int id)
{
    int i=0,tsk_cnt =0;
    
    task_t *tk = *t;
    while(tk != NULL)
    {
        tsk_cnt++;
        tk = tk->nxt;
    }
    printf("DEBUG: Task count %d\n",tsk_cnt);
    
    if(tsk_cnt <= id-1)
    {
        if(id == 0)
        {
            task_t *ref_tsk = *t;  // referring to the head
            *t = ref_tsk->nxt;
            free(ref_tsk);
        }
        else
        {
            // if the task is present in the middle
            // *head->1->2->3->NULL
            //           |
                         
            printf("DEBUG: Deletion in the middle\n");
            task_t *temp = *t;
            task_t *ref_tsk = NULL;
            while(i != id-1)
            {
                i++;
                temp = temp->nxt;
            }
            printf("DEBUG: Data %d\n",temp->task_id);
            ref_tsk = temp->nxt;   // 2 - task id
            temp->nxt = temp->nxt->nxt;   // pointing 1-> 3
            free(ref_tsk);
        }
    }
    else
    {
        printf("DEBUG: TASKS NOT AVAILABLE TO DELETE\n");
    }
}


// Function to display tasks
void display(task_t *t)
{
    task_t *ref_tsk = t;

    printf("\nTask List:\n");
    while (ref_tsk != NULL)
    {
        printf("ID: %d | Description: %s | Status: %s\n",
               ref_tsk->task_id, ref_tsk->descript, ref_tsk->task_status);
        ref_tsk = ref_tsk->nxt;
    }
}

// Main function
int main()
{
    task_t *t_head = NULL;
    
    add_task(&t_head, 1, "Buying clothes","INITIATED");
    add_task(&t_head, 2, "Drinking water","INITIATED");
    add_task(&t_head, 3, "Prepare for dinner","INITIATED");
    
    add_task(&t_head, 2, "Drinking water","COMPLETED");

    display(t_head);
    
    // delete_tsk(&t_head,3);
    // display(t_head);
    
    return 0;
}
