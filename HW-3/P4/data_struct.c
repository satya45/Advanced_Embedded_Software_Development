/*Author:Satya Mehta
 * Working of kernel data structure referred from 
 * https://isis.poly.edu/kulesh/stuff/src/klist
 *Some part of time calculation(delta_t) function referred
 * from Real-Time Embedded System course
 * @brief: This code provides information on how to use
 * Linux Kernel Data Structure.
 * I have used Linked List for this application
 * 
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sort.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/time.h>

struct anim{
	char *name;
	int count;
	struct list_head mylist;
};

struct anim1{
	
	char *name1;
	int count1;
	struct list_head mylist1;
};


struct timeval finish = {0, 0};

unsigned long count_data=0, count_final=0, count_free=0;
struct anim a;
struct anim1 a1;
struct anim *new, *new1;
struct anim1 *final;
struct timeval tstart, tend;


char *param1 = NULL;
unsigned long param2=0;

char *animal[100] = {"Frog", "Elephant", "Tiger", "Cat", "Dog", "Cat", "Dog", "Alligator", "Dog", "Crocodile", "Monkey", "Puma", "Fish", "Hippo", "Lion", "Zebra", "Leopard", "Snake", "Fish",
					"Frog", "Racoon", "Cat", "Dog", "Elephant", "Hyenas", "Penguin", "Bear", "Monkey", "Lion", "Tiger", "Fox", "Snake", "Gorilla", "Kite", "Squirrel", "Rat",
					"Coiote", "Wolf", "Chimpanzee", "Crab", "Monkey", "Dog", "Tiger", "Zebra", "Elephant", "Fish", "Cat", "Bat", "Giraffe", "Jackal"
				};
static int size = 50;//sizeof(animal)/sizeof(animal[0]); Initialized with 50 may change when passed config file in parameters.
int i,j, r;
static int cmp(const void *p1, const void *p2)
{
	return strcmp(*(char * const *)p1, *(char * const*)p2);
}


int delta_t(struct timeval *stop, struct timeval *start, struct timeval *delta_t)
{
  int dt_sec=stop->tv_sec - start->tv_sec;
  int dt_usec=stop->tv_usec - start->tv_usec;

  if(dt_sec >= 0)
  {
    if(dt_usec >= 0)
    {
      delta_t->tv_sec=dt_sec;
      delta_t->tv_usec=dt_usec;
    }
    else
    {
      delta_t->tv_sec=dt_sec-1;
      delta_t->tv_usec=1000000+dt_usec;
    }
  }
  return(1);
}

static int satya_init(void)
{
	do_gettimeofday(&tstart);
	INIT_LIST_HEAD((&a.mylist));
	INIT_LIST_HEAD((&a1.mylist1));
	printk("Data Structure module\r\n");
	sort(animal, size, sizeof(animal[0]), cmp, NULL);
	for(i = 0; i<size; i++)
	{
		printk("Sorted Array: %s\n", animal[i]);
	}
	
	new1 = kmalloc(sizeof(struct anim), GFP_KERNEL);
	count_data++;
	INIT_LIST_HEAD(&(new1->mylist));
	new1->name = animal[0];
	new1->count = 1;
	new = new1;
	list_add_tail(&new->mylist, &a.mylist);
	for(j = 1; j < size; j++)
	{
		if(new->name == animal[j])
		{
			new->count++;
		}
		else
		{	
		new = kmalloc(sizeof(struct anim), GFP_KERNEL);
		count_data++;
		INIT_LIST_HEAD(&(new->mylist));
		new->name = animal[j];
		new->count = 1;
		list_add_tail(&new->mylist, &a.mylist);
	    }
	} 
	printk("Number of Nodes in First linked list %ld", count_data);
	printk("Size of memory allocated for first linked list: %ld\n", count_data * sizeof(struct anim));
	list_for_each_entry(new, &(a.mylist), mylist)
	printk("%s and its count %d\n", new->name, new->count);
	if(param1==NULL && param2 == 0)
		{
			printk("No Paramters so only first linked list will be printed");
			list_for_each_entry(new, &(a.mylist), mylist)
			printk("%s and its count %d\n", new->name, new->count);
		}
	else if((param1==NULL && param2>0) || (param1!=NULL && param2 == 0 ))
		{
			if(param1==NULL && param2>0)
			{
				list_for_each_entry(new, &(a.mylist), mylist)
				{
					if(param2 < new->count)
					{
						final = kmalloc(sizeof(struct anim1), GFP_KERNEL);
						count_final++;
						INIT_LIST_HEAD(&(final->mylist1));
						final->name1 = new->name;
						final->count1 = new->count;
						list_add_tail(&final->mylist1, &a1.mylist1);	
					}			
				}
			}
			
			if(param1!=NULL && param2 == 0)
			{
				list_for_each_entry(new, &(a.mylist), mylist)
				{
					if(strcmp(param1, new->name)==0)
					{
						final = kmalloc(sizeof(struct anim1), GFP_KERNEL);
						count_final++;
						printk("Count %ld\n",count_final); 
						INIT_LIST_HEAD(&(final->mylist1));
						final->name1 = new->name;
						final->count1 = new->count;
						list_add_tail(&final->mylist1, &a1.mylist1);	
					}	
				}	
			}
			list_for_each_entry(final, &(a1.mylist1), mylist1)
			printk("Final List: %s and count %d\n",final->name1,final->count1); 
			printk("Size of memory allocate for second linked list: %ld\n", count_final* sizeof(struct anim));		
		}
		else
		{
			list_for_each_entry(new, &(a.mylist), mylist)
			{
				if((strcmp(param1, new->name)==0) || param2 < new->count)
				{
					final = kmalloc(sizeof(struct anim1), GFP_KERNEL);
					count_final++;
					INIT_LIST_HEAD(&(final->mylist1));
					final->name1 = new->name;
					final->count1 = new->count;
					list_add_tail(&final->mylist1, &a1.mylist1);						
				}				
			}
			list_for_each_entry(final, &(a1.mylist1), mylist1)
			printk("Final List: %s and count %d\n",final->name1,final->count1); 
			printk("Number of Nodes in second linked list %ld", count_final);
			printk("Size of memory allocated for second linked list: %ld\n", count_final* sizeof(struct anim));
			if(count_final == 0)
			{
				printk("No matches found\n");
			}
		}
	
	do_gettimeofday(&tend);
	delta_t(&tend, &tstart, &finish);
	printk("Time taken to insert module %ld sec, %ld miliseconds (%ld microsec),\n\n",finish.tv_sec,(finish.tv_usec/1000), (finish.tv_usec));
	return 0;
}

static void satya_exit(void)
{
	do_gettimeofday(&tstart);
	printk("Module removed\r\n");
	list_for_each_entry(new, &(a.mylist), mylist)
	{
	count_free++;
	kfree(new);
	}
	list_for_each_entry(final, &(a1.mylist1), mylist1)
	{
	count_free++;
	kfree(final);
	}
	printk("Amount of memory freed %ld:\n", count_free*sizeof(struct anim));
	do_gettimeofday(&tend);
	delta_t(&tend, &tstart, &finish);
	printk("Time taken to exit module %ld sec, %ld miliseconds (%ld microsec),\n\n",finish.tv_sec,(finish.tv_usec/1000), (finish.tv_usec));
		
}

module_init(satya_init);
module_exit(satya_exit);
module_param(param1,charp, 0);
module_param(param2, long, 0);
module_param_array(animal, charp, &size, 0);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Satya Mehta");
MODULE_DESCRIPTION("Data Structure Module-Animals");
MODULE_VERSION("1.0");
 
