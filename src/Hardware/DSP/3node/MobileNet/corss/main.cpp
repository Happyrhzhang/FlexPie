#include "datatrans.h" //datatrans-sensenet-common/commondef

void core_work();
extern "C" {
#include "SRIO/SRIO_Test.h"
}

extern volatile Uint32 RecvNum;
int main()
{
	uint32_t ui_core_num = CSL_chipReadReg(CSL_CHIP_DNUM);

	if(ui_core_num == 0)
	{
		KeyStone_common_CPU_init();
		/*print device information.
		Enable memory protection interrupts, EDC for MSMC RAM*/
//		KeyStone_common_device_init();

		//enable exception handling
		KeyStone_Exception_cfg(TRUE);

#if 1 	/*for debug only. Without these code you need reset DSP before your rerun the program*/
	//disable SRIO through PSC
	KeyStone_disable_PSC_module(CSL_PSC_PD_SRIO, CSL_PSC_LPSC_SRIO);
	KeyStone_disable_PSC_Power_Domain(CSL_PSC_PD_SRIO);

#endif

		SRIOInit(DeviceId);
	}


	if (ui_core_num == 0)
	{
		CACHE_setL1PSize(CACHE_L1_32KCACHE);
		CACHE_setL1DSize(CACHE_L1_32KCACHE);
		CACHE_setL2Size(CACHE_0KCACHE);
		CACHE_invAllL1p(CACHE_WAIT);
		//CGEM_regs->MAR[179]=0;

	    int i;
	    for(i = 128;i<256;i++){
	    	CACHE_enableCaching(i);
	    }
	    CACHE_setMemRegionInfo(12,1,1);
	}
	else
	{
		CACHE_setL1PSize(CACHE_L1_32KCACHE);
		CACHE_setL1DSize(CACHE_L1_32KCACHE);
		CACHE_setL2Size(CACHE_0KCACHE);
		CACHE_invAllL1p(CACHE_WAIT);
//		//CGEM_regs->MAR[179]=0;

	    int i;
	    for(i = 128;i<256;i++){
	    	CACHE_enableCaching(i);
	    }
	    CACHE_setMemRegionInfo(12,1,1);
	}
	//CACHE_disableCaching(141);
	CACHE_disableCaching(138);
	core_work();
    return 0;
}
void core_work()
{
	int i_frame = 0;
	int step = 0;

#if CALC_TOTAL_TIME
    long long StartTime = 0;
    long long EndTime = 0;
    TSCH = 0;
    TSCL = 0;
#endif

    uint32_t ui_core_num = CSL_chipReadReg(CSL_CHIP_DNUM);
    sensenet_t* s = sensenet_open(ui_core_num);

	if (ui_core_num == 0)
	{
		printf("core 0 ready!\n");
	}

	//for (i_frame = 0; true; i_frame++)
	{
		for (step = 0; step < 29; step++)
		{
			//1-8核同步工作控制
			if (ui_core_num == 0)
			{

				WaitAllCoreReady(s);
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
//				printf("0:%d\n",step);
//				TSC_delay_ms(1000);
				switch (step)
				{
					case 0:
						//处理第1个卷积层
						int offset;
//						if (DeviceId == 0)
//						{
							tiling_input_0(s->io->input_image1, s->io->tiling_input, 0, 0, 0);
//						}
//						else if (DeviceId == 3)
//						{
//							offset = img_width[0] * 111;
//							tiling_input_n(s->io->input_image1, s->io->tiling_input, offset, 0, 3);
//						}
//						else if(DeviceId == 1)
//						{
//							offset = 111;
//							tiling_input_1_1(s->io->input_image1, s->io->tiling_input, offset, 0, 1);
//						}
//						else if(DeviceId == 2)
//						{
//							offset = 111*224+111;
//							tiling_input_1_2(s->io->input_image1, s->io->tiling_input, offset, 0, 2);
//						}
						break;
//					case 1:
//						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 56, 56, 32/8);//后面四个数字需要每层核对 input ouput
//						break;
//					case 3:
//						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 56, 56, 64/8);//后面四个数字需要每层核对
//						break;
//					case 5:
//						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 28, 28, 128/8);//后面四个数字需要每层核对
//						break;
//					case 7:
//						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 28, 28, 128/8);//后面四个数字需要每层核对
//						break;
//					case 9:
//						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 14, 14, 256/8);//后面四个数字需要每层核对
//						break;
//					case 11:
//						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 14, 14, 256/8);//后面四个数字需要每层核对
//						break;
//					case 13:
//						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 7, 7, 512/8);//后面四个数字需要每层核对
//						break;
//					case 15:
//						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 7, 7, 512/8);//后面四个数字需要每层核对
//						break;
//					case 17:
//						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 7, 7, 512/8);//后面四个数字需要每层核对
//						break;
//					case 19:
//						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 7, 7, 512/8);//后面四个数字需要每层核对
//						break;
//					case 21:
//						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 7, 7, 512/8);//后面四个数字需要每层核对
//						break;
//					case 23:
//						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 7, 7, 512/8);//后面四个数字需要每层核对
//						break;
//					case 25:
//						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, img_tiling[step][DeviceId][0]-5, img_tiling[step][DeviceId][1]-5, 1024/8);//这层不太对
//						break;
//					default:
//						break;

					case 1:
						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 112/2, img_tiling[step][DeviceId][1]-2, 32/8);//后面四个数字需要每层核对 input ouput
						break;
					case 3:
						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 112/2, img_tiling[step][DeviceId][1]-2, 64/8);//后面四个数字需要每层核对
						break;
					case 5:
						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 56/2, img_tiling[step][DeviceId][1]-2, 128/8);//后面四个数字需要每层核对
						break;
					case 7:
						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 56/2, img_tiling[step][DeviceId][1]-2, 128/8);//后面四个数字需要每层核对
						break;
					case 9:
						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 28/2, img_tiling[step][DeviceId][1]-2, 256/8);//后面四个数字需要每层核对
						break;
					case 11:
						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 28/2, img_tiling[step][DeviceId][1]-2, 256/8);//后面四个数字需要每层核对
						break;
					case 13:
						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 14/2, img_tiling[step][DeviceId][1]-2, 512/8);//后面四个数字需要每层核对
						break;
					case 15:
						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 14/2, img_tiling[step][DeviceId][1]-2, 512/8);//后面四个数字需要每层核对
						break;
					case 17:
						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 14/2, img_tiling[step][DeviceId][1]-2, 512/8);//后面四个数字需要每层核对
						break;
					case 19:
						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 14/2, img_tiling[step][DeviceId][1]-2, 512/8);//后面四个数字需要每层核对
						break;
					case 21:
						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 14/2, img_tiling[step][DeviceId][1]-2, 512/8);//后面四个数字需要每层核对
						break;
					case 23:
						readdata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_output, 1, 14/2, img_tiling[step][DeviceId][1]-2, 512/8);//后面四个数字需要每层核对
						break;
					case 25:
						readdata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_input, 1, 7/2+1, img_tiling[step][DeviceId][1]-2, 1024/8);//这层不太对
						break;
					default:
						break;
				}

//				printf("1:%d\n",step);
//				TSC_delay_ms(1000);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("houchuli,core : %d step : %d houchuli time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif

				SetAllCoreStart(s);
#if CALC_TOTAL_TIME
				if (i_frame == 0 && step == 0)
				{
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
			}
			else
			{
				SetCoreReadyFlag(s);
				WaitCoreStartFlag(s);
			}
			//8核的工作
			if (step == 0)
			{
				LoadWeightBiasFromDDRToL2Channel_r1(s);
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				conv_3x3_outputgroup_L2(s->io->tiling_input,s->weight->weight1,s->bias->bias1,s->io->tiling_output,0,s->ui_core, 2, 0);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 1)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_input,s->weight->weight2,s->bias->bias2,s->io->tiling_output,1,s->ui_core, 3,1,1, 1);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 2)
			{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif

				dw_conv_1x1_outputgroup_L2(s->io->tiling_output,s->weight->weight3,s->bias->bias3,s->io->tiling_input,2,s->ui_core, 1, 2);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 3)
			{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_output,s->weight->weight4,s->bias->bias4,s->io->tiling_input,3,s->ui_core, 3,2,0, 3);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 4)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif

				dw_conv_1x1_outputgroup_L2(s->io->tiling_input,s->weight->weight5,s->bias->bias5,s->io->tiling_output,4,s->ui_core, 1, 4);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 5)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif

				dw_conv_3x3_inputgroup_L2(s->io->tiling_input,s->weight->weight6,s->bias->bias6,s->io->tiling_output,5,s->ui_core, 3,1,1, 5);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 6)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->tiling_output,s->weight->weight7,s->bias->bias7,s->io->tiling_input,6,s->ui_core, 1, 6);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 7)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif

				dw_conv_3x3_inputgroup_L2(s->io->tiling_output,s->weight->weight8,s->bias->bias8,s->io->tiling_input,7,s->ui_core, 3,2,0,7);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 8)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif

				dw_conv_1x1_outputgroup_L2(s->io->tiling_input,s->weight->weight9,s->bias->bias9,s->io->tiling_output,8,s->ui_core, 1,8);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 9)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_input,s->weight->weight10,s->bias->bias10,s->io->tiling_output,9,s->ui_core, 3,1,1,9);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 10)
			{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->tiling_output,s->weight->weight11,s->bias->bias11,s->io->tiling_input,10,s->ui_core, 1,10);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif

			}
			else if (step == 11)
			{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_output,s->weight->weight12,s->bias->bias12,s->io->tiling_input,11,s->ui_core, 3,2,0,11);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 12)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->tiling_input,s->weight->weight13,s->bias->bias13,s->io->tiling_output,12,s->ui_core, 1,12);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 13)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_input,s->weight->weight14,s->bias->bias14,s->io->tiling_output,13,s->ui_core, 3,1,1,13);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif

			}
			else if (step == 14)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->tiling_output,s->weight->weight15,s->bias->bias15,s->io->tiling_input,13,s->ui_core, 1,14);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 15)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_output,s->weight->weight16,s->bias->bias16,s->io->tiling_input,13,s->ui_core, 3,1,1,15);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if(step == 16)
			{
#if test_time
			long long StartTime = 0;
			long long EndTime = 0;
			if(s->ui_core == CORENUM)
			{
				TSCH = 0;
				TSCL = 0;
				StartTime = _itoll(TSCH, TSCL);
			}
#endif

			dw_conv_1x1_outputgroup_L2(s->io->tiling_input,s->weight->weight17,s->bias->bias17,s->io->tiling_output,13,s->ui_core, 1,16);
#if test_time
			if(s->ui_core == CORENUM)
			{
				EndTime = _itoll(TSCH, TSCL);
				printf("banyuncore : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
			}
#endif
			}
			else if (step == 17)
			{
				LoadWeightBiasFromDDRToL2Channel_r2(s);
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_input,s->weight->weight18,s->bias->bias18,s->io->tiling_output,13,s->ui_core, 3,1,1,17);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif

			}
			else if (step == 18)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->tiling_output,s->weight->weight19,s->bias->bias19,s->io->tiling_input,13,s->ui_core, 1,18);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif

			}
			else if (step == 19)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_output,s->weight->weight20,s->bias->bias20,s->io->tiling_input,13,s->ui_core, 3,1,1,19);

#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 20)
			{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->tiling_input,s->weight->weight21,s->bias->bias21,s->io->tiling_output,13,s->ui_core, 1,20);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 21)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_input,s->weight->weight22,s->bias->bias22,s->io->tiling_output,13,s->ui_core, 3,1,1,21);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 22)
			{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->tiling_output,s->weight->weight23,s->bias->bias23,s->io->tiling_input,13,s->ui_core, 1,22);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 23)
			{
				LoadWeightBiasFromDDRToL2Channel_r3(s);
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_output,s->weight->weight24,s->bias->bias24,s->io->tiling_input,14,s->ui_core, 3,2,0,23);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 24)
			{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->tiling_input,s->weight->weight25,s->bias->bias25,s->io->tiling_output,15,s->ui_core, 1,24);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 25)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_3x3_inputgroup_L2(s->io->tiling_input,s->weight->weight26,s->bias->bias26,s->io->tiling_output,16,s->ui_core, 3,2,4,25);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 26)
			{
				//LoadWeightBiasFromDDRToL2Channel_r4(s);
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->tiling_output,s->weight->weight27,s->bias->bias27,s->io->tiling_input,17,s->ui_core, 1,26);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif

			}
			else if (step == 27)
			{
				LoadWeightBiasFromDDRToL2Channel_r5(s);
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				dw_conv_1x1_outputgroup_L2(s->io->tiling_output,s->weight->weight28,s->bias->bias28,s->io->tiling_input + img_tiling[26][DeviceId][0]*img_tiling[26][DeviceId][1]*output_channel[18],18,s->ui_core, 1,27);
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}
			else if (step == 28)
			{

#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
				globalavg_pool_group(s->io->tiling_input,s->io->tiling_output,19,s->ui_core, 28);//没有汇总！！！！！！！！！！
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("core : %d step : %d total time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
			}

			//各核step结束后执行的工作
			if (ui_core_num == 0)
			{
				while(IsAllCoreDone(s) == false)
				{
					asm("  nop 9");
				}

//				printf("2:%d\n",step);
//				TSC_delay_ms(1000);
				if (step == 28)
				{

#if CALC_TOTAL_TIME
					EndTime = _itoll(TSCH, TSCL);
					printf("total time:%f ms\n", (EndTime - StartTime) / 1000000.0);
#endif
				}
				//ring
#if ring
				if(step <= 24 && step%2 == 0)
				{
#if test_time
				long long StartTime = 0;
				long long EndTime = 0;
				if(s->ui_core == CORENUM)
				{
					TSCH = 0;
					TSCL = 0;
					StartTime = _itoll(TSCH, TSCL);
				}
#endif
//				switch (step)
//				{
//					case 0:
//						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 56, 56, 32);//后面四个数字需要每层核对
//						break;
//					case 2:
//						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 56, 56, 64);
//						break;
//					case 4:
//						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 28, 28, 128);
//						break;
//					case 6:
//						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 28, 28, 128);
//						break;
//					case 8:
//						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 14, 14, 256);
//						break;
//					case 10:
//						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 14, 14, 256);
//						break;
//					case 12:
//						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 7, 7, 512);
//						break;
//					case 14:
//						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 7, 7, 512);
//						break;
//					case 16:
//						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 7, 7, 512);
//						break;
//					case 18:
//						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 7, 7, 512);
//						break;
//					case 20:
//						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 7, 7, 512);
//						break;
//					case 22:
//						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 7, 7, 512);
//						break;
//					case 24:
//						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, img_tiling[step+1][DeviceId][0]-5, img_tiling[step+1][DeviceId][1]-5, 1024);
//						break;
////					case 27:
////						//to big 没有汇总
////						break;
//					default:
//						break;
//				}


				switch (step)
				{
					case 0:
						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 112, img_tiling[step+1][DeviceId][1]-2, 32);//后面四个数字需要每层核对
						break;
					case 2:
						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 112, img_tiling[step+1][DeviceId][1]-2, 64);
						break;
					case 4:
						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 56, img_tiling[step+1][DeviceId][1]-2, 128);
						break;
					case 6:
						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 56, img_tiling[step+1][DeviceId][1]-2, 128);
						break;
					case 8:
						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 28, img_tiling[step+1][DeviceId][1]-2, 256);
						break;
					case 10:
						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 28, img_tiling[step+1][DeviceId][1]-2, 256);
						break;
					case 12:
						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 14, img_tiling[step+1][DeviceId][1]-2, 512);
						break;
					case 14:
						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 14, img_tiling[step+1][DeviceId][1]-2, 512);
						break;
					case 16:
						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 14, img_tiling[step+1][DeviceId][1]-2, 512);
						break;
					case 18:
						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 14, img_tiling[step+1][DeviceId][1]-2, 512);
						break;
					case 20:
						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 14, img_tiling[step+1][DeviceId][1]-2, 512);
						break;
					case 22:
						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 14, img_tiling[step+1][DeviceId][1]-2, 512);
						break;
					case 24:
						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 7, img_tiling[step+1][DeviceId][1]-2, 1024);
						break;
					default:
						break;
				}
//				printf("3:%d\n",step);
//				TSC_delay_ms(1000);



					while(1)
					{
						InvalidCache((void *)&RecvNum, 4);
						if(RecvNum == 2)
						{
							RecvNum -= 2;
							//printf("%d:; ",RecvNum);
							break;
						}
					}
#if test_time
				if(s->ui_core == CORENUM)
				{
					EndTime = _itoll(TSCH, TSCL);
					printf("chuanshu, core : %d step : %d chuanshu time:%lld cycles\n",s->ui_core, step, (EndTime - StartTime));
				}
#endif
				}
#endif



#if ps
				//ps
				switch (step)
				{
					case 0:
						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 112, img_tiling[step+1][DeviceId][1]-2, 32);//后面四个数字需要每层核对
						break;
					case 2:
						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 112, img_tiling[step+1][DeviceId][1]-2, 64);
						break;
					case 4:
						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 56, img_tiling[step+1][DeviceId][1]-2, 128);
						break;
					case 6:
						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 56, img_tiling[step+1][DeviceId][1]-2, 128);
						break;
					case 8:
						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 28, img_tiling[step+1][DeviceId][1]-2, 256);
						break;
					case 10:
						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 28, img_tiling[step+1][DeviceId][1]-2, 256);
						break;
					case 12:
						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 14, img_tiling[step+1][DeviceId][1]-2, 512);
						break;
					case 14:
						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 14, img_tiling[step+1][DeviceId][1]-2, 512);
						break;
					case 16:
						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 14, img_tiling[step+1][DeviceId][1]-2, 512);
						break;
					case 18:
						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 14, img_tiling[step+1][DeviceId][1]-2, 512);
						break;
					case 20:
						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 14, img_tiling[step+1][DeviceId][1]-2, 512);
						break;
					case 22:
						writedata(s->io->tiling_input, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 14, img_tiling[step+1][DeviceId][1]-2, 512);
						break;
					case 24:
						writedata(s->io->tiling_output, s->io->tiling_buffer_read, s->io->tiling_buffer_write,1, 7, img_tiling[step+1][DeviceId][1]-2, 1024);
						break;
					default:
						break;
				}
#endif

			}
			else
			{
				ResetCoreStartFlag(s);
			}
		}
	}
	while(1)
	{
		asm("  nop 9");
	}
}
