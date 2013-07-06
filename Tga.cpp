#include "stdafx.h"
#include "Tga.h"


TGAHeader header;
TGA tga;

bool LoadTGA(TextureImage *texture, char *fileName)							// ��TGA�ļ��������ڴ�
{

		// δѹ����TGAͷ
	GLubyte uTGAcompare[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
	// ѹ����TGAͷ
	GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};

	FILE* pFile=fopen(fileName,"rb");

	if(pFile==NULL)
	{
		MessageBox(NULL,_T("�޷���TGA�ļ�"),_T("����"),MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
	if(fread(&header,1,sizeof(header),pFile)==0)
	{
		fclose(pFile);
		MessageBox(NULL,_T("��ȡTGA�ļ�ͷ��ʧ��"),_T("����"),MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
	if(memcmp(uTGAcompare,&header,sizeof(header))==0)
	{
		return LoadUncompassTGA(texture,pFile);
	}
	else if(memcmp(cTGAcompare,&header,sizeof(header))==0)
	{
		return LoadCompassTGA(texture,pFile);
	}
	else
	{
		fclose(pFile);
		MessageBox(NULL,_T("�޷�ʶ���TGA�ļ�ͷ��"),_T("����"),MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
	fclose(pFile);
	return true;
}

bool LoadUncompassTGA(TextureImage *texture, FILE* pFile)							// ��TGA�ļ��������ڴ�
{

	GLubyte		header[6];													// ���������õ�ͼ����Ϣ�����ߣ�λ��
	GLuint		bytesPerPixel;												// ��¼ÿ����ɫ��ռ�õ��ֽ���
	GLuint		imageSize;													// ��¼�ļ���С
	GLuint		temp;														// ��ʱ����
	GLuint		type=GL_RGBA;												// ����Ĭ�ϵĸ�ʽΪGL_RGBA����32λͼ��

	if(fread(header,1,sizeof(header),pFile)!=sizeof(header))				// ����Ƕ�ȡ��������ͼ����Ϣ
	{
		if (pFile == NULL)													// �ļ������ڷ��ش���
			return false;							
		else
		{
			fclose(pFile);													// �ر��ļ����ش���
			return false;							
		}
	}
	texture->width  = header[1] * 256 + header[0];							// ��¼�ļ��߶�
	texture->height = header[3] * 256 + header[2];							// ��¼�ļ����

 	if(	texture->width	<=0	||												// ����Ƿ�С��0
		texture->height	<=0	||												// �߶��Ƿ�С��0
		(header[4]!=24 && header[4]!=32))									// TGA�ļ���24/32λ��
	{
		fclose(pFile);														// ���ʧ�ܹر��ļ������ش���
		return false;								
	}
	texture->bpp	= header[4];											// ��¼�ļ���λ��
	bytesPerPixel	= texture->bpp/8;										// ��¼ÿ��������ռ���ֽ���
	imageSize	= texture->width*texture->height*bytesPerPixel;				// ����TGA�ļ���������Ҫ���ڴ��С
	texture->imageData=(GLubyte *)malloc(imageSize);						// �����ڴ�ȥ����TGA����

	if(	texture->imageData==NULL ||											// ϵͳ�Ƿ�������㹻���ڴ棿
		fread(texture->imageData, 1, imageSize, pFile)!=imageSize)			// �Ƿ�ɹ������ڴ�?
	{
		if(texture->imageData!=NULL)										// �Ƿ������ݱ�����
			free(texture->imageData);										// ����ǣ����ͷ����������

		fclose(pFile);														// �ر��ļ�
		return false;														// ���ش���
	}
	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)						// ѭ�����е�����
	{																		// ����R��B��ֵ
		temp=texture->imageData[i];						
		texture->imageData[i] = texture->imageData[i + 2];		
		texture->imageData[i + 2] = temp;				
	}

	fclose (pFile);															// �ر��ļ�
	return true;
}

bool LoadCompassTGA(TextureImage *texture,FILE* pFile)
{

	if(fread(tga.header, sizeof(tga.header), 1, pFile) == 0)					
	{
		MessageBox(NULL, _T("Could not read info header"), _T("ERROR"), MB_OK);		
		if(pFile != NULL)													
		{
			fclose(pFile);													
		}
		return false;														
	}

	texture->width  = tga.header[1] * 256 + tga.header[0];					
	texture->height = tga.header[3] * 256 + tga.header[2];					
	texture->bpp	= tga.header[4];										
	tga.Width		= texture->width;										
	tga.Height		= texture->height;										
	tga.Bpp			= texture->bpp;											

	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))	
	{
		MessageBox(NULL, _T("Invalid texture information"), _T("ERROR"), MB_OK);	
		if(pFile != NULL)													
		{
			fclose(pFile);													
		}
		return false;														
	}

	tga.bytesPerPixel	= (tga.Bpp / 8);									
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		
	texture->imageData	= (GLubyte *)malloc(tga.imageSize);					

	if(texture->imageData == NULL)											
	{
		MessageBox(NULL, _T("Could not allocate memory for image"), _T("ERROR"), MB_OK);	
		fclose(pFile);														
		return false;														
	}

	GLuint pixelcount	= tga.Height * tga.Width;							
	GLuint currentpixel	= 0;												
	GLuint currentbyte	= 0;												
	GLubyte * colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);			

	do
	{
		GLubyte chunkheader = 0;											

		if(fread(&chunkheader, sizeof(GLubyte), 1, pFile) == 0)				
		{
			MessageBox(NULL, _T("Could not read RLE header"), _T("ERROR"), MB_OK);	
			if(pFile != NULL)												
			{
				fclose(pFile);												
			}
			if(texture->imageData != NULL)									
			{
				free(texture->imageData);									
			}
			return false;													
		}

		if(chunkheader < 128)												
		{																	
			chunkheader++;													
			for(short counter = 0; counter < chunkheader; counter++)		
			{
				if(fread(colorbuffer, 1, tga.bytesPerPixel, pFile) != tga.bytesPerPixel) 
				{
					MessageBox(NULL, _T("Could not read image data"), _T("ERROR"), MB_OK);		

					if(pFile != NULL)													
					{
						fclose(pFile);													
					}

					if(colorbuffer != NULL)												
					{
						free(colorbuffer);												
					}

					if(texture->imageData != NULL)										
					{
						free(texture->imageData);										
					}

					return false;														
				}
																						
				texture->imageData[currentbyte		] = colorbuffer[2];				    
				texture->imageData[currentbyte + 1	] = colorbuffer[1];
				texture->imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)												
				{
					texture->imageData[currentbyte + 3] = colorbuffer[3];				
				}

				currentbyte += tga.bytesPerPixel;										
				currentpixel++;															

				if(currentpixel > pixelcount)											
				{
					MessageBox(NULL, _T("Too many pixels read"), _T("ERROR"), NULL);			

					if(pFile != NULL)													
					{
						fclose(pFile);													
					}	

					if(colorbuffer != NULL)												
					{
						free(colorbuffer);												
					}

					if(texture->imageData != NULL)										
					{
						free(texture->imageData);										
					}

					return false;														
				}
			}
		}
		else																			
		{
			chunkheader -= 127;															
			if(fread(colorbuffer, 1, tga.bytesPerPixel, pFile) != tga.bytesPerPixel)		
			{	
				MessageBox(NULL, _T("Could not read from pFile"), _T("ERROR"), MB_OK);			

				if(pFile != NULL)														
				{
					fclose(pFile);														
				}

				if(colorbuffer != NULL)													
				{
					free(colorbuffer);													
				}

				if(texture->imageData != NULL)											
				{
					free(texture->imageData);											
				}

				return false;															
			}

			for(short counter = 0; counter < chunkheader; counter++)					
			{																			
				texture->imageData[currentbyte		] = colorbuffer[2];					
				texture->imageData[currentbyte + 1	] = colorbuffer[1];
				texture->imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)												
				{
					texture->imageData[currentbyte + 3] = colorbuffer[3];				
				}

				currentbyte += tga.bytesPerPixel;										
				currentpixel++;															

				if(currentpixel > pixelcount)											
				{
					MessageBox(NULL, _T("Too many pixels read"), _T("ERROR"), NULL);			

					if(pFile != NULL)													
					{
						fclose(pFile);													
					}	

					if(colorbuffer != NULL)												
					{
						free(colorbuffer);												
					}

					if(texture->imageData != NULL)										
					{
						free(texture->imageData);										
					}

					return false;														
				}
			}
		}
	}

	while(currentpixel < pixelcount);													
	fclose(pFile);																		
	return true;																		
}

