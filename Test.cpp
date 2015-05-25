	writeLine("Hello, World!");

	size_t size = 0;

	char* str = (char*) malloc(50 * sizeof *str);

	while(getCharFromEntry(terminalBufferPtr[size]) != 0)
	{
		if(size > 20)
			break;

		terminalPutChar(getCharFromEntry(terminalBufferPtr[size]));

		size++;
	}




		size_t terminalPos = 0;
	size_t curLine = 0;

	for(int i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++)
	{
		char c = screenBuffer[i].;

		if(c == '\n')
		{
			curLine++;
			terminalPos = VGA_WIDTH * curLine;
		}

		terminalBufferPtr[terminalPos] = createVGAEntry(c);

		if(c != '\n')
		{
			terminalPos++;
		}
	}