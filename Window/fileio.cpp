#include "osdepend.h"

#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "ini.h"

#pragma warning(disable:4996)		// "The POSIX name for this item is deprecated"

/* file handling routines */

/* gamename holds the driver name, filename is only used for ROMs and samples. */
/* if 'write' is not 0, the file is opened for write. Otherwise it is opened */
/* for read. */
void *osd_fopen(const char *gamename,const char *filename,int filetype,int write)
{
	char name[100];
	void *f;
	char *dirname;

	set_config_file("./aafe.ini");

	wrlog("opening mame file");

	switch (filetype)
	{
		case OSD_FILETYPE_ROM:
		case OSD_FILETYPE_SAMPLE:
			sprintf(name,"%s/%s",gamename,filename);
			wrlog("Trying Opening sample  name,%s/%s",gamename,filename);
			f = fopen(name,write ? "wb" : "rb");
			if (f == 0)
			{
				wrlog("Trying Opening sample zip name,%s/%s",gamename,filename);
				/* try with a .zip directory (if ZipMagic is installed) */
				sprintf(name,"%s.zip/%s",gamename,filename);
				f = fopen(name,write ? "wb" : "rb");

			}
			
			if (f == 0)
			{

				/* try again in the appropriate subdirectory */
				dirname = "";
				if (filetype == OSD_FILETYPE_ROM)
					dirname = (char *)get_config_string("directory","roms","ROMS");
				if (filetype == OSD_FILETYPE_SAMPLE)
					dirname = (char *)get_config_string("directory","samples","SAMPLES");

				wrlog("Trying Opening sample in dir %s name,%s/%s",dirname,gamename,filename);
				sprintf(name,"%s/%s/%s",dirname,gamename,filename);
				f = fopen(name,write ? "wb" : "rb");
				if (f == 0)
				{
					wrlog("Trying Opening sample in dir %s name as zip, last try,%s/%s",dirname,gamename,filename);
					/* try with a .zip directory (if ZipMagic is installed) */
					sprintf(name,"%s/%s.zip/%s",dirname,gamename,filename);
					f = fopen(name,write ? "wb" : "rb");
				}
				
			}
			return f;
			break;
		case OSD_FILETYPE_HIGHSCORE:
			/* try again in the appropriate subdirectory */
			 dirname = (char *)get_config_string("directory","hi","HI");

			sprintf(name,"%s/%s.hi",dirname,gamename);
			f = fopen(name,write ? "wb" : "rb");
			if (f == 0)
			{
				/* try with a .zip directory (if ZipMagic is installed) */
				sprintf(name,"%s.zip/%s.hi",dirname,gamename);
				f = fopen(name,write ? "wb" : "rb");
			}
			
			return f;
			break;
		case OSD_FILETYPE_CONFIG:
			/* try again in the appropriate subdirectory */
			 dirname = (char *)get_config_string("directory","cfg","CFG");

			sprintf(name,"%s/%s.cfg",dirname,gamename);

			wrlog("OPening file %s %s",dirname,gamename);

			f = fopen(name,write ? "wb" : "rb");
			if (f == 0)
			{
				wrlog("Tried to write config data but failed!!! %s %s",dirname,gamename);
				/* try with a .zip directory (if ZipMagic is installed) */
				sprintf(name,"%s.zip/%s.cfg",dirname,gamename);
				f = fopen(name,write ? "wb" : "rb");
			}
			
			return f;
			break;
		case OSD_FILETYPE_INPUTLOG:
			sprintf(name,"%s.inp",filename);
			return fopen(name,write ? "wb" : "rb");
			break;
		default:
			return 0;
			break;
	}
}


int osd_fread_scatter (void *file, void *buffer, int length, int increment)
{
	/*
	unsigned char *buf = buffer;
	FakeFileHandle *f = (FakeFileHandle *) file;
	unsigned char tempbuf[4096];
	int totread, r, i;

	switch( f->type )
	{
	case kPlainFile:
		totread = 0;
		while (length)
		{
			r = length;
			if( r > 4096 )
				r = 4096;
			r = fread (tempbuf, 1, r, f->file);
			if( r == 0 )
				return totread;		   // error
			for( i = 0; i < r; i++ )
			{
				*buf = tempbuf[i];
				buf += increment;
			}
			totread += r;
			length -= r;
		}
		return totread;
		break;
	case kZippedFile:
	case kRAMFile:
		// reading from the RAM image of a file 
		if( f->data )
		{
			if( length + f->offset > f->length )
				length = f->length - f->offset;
			for( i = 0; i < length; i++ )
			{
				*buf = f->data[f->offset + i];
				buf += increment;
			}
			f->offset += length;
			return length;
		}
		break;
	}
*/
	return 0;
}

int osd_fread(void *file,void *buffer,int length)
{
	return fread(buffer,1,length,(FILE *)file);
}


int osd_fread_swap(void *file,void *buffer,int length)
{
	int i;
	unsigned char *buf;
	unsigned char temp;
	int res;


	res = osd_fread(file,buffer,length);

	buf = (unsigned char *) buffer;
	for (i = 0;i < length;i+=2)
	{
		temp = buf[i];
		buf[i] = buf[i+1];
		buf[i+1] = temp;
	}

	return res;
}


int osd_fwrite(void *file,const void *buffer,int length)
{
	return fwrite(buffer,1,length,(FILE *)file);
}



int osd_fseek(void *file,int offset,int whence)
{
	return fseek((FILE *)file,offset,whence);
}



void osd_fclose(void *file)
{
	fclose((FILE *)file);
}

unsigned int osd_fcrc (void *file)
{
	
	return 0;
}
