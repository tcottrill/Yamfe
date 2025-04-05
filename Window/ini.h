#ifndef INI_H
#define INI_H

// C style naming conventions to be compatible with old allegro code
// NOT unicode compatible


void set_config_file(char* szFileName);

int get_config_int(char* szSection, char* szKey, int iDefaultValue);
float get_config_float(char* szSection, char* szKey, float fltDefaultValue);
bool get_config_bool(char* szSection, char* szKey, bool bolDefaultValue);

void set_config_int(char* szSection, char* szKey, int iValue);
void set_config_float(char* szSection, char* szKey, float fltValue);
void set_config_bool(char* szSection, char* szKey, bool bolValue);
char* get_config_string(const char* szSection, const char* szKey, const char* szDefaultValue);


#endif//INI_H