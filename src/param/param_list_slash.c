/*
 * param_list_slash.c
 *
 *  Created on: Dec 14, 2016
 *      Author: johan
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <slash/slash.h>

#include <param/param.h>
#include <param/param_list.h>

static int list(struct slash *slash)
{
	if (slash->argc > 1)
		param_list_print(slash->argv[1]);
	else
		param_list_print(NULL);
	return SLASH_SUCCESS;
}
slash_command_sub(param, list, list, "[str]", "List parameters");

static int download(struct slash *slash)
{
	if (slash->argc < 2)
		return SLASH_EUSAGE;

	unsigned int node = atoi(slash->argv[1]);
	unsigned int timeout = 1000;
	if (slash->argc >= 3)
		timeout = atoi(slash->argv[2]);

	param_list_download(node, timeout);

	return SLASH_SUCCESS;
}
slash_command_sub(param, download, download, "<node> [timeout]", NULL);

static int to_string(struct slash *slash) {
	int node_filter = -1;
	if (slash->argc >= 2)
		node_filter = atoi(slash->argv[1]);

	param_list_to_string(stdout, node_filter, 0);
	return SLASH_SUCCESS;
}

slash_command_sub(param, serialize, to_string, "<node_filter>", NULL);
