#include <stddef.h>
#include <stdint.h>

#include "String.h"
#include "Memory.h"

enum vgaColour
{
	COLOUR_BLACK,
	COLOUR_BLUE,
	COLOUR_GREEN,
	COLOUR_CYAN,
	COLOUR_RED,
	COLOUR_MAGENTA,
	COLOUR_BROWN,
	COLOUR_LIGHT_GREY,
	COLOUR_DARK_GREY,
	COLOUR_LIGHT_BLUE,
	COLOUR_LIGHT_GREEN,
	COLOUR_LIGHT_CYAN,
	COLOUR_LIGHT_RED,
	COLOUR_LIGHT_MAGENTA,
	COLOUR_LIGHT_BROWN,
	COLOUR_WHITE,
};

uint8_t makeColour(vgaColour front, vgaColour back)
{
	return front | back << 4;
}

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

uint8_t terminalColour;
uint16_t* terminalBufferPtr;

string* screenBuffer;
size_t curpos;

uint16_t createVGAEntry(char c)
{
	uint16_t c16 = (uint16_t) c;
	uint16_t colour16 = (uint16_t) terminalColour;

	return c16 | (colour16 << 8);
}

void initTerminal()
{
	terminalColour = makeColour(COLOUR_LIGHT_GREY, COLOUR_BLACK);
	terminalBufferPtr = (uint16_t*) 0xB8000;

	screenBuffer = (string*) malloc(VGA_HEIGHT * sizeof *screenBuffer);
	curpos = 0;

	for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
	{
		terminalBufferPtr[i] = createVGAEntry(' ');
	}
}

void blitText();

void shiftText()
{
	string* newBuffer = (string*) malloc(VGA_HEIGHT * sizeof *newBuffer);

	for(int i = 1; i < VGA_HEIGHT; i++)
	{
		newBuffer[i - 1] = screenBuffer[i];
	}

	screenBuffer = newBuffer;
	curpos--;

	blitText();
}

void blitString(string s, int line)
{
	const char* c = s.characters;
	int length = s.length;

	for(int i = 0; i < length; i++)
	{
		terminalBufferPtr[line * VGA_WIDTH + i] = createVGAEntry(c[i]);
	}
}

void blitText()
{
	for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
	{
		terminalBufferPtr[i] = createVGAEntry(' ');
	}

	for(int i = 0; i < VGA_HEIGHT; i++)
	{
		blitString(screenBuffer[i], i);
	}
}

size_t strlen(const char* str)
{
	size_t size = 0;

	while(str[size] != 0)
	{
		size++;
	}

	return size;
}

void writeLine(const char* str)
{
	if(curpos == VGA_HEIGHT)
	{
		shiftText();
	}

	string s (str);

	screenBuffer[curpos++] = s;

	blitText();
}

extern "C"
void kernelInit()
{
}

extern "C"
void kernelPreInit()
{
	initTerminal();

	writeLine("KernelPreInit... ");
	screenBuffer[0] = screenBuffer[0].append(string("[OK]"));
	blitText();
}
