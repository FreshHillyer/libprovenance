/*
*
* Author: Thomas Pasquier <tfjmp2@cam.ac.uk>
*
* Copyright (C) 2015 University of Cambridge
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2, as
* published by the Free Software Foundation.
*
*/
#ifndef __PROVENANCELIB_H
#define __PROVENANCELIB_H


#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/provenance.h>

struct provenance_ops{
  void (*init)(void);
  bool (*filter)(prov_msg_t* msg);
  bool (*long_filter)(long_prov_msg_t* msg);
  void (*log_edge)(struct edge_struct*);
  void (*log_task)(struct task_prov_struct*);
  void (*log_inode)(struct inode_prov_struct*);
  void (*log_str)(struct str_struct*);
  void (*log_disc)(struct disc_node_struct*);
  void (*log_msg)(struct msg_msg_struct*);
  void (*log_shm)(struct shm_struct*);
  void (*log_sock)(struct sock_struct*);
  void (*log_address)(struct address_struct*);
  void (*log_file_name)(struct file_name_struct*);
  void (*log_ifc)(struct ifc_context_struct*);
};

void prov_record(prov_msg_t* msg);
void long_prov_record(long_prov_msg_t* msg);

/*
* Function return boolean value corresponding to the presence or not of the
* provenance module in the kernel.
*/
bool provenance_is_present(void);

/* provenance usher functions */

/*
* @ops structure containing audit callbacks
* start and register callback. Note that there is no concurrency guarantee made.
* The application developper is expected to deal with concurrency issue.
*/
int provenance_register(struct provenance_ops* ops);

/*
* shutdown tightly the things that are running behind the scene.
*/
void provenance_stop(void);

/* security file manipulation */

/*
* @v boolean value
* enable or disable provenance data capture depending on the value of v. Will
* fail if the current process is not root.
*/
int provenance_set_enable(bool v);

/*
* return either or not the provenance capture is active.
*/
bool provenance_get_enable( void );

/*
* @v boolean value
* activate provenance on all kernel objects. WARNING the computer may slow down
* dramatically and the amount of data generated may be excessively large. Will
* fail if current process is not root.
*/
int provenance_set_all(bool v);

/*
* return either or not provenance on all kernel object is active.
*/
bool provenance_get_all( void );

/*
* @filter pointer to contain filter to read
* read the current state of the node filter.
*/
int provenance_get_node_filter( uint32_t* filter );

/*
* @filter value of node filter
* set node provenance capture filter.
*/
int provenance_add_node_filter( uint32_t filter );
int provenance_remove_node_filter( uint32_t filter );

/*
* @filter pointer to contain filter to read
* read the current state of the edge filter.
*/
int provenance_get_edge_filter( uint32_t* filter );

/*
* @filter value of node filter
* set edge provenance capture filter.
*/
int provenance_add_edge_filter( uint32_t filter );
int provenance_remove_edge_filter( uint32_t filter );

/*
* @v boolean value
* Hide the current process from provenance capture. Should be mostly used by the
* provenance capture service itself. Will fail if the current process is not
* root.
*/
int provenance_set_opaque(bool v);

/*
* @v boolean value
* Request the current process to be part of the provenance record (even if 'all'
* is not set).
*/
int provenance_set_tracked(bool v);

/*
* @v uint32_t value
* Assign an ID to the current machine. Will fail if the current process is not
* root.
*/
int provenance_set_machine_id(uint32_t v);

/*
* @v pointer to uint32_t value
* Read the machine ID corresponding to the current machine.
*/
int provenance_get_machine_id(uint32_t* v);

/*
* @node node data structure to be recorded
* API to dsiclose a provenance node. Some values should be left blank and Will
* be updated by the kernel.
*/
int provenance_disclose_node(struct disc_node_struct* node);

/*
* @edge edge data structure to be recorded
* API to dsiclose a provenance edge. Some values should be left blank and Will
* be updated by the kernel.
*/
int provenance_disclose_edge(struct edge_struct* edge);

/*
* @self point to a node data structure
* self if filled with the provenance information corresponding to the current
* process.
*/
int provenance_self(struct task_prov_struct* self);

#endif /* __PROVENANCELIB_H */
