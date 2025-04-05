
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include "sys_fileio.h"
#include "log.h"
#include "miniz.h"
#include <windows.h>  //for GetModuleFileName
#include <fstream>
#include <string>

/*
#include <filesystem> //C++ 17 experimental, just for fun

namespace fs = std::experimental::filesystem::v1;

std::string readFile(fs::path path)
{
	// Open the stream to 'lock' the file.
	std::ifstream f{ path };

	// Obtain the size of the file.
	const auto sz = fs::file_size(path);

	// Create a buffer.
	std::string result(sz, ' ');

	// Read the whole file into the buffer.
	f.read((char *) result.data(), sz);
	return result;
}
*/

//File operation variables

long filesz = 0;
size_t uncomp_size = 0;

#pragma warning (disable : 4996)

int getLastFileSize()
{
	return filesz;
}


size_t getlastZsize()
{
	return  uncomp_size;
}


std::wstring getCurrentDirectoryW()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}


std::string getCurrentDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}


int getFileSize(FILE *input)
{
	int fileSizeBytes;

	fseek(input, 0, SEEK_END);
	fileSizeBytes = ftell(input);
	fseek(input, 0, SEEK_SET);
	return fileSizeBytes;
}


uint8_t *loadFile(const std::string  &filename)
{
	uint8_t *buf = nullptr;
	filesz = 0;

	wrlog("Opening File");
	FILE *fd = fopen(filename.c_str(), "rb");
	if (!fd) { wrlog("Filed to find file!! %s.", filename); return (0); }

	filesz = getFileSize(fd);
	buf = (uint8_t *)malloc(filesz);
	fread(buf, 1, filesz, fd);
	fclose(fd);
	return buf;
}


uint8_t *loadFile(char *filename)
{
	uint8_t *buf = nullptr;
	filesz = 0;

	wrlog("Opening File");
	FILE *fd = fopen(filename, "rb");
	if (!fd) { wrlog("Filed to find file!! %s.", filename); return (0); }

	filesz = getFileSize(fd);
	buf = (uint8_t *)malloc(filesz);
	fread(buf, 1, filesz, fd);
	fclose(fd);
	return buf;
}


char *charloadFile(const char *filename)
{
	char *buf = nullptr;
	filesz = 0;

	wrlog("Opening File");
	FILE *fd = fopen(filename, "rb");
	if (!fd) { wrlog("Filed to find file!! %s.", filename); return (0); }

	filesz = getFileSize(fd);
	buf = (char *)malloc(filesz);
	fread(buf, 1, filesz, fd);
	fclose(fd);
	return buf;
}


bool saveFile(char *filename, unsigned char *buf, int size)
{
	FILE *fd = fopen(filename, "wb");
	if (!fd) { wrlog("Filed to save file %s.", filename); return false; }

	fwrite(buf, size, 1, fd);
	fclose(fd);
	return true;
}


void replaceExtension(std::string &str, std::string rep)
{
	//Usage
	//std::string test; 
	//test.assign("asteroids.png");
	//IO.ReplaceExtension(test,"exe");

	size_t extPos = str.rfind('.');
	// Add the new extension. 
	str.replace(extPos + 1, rep.length(), rep);
	rep.clear();
}


// ToDo: Add a debug clause in front of the logging to disable it
unsigned char *loadGenericZip(const char *archname, const char *filename)
{
	mz_bool status;
	mz_uint file_index = -1;
	mz_zip_archive zip_archive;
	mz_zip_archive_file_stat file_stat;

	unsigned char *buf = nullptr;
	int ret = 1; // Zero Means the file didn't load, 1 Means everything is hunky dory. We start with one. 

	wrlog("Opening Archive %s", archname);
	// Now try to open the archive.
	memset(&zip_archive, 0, sizeof(zip_archive));
	status = mz_zip_reader_init_file(&zip_archive, archname, 0);
	if (!status) { wrlog("Zip Archive %s not found. (Check your path?)", archname); ret = 0; goto end; }

	// Find the requested file, ignore case 
	file_index = mz_zip_reader_locate_file(&zip_archive, filename, 0, 0);
	if (file_index == -1) { wrlog("Error: File %s not found in Zip Archive %s", filename, archname); ret = 0; goto end; }

	// Get information on the current file
	status = mz_zip_reader_file_stat(&zip_archive, file_index, &file_stat);
	if (status != MZ_TRUE) { wrlog("Error reading Zip File Info, it's probably corrupt"); ret = 0; goto end; }

	//Fill in the size in case we need to get it later
	uncomp_size = (size_t)file_stat.m_uncomp_size;

	//Create the unsigned char buffer
	buf = (unsigned char *)malloc(uncomp_size);
	if (!buf) { wrlog("Failed to create char buffer, mem error?"); ret = 0; goto end; }

	// Read (decompress) the file 
	status = mz_zip_reader_extract_to_mem(&zip_archive, file_index, buf, uncomp_size, 0);
	if (status != MZ_TRUE) { wrlog("Failed to extract zip file to mem for some weird reason"); ret = 0; goto end; }

end:
	// Close the archive 
	wrlog("Closing Archive");
	mz_zip_reader_end(&zip_archive);

	if (ret) { wrlog("Zip file loaded Successfully: %d from archive %s", filename, archname); }
	else { wrlog("Zip file %d in archive %s failed to load!", filename, archname); return 0; }

	return buf;
}

// ToDo: Add a debug clause in front of the logging to disable it
bool saveGenericZip(const char *archname, const char *filename, unsigned char *data)
{
	bool status;

	status = mz_zip_add_mem_to_archive_file_in_place(archname, filename, data, strlen((char *)data) + 1, 0, 0, MZ_BEST_COMPRESSION);
	if (!status)
	{
		wrlog("mz_zip_add_mem_to_archive_file_in_place failed!");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
