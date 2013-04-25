#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ieee1284.h>

struct parport pp;
struct parport_list pp_lst;

int init(){
  int result;
  result = ieee1284_find_ports(&pp_lst, 0);
  if(result != E1284_OK){
    printf("Error while listing parallel ports\n");
    return 1;
  }

  //Print the found parallel ports
  struct parport** pp_array;
  unsigned int pp_count = 0;
  pp_array = pp_lst.portv;
  while ((*pp_array) != 0){
    printf("Parallel port '%s':\n", (**pp_array).name);
    printf("  Base addr: 0x%lX\n", (**pp_array).base_addr);
    printf("   ECR addr: 0x%lX\n", (**pp_array).hibase_addr);
    printf("   filename: '%s'\n", (**pp_array).filename);
    pp_array++;
    pp_count++;
  }
  //Return if no ports found
  if(pp_count == 0){
    return 2;
  }
  pp = *(pp_lst.portv[0]);
  ///Select a device (@FIXME hardwired to port 0)
  char device_id[1024];
  result = ieee1284_get_deviceid(&pp, -1, 0, device_id, 1024);
  switch (result){
  case E1284_OK:
    printf("Device id retrieved.\n");
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
    return 6;
  }
  //Free the port list
  //ieee1284_free_ports(&pp_lst);
  //Enumerate capabilities
  printf("Port capabilities:\n");
  if (pp_capable & CAP1284_RAW)
    printf("  Pin-level access available\n");
  if (pp_capable & CAP1284_NIBBLE)
    printf("  Nibble mode available\n");
  if (pp_capable & CAP1284_BYTE)
    printf("  Byte mode available\n");
  if (pp_capable & CAP1284_COMPAT)
    printf("  Compatibility mode available\n");
  if (pp_capable & CAP1284_BECP)
    printf("  Bounded ECP available\n");
  if (pp_capable & CAP1284_ECP)
    printf("  ECP (hardware) available\n");
  if (pp_capable & CAP1284_ECPRLE)
  printf("  ECP mode supports RLE compression\n");
  if (pp_capable & CAP1284_ECPSWE)
    printf("  ECP (software) available\n");
  if (pp_capable & CAP1284_EPP)
    printf("  EPP (hardware) available\n");
  if (pp_capable & CAP1284_EPPSL)
    printf("  EPPSL ??\n");
  if (pp_capable & CAP1284_EPPSWE)
    printf("  EPP (software) available\n");
  if (pp_capable & CAP1284_IRQ)
    printf("  IRQ line configured\n");
  if (pp_capable & CAP1284_DMA)
    printf("  DMA channel configured\n");

  //Claim exclusive access to the port
  for(int i = 0; i<100; i++){
    result = ieee1284_claim(&pp);
    if (result != E1284_OK ){
      if (i==99){
        printf("Unable to get (exclusive?) access to port after %d retries\n", 100);
        return 7;
      }
      continue;
    }
    else{
      printf("Port access granted on try %d\n", i + 1);
      break;
    }
  }
  return 0;
}

void cleanup(){
  //Release port
  ieee1284_release(&pp);
  ieee1284_close(&pp);
  //Release port-list memory
  ieee1284_free_ports(&pp_lst);
}

void mill(){
  //Transfer some data
  char pp_buf[] = ";IN;CS6;CA8;IP0,0,30500,20500;LT;PA;\r\n\r\nPU;\r\nVS5;!VZ3;PU3826,11234;PD4108,11260;PD4116,11226;PD4124,11194;PD4134,11164;PD4146,11138;PD4160,11112;PD4176,11090;PD4192,11070;PD4212,11052;PD4232,11036;PD4254,11024;PD4278,11012;PD4302,11002;PD4330,10994;PD4358,10990;PD4388,10986;PD4422,10986;PD4454,10986;PD4486,10990;PD4516,10994;PD4544,11000;PD4568,11008;PD4592,11018;PD4612,11030;PD4632,11044;PD4648,11060;PD4662,11076;PD4674,11092;PD4684,11108;PD4692,11126;PD4698,11144;PD4702,11164;PD4702,11182;PD4702,11196;PD4700,11208;PD4698,11218;PD4696,11230;PD4690,11240;PD4686,11250;PD4680,11260;PD4672,11270;PD4664,11278;PD4656,11286;PD4644,11294;PD4632,11302;PD4618,11310;PD4604,11318;PD4586,11324;PD4568,11332;PD4554,11336;PD4536,11342;PD4512,11348;PD4486,11356;PD4454,11364;PD4420,11372;PD4380,11382;PD4338,11394;PD4282,11408;PD4230,11424;PD4184,11440;PD4140,11458;PD4102,11476;PD4068,11494;PD4038,11514;PD4012,11534;PD3982,11564;PD3954,11596;PD3932,11630;PD3914,11666;PD3898,11702;PD3888,11740;PD3882,11780;PD3880,11822;PD3882,11850;PD3884,11876;PD3890,11902;PD3896,11928;PD3904,11954;PD3916,11978;PD3928,12002;PD3942,12026;PD3958,12050;PD3976,12072;PD3996,12092;PD4018,12110;PD4042,12128;PD4066,12144;PD4092,12158;PD4120,12172;PD4150,12182;PD4182,12194;PD4216,12202;PD4250,12208;PD4286,12214;PD4322,12218;PD4362,12220;PD4402,12222;PD4468,12220;PD4528,12214;PD4586,12204;PD4638,12192;PD4686,12174;PD4732,12154;PD4772,12130;PD4808,12102;PD4840,12072;PD4868,12038;PD4892,12002;PD4912,11964;PD4928,11922;PD4940,11880;PD4946,11834;PD4950,11786;PD4662,11774;PD4658,11800;PD4650,11826;PD4642,11848;PD4634,11870;PD4622,11888;PD4610,11906;PD4598,11920;PD4582,11934;PD4566,11944;PD4548,11954;PD4528,11964;PD4506,11970;PD4482,11976;PD4458,11978;PD4430,11982;PD4400,11982;PD4370,11982;PD4340,11978;PD4314,11974;PD4288,11970;PD4264,11962;PD4242,11952;PD4222,11942;PD4204,11930;PD4192,11922;PD4184,11912;PD4176,11902;PD4168,11892;PD4164,11880;PD4160,11868;PD4158,11854;PD4158,11842;PD4158,11828;PD4160,11816;PD4164,11806;PD4168,11794;PD4174,11784;PD4182,11774;PD4190,11764;PD4200,11754;PD4216,11742;PD4238,11732;PD4264,11720;PD4294,11708;PD4330,11696;PD4370,11684;PD4414,11672;PD4466,11658;PD4516,11646;PD4564,11634;PD4608,11620;PD4648,11608;PD4686,11596;PD4718,11582;PD4750,11570;PD4776,11556;PD4800,11542;PD4824,11526;PD4846,11510;PD4866,11492;PD4884,11474;PD4902,11454;PD4918,11432;PD4934,11410;PD4948,11386;PD4958,11360;PD4968,11334;PD4976,11306;PD4982,11278;PD4988,11246;PD4990,11214;PD4990,11182;PD4990,11150;PD4986,11120;PD4982,11090;PD4974,11062;PD4964,11034;PD4952,11006;PD4938,10978;PD4922,10950;PD4904,10924;PD4884,10900;PD4864,10878;PD4840,10856;PD4814,10838;PD4788,10820;PD4760,10804;PD4730,10790;PD4696,10778;PD4662,10768;PD4626,10758;PD4588,10750;PD4548,10746;PD4506,10740;PD4464,10738;PD4418,10738;PD4352,10740;PD4290,10746;PD4232,10756;PD4178,10770;PD4128,10788;PD4082,10808;PD4038,10834;PD4000,10864;PD3966,10898;PD3936,10934;PD3908,10976;PD3884,11020;PD3864,11068;PD3848,11120;PD3836,11174;PD3826,11234;PD3826,11234;PU;";
  pp_buf[0] = 0x03;
  ssize_t buf_len = strlen(pp_buf);
  ssize_t written = 0;
  ssize_t block = 16;
  while(written != buf_len){
    written += ieee1284_ecp_write_data (&pp, 0, pp_buf + written, (buf_len - written) < block ? (buf_len - written) : block);
    if (((written % 100) == 0) || (written == buf_len)) printf("[%d/%d] -- %d%%\n", written, buf_len, (written * 100) / buf_len);
  }

}

int main(int argc, char** argv){
  if (init() == 0){
    printf("Initialization success!\n");
    mill();
    cleanup();
    return 0;
  }
  else{
    return -1;
  }
}
