//============================================================================
// Name        : Sample_1.cpp
// Author      : Mike Smith
// Version     :
// Copyright   : © MWS
// Description : Hello World in C, Ansi-style
//============================================================================


#include "Common.h"
#include "Sample.h"


int main() {
	Sample app;
	app.Initialize();

	app.MainLoop();

	return 0;
}
