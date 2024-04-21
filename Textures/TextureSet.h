#ifndef TEXTURE_SET
#define TEXTURE_SET


extern unsigned int texture1;
extern unsigned int selectedTexture;

void ApplyTexture();

void ClearTexture();

unsigned char* ReadBPM(char* fileName);

#endif
