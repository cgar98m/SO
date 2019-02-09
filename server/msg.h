#ifndef _MSG_H
#define _MSG_H

#include <string.h>

#define MAX_TYPE 4
#define MAX_FILE 20

struct NewDataMsg {
	long id;
	char type[MAX_TYPE];
	long bytes;
	char file_name[MAX_FILE];
};

void cleanMsg(struct NewDataMsg * msg);

struct NewDataMsg defaultMsg();

struct NewDataMsg newMsg(long id, char type[MAX_TYPE], long bytes, char file_name[MAX_FILE]);

#endif