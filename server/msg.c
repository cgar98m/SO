#include "msg.h"

void cleanMsg(struct NewDataMsg * msg) {
	msg->id = -1;
	msg->type[MAX_TYPE - 1] = '\0';
	msg->bytes = 0;
	msg->file_name[MAX_FILE - 1] = '\0';
}

struct NewDataMsg defaultMsg() {
	struct NewDataMsg msg;
	msg.id = -1;
	msg.type[0] = '\0';
	msg.bytes = 0;
	msg.file_name[0] = '\0';
	return msg;
}

struct NewDataMsg newMsg(long id, char type[MAX_TYPE], long bytes, char file_name[MAX_FILE]) {
	struct NewDataMsg msg;
	msg.id = id;
	strcpy(msg.type, type);
	msg.bytes = bytes;
	strcpy(msg.file_name, file_name);
	return msg;
}