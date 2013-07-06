#include "stdafx.h"
#include "Tga.h"


TGAHeader header;
TGA tga;

bool LoadTGA(TextureImage *texture, char *fileName)							// 把TGA文件加载入内存
{

		// 未压缩的TGA头
	GLubyte uTGAcompare[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
	// 压缩的TGA头
	GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};

	FILE* pFile=fopen(fileName,"rb");

	if(pFile==NULL)
	{
		MessageBox(NULL,_T("无法打开TGA文件"),_T("错误"),MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
	if(fread(&header,1,sizeof(header),pFile)==0)
	{
		fclose(pFile);
		MessageBox(NULL,_T("读取TGA文件头部失败"),_T("错误"),MB_OK|MB_ICONEXCLAMATION);
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
		MessageBox(NULL,_T("无法识别的TGA文件头部"),_T("错误"),MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
	fclose(pFile);
	return true;
}

bool LoadUncompassTGA(TextureImage *texture, FILE* pFile)							// 把TGA文件加载入内存
{

	GLubyte		header[6];													// 保存最有用的图像信息，宽，高，位深
	GLuint		bytesPerPixel;												// 记录每个颜色所占用的字节数
	GLuint		imageSize;													// 记录文件大小
	GLuint		temp;														// 临时变量
	GLuint		type=GL_RGBA;												// 设置默认的格式为GL_RGBA，即32位图像

	if(fread(header,1,sizeof(header),pFile)!=sizeof(header))				// 如果是读取下面六个图像信息
	{
		if (pFile == NULL)													// 文件不存在返回错误
			return false;							
		else
		{
			fclose(pFile);													// 关闭文件返回错误
			return false;							
		}
	}
	texture->width  = header[1] * 256 + header[0];							// 记录文件高度
	texture->height = header[3] * 256 + header[2];							// 记录文件宽度

 	if(	texture->width	<=0	||												// 宽度是否小于0
		texture->height	<=0	||												// 高度是否小于0
		(header[4]!=24 && header[4]!=32))									// TGA文件是24/32位？
	{
		fclose(pFile);														// 如果失败关闭文件，返回错误
		return false;								
	}
	texture->bpp	= header[4];											// 记录文件的位深
	bytesPerPixel	= texture->bpp/8;										// 记录每个象素所占的字节数
	imageSize	= texture->width*texture->height*bytesPerPixel;				// 计算TGA文件加载所需要的内存大小
	texture->imageData=(GLubyte *)malloc(imageSize);						// 分配内存去保存TGA数据

	if(	texture->imageData==NULL ||											// 系统是否分配了足够的内存？
		fread(texture->imageData, 1, imageSize, pFile)!=imageSize)			// 是否成功读入内存?
	{
		if(texture->imageData!=NULL)										// 是否有数据被加载
			free(texture->imageData);										// 如果是，则释放载入的数据

		fclose(pFile);														// 关闭文件
		return false;														// 返回错误
	}
	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)						// 循环所有的像素
	{																		// 交换R和B的值
		temp=texture->imageData[i];						
		texture->imageData[i] = texture->imageData[i + 2];		
		texture->imageData[i + 2] = temp;				
	}

	fclose (pFile);															// 关闭文件
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

