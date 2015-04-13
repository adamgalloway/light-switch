/*
 *  Copyright (c) 2015, Parse, LLC. All rights reserved.
 *
 *  You are hereby granted a non-exclusive, worldwide, royalty-free license to use,
 *  copy, modify, and distribute this software in source code or binary form for use
 *  in connection with the web services and APIs provided by Parse.
 *
 *  As with any software that integrates with the Parse platform, your use of
 *  this software is subject to the Parse Terms of Service
 *  [https://www.parse.com/about/terms]. This copyright notice shall be
 *  included in all copies or substantial portions of the software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 *  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 *  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 *  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <json/json.h>
#include <stdio.h>
#include <libconfig.h>
#include <parse.h>

#define LINE_BUFSIZE 128

const char *parseAppId = NULL;
const char *parseKey = NULL;
const char *startScript = NULL;
const char *stopScript = NULL;

void runCommand(const char *command) {
  char line[LINE_BUFSIZE];
  FILE *pipe;
    
  pipe = popen(command, "r");
  if (pipe == NULL) {
    printf("error opening pipe");
    return;
  }

  // write results of script
  while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
    printf(line);
  }
    
  pclose(pipe);
}

void pushCallback(ParseClient client, int error, const char *buffer) {
  if (error == 0 && buffer != NULL) {
    struct json_object *new_obj = json_tokener_parse(buffer);

    new_obj = json_object_object_get(new_obj, "data");

    new_obj = json_object_object_get(new_obj, "alert");
    const char *message = json_object_get_string(new_obj);

    if (strcmp(message, "start-lights") == 0) {
      printf("we gonna startem\n");
      runCommand(startScript);
    } else if (strcmp(message, "stop-lights") == 0) {
      printf("we gonna stopem\n");
      runCommand(stopScript);
    } else {
      printf("received unhandled push: '%n'\n", buffer);
    }

  }
}

int main(int argc, char *argv[]) {

  config_t cfg, *cf;

  cf = &cfg;
  config_init(cf);

  if (!config_read_file(cf, "config.cfg")) {
    fprintf(stderr, "%s:%d - %s\n",
            config_error_file(cf),
            config_error_line(cf),
            config_error_text(cf));
    config_destroy(cf);
    return(EXIT_FAILURE);
  }

  if (!config_lookup_string(cf, "parseAppId", &parseAppId)) {
    fprintf(stderr, "error getting app id\n");
    return(EXIT_FAILURE);
  }

  if (!config_lookup_string(cf, "parseKey", &parseKey)) {
    fprintf(stderr, "error getting api key\n");
    return(EXIT_FAILURE);
  } 

  if (!config_lookup_string(cf, "startScript", &startScript)) {
    fprintf(stderr, "error getting start script path\n");
    return(EXIT_FAILURE);
  }

  if (!config_lookup_string(cf, "stopScript", &stopScript)) {
    fprintf(stderr, "error getting stop script path\n");
    return(EXIT_FAILURE);
  }

  ParseClient client = parseInitialize(parseAppId, parseKey);
  
  parseSetPushCallback(client, pushCallback);
  parseStartPushService(client);
 
  parseRunPushLoop(client);

  config_destroy(cf);

  return 0;
}

