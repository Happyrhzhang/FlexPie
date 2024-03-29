#include "datatrans.h" //datatrans-sensenet-common/commondef
extern "C" {
#include "SRIO/SRIO_Test.h"
}

extern volatile Uint32 RecvNum;

void core_work();
//extern struct_SRIOInstance SRIOInst;
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

	    *(Uint32*)0x8D100000 = 0;
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

//	TSC_delay_ms(2000);



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
//		TSC_delay_ms(2000);

//		for(int i = 0; i < 10; i++)
//		{
//			SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)(s->io->output_img1 + ((DeviceId + 4 - 1) % 4) * output_channel[0]*img_height[1]*img_width[1] / 4), (uint32_t)(s->io->output_img1 + ((DeviceId + 4 - 1) % 4) * output_channel[0]*img_height[1]*img_width[1] / 4), output_channel[0]*img_height[1]*img_width[1] / 4);
//			printf("%d\n",i);
//		}
	}

	//for (i_frame = 0; true; i_frame++)
	{
		for (step = 0; step < 29; step++)
		{
			//1-8核同步工作控制
			if (ui_core_num == 0)
			{
				WaitAllCoreReady(s);
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
				//LoadWeightBiasFromDDRToL2Channel_r1(s);
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

				conv_3x3_outputgroup_L2(s->io->input_image1,s->weight->weight1,s->bias->bias1,s->io->output_img1,0,s->ui_core, 2);
//				printf("!!!!!!!!!!!!!!!!!!!!%d\n",(uint32_t)s->io->output_img1);

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
				dw_conv_3x3_inputgroup_L2(s->io->output_img1,s->weight->weight2,s->bias->bias2,s->io->output_img2,1,s->ui_core, 3,1,1);

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

				dw_conv_1x1_outputgroup_L2(s->io->output_img2,s->weight->weight3,s->bias->bias3,s->io->output_img3,2,s->ui_core, 1);

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
				dw_conv_3x3_inputgroup_L2(s->io->output_img3,s->weight->weight4,s->bias->bias4,s->io->output_img4,3,s->ui_core, 3,2,0);

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

				dw_conv_1x1_outputgroup_L2(s->io->output_img4,s->weight->weight5,s->bias->bias5,s->io->output_img5,4,s->ui_core, 1);

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

				dw_conv_3x3_inputgroup_L2(s->io->output_img5,s->weight->weight6,s->bias->bias6,s->io->output_img6,5,s->ui_core, 3,1,1);

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
				dw_conv_1x1_outputgroup_L2(s->io->output_img6,s->weight->weight7,s->bias->bias7,s->io->output_img7,6,s->ui_core, 1);
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

				dw_conv_3x3_inputgroup_L2(s->io->output_img7,s->weight->weight8,s->bias->bias8,s->io->output_img8,7,s->ui_core, 3,2,0);

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

				dw_conv_1x1_outputgroup_L2(s->io->output_img8,s->weight->weight9,s->bias->bias9,s->io->output_img9,8,s->ui_core, 1);

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
				dw_conv_3x3_inputgroup_L2(s->io->output_img9,s->weight->weight10,s->bias->bias10,s->io->output_img10,9,s->ui_core, 3,1,1);
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
				dw_conv_1x1_outputgroup_L2(s->io->output_img10,s->weight->weight11,s->bias->bias11,s->io->output_img11,10,s->ui_core, 1);
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
				dw_conv_3x3_inputgroup_L2(s->io->output_img11,s->weight->weight12,s->bias->bias12,s->io->output_img12,11,s->ui_core, 3,2,0);
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
				dw_conv_1x1_outputgroup_L2(s->io->output_img12,s->weight->weight13,s->bias->bias13,s->io->output_img13,12,s->ui_core, 1);
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
				dw_conv_3x3_inputgroup_L2(s->io->output_img13,s->weight->weight14,s->bias->bias14,s->io->output_img14,13,s->ui_core, 3,1,1);
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
				dw_conv_1x1_outputgroup_L2(s->io->output_img14,s->weight->weight15,s->bias->bias15,s->io->output_img15,13,s->ui_core, 1);
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
				dw_conv_3x3_inputgroup_L2(s->io->output_img15,s->weight->weight16,s->bias->bias16,s->io->output_img16,13,s->ui_core, 3,1,1);
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

			dw_conv_1x1_outputgroup_L2(s->io->output_img16,s->weight->weight17,s->bias->bias17,s->io->output_img17,13,s->ui_core, 1);
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
				//LoadWeightBiasFromDDRToL2Channel_r2(s);
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
				dw_conv_3x3_inputgroup_L2(s->io->output_img17,s->weight->weight18,s->bias->bias18,s->io->output_img18,13,s->ui_core, 3,1,1);
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
				dw_conv_1x1_outputgroup_L2(s->io->output_img18,s->weight->weight19,s->bias->bias19,s->io->output_img19,13,s->ui_core, 1);
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
				dw_conv_3x3_inputgroup_L2(s->io->output_img19,s->weight->weight20,s->bias->bias20,s->io->output_img20,13,s->ui_core, 3,1,1);

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
				dw_conv_1x1_outputgroup_L2(s->io->output_img20,s->weight->weight21,s->bias->bias21,s->io->output_img21,13,s->ui_core, 1);
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
				dw_conv_3x3_inputgroup_L2(s->io->output_img21,s->weight->weight22,s->bias->bias22,s->io->output_img22,13,s->ui_core, 3,1,1);
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
				dw_conv_1x1_outputgroup_L2(s->io->output_img22,s->weight->weight23,s->bias->bias23,s->io->output_img23,13,s->ui_core, 1);
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
				//LoadWeightBiasFromDDRToL2Channel_r3(s);
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
				dw_conv_3x3_inputgroup_L2(s->io->output_img23,s->weight->weight24,s->bias->bias24,s->io->output_img24,14,s->ui_core, 3,2,0);
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
				dw_conv_1x1_outputgroup_L2(s->io->output_img24,s->weight->weight25,s->bias->bias25,s->io->output_img25,15,s->ui_core, 1);
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
				dw_conv_3x3_inputgroup_L2(s->io->output_img25,s->weight->weight26,s->bias->bias26,s->io->output_img26,16,s->ui_core, 3,2,4);
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
				dw_conv_1x1_outputgroup_L2(s->io->output_img26,s->weight->weight27,s->bias->bias27,s->io->output_img27,17,s->ui_core, 1);
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
				//LoadWeightBiasFromDDRToL2Channel_r5(s);
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
				dw_conv_1x1_outputgroup_L2(s->io->output_img26,s->weight->weight28,s->bias->bias28,s->io->output_img28,18,s->ui_core, 1);
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
				globalavg_pool_group(s->io->output_img29,s->io->output_img30,19,s->ui_core);
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




				if (step == 28)
				{

#if CALC_TOTAL_TIME
					EndTime = _itoll(TSCH, TSCL);
					printf("total time:%f ms\n", (EndTime - StartTime) / 1000000.0);
#endif
				}

#if ring
				for(int i = 0;i < 3;i++)
				{
					switch (step)
					{
						case 0:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img1 + ((DeviceId + 4 - i) % 4) * output_channel[0]*img_height[1]*img_width[1] / 4, (uint32_t)s->io->output_img1 + ((DeviceId + 4 - i) % 4) * output_channel[0]*img_height[1]*img_width[1] / 4, output_channel[0]*img_height[1]*img_width[1]);
							break;
//						case 1:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img2, (uint32_t)s->io->output_img2, output_channel[1]*img_height[2]*img_width[2]);
//							break;
						case 2:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4], (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4], output_channel[2]*img_height[3]*img_width[3] * 4);
							break;
//						case 3:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img4, (uint32_t)s->io->output_img4, output_channel[3]*img_height[4]*img_width[4]);
//							break;
						case 4:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4], (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4], output_channel[4]*img_height[5]*img_width[5] * 4);
							break;
//						case 5:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img6, (uint32_t)s->io->output_img6, output_channel[5]*img_height[6]*img_width[6]);
//							break;
						case 6:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4], (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4], output_channel[6]*img_height[7]*img_width[7] * 4);
							break;
//						case 7:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img8, (uint32_t)s->io->output_img8, output_channel[7]*img_height[8]*img_width[8]);
//							break;
						case 8:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4], (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4], output_channel[8]*img_height[9]*img_width[9] * 4);
							break;
//						case 9:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img10 , (uint32_t)s->io->output_img10 , output_channel[9]*img_height[10]*img_width[10]);
//							break;
						case 10:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , output_channel[10]*img_height[11]*img_width[11] * 4);
							break;
//						case 11:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img12 , (uint32_t)s->io->output_img12 , output_channel[11]*img_height[12]*img_width[12]);
//							break;
						case 12:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , output_channel[12]*img_height[13]*img_width[13] * 4);
							break;
//						case 13:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img14 , (uint32_t)s->io->output_img14 , output_channel[13]*img_height[13]*img_width[13]);
//							break;
						case 14:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , output_channel[13]*img_height[13]*img_width[13] * 4);
							break;
//						case 15:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img16 , (uint32_t)s->io->output_img16 , output_channel[13]*img_height[13]*img_width[13]);
//							break;
						case 16:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , output_channel[13]*img_height[13]*img_width[13] * 4);
							break;
//						case 17:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img18 , (uint32_t)s->io->output_img18 , output_channel[13]*img_height[13]*img_width[13]);
//							break;
						case 18:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , output_channel[13]*img_height[13]*img_width[13] * 4);
							break;
//						case 19:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img20 , (uint32_t)s->io->output_img20 , output_channel[13]*img_height[13]*img_width[13]);
//							break;
						case 20:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , output_channel[13]*img_height[13]*img_width[13] * 4);
							break;
//						case 21:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img22 , (uint32_t)s->io->output_img22 , output_channel[13]*img_height[13]*img_width[13]);
//							break;
						case 22:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , output_channel[13]*img_height[14]*img_width[14] * 4);
							break;
//						case 23:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img24 , (uint32_t)s->io->output_img24 , output_channel[14]*img_height[15]*img_width[15]);
//							break;
						case 24:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , output_channel[15]*img_height[16]*img_width[16] * 4);
							break;
//						case 25:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img26 , (uint32_t)s->io->output_img26 , output_channel[16]*img_height[17]*img_width[17]);
//							break;
						case 26:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , output_channel[17]*img_height[18]*img_width[18] * 4);
							break;
						case 27:
							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , (uint32_t)s->io->send_ddr[(DeviceId + 4 - i) % 4] , output_channel[18]*img_height[19]*img_width[19] * 4);
							break;
//						case 28:
//							SrioSendData(DeviceId, (DeviceId + 5) % 4,  (uint32_t)s->io->output_img29 + ((DeviceId + 4 - i) % 4) * output_channel[0]*img_height[1]*img_width[1] / 4, (uint32_t)s->io->output_img29 + ((DeviceId + 4 - i) % 4) * output_channel[19]*img_height[1]*img_width[1] / 4, output_channel[0]*img_height[1]*img_width[1]/4);
//							break;
						default:
							break;
					}
					//printf("srio test!!!");
					//TSC_delay_ms(100);


					if((step < 27 && step %2 == 0) || step == 27){

						while(1){
							//InvalidCache((void *)&RecvNum, 4);
							if(RecvNum != 0){
								break;
							}
							//printf("%d:,",RecvNum);
						}
						RecvNum -= 1;

					}

				}
//				while(num != 3)
//				{
//					;
//				}
#endif




#if ps
				//ps
				if((DeviceId != 0 && step < 27 && step % 2 == 0))
				{
					switch (step)
					{
						case 0:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->output_img1 + DeviceId * output_channel[0]*img_height[1]*img_width[1] / 4, (uint32_t)s->io->output_img1 + DeviceId * output_channel[0]*img_height[1]*img_width[1] / 4, output_channel[0]*img_height[1]*img_width[1]);
							break;
						case 2:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->send_ddr[DeviceId], (uint32_t)s->io->send_ddr[DeviceId], output_channel[2]*img_height[3]*img_width[3] * 4);
							break;
						case 4:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->send_ddr[DeviceId], (uint32_t)s->io->send_ddr[DeviceId], output_channel[4]*img_height[5]*img_width[5] * 4);
							break;
						case 6:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->send_ddr[DeviceId], (uint32_t)s->io->send_ddr[DeviceId], output_channel[6]*img_height[7]*img_width[7] * 4);
							break;
						case 8:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->send_ddr[DeviceId], (uint32_t)s->io->send_ddr[DeviceId], output_channel[8]*img_height[9]*img_width[9] * 4);
							break;
						case 10:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[10]*img_height[11]*img_width[11] * 4);
							break;
						case 12:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[12]*img_height[13]*img_width[13] * 4);
							break;
						case 14:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[13]*img_height[13]*img_width[13] * 4);
							break;
						case 16:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[13]*img_height[13]*img_width[13] * 4);
							break;
						case 18:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[13]*img_height[13]*img_width[13] * 4);
							break;
						case 20:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[13]*img_height[13]*img_width[13] * 4);
							break;
						case 22:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[13]*img_height[14]*img_width[14] * 4);
							break;
						case 24:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[15]*img_height[16]*img_width[16] * 4);
							break;
						case 26:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[17]*img_height[18]*img_width[18] * 4);
							break;
						case 27:
							SrioSendData(DeviceId, 0,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[18]*img_height[19]*img_width[19] * 4);
							break;
						default:
							break;
					}
					//等0给123片数据
					while(1)
					{
						InvalidCache((void *)&RecvNum, 4);
						if(RecvNum == 3)
						{
							RecvNum -= 3;
							//printf("%d:; ",RecvNum);
							break;
						}
					}

				}

				if((DeviceId == 0 && step < 27 && step % 2 == 0))
				{

					while(1)
					{
						InvalidCache((void *)&RecvNum, 4);
						if(RecvNum == 3)
						{
							RecvNum -= 3;
							//printf("%d:; ",RecvNum);
							break;
						}
					}

					for(int i = 1;i < 4;i++)
						for(int j = 0;j < 3;j++)
						{
							switch (step)
							{
								case 0:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->output_img1 + ((i+j+5) % 4) * output_channel[0]*img_height[1]*img_width[1] / 4, (uint32_t)s->io->output_img1 + ((i+j+5) % 4) * output_channel[0]*img_height[1]*img_width[1] / 4, output_channel[0]*img_height[1]*img_width[1]);
									break;
								case 2:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->send_ddr[((i+j+5) % 4)], (uint32_t)s->io->send_ddr[((i+j+5) % 4)], output_channel[2]*img_height[3]*img_width[3] * 4);
									break;
								case 4:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->send_ddr[((i+j+5) % 4)], (uint32_t)s->io->send_ddr[((i+j+5) % 4)], output_channel[4]*img_height[5]*img_width[5] * 4);
									break;
								case 6:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->send_ddr[((i+j+5) % 4)], (uint32_t)s->io->send_ddr[((i+j+5) % 4)], output_channel[6]*img_height[7]*img_width[7] * 4);
									break;
								case 8:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->send_ddr[((i+j+5) % 4)], (uint32_t)s->io->send_ddr[((i+j+5) % 4)], output_channel[8]*img_height[9]*img_width[9] * 4);
									break;
								case 10:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , output_channel[10]*img_height[11]*img_width[11] * 4);
									break;
								case 12:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , output_channel[12]*img_height[13]*img_width[13] * 4);
									break;
								case 14:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , output_channel[13]*img_height[13]*img_width[13] * 4);
									break;
								case 16:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , output_channel[13]*img_height[13]*img_width[13] * 4);
									break;
								case 18:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , output_channel[13]*img_height[13]*img_width[13] * 4);
									break;
								case 20:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , output_channel[13]*img_height[13]*img_width[13] * 4);
									break;
								case 22:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , output_channel[13]*img_height[14]*img_width[14] * 4);
									break;
								case 24:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , output_channel[15]*img_height[16]*img_width[16] * 4);
									break;
								case 26:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , output_channel[17]*img_height[18]*img_width[18] * 4);
									break;
								case 27:
									SrioSendData(DeviceId, i,  (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , (uint32_t)s->io->send_ddr[((i+j+5) % 4)] , output_channel[18]*img_height[19]*img_width[19] * 4);
									break;
								default:
									break;
							}
						}
				}
#endif

#if mesh
				for(int i = 0;i < 3;i++)
				{
					switch (step)
					{
						case 0:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->output_img1 + DeviceId * output_channel[0]*img_height[1]*img_width[1] / 4, (uint32_t)s->io->output_img1 + DeviceId * output_channel[0]*img_height[1]*img_width[1] / 4, output_channel[0]*img_height[1]*img_width[1]);
							break;
						case 2:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->send_ddr[DeviceId], (uint32_t)s->io->send_ddr[DeviceId], output_channel[2]*img_height[3]*img_width[3] * 4);
							break;
						case 4:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->send_ddr[DeviceId], (uint32_t)s->io->send_ddr[DeviceId], output_channel[4]*img_height[5]*img_width[5] * 4);
							break;
						case 6:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->send_ddr[DeviceId], (uint32_t)s->io->send_ddr[DeviceId], output_channel[6]*img_height[7]*img_width[7] * 4);
							break;
						case 8:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->send_ddr[DeviceId], (uint32_t)s->io->send_ddr[DeviceId], output_channel[8]*img_height[9]*img_width[9] * 4);
							break;
						case 10:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[10]*img_height[11]*img_width[11] * 4);
							break;
						case 12:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[12]*img_height[13]*img_width[13] * 4);
							break;
						case 14:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[13]*img_height[13]*img_width[13] * 4);
							break;
						case 16:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[13]*img_height[13]*img_width[13] * 4);
							break;
						case 18:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[13]*img_height[13]*img_width[13] * 4);
							break;
						case 20:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[13]*img_height[13]*img_width[13] * 4);
							break;
						case 22:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[13]*img_height[14]*img_width[14] * 4);
							break;
						case 24:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[15]*img_height[16]*img_width[16] * 4);
							break;
						case 26:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[17]*img_height[18]*img_width[18] * 4);
							break;
						case 27:
							SrioSendData(DeviceId, (DeviceId + i + 1) % 4,  (uint32_t)s->io->send_ddr[DeviceId] , (uint32_t)s->io->send_ddr[DeviceId] , output_channel[18]*img_height[19]*img_width[19] * 4);
							break;
						default:
							break;
					}
					//printf("srio test!!!");
					//TSC_delay_ms(100);


//					if((step < 27 && step %2 == 0) || step == 27){
//
//						while(1){
//							if(RecvNum != 0){
//								break;
//							}
//						}
//						RecvNum -= 1;
//
//					}

				}
				if((step < 27 && step %2 == 0) || step == 27){

					while(1){
						if(RecvNum == 3){
							break;
						}
					}
					RecvNum -= 3;

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
