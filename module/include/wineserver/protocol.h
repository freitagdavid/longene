/*
 * protocol.h
 *
 * Copyright (C) 2006  Insigma Co., Ltd
 *
 * This software has been developed while working on the Linux Unified Kernel
 * project (http://www.longene.org) in the Insigma Research Institute,  
 * which is a subdivision of Insigma Co., Ltd (http://www.insigma.com.cn).
 * 
 * The project is sponsored by Insigma Co., Ltd.
 *
 * The authors can be reached at linux@insigma.com.cn.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of  the GNU General  Public License as published by the
 * Free Software Foundation; either version 2 of the  License, or (at your
 * option) any later version.
 *
 * Revision History:
 *   Mar 2009 - Created.
 */

/* 
 * protocol.h:
 * Refered to Wine code
 */
#ifndef _WINESERVER_UK_PROTOCOL_H
#define _WINESERVER_UK_PROTOCOL_H

#include "win32.h"

#ifdef CONFIG_UNIFIED_KERNEL
typedef void *obj_handle_t;
typedef void *user_handle_t;
typedef unsigned short atom_t;
typedef unsigned int process_id_t;
typedef unsigned int thread_id_t;
typedef unsigned int data_size_t;
typedef unsigned int ioctl_code_t;
typedef unsigned __int64 file_pos_t;

struct request_header
{
	int          req;
	data_size_t  request_size;
	data_size_t  reply_size;
};

struct reply_header
{
	unsigned int error;
	data_size_t  reply_size;
};

struct request_max_size
{
	int pad[16];
};

#define FIRST_USER_HANDLE 0x0020
#define LAST_USER_HANDLE  0xffef

struct debug_event_exception
{
	EXCEPTION_RECORD record;
	int              first;
};

struct debug_event_create_thread
{
	obj_handle_t handle;
	void        *teb;
	void        *start;
};

struct debug_event_create_process
{
	obj_handle_t file;
	obj_handle_t process;
	obj_handle_t thread;
	void        *base;
	int          dbg_offset;
	int          dbg_size;
	void        *teb;
	void        *start;
	void        *name;
	int          unicode;
};

struct debug_event_exit
{
	int          exit_code;
};

struct debug_event_load_dll
{
	obj_handle_t handle;
	void        *base;
	int          dbg_offset;
	int          dbg_size;
	void        *name;
	int          unicode;
};

struct debug_event_unload_dll
{
	void       *base;
};

struct debug_event_output_string
{
	void       *string;
	int         unicode;
	int         length;
};

struct debug_event_rip_info
{
	int         error;
	int         type;
};

union debug_event_data
{
	struct debug_event_exception      exception;
	struct debug_event_create_thread  create_thread;
	struct debug_event_create_process create_process;
	struct debug_event_exit           exit;
	struct debug_event_load_dll       load_dll;
	struct debug_event_unload_dll     unload_dll;
	struct debug_event_output_string  output_string;
	struct debug_event_rip_info       rip_info;
};

typedef struct
{
	int                      code;
	union debug_event_data   info;
} debug_event_t;

struct send_fd
{
	thread_id_t tid;
	int         fd;
};

struct wake_up_reply
{
	void *cookie;
	int   signaled;
};

typedef long long timeout_t;
#define TIMEOUT_INFINITE (((timeout_t)0x7fffffff) << 32 | 0xffffffff)

typedef struct
{
	atom_t         atom;
	short          string;
	obj_handle_t   handle;
} property_data_t;

typedef struct
{
	int  left;
	int  top;
	int  right;
	int  bottom;
} rectangle_t;

typedef struct
{
	obj_handle_t    handle;
	void           *callback;
	void           *iosb;
	void           *arg;
	void           *apc;
	obj_handle_t    event;
	unsigned long   cvalue;
} async_data_t;

struct callback_msg_data
{
	void           *callback;
	unsigned long   data;
	unsigned long   result;
};

struct winevent_msg_data
{
	user_handle_t   hook;
	thread_id_t     tid;
	void           *hook_proc;

};

typedef union
{
	unsigned char            bytes[1];
	struct callback_msg_data callback;
	struct winevent_msg_data winevent;
} message_data_t;

typedef struct
{
	WCHAR          ch;
	unsigned short attr;
} char_info_t;

typedef struct
{
	unsigned int low_part;
	int          high_part;
} luid_t;

#define MAX_ACL_LEN 65535

struct security_descriptor
{
	unsigned int control;
	data_size_t  owner_len;
	data_size_t  group_len;
	data_size_t  sacl_len;
	data_size_t  dacl_len;
};

struct object_attributes
{
	obj_handle_t rootdir;
	data_size_t sd_len;
	data_size_t name_len;
};

struct token_groups
{
	unsigned int count;
};

enum apc_type
{
	APC_NONE,
	APC_USER,
	APC_TIMER,
	APC_ASYNC_IO,
	APC_VIRTUAL_ALLOC,
	APC_VIRTUAL_FREE,
	APC_VIRTUAL_QUERY,
	APC_VIRTUAL_PROTECT,
	APC_VIRTUAL_FLUSH,
	APC_VIRTUAL_LOCK,
	APC_VIRTUAL_UNLOCK,
	APC_MAP_VIEW,
	APC_UNMAP_VIEW,
	APC_CREATE_THREAD
};

typedef union
{
	enum apc_type type;
	struct
	{
		enum apc_type    type;
		void (__stdcall *func)(unsigned long,unsigned long,unsigned long);
		unsigned long    args[3];
	} user;
	struct
	{
		enum apc_type   type;
		void (__stdcall *func)(void*, unsigned int, unsigned int);
		timeout_t        time;
		void            *arg;
	} timer;
	struct
	{
		enum apc_type    type;
		unsigned int   (*func)(void*, void*, unsigned int, unsigned long *);
		void            *user;
		void            *sb;
		unsigned int     status;
	} async_io;
	struct
	{
		enum apc_type    type;
		void            *addr;
		unsigned long    size;
		unsigned int     zero_bits;
		unsigned int     op_type;
		unsigned int     prot;
	} virtual_alloc;
	struct
	{
		enum apc_type    type;
		void            *addr;
		unsigned long    size;
		unsigned int     op_type;
	} virtual_free;
	struct
	{
		enum apc_type    type;
		const void      *addr;
	} virtual_query;
	struct
	{
		enum apc_type    type;
		void            *addr;
		unsigned long    size;
		unsigned int     prot;
	} virtual_protect;
	struct
	{
		enum apc_type    type;
		const void      *addr;
		unsigned long    size;
	} virtual_flush;
	struct
	{
		enum apc_type    type;
		void            *addr;
		unsigned long    size;
	} virtual_lock;
	struct
	{
		enum apc_type    type;
		void            *addr;
		unsigned long    size;
	} virtual_unlock;
	struct
	{
		enum apc_type    type;
		obj_handle_t     handle;
		void            *addr;
		unsigned long    size;
		file_pos_t       offset;
		unsigned int     zero_bits;
		unsigned int     alloc_type;
		unsigned int     prot;
	} map_view;
	struct
	{
		enum apc_type    type;
		void            *addr;
	} unmap_view;
	struct
	{
		enum apc_type    type;
		void (__stdcall *func)(void*);
		void            *arg;
		unsigned long    reserve;
		unsigned long    commit;
		int              suspend;
	} create_thread;
} apc_call_t;

typedef union
{
	enum apc_type type;
	struct
	{
		enum apc_type    type;
		unsigned int     status;
		unsigned long    total;
	} async_io;
	struct
	{
		enum apc_type    type;
		unsigned int     status;
		void            *addr;
		unsigned long    size;
	} virtual_alloc;
	struct
	{
		enum apc_type    type;
		unsigned int     status;
		void            *addr;
		unsigned long    size;
	} virtual_free;
	struct
	{
		enum apc_type    type;
		unsigned int     status;
		void            *base;
		void            *alloc_base;
		unsigned long    size;
		unsigned int     state;
		unsigned int     prot;
		unsigned int     alloc_prot;
		unsigned int     alloc_type;
	} virtual_query;
	struct
	{
		enum apc_type    type;
		unsigned int     status;
		void            *addr;
		unsigned long    size;
		unsigned int     prot;
	} virtual_protect;
	struct
	{
		enum apc_type    type;
		unsigned int     status;
		const void      *addr;
		unsigned long    size;
	} virtual_flush;
	struct
	{
		enum apc_type    type;
		unsigned int     status;
		void            *addr;
		unsigned long    size;
	} virtual_lock;
	struct
	{
		enum apc_type    type;
		unsigned int     status;
		void            *addr;
		unsigned long    size;
	} virtual_unlock;
	struct
	{
		enum apc_type    type;
		unsigned int     status;
		void            *addr;
		unsigned long    size;
	} map_view;
	struct
	{
		enum apc_type    type;
		unsigned int     status;
	} unmap_view;
	struct
	{
		enum apc_type    type;
		unsigned int     status;
		thread_id_t      tid;
		obj_handle_t     handle;
	} create_thread;
} apc_result_t;

struct new_process_request
{
	struct request_header __header;
	int          inherit_all;
	unsigned int create_flags;
	int          socket_fd;
	int          child_pid;
	int          operation;
	obj_handle_t exe_file;
	obj_handle_t hstdin;
	obj_handle_t hstdout;
	obj_handle_t hstderr;
	unsigned int process_access;
	unsigned int process_attr;
	unsigned int thread_access;
	unsigned int thread_attr;
	/* VARARG(info,startup_info); */
	/* VARARG(env,unicode_str); */
};

struct new_process_reply
{
	struct reply_header __header;
	obj_handle_t info;
	process_id_t pid;
	obj_handle_t phandle;
	thread_id_t  tid;
	obj_handle_t thandle;
};

struct get_new_process_info_request
{
	struct request_header __header;
	obj_handle_t info;
};

struct get_new_process_info_reply
{
	struct reply_header __header;
	int          success;
	int          exit_code;
};

struct new_thread_request
{
    struct request_header __header;
    unsigned int access;
    unsigned int attributes;
    int          suspend;
    int          request_fd;
    int		 tid;
};
struct new_thread_reply
{
    struct reply_header __header;
    thread_id_t  tid;
    obj_handle_t handle;
};

struct get_startup_info_request
{
	struct request_header __header;
};

struct get_startup_info_reply
{
	struct reply_header __header;
	obj_handle_t exe_file;
	obj_handle_t hstdin;
	obj_handle_t hstdout;
	obj_handle_t hstderr;
	/* VARARG(info,startup_info); */
	/* VARARG(env,unicode_str); */
};

struct init_process_done_request
{
	struct request_header __header;
	void*        module;
	void*        entry;
	int          gui;
};

struct init_process_done_reply
{
	struct reply_header __header;
};

struct init_thread_request
{
	struct request_header __header;
	int          unix_pid;
	int          unix_tid;
	int          debug_level;
	void*        teb;
	void*        peb;
	void*        entry;
	void*        ldt_copy;
	int          reply_fd;
	int          wait_fd;
};

struct init_thread_reply
{
	struct reply_header __header;
	process_id_t pid;
	thread_id_t  tid;
	data_size_t  info_size;
	timeout_t    server_start;
	int          version;
};

struct load_dll_request
{
	struct request_header __header;
	obj_handle_t handle;
	void*        base;
	size_t       size;
	int          dbg_offset;
	int          dbg_size;
	void*        name;
	/* VARARG(filename,unicode_str); */
};

struct load_dll_reply
{
	struct reply_header __header;
};

struct unload_dll_request
{
	struct request_header __header;
	void*        base;
};

struct unload_dll_reply
{
	struct reply_header __header;
};

struct queue_apc_request
{
	struct request_header __header;
	obj_handle_t thread;
	obj_handle_t process;
	apc_call_t   call;
};

struct queue_apc_reply
{
	struct reply_header __header;
	obj_handle_t handle;
	int          self;
};

struct get_apc_result_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct get_apc_result_reply
{
	struct reply_header __header;
	apc_result_t result;
};

#define DUP_HANDLE_CLOSE_SOURCE  DUPLICATE_CLOSE_SOURCE
#define DUP_HANDLE_SAME_ACCESS   DUPLICATE_SAME_ACCESS
#define DUP_HANDLE_MAKE_GLOBAL   0x80000000

struct create_file_request
{
	struct request_header __header;
	unsigned int access;
	unsigned int attributes;
	unsigned int sharing;
	int          create;
	unsigned int options;
	unsigned int attrs;
	/* VARARG(objattr,object_attributes); */
	/* VARARG(filename,string); */
};

struct create_file_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct open_file_object_request
{
	struct request_header __header;
	unsigned int access;
	unsigned int attributes;
	obj_handle_t rootdir;
	unsigned int sharing;
	unsigned int options;
	/* VARARG(filename,unicode_str); */
};

struct open_file_object_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct alloc_file_handle_request
{
	struct request_header __header;
	unsigned int access;
	unsigned int attributes;
	int          fd;
};

struct alloc_file_handle_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct get_handle_fd_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct get_handle_fd_reply
{
	struct reply_header __header;
	int          type;
	int          removable;
	unsigned int access;
	unsigned int options;
	int          fd;            /* D.M. added for unified kernel */
};

enum server_fd_type
{
	FD_TYPE_INVALID,
	FD_TYPE_FILE,
	FD_TYPE_DIR,
	FD_TYPE_SOCKET,
	FD_TYPE_SERIAL,
	FD_TYPE_PIPE,
	FD_TYPE_MAILSLOT,
	FD_TYPE_CHAR,
	FD_TYPE_DEVICE,
	FD_TYPE_NB_TYPES
};

struct flush_file_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct flush_file_reply
{
	struct reply_header __header;
	obj_handle_t event;
};

struct lock_file_request
{
	struct request_header __header;
	obj_handle_t handle;
	file_pos_t   offset;
	file_pos_t   count;
	int          shared;
	int          wait;
};

struct lock_file_reply
{
	struct reply_header __header;
	obj_handle_t handle;
	int          overlapped;
};

struct unlock_file_request
{
	struct request_header __header;
	obj_handle_t handle;
	file_pos_t   offset;
	file_pos_t   count;
};

struct unlock_file_reply
{
	struct reply_header __header;
};

struct create_socket_request
{
	struct request_header __header;
	unsigned int access;
	unsigned int attributes;
	int          family;
	int          type;
	int          protocol;
	unsigned int flags;
};

struct create_socket_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct accept_socket_request
{
	struct request_header __header;
	obj_handle_t lhandle;
	unsigned int access;
	unsigned int attributes;
};

struct accept_socket_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct register_accept_async_request
{
	struct request_header __header;
	async_data_t data;
	obj_handle_t ahandle;
};

struct register_accept_async_reply
{
	struct reply_header __header;
};

struct set_socket_event_request
{
	struct request_header __header;
	obj_handle_t  handle;
	unsigned int  mask;
	obj_handle_t  event;
	user_handle_t window;
	unsigned int  msg;
};

struct set_socket_event_reply
{
	struct reply_header __header;
};

struct get_socket_event_request
{
	struct request_header __header;
	obj_handle_t handle;
	int          service;
	obj_handle_t c_event;
};

struct get_socket_event_reply
{
	struct reply_header __header;
	unsigned int mask;
	unsigned int pmask;
	unsigned int state;
	/* VARARG(errors,ints); */
};

struct enable_socket_event_request
{
	struct request_header __header;
	obj_handle_t handle;
	unsigned int mask;
	unsigned int sstate;
	unsigned int cstate;
};

struct enable_socket_event_reply
{
	struct reply_header __header;
};

struct set_socket_deferred_request
{
	struct request_header __header;
	obj_handle_t handle;
	obj_handle_t deferred;
};

struct set_socket_deferred_reply
{
	struct reply_header __header;
};

struct alloc_console_request
{
	struct request_header __header;
	unsigned int access;
	unsigned int attributes;
	process_id_t pid;
};

struct alloc_console_reply
{
	struct reply_header __header;
	obj_handle_t handle_in;
	obj_handle_t event;
};

struct free_console_request
{
	struct request_header __header;
};

struct free_console_reply
{
	struct reply_header __header;
};

#define CONSOLE_RENDERER_NONE_EVENT        0x00
#define CONSOLE_RENDERER_TITLE_EVENT       0x01
#define CONSOLE_RENDERER_ACTIVE_SB_EVENT   0x02
#define CONSOLE_RENDERER_SB_RESIZE_EVENT   0x03
#define CONSOLE_RENDERER_UPDATE_EVENT      0x04
#define CONSOLE_RENDERER_CURSOR_POS_EVENT  0x05
#define CONSOLE_RENDERER_CURSOR_GEOM_EVENT 0x06
#define CONSOLE_RENDERER_DISPLAY_EVENT     0x07
#define CONSOLE_RENDERER_EXIT_EVENT        0x08

struct console_renderer_event
{
	short event;
	union
	{
		struct update
		{
			short top;
			short bottom;
		} update;
		struct resize
		{
			short width;
			short height;
		} resize;
		struct cursor_pos
		{
			short x;
			short y;
		} cursor_pos;
		struct cursor_geom
		{
			short visible;
			short size;
		} cursor_geom;
		struct display
		{
			short left;
			short top;
			short width;
			short height;
		} display;
	} u;
};

struct get_console_renderer_events_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct get_console_renderer_events_reply
{
	struct reply_header __header;
	/* VARARG(data,bytes); */
};

struct open_console_request
{
	struct request_header __header;
	obj_handle_t from;

	unsigned int access;
	unsigned int attributes;
	int          share;
};

struct open_console_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct get_console_wait_event_request
{
	struct request_header __header;
};

struct get_console_wait_event_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct get_console_mode_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct get_console_mode_reply
{
	struct reply_header __header;
	int          mode;
};

struct set_console_mode_request
{
	struct request_header __header;
	obj_handle_t handle;
	int          mode;
};

struct set_console_mode_reply
{
	struct reply_header __header;
};

struct set_console_input_info_request
{
	struct request_header __header;
	obj_handle_t  handle;
	int           mask;
	obj_handle_t  active_sb;
	int           history_mode;
	int           history_size;
	int           edition_mode;
	int           input_cp;
	int           output_cp;
	user_handle_t win;
	/* VARARG(title,unicode_str); */
};

struct set_console_input_info_reply
{
	struct reply_header __header;
};

#define SET_CONSOLE_INPUT_INFO_ACTIVE_SB        0x01
#define SET_CONSOLE_INPUT_INFO_TITLE            0x02
#define SET_CONSOLE_INPUT_INFO_HISTORY_MODE     0x04
#define SET_CONSOLE_INPUT_INFO_HISTORY_SIZE     0x08
#define SET_CONSOLE_INPUT_INFO_EDITION_MODE     0x10
#define SET_CONSOLE_INPUT_INFO_INPUT_CODEPAGE   0x20
#define SET_CONSOLE_INPUT_INFO_OUTPUT_CODEPAGE  0x40
#define SET_CONSOLE_INPUT_INFO_WIN              0x80

struct get_console_input_info_request
{
	struct request_header __header;
	obj_handle_t  handle;
};

struct get_console_input_info_reply
{
	struct reply_header __header;
	int           history_mode;
	int           history_size;
	int           history_index;
	int           edition_mode;
	int           input_cp;
	int           output_cp;
	user_handle_t win;
	/* VARARG(title,unicode_str); */
};

struct append_console_input_history_request
{
	struct request_header __header;
	obj_handle_t handle;
	/* VARARG(line,unicode_str); */
};

struct append_console_input_history_reply
{
	struct reply_header __header;
};

struct get_console_input_history_request
{
	struct request_header __header;
	obj_handle_t handle;
	int          index;
};

struct get_console_input_history_reply
{
	struct reply_header __header;
	int          total;
	/* VARARG(line,unicode_str); */
};

struct create_console_output_request
{
	struct request_header __header;
	obj_handle_t handle_in;
	unsigned int access;
	unsigned int attributes;
	unsigned int share;
};

struct create_console_output_reply
{
	struct reply_header __header;
	obj_handle_t handle_out;
};

struct set_console_output_info_request
{
	struct request_header __header;
	obj_handle_t handle;
	int          mask;
	short int    cursor_size;
	short int    cursor_visible;
	short int    cursor_x;
	short int    cursor_y;
	short int    width;
	short int    height;
	short int    attr;
	short int    win_left;
	short int    win_top;
	short int    win_right;
	short int    win_bottom;
	short int    max_width;
	short int    max_height;
};

struct set_console_output_info_reply
{
	struct reply_header __header;
};

#define SET_CONSOLE_OUTPUT_INFO_CURSOR_GEOM     0x01
#define SET_CONSOLE_OUTPUT_INFO_CURSOR_POS      0x02
#define SET_CONSOLE_OUTPUT_INFO_SIZE            0x04
#define SET_CONSOLE_OUTPUT_INFO_ATTR            0x08
#define SET_CONSOLE_OUTPUT_INFO_DISPLAY_WINDOW  0x10
#define SET_CONSOLE_OUTPUT_INFO_MAX_SIZE        0x20

struct get_console_output_info_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct get_console_output_info_reply
{
	struct reply_header __header;
	short int    cursor_size;
	short int    cursor_visible;
	short int    cursor_x;
	short int    cursor_y;
	short int    width;
	short int    height;
	short int    attr;
	short int    win_left;
	short int    win_top;
	short int    win_right;
	short int    win_bottom;
	short int    max_width;
	short int    max_height;
};

struct write_console_input_request
{
	struct request_header __header;
	obj_handle_t handle;
	/* VARARG(rec,input_records); */
};

struct write_console_input_reply
{
	struct reply_header __header;
	int          written;
};

struct read_console_input_request
{
	struct request_header __header;
	obj_handle_t handle;
	int          flush;
};

struct read_console_input_reply
{
	struct reply_header __header;
	int          read;
	/* VARARG(rec,input_records); */
};

struct write_console_output_request
{
	struct request_header __header;
	obj_handle_t handle;
	int          x;
	int          y;
	int          mode;
	int          wrap;
	/* VARARG(data,bytes); */
};

struct write_console_output_reply
{
	struct reply_header __header;
	int          written;
	int          width;
	int          height;
};

enum char_info_mode
{
	CHAR_INFO_MODE_TEXT,
	CHAR_INFO_MODE_ATTR,
	CHAR_INFO_MODE_TEXTATTR,
	CHAR_INFO_MODE_TEXTSTDATTR
};

struct fill_console_output_request
{
	struct request_header __header;
	obj_handle_t handle;
	int          x;
	int          y;
	int          mode;
	int          count;
	int          wrap;
	char_info_t  data;
};

struct fill_console_output_reply
{
	struct reply_header __header;
	int          written;
};

struct read_console_output_request
{
	struct request_header __header;
	obj_handle_t handle;
	int          x;
	int          y;
	int          mode;
	int          wrap;
};

struct read_console_output_reply
{
	struct reply_header __header;
	int          width;
	int          height;
	/* VARARG(data,bytes); */
};

struct move_console_output_request
{
	struct request_header __header;
	obj_handle_t handle;
	short int    x_src;
	short int    y_src;
	short int    x_dst;
	short int    y_dst;
	short int    w;
	short int    h;
};

struct move_console_output_reply
{
	struct reply_header __header;
};

struct send_console_signal_request
{
	struct request_header __header;
	int          signal;
	process_id_t group_id;
};

struct send_console_signal_reply
{
	struct reply_header __header;
};

struct read_directory_changes_request
{
	struct request_header __header;
	unsigned int filter;
	obj_handle_t handle;
	int          subtree;
	int          want_data;
	async_data_t async;
};

struct read_directory_changes_reply
{
	struct reply_header __header;
};

struct read_change_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct read_change_reply
{
	struct reply_header __header;
	int          action;
	/* VARARG(name,string); */
};

struct create_mapping_request
{
	struct request_header __header;
	unsigned int access;
	unsigned int attributes;
	file_pos_t   size;
	int          protect;
	obj_handle_t file_handle;
	/* VARARG(objattr,object_attributes); */
};

struct create_mapping_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

#define VPROT_READ       0x01
#define VPROT_WRITE      0x02
#define VPROT_EXEC       0x04
#define VPROT_WRITECOPY  0x08
#define VPROT_GUARD      0x10
#define VPROT_NOCACHE    0x20
#define VPROT_COMMITTED  0x40
#define VPROT_IMAGE      0x80

struct open_mapping_request
{
	struct request_header __header;
	unsigned int access;
	unsigned int attributes;
	obj_handle_t rootdir;
	/* VARARG(name,unicode_str); */
};

struct open_mapping_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct get_mapping_info_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct get_mapping_info_reply
{
	struct reply_header __header;
	file_pos_t   size;
	int          protect;
	int          header_size;
	void*        base;
	obj_handle_t mapping;
	obj_handle_t shared_file;
};

#define SNAP_HEAPLIST   0x00000001
#define SNAP_PROCESS    0x00000002
#define SNAP_THREAD     0x00000004
#define SNAP_MODULE     0x00000008

struct create_snapshot_request
{
	struct request_header __header;
	unsigned int attributes;
	int          flags;
	process_id_t pid;
};

struct create_snapshot_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct next_process_request
{
	struct request_header __header;
	obj_handle_t handle;
	int          reset;
};

struct next_process_reply
{
	struct reply_header __header;
	int          count;
	process_id_t pid;
	process_id_t ppid;
	void*        heap;
	void*        module;
	int          threads;
	int          priority;
	int          handles;
	/* VARARG(filename,unicode_str); */
};

struct next_thread_request
{
	struct request_header __header;
	obj_handle_t handle;
	int          reset;
};

struct next_thread_reply
{
	struct reply_header __header;
	int          count;
	process_id_t pid;
	thread_id_t  tid;
	int          base_pri;
	int          delta_pri;
};

struct next_module_request
{
	struct request_header __header;
	obj_handle_t handle;
	int          reset;
};

struct next_module_reply
{
	struct reply_header __header;
	process_id_t pid;
	void*        base;
	size_t       size;
	/* VARARG(filename,unicode_str); */
};

struct wait_debug_event_request
{
	struct request_header __header;
	int           get_handle;
};

struct wait_debug_event_reply
{
	struct reply_header __header;
	process_id_t  pid;
	thread_id_t   tid;
	obj_handle_t  wait;
	/* VARARG(event,debug_event); */
};

struct queue_exception_event_request
{
	struct request_header __header;
	int              first;
	/* VARARG(record,exc_event); */
};

struct queue_exception_event_reply
{
	struct reply_header __header;
	obj_handle_t     handle;
};

struct get_exception_status_request
{
	struct request_header __header;
	obj_handle_t     handle;
};

struct get_exception_status_reply
{
	struct reply_header __header;
	/* VARARG(context,context); */
};

struct output_debug_string_request
{
	struct request_header __header;
	void*         string;
	int           unicode;
	int           length;
};
struct output_debug_string_reply
{
	struct reply_header __header;
};

struct continue_debug_event_request
{
	struct request_header __header;
	process_id_t pid;
	thread_id_t  tid;
	int          status;
};

struct continue_debug_event_reply
{
	struct reply_header __header;
};

struct debug_process_request
{
	struct request_header __header;
	process_id_t pid;
	int          attach;
};

struct debug_process_reply
{
	struct reply_header __header;
};

struct debug_break_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct debug_break_reply
{
	struct reply_header __header;
	int          self;
};

struct set_debugger_kill_on_exit_request
{
	struct request_header __header;
	int          kill_on_exit;
};

struct set_debugger_kill_on_exit_reply
{
	struct reply_header __header;
};

struct create_key_request
{
	struct request_header __header;
	obj_handle_t parent;
	unsigned int access;
	unsigned int attributes;
	unsigned int options;
	time_t       modif;
	data_size_t  namelen;
	/* VARARG(name,unicode_str,namelen); */
	/* VARARG(class,unicode_str); */
};

struct create_key_reply
{
	struct reply_header __header;
	obj_handle_t hkey;
	int          created;
};

struct open_key_request
{
	struct request_header __header;
	obj_handle_t parent;
	unsigned int access;
	unsigned int attributes;
	/* VARARG(name,unicode_str); */
};

struct open_key_reply
{
	struct reply_header __header;
	obj_handle_t hkey;
};

struct delete_key_request
{
	struct request_header __header;
	obj_handle_t hkey;
};

struct delete_key_reply
{
	struct reply_header __header;
};

struct flush_key_request
{
	struct request_header __header;
	obj_handle_t hkey;
};

struct flush_key_reply
{
	struct reply_header __header;
};

struct enum_key_request
{
	struct request_header __header;
	obj_handle_t hkey;
	int          index;
	int          info_class;
};

struct enum_key_reply
{
	struct reply_header __header;
	int          subkeys;
	int          max_subkey;
	int          max_class;
	int          values;
	int          max_value;
	int          max_data;
	time_t       modif;
	data_size_t  total;
	data_size_t  namelen;
	/* VARARG(name,unicode_str,namelen); */
	/* VARARG(class,unicode_str); */
};

struct set_key_value_request
{
	struct request_header __header;
	obj_handle_t hkey;
	int          type;
	data_size_t  namelen;
	/* VARARG(name,unicode_str,namelen); */
	/* VARARG(data,bytes); */
};

struct set_key_value_reply
{
	struct reply_header __header;
};

struct get_key_value_request
{
	struct request_header __header;
	obj_handle_t hkey;
	/* VARARG(name,unicode_str); */
};

struct get_key_value_reply
{
	struct reply_header __header;
	int          type;
	data_size_t  total;
	/* VARARG(data,bytes); */
};

struct enum_key_value_request
{
	struct request_header __header;
	obj_handle_t hkey;
	int          index;
	int          info_class;
};

struct enum_key_value_reply
{
	struct reply_header __header;
	int          type;
	data_size_t  total;
	data_size_t  namelen;
	/* VARARG(name,unicode_str,namelen); */
	/* VARARG(data,bytes); */
};

struct delete_key_value_request
{
	struct request_header __header;
	obj_handle_t hkey;
	/* VARARG(name,unicode_str); */
};

struct delete_key_value_reply
{
	struct reply_header __header;
};

struct load_registry_request
{
	struct request_header __header;
	obj_handle_t hkey;
	obj_handle_t file;
	/* VARARG(name,unicode_str); */
};

struct load_registry_reply
{
	struct reply_header __header;
};

struct unload_registry_request
{
	struct request_header __header;
	obj_handle_t hkey;
};

struct unload_registry_reply
{
	struct reply_header __header;
};

struct save_registry_request
{
	struct request_header __header;
	obj_handle_t hkey;
	obj_handle_t file;
};

struct save_registry_reply
{
	struct reply_header __header;
};

struct set_registry_notification_request
{
	struct request_header __header;
	obj_handle_t hkey;
	obj_handle_t event;
	int          subtree;
	unsigned int filter;
};

struct set_registry_notification_reply
{
	struct reply_header __header;
};

struct create_timer_request
{
	struct request_header __header;
	unsigned int access;
	unsigned int attributes;
	obj_handle_t rootdir;
	int          manual;
	/* VARARG(name,unicode_str); */
};

struct create_timer_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct open_timer_request
{
	struct request_header __header;
	unsigned int access;
	unsigned int attributes;
	obj_handle_t rootdir;
	/* VARARG(name,unicode_str); */
};

struct open_timer_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct set_timer_request
{
	struct request_header __header;
	obj_handle_t handle;
	timeout_t    expire;
	int          period;
	void*        callback;
	void*        arg;
};

struct set_timer_reply
{
	struct reply_header __header;
	int          signaled;
};

struct cancel_timer_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct cancel_timer_reply
{
	struct reply_header __header;
	int         signaled;
};

struct get_timer_info_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct get_timer_info_reply
{
	struct reply_header __header;
	timeout_t    when;
	int          signaled;
};

struct get_thread_context_request
{
	struct request_header __header;
	obj_handle_t handle;
	unsigned int flags;
	int          suspend;
};

struct get_thread_context_reply
{
	struct reply_header __header;
	int          self;
	/* VARARG(context,context); */
};

struct set_thread_context_request
{
	struct request_header __header;
	obj_handle_t handle;
	unsigned int flags;
	int          suspend;
	/* VARARG(context,context); */
};

struct set_thread_context_reply
{
	struct reply_header __header;
	int          self;
};

struct add_atom_request
{
	struct request_header __header;
	obj_handle_t  table;
	/* VARARG(name,unicode_str); */
};

struct add_atom_reply
{
	struct reply_header __header;
	atom_t        atom;
};

struct delete_atom_request
{
	struct request_header __header;
	obj_handle_t  table;
	atom_t        atom;
};

struct delete_atom_reply
{
	struct reply_header __header;
};

struct find_atom_request
{
	struct request_header __header;
	obj_handle_t table;
	/* VARARG(name,unicode_str); */
};

struct find_atom_reply
{
	struct reply_header __header;
	atom_t       atom;
};

struct get_atom_information_request
{
	struct request_header __header;
	obj_handle_t table;
	atom_t       atom;
};

struct get_atom_information_reply
{
	struct reply_header __header;
	int          count;
	int          pinned;
	data_size_t  total;
	/* VARARG(name,unicode_str); */
};

struct set_atom_information_request
{
	struct request_header __header;
	obj_handle_t table;
	atom_t       atom;
	int          pinned;
};

struct set_atom_information_reply
{
	struct reply_header __header;
};

struct empty_atom_table_request
{
	struct request_header __header;
	obj_handle_t table;
	int          if_pinned;
};

struct empty_atom_table_reply
{
	struct reply_header __header;
};

struct init_atom_table_request
{
	struct request_header __header;
	int          entries;
};

struct init_atom_table_reply
{
	struct reply_header __header;
	obj_handle_t table;
};

struct get_msg_queue_request
{
	struct request_header __header;
};

struct get_msg_queue_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct set_queue_fd_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct set_queue_fd_reply
{
	struct reply_header __header;
};

struct set_queue_mask_request
{
	struct request_header __header;
	unsigned int wake_mask;
	unsigned int changed_mask;
	int          skip_wait;
};

struct set_queue_mask_reply
{
	struct reply_header __header;
	unsigned int wake_bits;
	unsigned int changed_bits;
};

struct get_queue_status_request
{
	struct request_header __header;
	int          clear;
};

struct get_queue_status_reply
{
	struct reply_header __header;
	unsigned int wake_bits;
	unsigned int changed_bits;
};

struct get_process_idle_event_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct get_process_idle_event_reply
{
	struct reply_header __header;
	obj_handle_t event;
};

struct send_message_request
{
	struct request_header __header;
	thread_id_t     id;
	int             type;
	int             flags;
	user_handle_t   win;
	unsigned int    msg;
	unsigned long   wparam;
	unsigned long   lparam;
	timeout_t       timeout;
	/* VARARG(data,message_data); */
};

struct send_message_reply
{
	struct reply_header __header;
};

struct post_quit_message_request
{
	struct request_header __header;
	int             exit_code;
};

struct post_quit_message_reply
{
	struct reply_header __header;
};

enum message_type
{
	MSG_ASCII,
	MSG_UNICODE,
	MSG_NOTIFY,
	MSG_CALLBACK,
	MSG_CALLBACK_RESULT,
	MSG_OTHER_PROCESS,
	MSG_POSTED,
	MSG_HARDWARE,
	MSG_WINEVENT
};

#define SEND_MSG_ABORT_IF_HUNG  0x01

struct send_hardware_message_request
{
	struct request_header __header;
	thread_id_t     id;
	user_handle_t   win;
	unsigned int    msg;
	unsigned int    time;
	unsigned long   wparam;
	unsigned long   lparam;
	unsigned long   info;
	int             x;
	int             y;
};

struct send_hardware_message_reply
{
	struct reply_header __header;
};

struct get_message_request
{
	struct request_header __header;
	unsigned int    flags;
	user_handle_t   get_win;
	unsigned int    get_first;
	unsigned int    get_last;
	unsigned int    hw_id;
	unsigned int    wake_mask;
	unsigned int    changed_mask;
};

struct get_message_reply
{
	struct reply_header __header;
	user_handle_t   win;
	int             type;
	unsigned int    msg;
	unsigned long   wparam;
	unsigned long   lparam;
	unsigned long   info;
	int             x;
	int             y;
	unsigned int    time;
	unsigned int    hw_id;
	unsigned int    active_hooks;
	data_size_t     total;
	/* VARARG(data,message_data); */
};

struct reply_message_request
{
	struct request_header __header;
	unsigned long   result;
	int             remove;
	/* VARARG(data,bytes); */
};

struct reply_message_reply
{
	struct reply_header __header;
};

struct accept_hardware_message_request
{
	struct request_header __header;
	unsigned int    hw_id;
	int             remove;
	user_handle_t   new_win;
};

struct accept_hardware_message_reply
{
	struct reply_header __header;
};

struct get_message_reply_request
{
	struct request_header __header;
	int             cancel;
};

struct get_message_reply_reply
{
	struct reply_header __header;
	unsigned long   result;
	/* VARARG(data,bytes); */
};

struct set_win_timer_request
{
	struct request_header __header;
	user_handle_t   win;
	unsigned int    msg;
	unsigned int    rate;
	unsigned long   id;
	unsigned long   lparam;
};

struct set_win_timer_reply
{
	struct reply_header __header;
	unsigned long   id;
};

struct kill_win_timer_request
{
	struct request_header __header;
	user_handle_t   win;
	unsigned int    msg;
	unsigned long   id;
};

struct kill_win_timer_reply
{
	struct reply_header __header;
};

struct is_window_hung_request
{
	struct request_header __header;
	user_handle_t   win;
};

struct is_window_hung_reply
{
	struct reply_header __header;
	int is_hung;
};

struct get_serial_info_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct get_serial_info_reply
{
	struct reply_header __header;
	unsigned int readinterval;
	unsigned int readconst;
	unsigned int readmult;
	unsigned int writeconst;
	unsigned int writemult;
	unsigned int eventmask;
};

struct set_serial_info_request
{
	struct request_header __header;
	obj_handle_t handle;
	int          flags;
	unsigned int readinterval;
	unsigned int readconst;
	unsigned int readmult;
	unsigned int writeconst;
	unsigned int writemult;
	unsigned int eventmask;
};

struct set_serial_info_reply
{
	struct reply_header __header;
};

#define SERIALINFO_SET_TIMEOUTS  0x01
#define SERIALINFO_SET_MASK      0x02

struct register_async_request
{
	struct request_header __header;
	obj_handle_t handle;
	int          type;
	int          count;
	async_data_t async;
};

struct register_async_reply
{
	struct reply_header __header;
};

#define ASYNC_TYPE_READ  0x01
#define ASYNC_TYPE_WRITE 0x02
#define ASYNC_TYPE_WAIT  0x03

struct cancel_async_request
{
	struct request_header __header;
	obj_handle_t handle;
	unsigned __int64 iosb;
	int only_thread;
};

struct cancel_async_reply
{
	struct reply_header __header;
};

struct ioctl_request
{
	struct request_header __header;
	obj_handle_t   handle;
	ioctl_code_t   code;
	async_data_t   async;
	/* VARARG(in_data,bytes); */
};

struct ioctl_reply
{
	struct reply_header __header;
	obj_handle_t   wait;
	unsigned int   options;
	/* VARARG(out_data,bytes); */
};

struct get_ioctl_result_request
{
	struct request_header __header;
	obj_handle_t   handle;
	void*          user_arg;
};

struct get_ioctl_result_reply
{
	struct reply_header __header;
	/* VARARG(out_data,bytes); */
};

struct create_named_pipe_request
{
	struct request_header __header;
	unsigned int   access;
	unsigned int   attributes;
	obj_handle_t   rootdir;
	unsigned int   options;
	unsigned int   flags;
	unsigned int   maxinstances;
	unsigned int   outsize;
	unsigned int   insize;
	timeout_t      timeout;
	/* VARARG(name,unicode_str); */
};

struct create_named_pipe_reply
{
	struct reply_header __header;
	obj_handle_t   handle;
};

#define NAMED_PIPE_MESSAGE_STREAM_WRITE 0x0001
#define NAMED_PIPE_MESSAGE_STREAM_READ  0x0002
#define NAMED_PIPE_NONBLOCKING_MODE     0x0004
#define NAMED_PIPE_SERVER_END           0x8000

struct get_named_pipe_info_request
{
	struct request_header __header;
	obj_handle_t   handle;
};

struct get_named_pipe_info_reply
{
	struct reply_header __header;
	unsigned int   flags;
	unsigned int   maxinstances;
	unsigned int   instances;
	unsigned int   outsize;
	unsigned int   insize;
};

struct create_window_request
{
	struct request_header __header;
	user_handle_t  parent;
	user_handle_t  owner;
	atom_t         atom;
	void*          instance;
	/* VARARG(class,unicode_str); */
};

struct create_window_reply
{
	struct reply_header __header;
	user_handle_t  handle;
	user_handle_t  parent;
	user_handle_t  owner;
	int            extra;
	void*          class_ptr;
};

struct destroy_window_request
{
	struct request_header __header;
	user_handle_t  handle;
};

struct destroy_window_reply
{
	struct reply_header __header;
};

struct get_desktop_window_request
{
	struct request_header __header;
	int            force;
};

struct get_desktop_window_reply
{
	struct reply_header __header;
	user_handle_t  handle;
};

struct set_window_owner_request
{
	struct request_header __header;
	user_handle_t  handle;
	user_handle_t  owner;
};

struct set_window_owner_reply
{
	struct reply_header __header;
	user_handle_t  full_owner;
	user_handle_t  prev_owner;
};

struct get_window_info_request
{
	struct request_header __header;
	user_handle_t  handle;
};

struct get_window_info_reply
{
	struct reply_header __header;
	user_handle_t  full_handle;
	user_handle_t  last_active;
	process_id_t   pid;
	thread_id_t    tid;
	atom_t         atom;
	int            is_unicode;
};

struct set_window_info_request
{
	struct request_header __header;
	unsigned int   flags;
	user_handle_t  handle;
	unsigned int   style;
	unsigned int   ex_style;
	unsigned int   id;
	int            is_unicode;
	void*          instance;
	unsigned long  user_data;
	int            extra_offset;
	data_size_t    extra_size;
	unsigned long  extra_value;
};

struct set_window_info_reply
{
	struct reply_header __header;
	unsigned int   old_style;
	unsigned int   old_ex_style;
	unsigned int   old_id;
	void*          old_instance;
	unsigned long  old_user_data;
	unsigned long  old_extra_value;
};

#define SET_WIN_STYLE     0x01
#define SET_WIN_EXSTYLE   0x02
#define SET_WIN_ID        0x04
#define SET_WIN_INSTANCE  0x08
#define SET_WIN_USERDATA  0x10
#define SET_WIN_EXTRA     0x20
#define SET_WIN_UNICODE   0x40

struct set_parent_request
{
	struct request_header __header;
	user_handle_t  handle;
	user_handle_t  parent;
};

struct set_parent_reply
{
	struct reply_header __header;
	user_handle_t  old_parent;
	user_handle_t  full_parent;
};

struct get_window_parents_request
{
	struct request_header __header;
	user_handle_t  handle;
};

struct get_window_parents_reply
{
	struct reply_header __header;
	int            count;
	/* VARARG(parents,user_handles); */
};

struct get_window_children_request
{
	struct request_header __header;
	obj_handle_t   desktop;
	user_handle_t  parent;
	atom_t         atom;
	thread_id_t    tid;
	/* VARARG(class,unicode_str); */
};

struct get_window_children_reply
{
	struct reply_header __header;
	int            count;
	/* VARARG(children,user_handles); */
};

struct get_window_children_from_point_request
{
	struct request_header __header;
	user_handle_t  parent;
	int            x;
	int            y;
};

struct get_window_children_from_point_reply
{
	struct reply_header __header;
	int            count;
	/* VARARG(children,user_handles); */
};

struct get_window_tree_request
{
	struct request_header __header;
	user_handle_t  handle;
};

struct get_window_tree_reply
{
	struct reply_header __header;
	user_handle_t  parent;
	user_handle_t  owner;
	user_handle_t  next_sibling;
	user_handle_t  prev_sibling;
	user_handle_t  first_sibling;
	user_handle_t  last_sibling;
	user_handle_t  first_child;
	user_handle_t  last_child;
};

struct set_window_pos_request
{
	struct request_header __header;
	unsigned int   flags;
	user_handle_t  handle;
	user_handle_t  previous;
	rectangle_t    window;
	rectangle_t    client;
	/* VARARG(valid,rectangles); */
};

struct set_window_pos_reply
{
	struct reply_header __header;
	unsigned int   new_style;
	unsigned int   new_ex_style;
	rectangle_t    visible;
};

struct set_window_visible_rect_request
{
	struct request_header __header;
	unsigned int   flags;
	user_handle_t  handle;
	rectangle_t    visible;
};

struct set_window_visible_rect_reply
{
	struct reply_header __header;
};

struct get_window_rectangles_request
{
	struct request_header __header;
	user_handle_t  handle;
};

struct get_window_rectangles_reply
{
	struct reply_header __header;
	rectangle_t    window;
	rectangle_t    visible;
	rectangle_t    client;
};

struct get_window_text_request
{
	struct request_header __header;
	user_handle_t  handle;
};

struct get_window_text_reply
{
	struct reply_header __header;
	/* VARARG(text,unicode_str); */
};

struct set_window_text_request
{
	struct request_header __header;
	user_handle_t  handle;
	/* VARARG(text,unicode_str); */
};

struct set_window_text_reply
{
	struct reply_header __header;
};

struct get_windows_offset_request
{
	struct request_header __header;
	user_handle_t  from;
	user_handle_t  to;
};

struct get_windows_offset_reply
{
	struct reply_header __header;
	int            x;
	int            y;
};

struct get_visible_region_request
{
	struct request_header __header;
	user_handle_t  window;
	unsigned int   flags;
};

struct get_visible_region_reply
{
	struct reply_header __header;
	user_handle_t  top_win;
	rectangle_t    top_rect;
	rectangle_t    win_rect;
	data_size_t    total_size;
	/* VARARG(region,rectangles); */
};

struct get_window_region_request
{
	struct request_header __header;
	user_handle_t  window;
};

struct get_window_region_reply
{
	struct reply_header __header;
	data_size_t    total_size;
	/* VARARG(region,rectangles); */
};

struct set_window_region_request
{
	struct request_header __header;
	user_handle_t  window;
	int            redraw;
	/* VARARG(region,rectangles); */
};

struct set_window_region_reply
{
	struct reply_header __header;
};

struct get_update_region_request
{
	struct request_header __header;
	user_handle_t  window;
	user_handle_t  from_child;
	unsigned int   flags;
};

struct get_update_region_reply
{
	struct reply_header __header;
	user_handle_t  child;
	unsigned int   flags;
	data_size_t    total_size;
	/* VARARG(region,rectangles); */
};

#define UPDATE_NONCLIENT       0x01
#define UPDATE_ERASE           0x02
#define UPDATE_PAINT           0x04
#define UPDATE_INTERNALPAINT   0x08
#define UPDATE_ALLCHILDREN     0x10
#define UPDATE_NOCHILDREN      0x20
#define UPDATE_NOREGION        0x40
#define UPDATE_DELAYED_ERASE   0x80

struct update_window_zorder_request
{
	struct request_header __header;
	user_handle_t  window;
	rectangle_t    rect;
};

struct update_window_zorder_reply
{
	struct reply_header __header;
};

struct redraw_window_request
{
	struct request_header __header;
	user_handle_t  window;
	unsigned int   flags;
	/* VARARG(region,rectangles); */
};

struct redraw_window_reply
{
	struct reply_header __header;
};

struct set_window_property_request
{
	struct request_header __header;
	user_handle_t  window;
	atom_t         atom;
	obj_handle_t   handle;
	/* VARARG(name,unicode_str); */
};

struct set_window_property_reply
{
	struct reply_header __header;
};

struct remove_window_property_request
{
	struct request_header __header;
	user_handle_t  window;
	atom_t         atom;
	/* VARARG(name,unicode_str); */
};

struct remove_window_property_reply
{
	struct reply_header __header;
	obj_handle_t   handle;
};

struct get_window_property_request
{
	struct request_header __header;
	user_handle_t  window;
	atom_t         atom;
	/* VARARG(name,unicode_str); */
};

struct get_window_property_reply
{
	struct reply_header __header;
	obj_handle_t   handle;
};

struct get_window_properties_request
{
	struct request_header __header;
	user_handle_t  window;
};

struct get_window_properties_reply
{
	struct reply_header __header;
	int            total;
	/* VARARG(props,properties); */
};

struct create_winstation_request
{
	struct request_header __header;
	unsigned int flags;
	unsigned int access;
	unsigned int attributes;
	/* VARARG(name,unicode_str); */
};

struct create_winstation_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct open_winstation_request
{
	struct request_header __header;
	unsigned int access;
	unsigned int attributes;
	/* VARARG(name,unicode_str); */
};

struct open_winstation_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct close_winstation_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct close_winstation_reply
{
	struct reply_header __header;
};

struct get_process_winstation_request
{
	struct request_header __header;
};

struct get_process_winstation_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct set_process_winstation_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct set_process_winstation_reply
{
	struct reply_header __header;
};

struct enum_winstation_request
{
	struct request_header __header;
	unsigned int index;
};

struct enum_winstation_reply
{
	struct reply_header __header;
	unsigned int next;
	/* VARARG(name,unicode_str); */
};

struct create_desktop_request
{
	struct request_header __header;
	unsigned int flags;
	unsigned int access;
	unsigned int attributes;
	/* VARARG(name,unicode_str); */
};

struct create_desktop_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct open_desktop_request
{
	struct request_header __header;
	obj_handle_t winsta;
	unsigned int flags;
	unsigned int access;
	unsigned int attributes;
	/* VARARG(name,unicode_str); */
};

struct open_desktop_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct close_desktop_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct close_desktop_reply
{
	struct reply_header __header;
};

struct get_thread_desktop_request
{
	struct request_header __header;
	thread_id_t  tid;
};

struct get_thread_desktop_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct set_thread_desktop_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct set_thread_desktop_reply
{
	struct reply_header __header;
};

struct enum_desktop_request
{
	struct request_header __header;
	obj_handle_t winstation;
	unsigned int index;
};

struct enum_desktop_reply
{
	struct reply_header __header;
	unsigned int next;
	/* VARARG(name,unicode_str); */
};

struct set_user_object_info_request
{
	struct request_header __header;
	obj_handle_t handle;
	unsigned int flags;
	unsigned int obj_flags;
};

struct set_user_object_info_reply
{
	struct reply_header __header;
	int          is_desktop;
	unsigned int old_obj_flags;
	/* VARARG(name,unicode_str); */
};

#define SET_USER_OBJECT_FLAGS 1

struct attach_thread_input_request
{
	struct request_header __header;
	thread_id_t    tid_from;
	thread_id_t    tid_to;
	int            attach;
};

struct attach_thread_input_reply
{
	struct reply_header __header;
};

struct get_thread_input_request
{
	struct request_header __header;
	thread_id_t    tid;
};

struct get_thread_input_reply
{
	struct reply_header __header;
	user_handle_t  focus;
	user_handle_t  capture;
	user_handle_t  active;
	user_handle_t  foreground;
	user_handle_t  menu_owner;
	user_handle_t  move_size;
	user_handle_t  caret;
	rectangle_t    rect;
};

struct get_last_input_time_request
{
	struct request_header __header;
};

struct get_last_input_time_reply
{
	struct reply_header __header;
	unsigned int time;
};

struct get_key_state_request
{
	struct request_header __header;
	thread_id_t    tid;
	int            key;
};

struct get_key_state_reply
{
	struct reply_header __header;
	unsigned char  state;
	/* VARARG(keystate,bytes); */
};

struct set_key_state_request
{
	struct request_header __header;
	thread_id_t    tid;
	/* VARARG(keystate,bytes); */
};

struct set_key_state_reply
{
	struct reply_header __header;
};

struct set_foreground_window_request
{
	struct request_header __header;
	user_handle_t  handle;
};

struct set_foreground_window_reply
{
	struct reply_header __header;
	user_handle_t  previous;
	int            send_msg_old;
	int            send_msg_new;
};

struct set_focus_window_request
{
	struct request_header __header;
	user_handle_t  handle;
};

struct set_focus_window_reply
{
	struct reply_header __header;
	user_handle_t  previous;
};

struct set_active_window_request
{
	struct request_header __header;
	user_handle_t  handle;
};

struct set_active_window_reply
{
	struct reply_header __header;
	user_handle_t  previous;
};

struct set_capture_window_request
{
	struct request_header __header;
	user_handle_t  handle;
	unsigned int   flags;
};

struct set_capture_window_reply
{
	struct reply_header __header;
	user_handle_t  previous;
	user_handle_t  full_handle;
};

#define CAPTURE_MENU     0x01
#define CAPTURE_MOVESIZE 0x02

struct set_caret_window_request
{
	struct request_header __header;
	user_handle_t  handle;
	int            width;
	int            height;
};

struct set_caret_window_reply
{
	struct reply_header __header;
	user_handle_t  previous;
	rectangle_t    old_rect;
	int            old_hide;
	int            old_state;
};

struct set_caret_info_request
{
	struct request_header __header;
	unsigned int   flags;
	user_handle_t  handle;
	int            x;
	int            y;
	int            hide;
	int            state;
};

struct set_caret_info_reply
{
	struct reply_header __header;
	user_handle_t  full_handle;
	rectangle_t    old_rect;
	int            old_hide;
	int            old_state;
};

#define SET_CARET_POS        0x01
#define SET_CARET_HIDE       0x02
#define SET_CARET_STATE      0x04

struct set_hook_request
{
	struct request_header __header;
	int            id;
	process_id_t   pid;
	thread_id_t    tid;
	int            event_min;
	int            event_max;
	int            flags;
	void*          proc;
	int            unicode;
	/* VARARG(module,unicode_str); */
};

struct set_hook_reply
{
	struct reply_header __header;
	user_handle_t  handle;
	unsigned int   active_hooks;
};

struct remove_hook_request
{
	struct request_header __header;
	user_handle_t  handle;
	int            id;
	void*          proc;
};

struct remove_hook_reply
{
	struct reply_header __header;
	unsigned int   active_hooks;
};

struct start_hook_chain_request
{
	struct request_header __header;
	int            id;
	int            event;
	user_handle_t  window;
	int            object_id;
	int            child_id;
};

struct start_hook_chain_reply
{
	struct reply_header __header;
	user_handle_t  handle;
	process_id_t   pid;
	thread_id_t    tid;
	void*          proc;
	int            unicode;
	unsigned int   active_hooks;
	/* VARARG(module,unicode_str); */
};

struct finish_hook_chain_request
{
	struct request_header __header;
	int            id;
};

struct finish_hook_chain_reply
{
	struct reply_header __header;
};

struct get_hook_info_request
{
	struct request_header __header;
	user_handle_t  handle;
	int            get_next;
	int            event;
	user_handle_t  window;
	int            object_id;
	int            child_id;
};

struct get_hook_info_reply
{
	struct reply_header __header;
	user_handle_t  handle;
	int            id;
	process_id_t   pid;
	thread_id_t    tid;
	void*          proc;
	int            unicode;
	/* VARARG(module,unicode_str); */
};

struct create_class_request
{
	struct request_header __header;
	int            local;
	atom_t         atom;
	unsigned int   style;
	void*          instance;
	int            extra;
	int            win_extra;
	void*          client_ptr;
	/* VARARG(name,unicode_str); */
};

struct create_class_reply
{
	struct reply_header __header;
	atom_t         atom;
};

struct destroy_class_request
{
	struct request_header __header;
	atom_t         atom;
	void*          instance;
	/* VARARG(name,unicode_str); */
};

struct destroy_class_reply
{
	struct reply_header __header;
	void*          client_ptr;
};

struct set_class_info_request
{
	struct request_header __header;
	user_handle_t  window;
	unsigned int   flags;
	atom_t         atom;
	unsigned int   style;
	int            win_extra;
	void*          instance;
	int            extra_offset;
	data_size_t    extra_size;
	unsigned long  extra_value;
};

struct set_class_info_reply
{
	struct reply_header __header;
	atom_t         old_atom;
	unsigned int   old_style;
	int            old_extra;
	int            old_win_extra;
	void*          old_instance;
	unsigned long  old_extra_value;
};

#define SET_CLASS_ATOM      0x0001
#define SET_CLASS_STYLE     0x0002
#define SET_CLASS_WINEXTRA  0x0004
#define SET_CLASS_INSTANCE  0x0008
#define SET_CLASS_EXTRA     0x0010

struct set_clipboard_info_request
{
	struct request_header __header;
	unsigned int   flags;
	user_handle_t  clipboard;
	user_handle_t  owner;
	user_handle_t  viewer;
	unsigned int   seqno;
};

struct set_clipboard_info_reply
{
	struct reply_header __header;
	unsigned int   flags;
	user_handle_t  old_clipboard;
	user_handle_t  old_owner;
	user_handle_t  old_viewer;
	unsigned int   seqno;
};

#define SET_CB_OPEN      0x001
#define SET_CB_OWNER     0x002
#define SET_CB_VIEWER    0x004
#define SET_CB_SEQNO     0x008
#define SET_CB_RELOWNER  0x010
#define SET_CB_CLOSE     0x020
#define CB_OPEN          0x040
#define CB_OWNER         0x080
#define CB_PROCESS       0x100

struct open_token_request
{
	struct request_header __header;
	obj_handle_t   handle;
	unsigned int   access;
	unsigned int   attributes;
	unsigned int   flags;
};

struct open_token_reply
{
	struct reply_header __header;
	obj_handle_t   token;
};

#define OPEN_TOKEN_THREAD   1
#define OPEN_TOKEN_AS_SELF  2

struct set_global_windows_request
{
	struct request_header __header;
	unsigned int   flags;
	user_handle_t  shell_window;
	user_handle_t  shell_listview;
	user_handle_t  progman_window;
	user_handle_t  taskman_window;
};

struct set_global_windows_reply
{
	struct reply_header __header;
	user_handle_t  old_shell_window;
	user_handle_t  old_shell_listview;
	user_handle_t  old_progman_window;
	user_handle_t  old_taskman_window;
};

#define SET_GLOBAL_SHELL_WINDOWS   0x01
#define SET_GLOBAL_PROGMAN_WINDOW  0x02
#define SET_GLOBAL_TASKMAN_WINDOW  0x04

struct adjust_token_privileges_request
{
	struct request_header __header;
	obj_handle_t  handle;
	int           disable_all;
	int           get_modified_state;
	/* VARARG(privileges,LUID_AND_ATTRIBUTES); */
};

struct adjust_token_privileges_reply
{
	struct reply_header __header;
	unsigned int  len;
	/* VARARG(privileges,LUID_AND_ATTRIBUTES); */
};

struct get_token_privileges_request
{
	struct request_header __header;
	obj_handle_t  handle;
};

struct get_token_privileges_reply
{
	struct reply_header __header;
	unsigned int  len;
	/* VARARG(privileges,LUID_AND_ATTRIBUTES); */
};

struct check_token_privileges_request
{
	struct request_header __header;
	obj_handle_t  handle;
	int           all_required;
	/* VARARG(privileges,LUID_AND_ATTRIBUTES); */
};

struct check_token_privileges_reply
{
	struct reply_header __header;
	int           has_privileges;
	/* VARARG(privileges,LUID_AND_ATTRIBUTES); */
};

struct duplicate_token_request
{
	struct request_header __header;
	obj_handle_t  handle;
	unsigned int  access;
	unsigned int  attributes;
	int           primary;
	int           impersonation_level;
};

struct duplicate_token_reply
{
	struct reply_header __header;
	obj_handle_t  new_handle;
};

struct access_check_request
{
	struct request_header __header;
	obj_handle_t    handle;
	unsigned int    desired_access;
	unsigned int    mapping_read;
	unsigned int    mapping_write;
	unsigned int    mapping_execute;
	unsigned int    mapping_all;
	/* VARARG(sd,security_descriptor); */
};

struct access_check_reply
{
	struct reply_header __header;
	unsigned int    access_granted;
	unsigned int    access_status;
	unsigned int    privileges_len;
	/* VARARG(privileges,LUID_AND_ATTRIBUTES); */
};

struct get_token_user_request
{
	struct request_header __header;
	obj_handle_t    handle;
};

struct get_token_user_reply
{
	struct reply_header __header;
	data_size_t     user_len;
	/* VARARG(user,SID); */
};

struct get_token_groups_request
{
	struct request_header __header;
	obj_handle_t    handle;
};

struct get_token_groups_reply
{
	struct reply_header __header;
	data_size_t     user_len;
	/* VARARG(user,token_groups); */
};

struct set_security_object_request
{
	struct request_header __header;
	obj_handle_t    handle;
	unsigned int    security_info;
	/* VARARG(sd,security_descriptor); */
};

struct set_security_object_reply
{
	struct reply_header __header;
};

struct get_security_object_request
{
	struct request_header __header;
	obj_handle_t    handle;
	unsigned int    security_info;
};

struct get_security_object_reply
{
	struct reply_header __header;
	unsigned int    sd_len;
	/* VARARG(sd,security_descriptor); */
};

struct create_mailslot_request
{
	struct request_header __header;
	unsigned int   access;
	unsigned int   attributes;
	obj_handle_t   rootdir;
	unsigned int   max_msgsize;
	timeout_t      read_timeout;
	/* VARARG(name,unicode_str); */
};

struct create_mailslot_reply
{
	struct reply_header __header;
	obj_handle_t   handle;
};

struct set_mailslot_info_request
{
	struct request_header __header;
	obj_handle_t   handle;
	unsigned int   flags;
	timeout_t      read_timeout;
};

struct set_mailslot_info_reply
{
	struct reply_header __header;
	unsigned int   max_msgsize;
	timeout_t      read_timeout;
};

#define MAILSLOT_SET_READ_TIMEOUT  1

struct create_directory_request
{
	struct request_header __header;
	unsigned int   access;
	unsigned int   attributes;
	obj_handle_t   rootdir;
	/* VARARG(directory_name,unicode_str); */
};

struct create_directory_reply
{
	struct reply_header __header;
	obj_handle_t   handle;
};

struct open_directory_request
{
	struct request_header __header;
	unsigned int   access;
	unsigned int   attributes;
	obj_handle_t   rootdir;
	/* VARARG(directory_name,unicode_str); */
};

struct open_directory_reply
{
	struct reply_header __header;
	obj_handle_t   handle;
};

struct get_directory_entry_request
{
	struct request_header __header;
	obj_handle_t   handle;
	unsigned int   index;
};

struct get_directory_entry_reply
{
	struct reply_header __header;
	size_t         name_len;
	/* VARARG(name,unicode_str,name_len); */
	/* VARARG(type,unicode_str); */
};

struct create_symlink_request
{
	struct request_header __header;
	unsigned int   access;
	unsigned int   attributes;
	obj_handle_t   rootdir;
	data_size_t    name_len;
	/* VARARG(name,unicode_str,name_len); */
	/* VARARG(target_name,unicode_str); */
};

struct create_symlink_reply
{
	struct reply_header __header;
	obj_handle_t   handle;
};

struct open_symlink_request
{
	struct request_header __header;
	unsigned int   access;
	unsigned int   attributes;
	obj_handle_t   rootdir;
	/* VARARG(name,unicode_str); */
};

struct open_symlink_reply
{
	struct reply_header __header;
	obj_handle_t   handle;
};

struct query_symlink_request
{
	struct request_header __header;
	obj_handle_t   handle;
};

struct query_symlink_reply
{
	struct reply_header __header;
	/* VARARG(target_name,unicode_str); */
};

struct get_token_impersonation_level_request
{
	struct request_header __header;
	obj_handle_t   handle;
};

struct get_token_impersonation_level_reply
{
	struct reply_header __header;
	int            impersonation_level;
};

struct allocate_locally_unique_id_request
{
	struct request_header __header;
};

struct allocate_locally_unique_id_reply
{
	struct reply_header __header;
	luid_t         luid;
};

struct create_device_manager_request
{
	struct request_header __header;
	unsigned int access;
	unsigned int attributes;
};

struct create_device_manager_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct create_device_request
{
	struct request_header __header;
	unsigned int access;
	unsigned int attributes;
	obj_handle_t rootdir;
	obj_handle_t manager;
	void*        user_ptr;
	/* VARARG(name,unicode_str); */
};

struct create_device_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct delete_device_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct delete_device_reply
{
	struct reply_header __header;
};

struct get_next_device_request_request
{
	struct request_header __header;
	obj_handle_t manager;
	obj_handle_t prev;
	unsigned int status;
	/* VARARG(prev_data,bytes); */
};

struct get_next_device_request_reply
{
	struct reply_header __header;
	obj_handle_t next;
	ioctl_code_t code;
	void*        user_ptr;
	data_size_t  in_size;
	data_size_t  out_size;
	/* VARARG(next_data,bytes); */
};

struct make_process_system_request
{
	struct request_header __header;
};

struct make_process_system_reply
{
	struct reply_header __header;
	obj_handle_t event;
};

struct get_token_statistics_request
{
	struct request_header __header;
	obj_handle_t   handle;
};

struct get_token_statistics_reply
{
	struct reply_header __header;
	luid_t         token_id;
	luid_t         modified_id;
	int            primary;
	int            impersonation_level;
	int            group_count;
	int            privilege_count;
};

struct create_completion_request
{
	struct request_header __header;
	unsigned int access;
	unsigned int attributes;
	unsigned int concurrent;
	obj_handle_t rootdir;
	/* VARARG(filename,string); */
};

struct create_completion_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct open_completion_request
{
	struct request_header __header;
	unsigned int access;
	unsigned int attributes;
	obj_handle_t rootdir;
	/* VARARG(filename,string); */
};

struct open_completion_reply
{
	struct reply_header __header;
	obj_handle_t handle;
};

struct add_completion_request
{
	struct request_header __header;
	obj_handle_t  handle;
	unsigned long ckey;
	unsigned long cvalue;
	unsigned long information;
	unsigned int  status;
};

struct add_completion_reply
{
	struct reply_header __header;
};

struct remove_completion_request
{
	struct request_header __header;
	obj_handle_t handle;
};

struct remove_completion_reply
{
	struct reply_header __header;
	unsigned long ckey;
	unsigned long cvalue;
	unsigned long information;
	unsigned int  status;
};

struct query_completion_request
{
	struct request_header __header;
	obj_handle_t  handle;
};

struct query_completion_reply
{
	struct reply_header __header;
	unsigned int  depth;
};

struct set_completion_info_request
{
	struct request_header __header;
	obj_handle_t  handle;
	obj_handle_t  chandle;
	unsigned long ckey;
};

struct set_completion_info_reply
{
	struct reply_header __header;
};

struct add_fd_completion_request
{
	struct request_header __header;
	obj_handle_t   handle;
	unsigned long  cvalue;
	unsigned int   status;
	unsigned long  information;
};

struct add_fd_completion_reply
{
	struct reply_header __header;
};

struct load_init_registry_request
{
	struct request_header __header;
	/*FIXME */

	WCHAR * keyname;
	int keylen;
	char * filename;
	int filelen;
};

struct save_branch_request
{
	struct request_header __header;
	/*FIXME */
	int branch_num;
};

struct load_init_registry_reply 
{   
	struct reply_header __header;
	/*FIXME */
}; 

struct save_branch_reply
{   
	struct reply_header __header;
	/*FIXME */
}; 

struct async_set_result_request
{
	struct request_header __header;
	obj_handle_t          handle;
	obj_handle_t          event;
	unsigned long         cvalue;
	unsigned int          status;
	unsigned long         total;
	void                 *callback;
	void                 *arg1;
	void                 *arg2;
};

struct async_set_result_reply
{
	struct reply_header __header;
};

struct get_window_layered_info_request
{
    struct request_header __header;
    user_handle_t  handle;
};
struct get_window_layered_info_reply
{
    struct reply_header __header;
    unsigned int   color_key;
    unsigned int   alpha;
    unsigned int   flags;
    char __pad_20[4];
};


struct set_window_layered_info_request
{
    struct request_header __header;
    user_handle_t  handle;
    unsigned int   color_key;
    unsigned int   alpha;
    unsigned int   flags;
    char __pad_28[4];
};
struct set_window_layered_info_reply
{
    struct reply_header __header;
};

enum request
{
	REQ_new_process,
	REQ_get_new_process_info,
	REQ_new_thread,
	REQ_get_startup_info,
	REQ_init_process_done,
	REQ_init_thread,
	REQ_terminate_process,
	REQ_terminate_thread,
	REQ_get_process_info,
	REQ_set_process_info,
	REQ_get_thread_info,
	REQ_set_thread_info,
	REQ_get_dll_info,
	REQ_suspend_thread,
	REQ_resume_thread,
	REQ_load_dll,
	REQ_unload_dll,
	REQ_queue_apc,
	REQ_get_apc_result,
	REQ_close_handle,
	REQ_set_handle_info,
	REQ_dup_handle,
	REQ_open_process,
	REQ_open_thread,
	REQ_select,
	REQ_create_event,
	REQ_event_op,
	REQ_open_event,
	REQ_create_mutex,
	REQ_release_mutex,
	REQ_open_mutex,
	REQ_create_semaphore,
	REQ_release_semaphore,
	REQ_open_semaphore,
	REQ_create_file,
	REQ_open_file_object,
	REQ_alloc_file_handle,
	REQ_get_handle_fd,
	REQ_flush_file,
	REQ_lock_file,
	REQ_unlock_file,
	REQ_create_socket,
	REQ_accept_socket,
	REQ_register_accept_async,
	REQ_set_socket_event,
	REQ_get_socket_event,
	REQ_enable_socket_event,
	REQ_set_socket_deferred,
	REQ_alloc_console,
	REQ_free_console,
	REQ_get_console_renderer_events,
	REQ_open_console,
	REQ_get_console_wait_event,
	REQ_get_console_mode,
	REQ_set_console_mode,
	REQ_set_console_input_info,
	REQ_get_console_input_info,
	REQ_append_console_input_history,
	REQ_get_console_input_history,
	REQ_create_console_output,
	REQ_set_console_output_info,
	REQ_get_console_output_info,
	REQ_write_console_input,
	REQ_read_console_input,
	REQ_write_console_output,
	REQ_fill_console_output,
	REQ_read_console_output,
	REQ_move_console_output,
	REQ_send_console_signal,
	REQ_read_directory_changes,
	REQ_read_change,
	REQ_create_mapping,
	REQ_open_mapping,
	REQ_get_mapping_info,
	REQ_create_snapshot,
	REQ_next_process,
	REQ_next_thread,
	REQ_next_module,
	REQ_wait_debug_event,
	REQ_queue_exception_event,
	REQ_get_exception_status,
	REQ_output_debug_string,
	REQ_continue_debug_event,
	REQ_debug_process,
	REQ_debug_break,
	REQ_set_debugger_kill_on_exit,
	REQ_read_process_memory,
	REQ_write_process_memory,
	REQ_create_key,
	REQ_open_key,
	REQ_delete_key,
	REQ_flush_key,
	REQ_enum_key,
	REQ_set_key_value,
	REQ_get_key_value,
	REQ_enum_key_value,
	REQ_delete_key_value,
	REQ_load_registry,
	REQ_unload_registry,
	REQ_save_registry,
	REQ_set_registry_notification,
	REQ_create_timer,
	REQ_open_timer,
	REQ_set_timer,
	REQ_cancel_timer,
	REQ_get_timer_info,
	REQ_get_thread_context,
	REQ_set_thread_context,
	REQ_get_selector_entry,
	REQ_add_atom,
	REQ_delete_atom,
	REQ_find_atom,
	REQ_get_atom_information,
	REQ_set_atom_information,
	REQ_empty_atom_table,
	REQ_init_atom_table,
	REQ_get_msg_queue,
	REQ_set_queue_fd,
	REQ_set_queue_mask,
	REQ_get_queue_status,
	REQ_get_process_idle_event,
	REQ_send_message,
	REQ_post_quit_message,
	REQ_send_hardware_message,
	REQ_get_message,
	REQ_reply_message,
	REQ_accept_hardware_message,
	REQ_get_message_reply,
	REQ_set_win_timer,
	REQ_kill_win_timer,
	REQ_is_window_hung,
	REQ_get_serial_info,
	REQ_set_serial_info,
	REQ_register_async,
	REQ_cancel_async,
	REQ_ioctl,
	REQ_get_ioctl_result,
	REQ_create_named_pipe,
	REQ_get_named_pipe_info,
	REQ_create_window,
	REQ_destroy_window,
	REQ_get_desktop_window,
	REQ_set_window_owner,
	REQ_get_window_info,
	REQ_set_window_info,
	REQ_set_parent,
	REQ_get_window_parents,
	REQ_get_window_children,
	REQ_get_window_children_from_point,
	REQ_get_window_tree,
	REQ_set_window_pos,
	REQ_set_window_visible_rect,
	REQ_get_window_rectangles,
	REQ_get_window_text,
	REQ_set_window_text,
	REQ_get_windows_offset,
	REQ_get_visible_region,
	REQ_get_window_region,
	REQ_set_window_region,
	REQ_get_update_region,
	REQ_update_window_zorder,
	REQ_redraw_window,
	REQ_set_window_property,
	REQ_remove_window_property,
	REQ_get_window_property,
	REQ_get_window_properties,
	REQ_create_winstation,
	REQ_open_winstation,
	REQ_close_winstation,
	REQ_get_process_winstation,
	REQ_set_process_winstation,
	REQ_enum_winstation,
	REQ_create_desktop,
	REQ_open_desktop,
	REQ_close_desktop,
	REQ_get_thread_desktop,
	REQ_set_thread_desktop,
	REQ_enum_desktop,
	REQ_set_user_object_info,
	REQ_attach_thread_input,
	REQ_get_thread_input,
	REQ_get_last_input_time,
	REQ_get_key_state,
	REQ_set_key_state,
	REQ_set_foreground_window,
	REQ_set_focus_window,
	REQ_set_active_window,
	REQ_set_capture_window,
	REQ_set_caret_window,
	REQ_set_caret_info,
	REQ_set_hook,
	REQ_remove_hook,
	REQ_start_hook_chain,
	REQ_finish_hook_chain,
	REQ_get_hook_info,
	REQ_create_class,
	REQ_destroy_class,
	REQ_set_class_info,
	REQ_set_clipboard_info,
	REQ_open_token,
	REQ_set_global_windows,
	REQ_adjust_token_privileges,
	REQ_get_token_privileges,
	REQ_check_token_privileges,
	REQ_duplicate_token,
	REQ_access_check,
	REQ_get_token_user,
	REQ_get_token_groups,
	REQ_set_security_object,
	REQ_get_security_object,
	REQ_create_mailslot,
	REQ_set_mailslot_info,
	REQ_create_directory,
	REQ_open_directory,
	REQ_get_directory_entry,
	REQ_create_symlink,
	REQ_open_symlink,
	REQ_query_symlink,
	REQ_get_object_info,
	REQ_get_token_impersonation_level,
	REQ_allocate_locally_unique_id,
	REQ_create_device_manager,
	REQ_create_device,
	REQ_delete_device,
	REQ_get_next_device_request,
	REQ_make_process_system,
	REQ_get_token_statistics,
	REQ_create_completion,
	REQ_open_completion,
	REQ_add_completion,
	REQ_remove_completion,
	REQ_query_completion,
	REQ_set_completion_info,
	REQ_add_fd_completion,
	REQ_get_window_layered_info,
	REQ_set_window_layered_info,
	REQ_async_set_result,
	REQ_load_init_registry,
	REQ_save_branch,
	REQ_NB_REQUESTS
};

union generic_request
{
	struct request_max_size max_size;
	struct request_header request_header;
	struct new_process_request new_process_request;
	struct get_new_process_info_request get_new_process_info_request;
	struct get_startup_info_request get_startup_info_request;
	struct init_process_done_request init_process_done_request;
	struct init_thread_request init_thread_request;
	struct load_dll_request load_dll_request;
	struct unload_dll_request unload_dll_request;
	struct queue_apc_request queue_apc_request;
	struct get_apc_result_request get_apc_result_request;
	struct create_file_request create_file_request;
	struct open_file_object_request open_file_object_request;
	struct alloc_file_handle_request alloc_file_handle_request;
	struct get_handle_fd_request get_handle_fd_request;
	struct flush_file_request flush_file_request;
	struct lock_file_request lock_file_request;
	struct unlock_file_request unlock_file_request;
	struct create_socket_request create_socket_request;
	struct accept_socket_request accept_socket_request;
	struct register_accept_async_request register_accept_async_request;
	struct set_socket_event_request set_socket_event_request;
	struct get_socket_event_request get_socket_event_request;
	struct enable_socket_event_request enable_socket_event_request;
	struct set_socket_deferred_request set_socket_deferred_request;
	struct alloc_console_request alloc_console_request;
	struct free_console_request free_console_request;
	struct get_console_renderer_events_request get_console_renderer_events_request;
	struct open_console_request open_console_request;
	struct get_console_wait_event_request get_console_wait_event_request;
	struct get_console_mode_request get_console_mode_request;
	struct set_console_mode_request set_console_mode_request;
	struct set_console_input_info_request set_console_input_info_request;
	struct get_console_input_info_request get_console_input_info_request;
	struct append_console_input_history_request append_console_input_history_request;
	struct get_console_input_history_request get_console_input_history_request;
	struct create_console_output_request create_console_output_request;
	struct set_console_output_info_request set_console_output_info_request;
	struct get_console_output_info_request get_console_output_info_request;
	struct write_console_input_request write_console_input_request;
	struct read_console_input_request read_console_input_request;
	struct write_console_output_request write_console_output_request;
	struct fill_console_output_request fill_console_output_request;
	struct read_console_output_request read_console_output_request;
	struct move_console_output_request move_console_output_request;
	struct send_console_signal_request send_console_signal_request;
	struct read_directory_changes_request read_directory_changes_request;
	struct read_change_request read_change_request;
	struct create_mapping_request create_mapping_request;
	struct open_mapping_request open_mapping_request;
	struct get_mapping_info_request get_mapping_info_request;
	struct create_snapshot_request create_snapshot_request;
	struct next_process_request next_process_request;
	struct next_thread_request next_thread_request;
	struct next_module_request next_module_request;
	struct wait_debug_event_request wait_debug_event_request;
	struct queue_exception_event_request queue_exception_event_request;
	struct get_exception_status_request get_exception_status_request;
	struct output_debug_string_request output_debug_string_request;
	struct continue_debug_event_request continue_debug_event_request;
	struct debug_process_request debug_process_request;
	struct debug_break_request debug_break_request;
	struct set_debugger_kill_on_exit_request set_debugger_kill_on_exit_request;
	struct create_key_request create_key_request;
	struct open_key_request open_key_request;
	struct delete_key_request delete_key_request;
	struct flush_key_request flush_key_request;
	struct enum_key_request enum_key_request;
	struct set_key_value_request set_key_value_request;
	struct get_key_value_request get_key_value_request;
	struct enum_key_value_request enum_key_value_request;
	struct delete_key_value_request delete_key_value_request;
	struct load_registry_request load_registry_request;
	struct unload_registry_request unload_registry_request;
	struct save_registry_request save_registry_request;
	struct set_registry_notification_request set_registry_notification_request;
	struct create_timer_request create_timer_request;
	struct open_timer_request open_timer_request;
	struct set_timer_request set_timer_request;
	struct cancel_timer_request cancel_timer_request;
	struct get_timer_info_request get_timer_info_request;
	struct get_thread_context_request get_thread_context_request;
	struct set_thread_context_request set_thread_context_request;
	struct add_atom_request add_atom_request;
	struct delete_atom_request delete_atom_request;
	struct find_atom_request find_atom_request;
	struct get_atom_information_request get_atom_information_request;
	struct set_atom_information_request set_atom_information_request;
	struct empty_atom_table_request empty_atom_table_request;
	struct init_atom_table_request init_atom_table_request;
	struct get_msg_queue_request get_msg_queue_request;
	struct set_queue_fd_request set_queue_fd_request;
	struct set_queue_mask_request set_queue_mask_request;
	struct get_queue_status_request get_queue_status_request;
	struct get_process_idle_event_request get_process_idle_event_request;
	struct send_message_request send_message_request;
	struct post_quit_message_request post_quit_message_request;
	struct send_hardware_message_request send_hardware_message_request;
	struct get_message_request get_message_request;
	struct reply_message_request reply_message_request;
	struct accept_hardware_message_request accept_hardware_message_request;
	struct get_message_reply_request get_message_reply_request;
	struct set_win_timer_request set_win_timer_request;
	struct kill_win_timer_request kill_win_timer_request;
	struct is_window_hung_request is_window_hung_request;
	struct get_serial_info_request get_serial_info_request;
	struct set_serial_info_request set_serial_info_request;
	struct register_async_request register_async_request;
	struct cancel_async_request cancel_async_request;
	struct ioctl_request ioctl_request;
	struct get_ioctl_result_request get_ioctl_result_request;
	struct create_named_pipe_request create_named_pipe_request;
	struct get_named_pipe_info_request get_named_pipe_info_request;
	struct create_window_request create_window_request;
	struct destroy_window_request destroy_window_request;
	struct get_desktop_window_request get_desktop_window_request;
	struct set_window_owner_request set_window_owner_request;
	struct get_window_info_request get_window_info_request;
	struct set_window_info_request set_window_info_request;
	struct set_parent_request set_parent_request;
	struct get_window_parents_request get_window_parents_request;
	struct get_window_children_request get_window_children_request;
	struct get_window_children_from_point_request get_window_children_from_point_request;
	struct get_window_tree_request get_window_tree_request;
	struct set_window_pos_request set_window_pos_request;
	struct set_window_visible_rect_request set_window_visible_rect_request;
	struct get_window_rectangles_request get_window_rectangles_request;
	struct get_window_text_request get_window_text_request;
	struct set_window_text_request set_window_text_request;
	struct get_windows_offset_request get_windows_offset_request;
	struct get_visible_region_request get_visible_region_request;
	struct get_window_region_request get_window_region_request;
	struct set_window_region_request set_window_region_request;
	struct get_update_region_request get_update_region_request;
	struct update_window_zorder_request update_window_zorder_request;
	struct redraw_window_request redraw_window_request;
	struct set_window_property_request set_window_property_request;
	struct remove_window_property_request remove_window_property_request;
	struct get_window_property_request get_window_property_request;
	struct get_window_properties_request get_window_properties_request;
	struct create_winstation_request create_winstation_request;
	struct open_winstation_request open_winstation_request;
	struct close_winstation_request close_winstation_request;
	struct get_process_winstation_request get_process_winstation_request;
	struct set_process_winstation_request set_process_winstation_request;
	struct enum_winstation_request enum_winstation_request;
	struct create_desktop_request create_desktop_request;
	struct open_desktop_request open_desktop_request;
	struct close_desktop_request close_desktop_request;
	struct get_thread_desktop_request get_thread_desktop_request;
	struct set_thread_desktop_request set_thread_desktop_request;
	struct enum_desktop_request enum_desktop_request;
	struct set_user_object_info_request set_user_object_info_request;
	struct attach_thread_input_request attach_thread_input_request;
	struct get_thread_input_request get_thread_input_request;
	struct get_last_input_time_request get_last_input_time_request;
	struct get_key_state_request get_key_state_request;
	struct set_key_state_request set_key_state_request;
	struct set_foreground_window_request set_foreground_window_request;
	struct set_focus_window_request set_focus_window_request;
	struct set_active_window_request set_active_window_request;
	struct set_capture_window_request set_capture_window_request;
	struct set_caret_window_request set_caret_window_request;
	struct set_caret_info_request set_caret_info_request;
	struct set_hook_request set_hook_request;
	struct remove_hook_request remove_hook_request;
	struct start_hook_chain_request start_hook_chain_request;
	struct finish_hook_chain_request finish_hook_chain_request;
	struct get_hook_info_request get_hook_info_request;
	struct create_class_request create_class_request;
	struct destroy_class_request destroy_class_request;
	struct set_class_info_request set_class_info_request;
	struct set_clipboard_info_request set_clipboard_info_request;
	struct open_token_request open_token_request;
	struct set_global_windows_request set_global_windows_request;
	struct adjust_token_privileges_request adjust_token_privileges_request;
	struct get_token_privileges_request get_token_privileges_request;
	struct check_token_privileges_request check_token_privileges_request;
	struct duplicate_token_request duplicate_token_request;
	struct access_check_request access_check_request;
	struct get_token_user_request get_token_user_request;
	struct get_token_groups_request get_token_groups_request;
	struct set_security_object_request set_security_object_request;
	struct get_security_object_request get_security_object_request;
	struct create_mailslot_request create_mailslot_request;
	struct set_mailslot_info_request set_mailslot_info_request;
	struct create_directory_request create_directory_request;
	struct open_directory_request open_directory_request;
	struct get_directory_entry_request get_directory_entry_request;
	struct create_symlink_request create_symlink_request;
	struct open_symlink_request open_symlink_request;
	struct query_symlink_request query_symlink_request;
	struct get_token_impersonation_level_request get_token_impersonation_level_request;
	struct allocate_locally_unique_id_request allocate_locally_unique_id_request;
	struct create_device_manager_request create_device_manager_request;
	struct create_device_request create_device_request;
	struct delete_device_request delete_device_request;
	struct get_next_device_request_request get_next_device_request_request;
	struct make_process_system_request make_process_system_request;
	struct get_token_statistics_request get_token_statistics_request;
	struct create_completion_request create_completion_request;
	struct open_completion_request open_completion_request;
	struct add_completion_request add_completion_request;
	struct remove_completion_request remove_completion_request;
	struct query_completion_request query_completion_request;
	struct set_completion_info_request set_completion_info_request;
	struct add_fd_completion_request add_fd_completion_request;
	struct get_window_layered_info_request get_window_layered_info_request;
	struct set_window_layered_info_request set_window_layered_info_request;
	struct async_set_result_request async_set_result_request;
};
union generic_reply
{
	struct request_max_size max_size;
	struct reply_header reply_header;
	struct new_process_reply new_process_reply;
	struct get_new_process_info_reply get_new_process_info_reply;
	struct get_startup_info_reply get_startup_info_reply;
	struct init_process_done_reply init_process_done_reply;
	struct init_thread_reply init_thread_reply;
	struct load_dll_reply load_dll_reply;
	struct unload_dll_reply unload_dll_reply;
	struct queue_apc_reply queue_apc_reply;
	struct get_apc_result_reply get_apc_result_reply;
	struct create_file_reply create_file_reply;
	struct open_file_object_reply open_file_object_reply;
	struct alloc_file_handle_reply alloc_file_handle_reply;
	struct get_handle_fd_reply get_handle_fd_reply;
	struct flush_file_reply flush_file_reply;
	struct lock_file_reply lock_file_reply;
	struct unlock_file_reply unlock_file_reply;
	struct create_socket_reply create_socket_reply;
	struct accept_socket_reply accept_socket_reply;
	struct register_accept_async_reply register_accept_async_reply;
	struct set_socket_event_reply set_socket_event_reply;
	struct get_socket_event_reply get_socket_event_reply;
	struct enable_socket_event_reply enable_socket_event_reply;
	struct set_socket_deferred_reply set_socket_deferred_reply;
	struct alloc_console_reply alloc_console_reply;
	struct free_console_reply free_console_reply;
	struct get_console_renderer_events_reply get_console_renderer_events_reply;
	struct open_console_reply open_console_reply;
	struct get_console_wait_event_reply get_console_wait_event_reply;
	struct get_console_mode_reply get_console_mode_reply;
	struct set_console_mode_reply set_console_mode_reply;
	struct set_console_input_info_reply set_console_input_info_reply;
	struct get_console_input_info_reply get_console_input_info_reply;
	struct append_console_input_history_reply append_console_input_history_reply;
	struct get_console_input_history_reply get_console_input_history_reply;
	struct create_console_output_reply create_console_output_reply;
	struct set_console_output_info_reply set_console_output_info_reply;
	struct get_console_output_info_reply get_console_output_info_reply;
	struct write_console_input_reply write_console_input_reply;
	struct read_console_input_reply read_console_input_reply;
	struct write_console_output_reply write_console_output_reply;
	struct fill_console_output_reply fill_console_output_reply;
	struct read_console_output_reply read_console_output_reply;
	struct move_console_output_reply move_console_output_reply;
	struct send_console_signal_reply send_console_signal_reply;
	struct read_directory_changes_reply read_directory_changes_reply;
	struct read_change_reply read_change_reply;
	struct create_mapping_reply create_mapping_reply;
	struct open_mapping_reply open_mapping_reply;
	struct get_mapping_info_reply get_mapping_info_reply;
	struct create_snapshot_reply create_snapshot_reply;
	struct next_process_reply next_process_reply;
	struct next_thread_reply next_thread_reply;
	struct next_module_reply next_module_reply;
	struct wait_debug_event_reply wait_debug_event_reply;
	struct queue_exception_event_reply queue_exception_event_reply;
	struct get_exception_status_reply get_exception_status_reply;
	struct output_debug_string_reply output_debug_string_reply;
	struct continue_debug_event_reply continue_debug_event_reply;
	struct debug_process_reply debug_process_reply;
	struct debug_break_reply debug_break_reply;
	struct set_debugger_kill_on_exit_reply set_debugger_kill_on_exit_reply;
	struct create_key_reply create_key_reply;
	struct open_key_reply open_key_reply;
	struct delete_key_reply delete_key_reply;
	struct flush_key_reply flush_key_reply;
	struct enum_key_reply enum_key_reply;
	struct set_key_value_reply set_key_value_reply;
	struct get_key_value_reply get_key_value_reply;
	struct enum_key_value_reply enum_key_value_reply;
	struct delete_key_value_reply delete_key_value_reply;
	struct load_registry_reply load_registry_reply;
	struct unload_registry_reply unload_registry_reply;
	struct save_registry_reply save_registry_reply;
	struct set_registry_notification_reply set_registry_notification_reply;
	struct create_timer_reply create_timer_reply;
	struct open_timer_reply open_timer_reply;
	struct set_timer_reply set_timer_reply;
	struct cancel_timer_reply cancel_timer_reply;
	struct get_timer_info_reply get_timer_info_reply;
	struct get_thread_context_reply get_thread_context_reply;
	struct set_thread_context_reply set_thread_context_reply;
	struct add_atom_reply add_atom_reply;
	struct delete_atom_reply delete_atom_reply;
	struct find_atom_reply find_atom_reply;
	struct get_atom_information_reply get_atom_information_reply;
	struct set_atom_information_reply set_atom_information_reply;
	struct empty_atom_table_reply empty_atom_table_reply;
	struct init_atom_table_reply init_atom_table_reply;
	struct get_msg_queue_reply get_msg_queue_reply;
	struct set_queue_fd_reply set_queue_fd_reply;
	struct set_queue_mask_reply set_queue_mask_reply;
	struct get_queue_status_reply get_queue_status_reply;
	struct get_process_idle_event_reply get_process_idle_event_reply;
	struct send_message_reply send_message_reply;
	struct post_quit_message_reply post_quit_message_reply;
	struct send_hardware_message_reply send_hardware_message_reply;
	struct get_message_reply get_message_reply;
	struct reply_message_reply reply_message_reply;
	struct accept_hardware_message_reply accept_hardware_message_reply;
	struct get_message_reply_reply get_message_reply_reply;
	struct set_win_timer_reply set_win_timer_reply;
	struct kill_win_timer_reply kill_win_timer_reply;
	struct is_window_hung_reply is_window_hung_reply;
	struct get_serial_info_reply get_serial_info_reply;
	struct set_serial_info_reply set_serial_info_reply;
	struct register_async_reply register_async_reply;
	struct cancel_async_reply cancel_async_reply;
	struct ioctl_reply ioctl_reply;
	struct get_ioctl_result_reply get_ioctl_result_reply;
	struct create_named_pipe_reply create_named_pipe_reply;
	struct get_named_pipe_info_reply get_named_pipe_info_reply;
	struct create_window_reply create_window_reply;
	struct destroy_window_reply destroy_window_reply;
	struct get_desktop_window_reply get_desktop_window_reply;
	struct set_window_owner_reply set_window_owner_reply;
	struct get_window_info_reply get_window_info_reply;
	struct set_window_info_reply set_window_info_reply;
	struct set_parent_reply set_parent_reply;
	struct get_window_parents_reply get_window_parents_reply;
	struct get_window_children_reply get_window_children_reply;
	struct get_window_children_from_point_reply get_window_children_from_point_reply;
	struct get_window_tree_reply get_window_tree_reply;
	struct set_window_pos_reply set_window_pos_reply;
	struct set_window_visible_rect_reply set_window_visible_rect_reply;
	struct get_window_rectangles_reply get_window_rectangles_reply;
	struct get_window_text_reply get_window_text_reply;
	struct set_window_text_reply set_window_text_reply;
	struct get_windows_offset_reply get_windows_offset_reply;
	struct get_visible_region_reply get_visible_region_reply;
	struct get_window_region_reply get_window_region_reply;
	struct set_window_region_reply set_window_region_reply;
	struct get_update_region_reply get_update_region_reply;
	struct update_window_zorder_reply update_window_zorder_reply;
	struct redraw_window_reply redraw_window_reply;
	struct set_window_property_reply set_window_property_reply;
	struct remove_window_property_reply remove_window_property_reply;
	struct get_window_property_reply get_window_property_reply;
	struct get_window_properties_reply get_window_properties_reply;
	struct create_winstation_reply create_winstation_reply;
	struct open_winstation_reply open_winstation_reply;
	struct close_winstation_reply close_winstation_reply;
	struct get_process_winstation_reply get_process_winstation_reply;
	struct set_process_winstation_reply set_process_winstation_reply;
	struct enum_winstation_reply enum_winstation_reply;
	struct create_desktop_reply create_desktop_reply;
	struct open_desktop_reply open_desktop_reply;
	struct close_desktop_reply close_desktop_reply;
	struct get_thread_desktop_reply get_thread_desktop_reply;
	struct set_thread_desktop_reply set_thread_desktop_reply;
	struct enum_desktop_reply enum_desktop_reply;
	struct set_user_object_info_reply set_user_object_info_reply;
	struct attach_thread_input_reply attach_thread_input_reply;
	struct get_thread_input_reply get_thread_input_reply;
	struct get_last_input_time_reply get_last_input_time_reply;
	struct get_key_state_reply get_key_state_reply;
	struct set_key_state_reply set_key_state_reply;
	struct set_foreground_window_reply set_foreground_window_reply;
	struct set_focus_window_reply set_focus_window_reply;
	struct set_active_window_reply set_active_window_reply;
	struct set_capture_window_reply set_capture_window_reply;
	struct set_caret_window_reply set_caret_window_reply;
	struct set_caret_info_reply set_caret_info_reply;
	struct set_hook_reply set_hook_reply;
	struct remove_hook_reply remove_hook_reply;
	struct start_hook_chain_reply start_hook_chain_reply;
	struct finish_hook_chain_reply finish_hook_chain_reply;
	struct get_hook_info_reply get_hook_info_reply;
	struct create_class_reply create_class_reply;
	struct destroy_class_reply destroy_class_reply;
	struct set_class_info_reply set_class_info_reply;
	struct set_clipboard_info_reply set_clipboard_info_reply;
	struct open_token_reply open_token_reply;
	struct set_global_windows_reply set_global_windows_reply;
	struct adjust_token_privileges_reply adjust_token_privileges_reply;
	struct get_token_privileges_reply get_token_privileges_reply;
	struct check_token_privileges_reply check_token_privileges_reply;
	struct duplicate_token_reply duplicate_token_reply;
	struct access_check_reply access_check_reply;
	struct get_token_user_reply get_token_user_reply;
	struct get_token_groups_reply get_token_groups_reply;
	struct set_security_object_reply set_security_object_reply;
	struct get_security_object_reply get_security_object_reply;
	struct create_mailslot_reply create_mailslot_reply;
	struct set_mailslot_info_reply set_mailslot_info_reply;
	struct create_directory_reply create_directory_reply;
	struct open_directory_reply open_directory_reply;
	struct get_directory_entry_reply get_directory_entry_reply;
	struct create_symlink_reply create_symlink_reply;
	struct open_symlink_reply open_symlink_reply;
	struct query_symlink_reply query_symlink_reply;
	struct get_token_impersonation_level_reply get_token_impersonation_level_reply;
	struct allocate_locally_unique_id_reply allocate_locally_unique_id_reply;
	struct create_device_manager_reply create_device_manager_reply;
	struct create_device_reply create_device_reply;
	struct delete_device_reply delete_device_reply;
	struct get_next_device_request_reply get_next_device_request_reply;
	struct make_process_system_reply make_process_system_reply;
	struct get_token_statistics_reply get_token_statistics_reply;
	struct create_completion_reply create_completion_reply;
	struct open_completion_reply open_completion_reply;
	struct add_completion_reply add_completion_reply;
	struct remove_completion_reply remove_completion_reply;
	struct query_completion_reply query_completion_reply;
	struct set_completion_info_reply set_completion_info_reply;
	struct add_fd_completion_reply add_fd_completion_reply;
	struct get_window_layered_info_reply get_window_layered_info_reply;
	struct set_window_layered_info_reply set_window_layered_info_reply;
	struct async_set_result_reply async_set_result_reply;
};

#define SERVER_PROTOCOL_VERSION 341

#endif /* CONFIG_UNIFIED_KERNEL */
#endif /* _WINESERVER_UK_PROTOCOL_H */
