void dot_matrix_clear();

//unsigned char  test_eeprom[0x100];
unsigned char  test_eeprom[0x20];

//test IR
//unsigned int test_IR_tmr[0x250];
//unsigned int test_IR_flag=0x05;
///////

//test remocon data
unsigned char IR_data[20];
unsigned char IR_data_flag=0;


#define SCL 		PORTD_Bit0
#define SDA_Out 		PORTD_Bit1
#define SDA_ST 		DDRD_Bit1
#define SDA_In 		PIND_Bit1



#define Low 0
#define High 1

#define BUSY 1
#define IDLE 0

#define CLK 16000000
#define PRESCALE 8
#define TIME_US 100
#define TIMER0_CNT ( CLK * TIME_US / (8 * 1000000) )

enum PORT128 {PPCA=1,PPCB,PPCC,PPCD,PPCE,PPCF,PPCG};

#define HIGH 1
#define LOW 0
#define OFF 1
#define ON 0

#define	U8	unsigned char
#define	U16	unsigned int
#define	U32	unsigned long
#define S32  long

//EEPROM
//rom_add_pt : 주소데이터의 시작 주소(1Byte)
//rom_cycle : 주소데이터의 반복 횟수(2Byte)
//rom_address : 데이터의 주소(2Bytes)
//rom_data_cnt : 데이터의 카운트(1Byte)
//rom_data : 데이터(총 2Bytes: 볼륨 8bits, 채널 4bits, 필터 3bits, 위상 1bit)
/*
U8 rom_add_pt_low=0;
U8 rom_add_pt_high=0;
U8 rom_cnt_low=0;
U8 rom_cnt_mid=0;
U8 rom_cnt_high=0;
*/
U16 rom_add_pt=0;
U32 rom_cnt=0;
U8 rom_cnt_high=0;
U8 rom_cnt_mid=0;
U8 rom_cnt_low=0;
U8 rom_func_data=0;
U8 rom_add_pt_high=0;
U8 rom_add_pt_low=0;
U8 rom_device_high=0;
U8 rom_device_low=0;
U8 rom_add_check_sum=0;
//U16 rom_cycle=0;
//U16 rom_address=0;
//U8 rom_data_cnt=0;
//U16 rom_data=0;

//U8 pEEPROM_addr=0;		//current address
//U16 pEEPROM_num=0;		//write number
//U8 pEEPROM_data=1;		//current data,   //1=led off,     0=led on

U8 phase_data=1;

U8 interrupt_eicra=0xB5;
U8 init_setting_check=0;    //include remocon interrupt,
//U32 test_yu=0;

U8 ess_automute=0;
U8 ess_lock_ck=0;
U8 old_ess_automute=0;
U8 old_ess_lock_ck=0;
U8 exMute=0;
U8 old_exMute=0;
U16 key_tmr=0;
U8 tmr_osc_ck=0;
//U8 tmr_osc_ck2=0;
U8 key_int_flag=0;
U16 rom_tmr=0;
U8 rom_save_flag=1;
U8 rom_save_flag2=1;

//U8 key_enable=1;
//U8 remocon_enable=1;



#define DOT_DATA	PORTA

//#define POWER_SW  	PORTB_Bit6
//#define DAC_POWER_SW  	PORTB_Bit7

#define M2_DOT_ENABLE  	PORTC_Bit7

#define DOT_CLR_FLAG	PORTD_Bit7
#define KEY_DATA2       PIND_Bit6
#define KEY_DATA1       PIND_Bit5
#define KEY_DATA0       PIND_Bit4
#define KEY_FLAG        PIND_Bit3

#define AUTOMUTE        PINE_Bit7
#define ESS_LOCK        PINE_Bit6
#define I2S_SEL		PORTE_Bit3

#define DOT_ADDR0	PORTF_Bit0
#define DOT_ADDR1	PORTF_Bit1
#define DOT_ADDR2	PORTF_Bit2
#define DOT_ADDR3	PORTF_Bit3

#define DOT_WR		PORTG_Bit2
#define M1_DOT_ENABLE	PORTG_Bit1
#define DOT_ADDR4	PORTG_Bit0

//timer
U8 Time_500us=0;
U16 Time_50ms=0;

//display
U8 dot_light_reg=1;
U8 ch_led_data=0;		//ch0(coax1)
U8 ch_led_enable=1;
U8 sr_led_data=6;                 //sr = 0
U8 display_num=100;
//U32 audio_level=0x7fffffff;
//U8 led_start_flag=0;


//volume
U8 ess_lch_master_trim=0;
U8 ess_rch_master_trim=0;
U8 vol_dB=0xff;
//U8 ch0_vol_dB=0;
//U8 ch1_vol_dB=0;
//U8 ch2_vol_dB=0;
//U8 ch3_vol_dB=0;
//U8 ch4_vol_dB=0;
//U8 ch5_vol_dB=0;
//U8 ch6_vol_dB=0;
//U8 ch7_vol_dB=0;

U8 i2s_flag=1;
U8 usb_sr=48; 		//USB 48=48kHz affiliation,  44=44.1kHz affiliation

U8 button_data=0;
U8 filter_flag=0;
U8 mute_enable=1;
U8 key_func=0;
U16 key_func_tmr=0;
U8 key_condition=0;


//es9018
U8 es9018_reg10=0xce;		//0xcf = mute, 0xce = unmute
U8 es9018_lock=0;
U8 es9018_lock_flag=0;
U16 dac_audio_timer=0;
U16 sample_rate=0;
U16 sample_rate2=0;
/*
U32 max38=0xd000000;

U32 min44=0xEB00000;
U32 max44=0xEC00000;

U32 min48=0xF000000;
U32 max48=0x10A00000;

U32 min88=0x1D000000;
U32 max88=0x1E000000;

U32 min96=0x1F000000;
U32 max96=0x20A00000;

U32 min176=0x3A000000;
U32 max176=0x3BA00000;

U32 min192=0x3F000000;
U32 max192=0x40A00000;

U32 min352=0x4F000000;


U16 min44=0x907;
U16 max44=0x909;

U16 min48=0x9D3;
U16 max48=0x9D5;

U16 min88=0x120F;
U16 max88=0x1211;

U16 min96=0x13A8;
U16 max96=0x13AA;

U16 min176=0x241F;
U16 max176=0x2421;

U16 min192=0x2751;
U16 max192=0x2753;

*/

U16 min44=0x8CE;
U16 max44=0x937;

U16 min48=0x9A0;
U16 max48=0xA09;

U16 min88=0x11D1;
U16 max88=0x123A;

U16 min96=0x1374;
U16 max96=0x13DD;

U16 min176=0x23D7;
U16 max176=0x243F;

U16 min192=0x271D;
U16 max192=0x2786;

U8 tmr_osc=0;

//CS8416
U8 interface_ch=0x80;
U16 cs_audio_timer=0;

//XMOS
U16 usb_audio_timer=0;


U8 KeyReady=1;

U8 I2C_error=0;

U8 ch_name[8][6] = {
  "COAX1 ",
  "COAX2 ",
  " AES1 ",
  " AES2 ",
  " BNC  ",
  " OPT1 ",
  " OPT2 ",
  " USB  "
};
U8 sr_name[7][6] = {
  " 44.1 ",
  " 48.0 ",
  " 88.2 ",
  " 96.0 ",
  "176.4 ",
  "192.0 ",
  "      "
};
U8 filter_name[3][8] = {
  "FILTER1 ",
  "FILTER2 ",
  "FILTER3 "
};
U8 phase_name[8]={" INVERSE"};
U8 normal_name[8]={"  NORMAL"};

U8 ch_data;
U8 sr_data;
U8 filter_data;
U8 inverse_data;

U8 dot_string[16];
U8 ess_volume[4];


//sr_minphase_stage1 & sr_minphase_stage2,  Slow Roll-off
S32 f1_coeff_st1[64]={
0
,1656381
,9659303
,34409768
,93398781
,209942799
,407444422
,699022478
,1074874807
,1492352005
,1874902824
,2124488166
,2147483647
,1887806213
,1355685389
,639107586
,-110755555
,-719588677
,-1044031603
,-1020728165
,-690754479
,-187854371
,307259942
,630274318
,690105478
,497041076
,151573293
,-200494787
,-427342979
,-460267547
,-313535895
,-69239327
,163487939
,295508341
,290781514
,173357845
,8625355
,-129830628
,-191689217
,-166581017
,-81244643
,18370414
,89760574
,110702807
,84163673
,31575802
,-20180401
,-51101617
,-54545855
,-36486473
,-9671202
,13456521
,25681054
,26272207
,19136308
,9640102
,1930723
,-2245673
,-3249107
,-2468805
,-1263225
,-407167
,0
,0
};

S32 f1_coeff_st2[16]={
1475681183
,1475681183
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
};          

//sr_linphase_stage1 & sr_linphase_stage2,  Slow Roll-off
S32 f2_coeff_st1[64]={
0
,-1801695
,-4008643
,-5121013
,-2214480
,5996574
,16606227
,22319443
,15460455
,-5254559
,-30080590
,-41104893
,-23628756
,20097715
,65638314
,77510493
,33703685
,-51685218
,-126749606
,-129504345
,-32841829
,122779073
,237746372
,208859258
,4955037
,-285795146
,-476804265
,-374025116
,107471796
,869797120
,1652694820
,2147483647
,2147483647
,1652694820
,869797120
,107471796
,-374025116
,-476804265
,-285795146
,4955037
,208859258
,237746372
,122779073
,-32841829
,-129504345
,-126749606
,-51685218
,33703685
,77510493
,65638314
,20097715
,-23628756
,-41104893
,-30080590
,-5254559
,15460455
,22319443
,16606227
,5996574
,-2214480
,-5121013
,-4008643
,-1801695
,0
};

S32 f2_coeff_st2[16]={
2147400132
,2147400132
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
};


void DelayTime(U16 time_end)
{
   while(time_end--);
}

void DelayTime_ms(U16 time_end){  //msec
  U8 i;
  for(i=0; i<=time_end+1; i++) DelayTime(16000);  //1msec
}

