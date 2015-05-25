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

uint16_t* terminalBufferPtr;

string* screenBuffer;
size_t curpos;

uint16_t createVGAEntry(char c, uint8_t colour)
{
	uint16_t c16 = (uint16_t) c;
	uint16_t colour16 = (uint16_t) colour;

	return c16 | (colour16 << 8);
}

void initTerminal()
{
	terminalBufferPtr = (uint16_t*) 0xB8000;

	screenBuffer = (string*) malloc(VGA_HEIGHT * sizeof *screenBuffer);
	curpos = 0;

	for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
	{
		terminalBufferPtr[i] = createVGAEntry(' ', makeColour(COLOUR_LIGHT_GREY, COLOUR_BLACK));
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
	uint8_t colour = makeColour(COLOUR_LIGHT_GREY, COLOUR_BLACK);

	int pos = 0;

	for(int i = 0; i < s.length(); i++)
	{
		char c = s[i];

		if(c == '&')
		{
			i++;

			c = s[i];

			if(c == '&')
			{
				terminalBufferPtr[line * VGA_WIDTH + i] = createVGAEntry(c, colour);
			}
			else
			{
				switch(c)
				{
					case 'G':
						colour = makeColour(COLOUR_LIGHT_GREEN, COLOUR_BLACK);
						break;
					case 'L':
						colour = makeColour(COLOUR_LIGHT_GREY, COLOUR_BLACK);
						break;
				}
			}
		}
		else
		{
			terminalBufferPtr[line * VGA_WIDTH + pos] = createVGAEntry(c, colour);

			pos++;
		}
	}
}

void blitText()
{
	for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
	{
		terminalBufferPtr[i] = createVGAEntry(' ', makeColour(COLOUR_LIGHT_GREY, COLOUR_BLACK));
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

string getLine(int line)
{
	return screenBuffer[line];
}

void setLine(int line, string s)
{
	screenBuffer[line] = s;

	blitText();
}

void appendLine(int line, string s)
{
	setLine(line, getLine(line).append(s));
}

extern "C"
void kernelInit()
{
	writeLine("KernelInit... ");
	appendLine(1, string("[&GOK&L]"));
}

extern "C"
void kernelPreInit()
{
	initTerminal();

	writeLine("KernelPreInit... ");
	appendLine(0, string("[&GOK&L]"));
}
