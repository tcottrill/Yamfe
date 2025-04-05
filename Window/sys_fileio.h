#pragma once

#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include "log.h"

//Get Current Directory
std::wstring getCurrentDirectoryW();
std::string getCurrentDirectory();

//Get information on last file operation
int getLastFileSize();
int getFileSize(FILE *input);

//Get Information on last Compression Operation
size_t getlastZsize();

//Load\Save File
char *charloadFile(const char *filename);
uint8_t *loadFile(char *filename);
uint8_t *loadFile(const std::string  &filename);
bool saveFile(char *filename, unsigned char *buf, int size);

//Load\Save Zip
unsigned char *loadGenericZip(const char *archname, const char *filename);
bool saveGenericZip(const char *archname, const char *filename, unsigned char *data);

//Replace File Extension
void replaceExtension(std::string &str, std::string rep);

#endif