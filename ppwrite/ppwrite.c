#include "ppwrite.h"
#include <string.h>


/**
 * Initialize libieee1284 and claim a parallell port by name
 * @param  pp_name parallel port name to find and open, for example parport0
 * @return PP_OK on success
 */
int init(char* pp_name){
  int result;
  if(pp_name)
    printf("Initializing parallel port '%s'.\n", pp_name);
  else
    printf("Initializing parallel port.\n");
  result = ieee1284_find_ports(&pp_lst, 0);
  if(result != E1284_OK){
    printf("Error while listing parallel ports\n");
    return PP_ERROR;
  }

  //Search for the parallel port named pp_name
  struct parport** pp_array;
  unsigned int pp_count = 0;
  pp_array = pp_lst.portv;
  while ((*pp_array) != 0){
    printf("Parallel port '%s':\n", (**pp_array).name);
    printf("  Base addr: 0x%lX\n", (**pp_array).base_addr);
    printf("   ECR addr: 0x%lX\n", (**pp_array).hibase_addr);
    printf("   filename: '%s'\n", (**pp_array).filename);
    //If pp_name is null, we use the first port found
    if (pp_name == 0){
      pp = *(pp_lst.portv[0]);
      pp_count = 1;
      break;
    }
    //If not, we compare the names
    if (strcmp(pp_name, (**pp_array).name) == 0){
      pp = *(pp_lst.portv[0]);
      pp_count = 1;
      break;
    }
    pp_array++;
  }
  //Return if no ports found
  if(pp_count == 0){
    return PP_NOTFOUND;
  }

  //Try to get the device ID
  char device_id[1025];
  uint16_t device_id_len;
  result = ieee1284_get_deviceid(&pp, -1, 0, device_id, 1024);

  switch (result){
  case E1284_OK:
    printf("Device id retrieved:\n");
    device_id_len = device_id[0] * 256 + device_id[1];
    device_id_len = device_id_len < 1024-2 ? device_id_len+2 : 1024;
    device_id[device_id_len] = 0;
    printf("%s", device_id + 2);
    printf("\nEnd of device ID\n");
    break;
  case E1284_NOID:
    printf("[WARN] No ID provided\n");
    break;
  case E1284_NOTAVAIL:
    printf("[WARN] Unable to retrieve ID\n");
    break;
  case E1284_INIT:
    printf("[ERR!] Could not initialize port\n");
    return 4;
  default:
    printf("[ERR!] Error retrieving id (%d)\n", result);
    return 5;
  }

  //Try to open the port
  int pp_capable;
  result = ieee1284_open(&pp, 0, &pp_capable); //can use F1284_EXCL for eclusive access
  if (result != E1284_OK ){
    printf("[ERR!] Unable to open port\n");
    return PP_ERROR;
  }
  pp_has_ecp = 0;
  pp_has_epp = 0;
  pp_has_compat = 0;
  //Enumerate capabilities
  printf("Port capabilities:\n");
  if (pp_capable & CAP1284_RAW)
    printf("  Pin-level access available\n");
  if (pp_capable & CAP1284_NIBBLE)
    printf("  Nibble mode available\n");
  if (pp_capable & CAP1284_BYTE)
    printf("  Byte mode available\n");
  if (pp_capable & CAP1284_COMPAT){
    printf("  Compatibility mode available\n");
    pp_has_compat = 1;
  }
  if (pp_capable & CAP1284_BECP)
    printf("  Bounded ECP available\n");
  if (pp_capable & CAP1284_ECP){
    printf("  ECP (hardware) available\n");
    pp_has_ecp = 1;
  }
  if (pp_capable & CAP1284_ECPRLE)
  printf("  ECP mode supports RLE compression\n");
  if (pp_capable & CAP1284_ECPSWE){
    printf("  ECP (software) available\n");
    pp_has_ecp = 1;
  }
  if (pp_capable & CAP1284_EPP){
    printf("  EPP (hardware) available\n");
    pp_has_epp = 1;
  }
  if (pp_capable & CAP1284_EPPSL)
    printf("  EPPSL ??\n");
  if (pp_capable & CAP1284_EPPSWE){
    printf("  EPP (software) available\n");
    pp_has_epp = 1;
  }
  if (pp_capable & CAP1284_IRQ)
    printf("  IRQ line configured\n");
  if (pp_capable & CAP1284_DMA)
    printf("  DMA channel configured\n");

  //Claim access to the port
  for(int i = 0; i<100; i++){
    result = ieee1284_claim(&pp);
    if (result != E1284_OK ){
      if (i==99){
        printf("Unable to get (exclusive?) access to port after %d retries\n", 100);
        return PP_ERROR;
      }
      continue;
    }
    else{
      printf("Port access granted on try %d\n", i + 1);
      break;
    }
  }
  return PP_OK;
}

void cleanup(){
  //Release port
  ieee1284_release(&pp);
  ieee1284_close(&pp);
  //Release port-list memory
  ieee1284_free_ports(&pp_lst);
}

int mill(char* file_name, char mode){
  char buff[PP_FREAD_BUFFER];
  int  buff_size = 0;
  int  written = 0;
  //Check for mode availability
  if((mode == 'x') && (!pp_has_ecp)){
    printf("System does not support ECP mode.\n");
    return PP_ERROR;
  }
  if((mode == 'c') && (!pp_has_compat)){
    printf("System does not support compatibility (SPP) mode.\n");
    return PP_ERROR;
  }
  if((mode == 'e') && (!pp_has_epp)){
    printf("System does not support ECP mode.\n");
    return PP_ERROR;
  }
  //Open the file
  FILE* in_file;
  if (strlen(file_name) == 0)
    in_file = stdin;
  else{
    in_file = fopen(file_name, "rb");
    if(in_file == 0)
      return PP_ERROR;
  }
  buff_size = fread(buff, 1, PP_FREAD_BUFFER, in_file);
  while(buff_size != 0){
    switch(mode){
    case 'c':
      while(written != buff_size){
        written += ieee1284_compat_write (&pp, 0, buff + written, (buff_size - written) < PP_BLOCK_SIZE ? (buff_size - written) : PP_BLOCK_SIZE);
        //if (((written % 100) == 0) || (written == buf_len)) printf("[%d/%d] -- %d%%\n", written, buf_len, (written * 100) / buf_len);
      }
      return PP_ERROR;
    case 'x':
      while(written != buff_size){
        written += ieee1284_ecp_write_data (&pp, 0, buff + written, (buff_size - written) < PP_BLOCK_SIZE ? (buff_size - written) : PP_BLOCK_SIZE);
        //if (((written % 100) == 0) || (written == buf_len)) printf("[%d/%d] -- %d%%\n", written, buf_len, (written * 100) / buf_len);
      }
      break;
    case 'e':
      while(written != buff_size){
        written += ieee1284_epp_write_data (&pp, 0, buff + written, (buff_size - written) < PP_BLOCK_SIZE ? (buff_size - written) : PP_BLOCK_SIZE);
        //if (((written % 100) == 0) || (written == buf_len)) printf("[%d/%d] -- %d%%\n", written, buf_len, (written * 100) / buf_len);
      }
      return PP_ERROR;
    }
    buff_size = fread(buff, 1, PP_FREAD_BUFFER, in_file);
    written = 0;
  }
  //Transfer some data
//  char pp_buf[] = ";PU;";
//  pp_buf[0] = 0x03;
//  ssize_t buf_len = strlen(pp_buf);
//  ssize_t block = 16;
//  while(written != buf_len){
//    written += ieee1284_ecp_write_data (&pp, 0, pp_buf + written, (buf_len - written) < block ? (buf_len - written) : block);
//  }

}

int main(int argc, char** argv){
  char port_name [100] = "";
  char* port_name_p = port_name;
  char file_name [1024] = "";
  char mode = 'x';
  //
  // Parse the command line.
  //
  // ppwrite [-c | -e | -x] [-p<PORT_NAME>] file
  //
  // -p<PORT NAME>  Select port name
  // -c             Compatibility mode (SPP)
  // -e             Enhanced Parallel Port mode (EPP)
  // -x             Extended capability Port (ECP)
  //
  for(int i=1;i<argc;i++){
    if(argv[i][0] == '-'){
      if (argv[i][1] == 'p'){
        strcpy(port_name, argv[i] + 2);
      }
      if (argv[i][1] == 'c')
        mode = 'c';
      if (argv[i][1] == 'e')
        mode = 'e';
      if (argv[i][1] == 'x')
        mode = 'x';
      if (argv[i][1] == 'h'){
        printf("ppwrite [-c | -e | -x] [-p<PORT_NAME>] file\n");
        printf("\n");
        printf("-p<PORT NAME>  Select port name\n");
        printf("-c             Compatibility mode (SPP)\n");
        printf("-e             Enhanced Parallel Port mode (EPP)\n");
        printf("-x             Extended capability Port (ECP)\n");
        printf("-h             This text\n");
        return 0;
      }
    }
    else{
      strcpy(file_name, argv[i]);
    }
  }
  //Try to init the selected port
  if (port_name[0] == '\0')
    port_name_p = 0;
  if (init(port_name_p) == 0){
    printf("-------------------------\n");
    printf(" Initialization success!\n");
    printf(" Starting transfer\n");
    printf("-------------------------\n");
    if (mill(file_name, mode) == 0){
      printf("Transfer completed\n");
    }
    else{
      printf("Transfer error\n");
    }
    cleanup();
    return 0;
  }
  else{
    printf("Could not write '%s' into port '%s'\n", file_name, port_name);
    return -1;
  }
}
