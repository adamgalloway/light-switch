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
#include <parse.h>

#define LINE_BUFSIZE 128

void runCommand(const char *command) {
  char line[LINE_BUFSIZE];
  FILE *pipe;
    
  pipe = popen(command, "r");
  if (pipe == NULL) {
    printf("error opening pipe");
    return;
  }

  while (fgets(line, LINE_BUFSIZE, pipe) != NULL) {
    printf(line);
  }
    
  pclose(pipe);
}

void myPushCallback(ParseClient client, int error, const char *buffer) {
  if (error == 0 && buffer != NULL) {
    struct json_object *new_obj = json_tokener_parse(buffer);

    new_obj = json_object_object_get(new_obj, "data");

    new_obj = json_object_object_get(new_obj, "alert");
    const char *message = json_object_get_string(new_obj);

    if (strcmp(message, "start-lights") == 0) {
      printf("we gonna startem\n");
      runCommand("/home/pi/start-lights.sh");
    } else if (strcmp(message, "stop-lights") == 0) {
      printf("we gonna stopem\n");
      runCommand("/home/pi/stop-lights.sh");
    } else {
      printf("received unhandled push: '%n'\n", buffer);
    }

  }
}

int main(int argc, char *argv[]) {

  ParseClient client = parseInitialize("key", "key");
 
  parseSetPushCallback(client, myPushCallback);
  parseStartPushService(client);
 
  parseRunPushLoop(client);

  return 0;
}

