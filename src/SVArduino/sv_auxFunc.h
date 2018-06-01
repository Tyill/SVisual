#pragma once


namespace svisual{

void parseBytes(const char* str, char sep, byte* bytes, int maxBytes, int base) {
	for (int i = 0; i < maxBytes; ++i) {
		bytes[i] = strtoul(str, NULL, base);  
		str = strchr(str, sep);               
		if (str == NULL || *str == '\0') {
			break;                            
		}
		str++;                                
	}
}
}