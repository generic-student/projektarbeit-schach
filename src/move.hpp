#pragma once

namespace sm
{
	struct Move
	{
		//define a constructor because default values cannot be set in bitfields
		Move() {
			capture = false;
			promotion = '\0';
		}

		unsigned short startCol : 4;
		unsigned short startRow : 4;
		unsigned short targetCol : 4;
		unsigned short targetRow : 4;

		bool capture : 8; // will be set to use 8bits because the struct would be padded with the remaining bits anyways
		unsigned short captureRow : 4;
		unsigned short captureCol : 4;

		char promotion : 8;
	};

}