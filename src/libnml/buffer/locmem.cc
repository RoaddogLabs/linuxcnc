/************************************************************************
File: locmem.cc
Purpose: Implements LOCMEM which is a derived class of CMS that serves primarily
to provide addresses that match when matching buffer names are passed to
the constructor. It is useful in allowing control modules to use the
same inteface to communicate as would be required if they were not
running in the same process even though to use LOCMEM they must be.
**********************************************************************/

#include "locmem.hh"		// class LOCMEM
#include "cms.hh"		// class CMS
#include "linklist.hh"		// class LinkedList
#include "rcs_print.hh"		// rcs_print_error()

extern "C" {

#include <stdlib.h>		// malloc()
#include <string.h>		// strcpy(), strcmp()

} LinkedList *LOCMEM::buffers_list = (LinkedList *) NULL;

LOCMEM::LOCMEM(char *bufline, char *procline, int set_to_server,
    int set_to_master):CMS(bufline, procline, set_to_server)
{
    my_node = (BUFFERS_LIST_NODE *) NULL;
    lm_addr = NULL;

    if (set_to_master == 1) {
	is_local_master = 1;
    }
    if (set_to_master == -1) {
	is_local_master = 0;
    }

    if (is_local_master) {
	if (buffers_list == NULL) {
	    buffers_list = new LinkedList;
	}
	if (buffers_list == NULL) {
	    rcs_print_error("LOCMEM: Can't create buffers_list.\n");
	    status = CMS_CREATE_ERROR;
	    return;
	}
	my_node = new BUFFERS_LIST_NODE;
	lm_addr = my_node->addr = malloc(size);
	if (my_node == NULL || lm_addr == NULL) {
	    rcs_print_error("Can't malloc needed space.\n");
	    status = CMS_CREATE_ERROR;
	    return;
	}
	my_node->size = size;
	strcpy(my_node->name, BufferName);
	memset(my_node->addr, 0, size);
	buffer_id = buffers_list->storeAtTail(my_node, sizeof(my_node), 0);
	return;
    }

    if (buffers_list == NULL) {
	rcs_print_error("LOCMEM: buffers_list is NULL.\n");
	status = CMS_NO_MASTER_ERROR;
	return;
    }

    /* Search for a matching buffer name. */
    my_node = (BUFFERS_LIST_NODE *) buffers_list->getHead();
    while (my_node != NULL) {
	if (!strcmp(BufferName, my_node->name)) {
	    /* Found it!!! */
	    if (my_node->size != size) {
		rcs_print_error("LOCMEM - size mismatch for buffer %s.\n",
		    BufferName);
		status = CMS_CONFIG_ERROR;
		return;
	    }
	    buffer_id = buffers_list->getCurrentId();
	    lm_addr = my_node->addr;
	    return;
	}
	my_node = (BUFFERS_LIST_NODE *) buffers_list->getNext();
    }
    rcs_print_error("LOCMEM: buffer not found on buffers_list.\n");
    status = CMS_NO_MASTER_ERROR;
    return;

}

LOCMEM::~LOCMEM()
{
    if (NULL != buffers_list) {
	buffers_list->deleteNode(buffer_id);
	if (0 == buffers_list->listSize) {
	    delete buffers_list;
	    buffers_list = (LinkedList *) NULL;
	}
    }
}

CMS_STATUS LOCMEM::main_access(void *local_address)
{
    internal_access(lm_addr, size, local_address);
    return status;
}
