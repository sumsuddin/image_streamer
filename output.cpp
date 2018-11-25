#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <pthread.h>
#include <syslog.h>

#include <linux/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>

#include "include/image_streamer.h"
#include "include/httpd.h"

#define LOG(...) { char _bf[1024] = {0}; snprintf(_bf, sizeof(_bf)-1, __VA_ARGS__); fprintf(stderr, "%s", _bf); syslog(LOG_INFO, "%s", _bf); }
/*
 * keep context for each server
 */
context server;

/*** plugin interface functions ***/
/******************************************************************************
Description.: Initialize this plugin.
              parse configuration parameters,
              store the parsed values in global variables
Input Value.: All parameters to work with.
              Among many other variables the "param->id" is quite important -
              it is used to distinguish between several server instances
Return Value: 0 if everything is OK, other values signal an error
******************************************************************************/
int output::init(globals *global)
{
    int  port;
    char *credentials, *hostname = NULL;

    port = htons(8877);
    credentials = NULL;


    server.pglobal = global;
    server.conf.port = port;
    server.conf.hostname = hostname;
    server.conf.credentials = credentials;

    OPRINT("HTTP TCP port........: %d\n", ntohs(port));
    OPRINT("HTTP Listen Address..: %s\n", hostname);
    OPRINT("username:password....: %s\n", (credentials == NULL) ? "disabled" : credentials);

    return 0;
}

/******************************************************************************
Description.: this will stop the server thread, client threads
              will not get cleaned properly, because they run detached and
              no pointer is kept. This is not a huge issue, because this
              funtion is intended to clean up the biggest mess on shutdown.
Input Value.: id determines which server instance to send commands to
Return Value: always 0
******************************************************************************/
int output_stop()
{
    pthread_cancel(server.threadID);

    return 0;
}

/******************************************************************************
Description.: This creates and starts the server thread
Input Value.: id determines which server instance to send commands to
Return Value: always 0
******************************************************************************/
int output_run()
{
    /* create thread and pass context to thread function */
    pthread_create(&(server.threadID), NULL, server_thread, &(server));
    pthread_detach(server.threadID);

    return 0;
}

int output::stop() {
    return output_stop();
}
int output::run() {
    return output_run();
}
