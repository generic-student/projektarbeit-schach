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

		static Move null() {
			return Move(0,0,0,0, false, 0, 0,  '\0');
		}

		constexpr Move( unsigned short sRow, unsigned short sCol,
			unsigned short tRow, unsigned short tCol,
			bool c, unsigned short cRow, unsigned short cCol, char p) 
				: startRow(sRow), startCol(sCol), targetRow(tRow), targetCol(tCol),
				capture(c), captureRow(cRow), captureCol(cCol), promotion(p)
			{}

		unsigned short startRow : 4;
		unsigned short startCol : 4;
		unsigned short targetRow : 4;
		unsigned short targetCol : 4;

		bool capture : 8; // will be set to use 8bits because the struct would be padded with the remaining bits anyways
		unsigned short captureRow : 4;
		unsigned short captureCol : 4;

		char promotion : 8;
	};

}