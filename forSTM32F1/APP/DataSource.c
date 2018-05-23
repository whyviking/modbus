#include "DataSource.h"
#include "Driver.h"
#include "config.h"
#include "ucos_ii.h"
#include "mylib.h"
#include "app.h"
#include "ups_config.h"
#include <stdio.h>
#if 	   Modbus_mod
 uint8_t mod_flag=0;
#endif

__align(8) static  OS_STK   TaskPort1DHStk[TASK_PORT1DH_STK_SIZE];
__align(8) static  OS_STK   TaskPort2DHStk[TASK_PORT2DH_STK_SIZE]; 
__align(8) static  OS_STK   TaskPort1TXStk[TASK_PORT1TX_STK_SIZE];
__align(8) static  OS_STK   TaskPort2TXStk[TASK_PORT2TX_STK_SIZE];  
Inquire_t inquiremap1[50]             = {0x00};
Inquire_t inquiremap2[50]             = {0x00};
OS_EVENT *ups1_cmd_sem;
OS_EVENT *ups2_cmd_sem;
uint8_t Analog_cmdnum[2]={0,0};
uint8_t Digital_cmdnum[2]={0,0};
uint8_t   maplength[2]                = {0,0};
uint8_t   sendindex[2]                = {0,0};

uint8_t   comstate[DEV_NUM]           = {0x00};

uint16_t  Analog_Buff[2][32]     = {0x00};              // 存放 ups 模拟量

uint8_t   Digital_Buff[2][DIGITAL_NUM]   = {0x00};              // 存放 ups 状态量
uint16_t DIGITAL_addr[2][100];  // 02  地址
uint16_t DIGITAL_addr_num[2][100];  // //包含了几个数据

uint16_t DIGITAL_readnum[2][100];  // 02  地址 读取数量
uint8_t DIGITAL_datanum[2][100];  // 02  地址 包含数据个数
uint8_t DIGITAL_errnum[2][100];   //每个开关量发生次数
uint8_t DIGITAL_right[2][100];   //每个开关量正常状态
uint8_t DIGITAL_stat[2][100];   //每个开关量状态
#define DIGITAL_errnum_MAX  1
uint8_t PORT_ADDR[2]={1,2};
uint8_t DIGITAL_OR[100];
uint8_t DIGITAL_total;
//综合判断



void TaskPort1DH(void *p_arg);
void TaskPort2DH(void *p_arg);
void TaskPort1TX(void *p_arg);
void TaskPort2TX(void *p_arg);
uint8_t InsertMap(uint8_t comid, uint8_t devaddr, uint8_t cmmd, uint16_t startaddr, uint16_t num)
{
    Inquire_t *p      = 0x00;
    uint8_t   *length = 0x00;    
    switch(comid)
    {
        case PORT_1_ADDR:
            length = &maplength[0];
            p = &inquiremap1[*length];
            break;
        case PORT_2_ADDR:
            length = &maplength[1];
            p = &inquiremap2[*length];
            break; 
        default:
            break;
    }    
    if(p == 0x00)
    {
        return 0;
    }    
    p->addr      = devaddr;
    p->cmmd      = cmmd;
    p->startaddr = my_ltob(startaddr-1);
    p->num       = my_ltob(num);    
    p->crc       = my_crccheck((uint8_t *)p, 6);   
    (*length)++;   
    return 0;
}
//  排序
void sequence_map (void )
{
	  uint16_t temp[4]={0,0,0};
	  uint8_t i=0,j=0;
	  while(Digital_map[j][0]>0)	     
	  {
	  		j++;	  
	  }
	  for(;j>0;j--)
	  {
				for(i=0;i<(j-1);i++)
				{
					 if(  Digital_map[i][0]> Digital_map[i+1][0] )
					 {
							temp[0]=Digital_map[i][0];
							temp[1]=Digital_map[i][1];
							temp[2]=Digital_map[i][2];					 
							Digital_map[i][0]=Digital_map[i+1][0];
							Digital_map[i][1]=Digital_map[i+1][1];
							Digital_map[i][2]=Digital_map[i+1][2];								 
							Digital_map[i+1][0]=temp[0];
							Digital_map[i+1][1]=temp[1];
							Digital_map[i+1][2]=temp[2];						 
					 }		  		  
				}		 
	  }
}

uint8_t OR_NUM(uint8_t j)
{
   uint8_t i=0;
	 while(Digital_map[j][0]>0)
	 {
		 while(Digital_map[i][0]>0)
		 {
				if(Digital_map[j][1]==Digital_map[i][1]) 
				{
					DIGITAL_OR[j]++;
				}
				i++;					
		 }		 
		 j++;
		 i=0;
   }
	 DIGITAL_total=j;
	 return 0;
	
}


void CreatDataSourceTask(void)
{
	uint8_t i = 0x00,port=0,j=0;
	ups1_cmd_sem= OSSemCreate(0); 
  ups2_cmd_sem= OSSemCreate(0);
	sequence_map(); //排序	
  OR_NUM(0);     //提区开关量或逻辑的个数
   i=0;
	 while(Analog_map[i][0]>0)
	 {
		 InsertMap(PORT_ADDR[port],Modbus_addr[port],COMMD_ANALONG,Analog_map[i][0],Analog_map[i][1]);
		 Analog_cmdnum[port]++;
		 i++;
	 }
	 i=0;
	 DIGITAL_addr[port][j]=Digital_map[i][0];  //每条命令的起始地址
	 DIGITAL_addr_num[port][j]=i;             //起始的序号
	 while(Digital_map[i][0]>0)
	 {
		  if(((Digital_map[i][0]-DIGITAL_addr[port][j])+1)<64 )
		  {			
		     DIGITAL_datanum[port][j]++;
			   if(Digital_map[i+1][0]==0)
			   {
					  DIGITAL_readnum[port][j]=(Digital_map[i][0]-DIGITAL_addr[port][j])+1;
			   	  InsertMap(PORT_ADDR[port], Modbus_addr[port], COMMD_DIGITAL, DIGITAL_addr[port][j], DIGITAL_readnum[port][j]);
			   		break;
			   }
         i++;				 
		  }
		  else
		  {
				  DIGITAL_readnum[port][j]=(Digital_map[i-1][0]-DIGITAL_addr[port][j])+1;
		      InsertMap(PORT_ADDR[port], Modbus_addr[port], COMMD_DIGITAL, DIGITAL_addr[port][j], DIGITAL_readnum[port][j]);	
		      j++;	
			    DIGITAL_addr[port][j]=Digital_map[i][0];  //下一帧起始地址
			    DIGITAL_addr_num[port][j]=i;		  	    //下一帧起始序号		  	     
		  }	 	 	 	  	
	 }
	 port=1;
	 i=0;
	 j=0;
	 while(Analog_map[i][0]>0)
	 {
		 InsertMap(PORT_ADDR[port],Modbus_addr[port],COMMD_ANALONG,Analog_map[i][0],Analog_map[i][1]);
		 Analog_cmdnum[port]++;
		 i++;
	 }
	 i=0;
	 DIGITAL_addr[port][j]=Digital_map[i][0];
	 DIGITAL_addr_num[port][j]=i;
	 while(Digital_map[i][0]>0)
	 {

		  if(((Digital_map[i][0]-DIGITAL_addr[port][j])+1)<64 )
		  {
			
		     DIGITAL_datanum[port][j]++;
			   if(Digital_map[i+1][0]==0)
			   {
					  DIGITAL_readnum[port][j]=(Digital_map[i][0]-DIGITAL_addr[port][j])+1;
			   	  InsertMap(PORT_ADDR[port], Modbus_addr[port], COMMD_DIGITAL, DIGITAL_addr[port][j], DIGITAL_readnum[port][j]);
			   		break;
			   }	
			   i++;	 
		  }
		  else
		  {
				  DIGITAL_readnum[port][j]=(Digital_map[i-1][0]-DIGITAL_addr[port][j])+1;
		      InsertMap(PORT_ADDR[port], Modbus_addr[port], COMMD_DIGITAL, DIGITAL_addr[port][j], DIGITAL_readnum[port][j]);	
		      j++;	
			    DIGITAL_addr[port][j]=Digital_map[i][0];  //下一帧起始地址
			    DIGITAL_addr_num[port][j]=i;		  	    //下一帧起始序号
		  	     
		  }	 	 
	 	 	
	 }

    OSTimeDlyHMSM(0, 0, 0, 50);



    OSTaskCreate (TaskPort1DH,        
                  (void *)0,
                  &TaskPort1DHStk[TASK_PORT1DH_STK_SIZE - 1],
                  TASK_PORT1DH_PRIO);
                  
    OSTaskCreate (TaskPort2DH,        
                  (void *)0,
                  &TaskPort2DHStk[TASK_PORT2DH_STK_SIZE - 1],
                  TASK_PORT2DH_PRIO);

    OSTaskCreate (TaskPort1TX,        
                  (void *)0,
                  &TaskPort1TXStk[TASK_PORT1TX_STK_SIZE - 1],
                  TASK_PORT1TX_PRIO);
                  
    OSTaskCreate (TaskPort2TX,        
                  (void *)0,
                  &TaskPort2TXStk[TASK_PORT2TX_STK_SIZE - 1],
                  TASK_PORT2TX_PRIO);
}    

void digital_set(uint8_t addr)
{ 
		uint8_t temp=0,temp1=0,i,offset=0;	
		for(i=0;i<DIGITAL_total;i++)
		{
			 temp= (Digital_map[i][1]-1)%8;
			 temp1=0x01<<temp;	  
			 offset=(Digital_map[i][1]-1)/8;
			 if(DIGITAL_stat[addr][i]>0)  //报警
			 {				 
					//报警超时判断
					if(DIGITAL_errnum[addr][i]>DIGITAL_errnum_MAX)   //报警超时判断						 
					{
						 Digital_Buff[addr][offset]|=temp1;
					}else
					{
						 DIGITAL_errnum[addr][i]++;    
					}		
			 }else
			 {
					 temp=~temp1;					 
					 if(DIGITAL_right[addr][Digital_map[i][1]]>=DIGITAL_OR[i]) 
					 {
						 Digital_Buff[addr][offset]&=temp;	
					 }					 
					 DIGITAL_errnum[addr][i]=0; //报警超时判断			 
			 }
		}
		for(i=0;i<36;i++)
		{
		  DIGITAL_right[addr][i]=0;
		}	
		for(i=0;i<DIGITAL_total;i++)
		{		
			DIGITAL_stat[addr][i]=0;
		}

}
void DigitalDataHandle_PLUS(uint8_t comid, uint8_t *src)
{
      uint8_t temp=0,addr=4,j,i;

      if(comid == PORT_1_ADDR)  addr=0;
	    if(comid == PORT_2_ADDR)  addr=1; 
	
	  j=sendindex[addr]-Analog_cmdnum[addr];
	  for(i=0;i<DIGITAL_datanum[addr][j] ;i++)
	  {			       
	  	 temp= (Digital_map[DIGITAL_addr_num[addr][j]+i][0]-DIGITAL_addr[addr][j]);
		   if(( (src[ 3+temp/8 ] >>  (temp%8) )&0x01	)>0)
			 {					 
					 if(Digital_map[DIGITAL_addr_num[addr][j]+i][2]>0) 
					 {		
                DIGITAL_stat[addr][DIGITAL_addr_num[addr][j]+i]=0;	
						    DIGITAL_right[addr][Digital_map[DIGITAL_addr_num[addr][j]+i][1]]++;   //实现或逻辑				
					 }
					 else //报警
					 {	
						   DIGITAL_stat[addr][DIGITAL_addr_num[addr][j]+i]=1;						 
					 }			 
			 }				     
			 else
			 {			   			 
					if(Digital_map[DIGITAL_addr_num[addr][j]+i][2]>0)  //报警
					{					
                DIGITAL_stat[addr][DIGITAL_addr_num[addr][j]+i]=1;					
					}
					else
					{
						   DIGITAL_stat[addr][DIGITAL_addr_num[addr][j]+i]=0;	
               DIGITAL_right[addr][Digital_map[DIGITAL_addr_num[addr][j]+i][1]]++;  //正确次数递增
					}								
		   }				
	  }
 }


void AnalongDataHandle(uint8_t comid, uint8_t *src)
{
    uint8_t  i      = 0x00,addr;
    uint8_t  length = 0x00;
    uint8_t  index  = 0x00;
    uint16_t *p     = (uint16_t *)&src[3];   
    switch(comid)
    {
        case PORT_1_ADDR:
             index = Analog_map[sendindex[0]][2];
			       addr=0;
						 if(sendindex[0]==0)
						 {
							 //清空当前发送buffer
//							 for(i=0;i<3;i++)
//							 Digital_Buff[0][i]=0;
//							 for(i=0;i<32;i++)
//							 Analog_Buff[0][i]=0;

							 
						 }
             break;
        case PORT_2_ADDR:
	           addr=1;
             index =  Analog_map[sendindex[1]][2];
						 if(sendindex[1]==0)
						 {
							//清空当前buffer
//							 for(i=0;i<3;i++)
//							 Digital_Buff[1][i]=0;
//							 for(i=0;i<32;i++)
//							 Analog_Buff[1][i]=0;
						 
						 }
             break; 
        default:
             break;
    } 	  
    length = src[2]/2;
    for(i=0; i<length; i++)                                 // Inquire/Report R,S,T (Out)
    {					
			Analog_Buff[addr][index -1] = my_ltob(p[i])*Analog_map_coefficient[Analog_map[sendindex[addr]][3]];
		  index++;
		}	     
}


/*
*********************************************************************************************************
*                                        校验接收到的数据
*
* Description: 验证接收数据，主要验证长度，校验和，以及地址
*
* Arguments  : *p 接收缓冲区指针
*
* Returns    : status 验证通过返回 0，不通过返回错误代码
*********************************************************************************************************
*/
uint8_t RxCheck(Rx_t *p)
{
    uint8_t  status = ENOERR;

    if(p == 0x00)
    {
        status |= EBUFFNULL;
        return status;
    }
    
    if(*(uint16_t *)(p->buff + p->buff[2] + 3) != my_crccheck(p->buff, p->buff[2] + 3))
    {
        status |= ECHECKERR;
        return status;
    }
    
	return status;
}

/*
*********************************************************************************************************
*                                        校验接收到的数据
*
* Description: 验证接收数据，主要验证长度，校验和，以及地址
*
* Arguments  : *p 接收缓冲区指针
*
* Returns    : status 验证通过返回 0，不通过返回错误代码
*********************************************************************************************************
*/
void TaskPort1DH(void *p_arg)
{
	uint8_t  err        = 0x00;
	Rx_t     *p         = 0x00;
	
	while(1)
	{
		p = OSMboxPend(rx_mbox_port[1], 0, &err);   // 等待	
		if(RxCheck(p))
		{
		   	OSSemPost(ups1_cmd_sem); 
			continue;
		}
		
        comstate[0]++;

		switch(p->buff[1])
		{
			case COMMD_DIGITAL:
                DigitalDataHandle_PLUS(p->comid, p->buff);
				break;
			case COMMD_ANALONG:
                AnalongDataHandle(p->comid, p->buff);
				break;
			default:
				break;
		}


		OSSemPost(ups1_cmd_sem); 
	}
}

/*
*********************************************************************************************************
*                                        校验接收到的数据
*
* Description: 验证接收数据，主要验证长度，校验和，以及地址
*
* Arguments  : *p 接收缓冲区指针
*
* Returns    : status 验证通过返回 0，不通过返回错误代码
*********************************************************************************************************
*/
void TaskPort2DH(void *p_arg)
{
	uint8_t  err        = 0x00;
	Rx_t     *p         = 0x00;
	
	while(1)
	{
		p = OSMboxPend(rx_mbox_port[2], 0, &err);   // 等待	
		if(RxCheck(p))
		{
			OSSemPost(ups2_cmd_sem); 
			
			continue;
		}
        comstate[1]++;
		
		switch(p->buff[1])
		{
			case COMMD_DIGITAL:
                DigitalDataHandle_PLUS(p->comid, p->buff);
				break;
			case COMMD_ANALONG:
                AnalongDataHandle(p->comid, p->buff);
				break;
			default:
				break;
		}
			OSSemPost(ups2_cmd_sem); 
	}
}




void TaskPort1TX(void *p_arg)
{
	uint8_t i=0,err;


    while(1)
    {
        if(maplength[0] == 0x00)
        {
            OSTimeDlyHMSM(0, 0, 1, 0);
        }       
        for(i=0; i<maplength[0]; i++)
        {
            OSTimeDlyHMSM(0, 0, 0, 100);
            sendindex[0] = i;
            USART_Send(PORT_1, (uint8_t *)&inquiremap1[i], sizeof(Inquire_t));
		        OSSemPend(ups1_cmd_sem, 250, &err);   // 等待信号 						
        }
				digital_set(0);
    }
}
void TaskPort2TX(void *p_arg)
{
	uint8_t  i=0,err;
    while(1)
    {
        if(maplength[1] == 0x00)
        {
            OSTimeDlyHMSM(0, 0, 1, 0);
        }
        for(i=0; i<maplength[1]; i++)
        {
            OSTimeDlyHMSM(0, 0, 0, 100);
            sendindex[1] = i;
            USART_Send(PORT_2, (uint8_t *)&inquiremap2[i], sizeof(Inquire_t));
			      OSSemPend(ups2_cmd_sem, 250, &err);   // 等待信号 
        }				
        digital_set(1);
    }
}

void ComError(uint8_t id)
{
    uint8_t i     = 0x00;


    for(i=0; i<32; i++)     //模拟量清零
    {
        Analog_Buff[id][i] = 0x00;

    }   
    Digital_Buff[id][0]  = 0x01; 
		Digital_Buff[id][1]  = 0x00;
		Digital_Buff[id][2]  = 0x00;

     
}


void GetCom(uint8_t *dat)
{
    dat[0] = comstate[0];
    dat[1] = comstate[1];

}

void SetCom(uint8_t id)
{

  comstate[id] = 0x00;
	Digital_Buff[id][0]  &= 0xfe;

}

uint8_t GetAnalogData(uint16_t *dst)
{   

    uint8_t i;
    for(i=0; i<32; i++)
    {
        dst[i] = Analog_Buff[0][i];

    }
     for(i=32; i<64; i++)
    {
        dst[i] = Analog_Buff[1][i-32];
    }   
    return 0;
}

uint8_t GetDigitalData(uint8_t *dst)
{
    uint8_t i = 0;
    for(i=0; i<3; i++)
    {
        dst[i] = Digital_Buff[0][i];
    }
    for(i=3; i<6; i++)
    {
        dst[i] = Digital_Buff[1][i-3];
    }
    
    return 0;
}
// 合肥1 特别版  36开关量
//uint8_t GetDigitalData(uint8_t *dst)
//{
//    uint8_t i = 0;


//    for(i=0; i<3; i++)
//    {
//        dst[i] = Digital_Buff[0][i];
//    }
//		dst[3]=0;
//		dst[4]=0x00;
//		dst[5]=0x00;
//		dst[6]=0x00;
//		dst[7]=0x00;
//		dst[4]|=(Digital_Buff[1][0]<<4);  //36位开始		
//		dst[5]|=(Digital_Buff[1][0]>>4);
//	  dst[5]|=(Digital_Buff[1][1]<<4);	
//	  dst[6]|=(Digital_Buff[1][1]>>4);			
//	  dst[6]|=(Digital_Buff[1][2]<<4);			
//    dst[7]|=(Digital_Buff[1][2]>>4);	
//    
//    return 0;
//}



 uint8_t GetUpsSta(UP_Data_t *d)
 {
	//判断供电状态
	if((d->switc[1]&0x04)==1)
	{
	   	 d->ups_sta[0]=0x30;	
	} else	   d->ups_sta[0]=0x31;



	if((d->switc[4]&0x04)==1)
	{
	   	 d->ups_sta[1]=0x30;	
	} else	   d->ups_sta[1]=0x31;

    //判断其他故障

	if((d->switc[0]>0||(d->switc[1]&(~0x04))>0||d->switc[3])>0)
	{
		 d->ups_sta[0]=0x32;		
	}

 	if((d->switc[3]>0||(d->switc[4]&(~0x04))>0||d->switc[5])>0)
	{
		 d->ups_sta[1]=0x32;		
	}


 	
 	//判断通信故障
    if(	(d->switc[0]&0x01) ==1  )
	{
	     d->ups_sta[0]=0x33; 	    
	}


     if((d->switc[3]&0x01) ==1  )
	{
	     d->ups_sta[1]=0x33; 	     
	}


	return 0;

 }

