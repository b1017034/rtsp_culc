#include <stdio.h>
#include "rtsp/client.h"
#include "libpomp.h"

int main() {
  printf("Hello, World!\n");
  struct rtsp_client **client = NULL;
  struct pomp_loop *loop = pomp_loop_new();
  const struct rtsp_client_cbs *cbs;
  void userdata();

  rtsp_client_new(loop, "test", cbs, userdata, client);
  return 0;
}

void userdata(){

}
