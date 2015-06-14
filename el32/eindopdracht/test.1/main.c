
#include <stdio.h>      // for stderr
#include <stdlib.h>     // for exit_succes
#include <libusb-1.0/libusb.h>


struct book 
{
	int	number;
	char *title;
};

int book_list(struct book *, int);

int
main ()
{
	int num_books = 3;
	int i;
	struct book *list = malloc(sizeof(*list) * num_books);
	
	for(i=0; i < num_books; i++)
	{
		list[i].number = i;
		list[i].title = "hallo";
	}
	
	book_list(list, num_books);
	
	free(list);

	libusb_device_handle *dh;

	printf("Size = %d\n",sizeof(dh) );	
	return (EXIT_SUCCESS);
}


int book_list(struct book *list, int num_books)
{
	int i;
	for(i=0; i < num_books; ++i)
		printf("[BOOK LIST] number %d title %s\n", list[i].number, list[i].title);
		
	i  = 0;
	struct book *b;
	while ((b = list[i++]) != NULL) 
		printf("[BOOK LIST] number %d title %s\n", b->number, b->title);
		
	return (EXIT_SUCCESS);
}
