#include <stdio.h>
#include "rtsp/client.h"
#include "libpomp.h"

int main() {
  printf("Hello, World!\n");
  struct rtsp_client *client = NULL;
  struct pomp_loop *loop = pomp_loop_new();
  const struct rtsp_client_cbs *cbs;
  const char *local_path = "127.0.0.1:8533/test";
  void userdata();

  rtsp_client_new(loop, "test", cbs, userdata, &client);
  rtsp_client_describe(client, local_path, userdata, 100);
  return 0;
}

void userdata(){

}
