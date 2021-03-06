/*
unsigned int data_old[20] = {0xf24e6008, 0xf2c4000f, 0xf04f0120, 0x6001bf00, 0xf2450000, 0xf2c40002, 0xf244313b, 0xf6c4414f,
							 0xF8C01520, 0xf04f0104, 0xf8c01400, 0xf8c01510, 0xf8c0151c, 0xf2490239, 0xf2c0020d, 0x1e52bf01, 
							 0xf2490239, 0xf2c0020d, 0x43c9f8c0, 0x13fce7f5};
*/

unsigned int data[23] =     {0x6008f24e, 0x000ff2c4, 0x0120f04f, 0xbf006001, 0x0000f245, 0x0002f2c4, 0x314bf244, 0x414ff6c4,
							 0x1520f8c0, 0x0104f04f, 0x1400f8c0, 0x1510f8c0, 0x151cf8c0, 0x0239f249, 0x020df2c0, 0xbf011e52,
							 0x0239f249, 0x020df2c0, 0xbf011e52, 0x0239f249, 0x020df2c0, 0xf8c043c9, 0xe7f513fc};
	
//unsigned int data_bad[2={0x4ef20860, 0xc4f20f00, 0x4ff02001, 0x016000bf, 0x45f20000, 0xc4f20200, 0x44f23b31, 0xc4f64f41,
//							 0xc0f82015, 0x4ff00401, 0xc0f80014, 0xc0f81015, 0xc0f81c15, 0x49f23902, 0xc0f20d02, 0x521e01bf,
//							 0x49f23902, 0xc0f20d02, 0xc943c0f8, 0xfc13f5e7};




int main(void)
{	
	unsigned int* key_address = (unsigned int*) 0x400fe1d0;
	unsigned int* FLASH_BASE = (unsigned int*) 0x400fd000;
	char i;
	unsigned int key;
	
	if((key_address[0x0] & 0x8) == 0x8)
		key = 0xa4420000;
	else
		key = 0x71d50000;
	
	FLASH_BASE[0x0] = 0x1400;
	FLASH_BASE[0x8/4] = key | 0x2;
	while((FLASH_BASE[0x8/4] & 0x2) == 0x2);	//erasing flash until bit cleared

	for(i=0; i<23; i++)
	{
		FLASH_BASE[0x4/4] = data[i];			//load next chunk of data
		FLASH_BASE[0x0] = 0x1400 + 4*i;			//increment location to store data
		FLASH_BASE[0x8/4] = key | 1;			//enable write
		while((FLASH_BASE[0x8/4] & 0x1) == 0x1);
	}
	
	FLASH_BASE[0x4/4] = 0x14000000;	
	FLASH_BASE[0x0] = 0x4;
	FLASH_BASE[0x8/4] = key | 1;
	
	__nop();
	
	while(1);
}
